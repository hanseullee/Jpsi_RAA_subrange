#!/usr/bin/env bash
#
# Usage:
#   bash run_ctau_binned_mass.sh [options]
#
# Examples:
#   bash run_ctau_binned_mass.sh
#   bash run_ctau_binned_mass.sh --jobs=8 --logy
#   bash run_ctau_binned_mass.sh --dry-run
#
# Options:
#   --jobs=N              Number of ROOT jobs to run in parallel. Default: 4
#   --logy                Save mass-slice plots with log y-axis.
#   --dry-run             Print commands without running ROOT.
#   -h, --help            Show help.
#
# Outputs:
#   logs/run_ctau_binned_mass_YYYYMMDD_HHMMSS/*.log
#   logs/run_ctau_binned_mass_YYYYMMDD_HHMMSS/*.cmd

set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "${script_dir}"

jobs=8
dry_run=0
use_log_y=false
timestamp="$(date +"%Y%m%d_%H%M%S")"
log_dir="logs/run_ctau_binned_mass_${timestamp}"

usage() {
  cat <<USAGE
Usage: bash $(basename "$0") [options]

Runs draw_ctau_binned_mass.C for the Run 2 PbPb J/psi RAA bins.
Centrality subranges are run only for the pT-integrated bins; other pT bins use cBin 0-180.

Options:
  --jobs=N              Number of ROOT jobs to run in parallel. Default: ${jobs}
  --logy                Save mass-slice plots with log y-axis.
  --dry-run             Print commands without running ROOT.
  -h, --help            Show this help.
USAGE
}

for arg in "$@"; do
  case "$arg" in
    --jobs=*)
      jobs="${arg#*=}"
      ;;
    --logy)
      use_log_y=true
      ;;
    --dry-run)
      dry_run=1
      ;;
    -h|--help)
      usage
      exit 0
      ;;
    *)
      echo "[ERROR] unknown option: ${arg}" >&2
      usage >&2
      exit 1
      ;;
  esac
done

require_positive_int() {
  local name="$1"
  local value="$2"
  if ! [[ "${value}" =~ ^[0-9]+$ ]] || (( value < 1 )); then
    echo "[ERROR] ${name} must be a positive integer: ${value}" >&2
    exit 1
  fi
}

require_positive_int "--jobs" "${jobs}"

# Format:
#   "yLow yHigh|ptLow ptHigh centLowPct centHighPct;..."
# Differential pT bins use integrated cBin 0-180; pT-integrated bins scan centrality.
bin_groups=(
  #"1.6 2.4|3.5 6.5 0 90;6.5 9.0 0 90;9.0 12.0 0 90;12.0 40.0 0 90;3.5 40.0 0 10;3.5 40.0 10 30;3.5 40.0 30 50;3.5 40.0 50 90;3.5 40.0 0 90"
  #"0.0 1.6|6.5 9.0 0 90;9.0 12.0 0 90;12.0 15.0 0 90;15.0 20.0 0 90;20.0 25.0 0 90;25.0 40.0 0 90;6.5 40.0 0 10;6.5 40.0 10 20;6.5 40.0 20 30;6.5 40.0 30 40;6.5 40.0 40 50;6.5 40.0 50 90;6.5 40.0 0 90"
  "0.0 0.6|6.5 8.5 0 90;8.5 11.0 0 90;11.0 15.0 0 90;15.0 20.0 0 90;20.0 40.0 0 90"
)
format_tag() {
  local value="$1"
  value="${value//./p}"
  value="${value//-/m}"
  echo "${value}"
}

cent_to_cbin() {
  local percent="$1"
  if ! [[ "$percent" =~ ^[0-9]+$ ]]; then
    echo "[ERROR] centrality percent must be an integer: ${percent}" >&2
    exit 1
  fi
  echo $((percent * 2))
}

mkdir -p "${log_dir}"

echo "[INFO] rapidity groups: ${#bin_groups[@]}"
echo "[INFO] parallel jobs: ${jobs}"
echo "[INFO] logs: ${log_dir}"

