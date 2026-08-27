# Binning 결정 기록

## 결정

PbPb J/ψ production 체인의 active rapidity/pT binning을 pp J/ψ 체인과 동일하게 맞춘다.

- rapidity: `1.8 < |y| < 2.4`
- differential pT: `3–4.5`, `4.5–5.5`, `5.5–6.5`, `6.5–7.5`, `7.5–8.5`, `8.5–9.5`, `9.5–11`, `11–15`, `15–20`, `20–40 GeV/c`
- broad pT: `3–6.5`, `6.5–40 GeV/c`
- PbPb centrality: pp-matched 12개 bin은 `0–90%` (`cBin 0–180`)
- Legacy 2D Fit centrality 추가: `3 < pT < 40 GeV/c`에서 `0–10%`, `10–30%`, `30–50%`, `50–90%` (`cBin 0–20`, `20–60`, `60–100`, `100–180`)

## 배경

기존 PbPb active list는 `|y| < 1.6` 및 `1.6 < |y| < 2.4`와 서로 다른 pT·centrality 구성을 사용했다. pp와 PbPb 결과를 동일한 phase-space에서 비교하려면 rapidity와 pT edge가 일치해야 한다.

## 검토한 대안

1. 기존 PbPb binning을 유지하고 결과 비교 시 재구간화한다.
2. pp와 동일한 rapidity/pT bin을 사용하되 broad pT bin마다 기존 centrality scan을 유지한다.
3. pp와 동일한 12개 rapidity/pT bin을 사용하고 PbPb centrality를 모두 `0–90%`로 통합한다.
4. 3번 구성에 Legacy 2D Fit의 `3–40 GeV/c` centrality 4개 bin을 추가한다.

## 선택 근거

pp와 동일한 rapidity/pT bin을 유지하면서 사용자가 후속 요청한 Legacy 2D Fit centrality bin도 재현하기 위해 4번을 선택했다. Legacy nominal driver `Macros/Jpsi/06final.sh`의 active `cBin` edge를 그대로 percent 단위로 변환했다.

## 결과에 미치는 영향

- pp/PbPb의 rapidity 및 pT bin edge가 일치해 bin 재조합 없이 직접 비교할 수 있다.
- 기존 midrapidity 및 `1.6 < |y| < 1.8` 영역은 active production list에서 제외된다.
- Legacy `3–40 GeV/c` centrality scan 4개 bin이 active list에 추가된다.
- 기존 PbPb 산출물은 새 rapidity 범위의 결과를 대신하지 않으므로 pp-matched 12개와 Legacy centrality 4개, 총 16개 bin을 실행해야 한다.

## 검증 상태

- `bash -n pb_jpsi/run_pp_jpsi_chain.sh`: 통과
- 임시 디렉터리 전체 mass-only dry-run: `total_bins=16`, `failed_bins=0`, `killed_jobs=0`
- pp-matched 12개 호출은 `cBin 0–180`, Legacy centrality 4개 호출은 `cBin 0–20`, `20–60`, `60–100`, `100–180`을 사용함을 확인
- 실제 ROOT fit 및 산출물 검증: 미수행. 현재 PbPb 입력 ROOT 경로 확인이 필요함

## 관련 노트

- [[notes/B_fraction]]
- [[analysis_notes]]
- [[records/daily/2026-07-24]]
- [[records/handoff]]


## 변경 이력

### 2026-07-24 10:15

- Legacy nominal `06final.sh`의 `3–40 GeV/c` centrality 4개 bin을 active list에 추가했다.
- 앞선 `0–90%` 단일 centrality 구성은 pp-matched 12개 bin에만 유지한다.