run_bin() {
  local y_low="$1"
  local y_high="$2"
  local pt_low="$3"
  local pt_high="$4"
  local cent_low_pct="$5"
  local cent_high_pct="$6"
  local c_low c_high label log_file
  c_low="$(cent_to_cbin "${cent_low_pct}")"
  c_high="$(cent_to_cbin "${cent_high_pct}")"
  label="c$(format_tag "${c_low}")_$(format_tag "${c_high}")_y$(format_tag "${y_low}")_$(format_tag "${y_high}")_pt$(format_tag "${pt_low}")_$(format_tag "${pt_high}")"
  log_file="${log_dir}/${label}.log"

  local cmd=(root -l -b -q "draw_ctau_binned_mass.C(${pt_low},${pt_high},${y_low},${y_high},${use_log_y},${c_low},${c_high})")
  printf '%q ' "${cmd[@]}" > "${log_file}.cmd"
  printf '\n' >> "${log_file}.cmd"

  echo "[RUN] ${label} -> ${log_file}"
  if [[ "${dry_run}" -eq 1 ]]; then
    {
      printf '[DRY-RUN] '
      printf '%q ' "${cmd[@]}"
      printf '\n'
    } > "${log_file}"
    return 0
  fi

  if ! "${cmd[@]}" > "${log_file}" 2>&1; then
    return 1
  fi
  if grep -q "ERROR:" "${log_file}"; then
    return 1
  fi
}

active_pids=()
active_labels=()
active_logs=()
failed=0
bins_total=0

wait_for_oldest() {
  local pid="${active_pids[0]}"
  local label="${active_labels[0]}"
  local log_file="${active_logs[0]}"

  if wait "${pid}"; then
    echo "[DONE] ${label}"
  else
    echo "[ERROR] ${label} failed: ${log_file}" >&2
    failed=$((failed + 1))
  fi

  active_pids=("${active_pids[@]:1}")
  active_labels=("${active_labels[@]:1}")
  active_logs=("${active_logs[@]:1}")
}

for group in "${bin_groups[@]}"; do
  IFS='|' read -r y_bin bin_list <<< "${group}"
  read -r y_low y_high <<< "${y_bin}"
  IFS=';' read -r -a bin_specs <<< "${bin_list}"

  for bin_spec in "${bin_specs[@]}"; do
    [[ -z "${bin_spec// }" ]] && continue
    read -r pt_low pt_high cent_low_pct cent_high_pct <<< "${bin_spec}"
    c_low="$(cent_to_cbin "${cent_low_pct}")"
    c_high="$(cent_to_cbin "${cent_high_pct}")"
    label="c$(format_tag "${c_low}")_$(format_tag "${c_high}")_y$(format_tag "${y_low}")_$(format_tag "${y_high}")_pt$(format_tag "${pt_low}")_$(format_tag "${pt_high}")"
    log_file="${log_dir}/${label}.log"

    bins_total=$((bins_total + 1))
    run_bin "${y_low}" "${y_high}" "${pt_low}" "${pt_high}" "${cent_low_pct}" "${cent_high_pct}" &
    active_pids+=("$!")
    active_labels+=("${label}")
    active_logs+=("${log_file}")

    if (( ${#active_pids[@]} >= jobs )); then
      wait_for_oldest
    fi
  done
done

while (( ${#active_pids[@]} > 0 )); do
  wait_for_oldest
done

summary_file="${log_dir}/summary.log"
{
  echo "[INFO] rapidity_groups=${#bin_groups[@]}"
  echo "[INFO] total_bins=${bins_total}"
  echo "[INFO] jobs=${jobs}"
  echo "[INFO] dry_run=${dry_run}"
  echo "[INFO] use_log_y=${use_log_y}"
  echo "[INFO] failed_bins=${failed}"
  echo "[INFO] generated_logs=${log_dir}"
} > "${summary_file}"

echo "[INFO] summary: ${summary_file}"
if (( failed > 0 )); then
  exit 1
fi
