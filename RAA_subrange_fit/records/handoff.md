### 인계

#### 완료한 작업
- 처음 사용하는 사람을 위한 한국어 실행 안내 README.md를 추가했다.
- pp/PbPb 실행 체인, binning, dry-run, 단일·전체 실행, mass-slice 재실행 및 로그 확인 방법을 정리했다.

#### 변경 파일
- README.md
- 00_Project_Index.md
- records/dev_log.md
- records/daily/2026-08-14.md

#### 실행한 명령
```bash
bash pp_jpsi/run_pp_jpsi_chain.sh --help
bash pb_jpsi/run_pp_jpsi_chain.sh --help
```

#### 결과
- 문서 작성 완료. 실제 ROOT fit은 실행하지 않았다.

#### 수행하지 못한 검증
- 새 환경에서 입력 ROOT를 사용한 단일 bin runtime 검증은 수행하지 않았다.

#### 남은 문제
- 입력 ROOT 및 rootlogon.C가 환경별 절대 경로에 의존한다.

#### 권장 다음 작업
- README의 dry-run 후 대표 pp/PbPb bin을 --bin-jobs=1로 실행한다.

### 인계

#### 완료한 작업
- 작업 기록을 `records/` 전용 디렉터리로 재구성했다.
- 기존 `daily/`를 `records/daily/`로, 루트의 `dev_log.md`와 `handoff.md`를 각각 `records/dev_log.md`, `records/handoff.md`로 이동했다.
- 기존 일일 기록·개발 기록·인계 기록을 한국어로 정리하고, `AGENTS.md`와 `CLAUDE.md`에 새 경로 및 한국어 작성 규칙을 반영했다.

#### 변경 파일
- `AGENTS.md`
- `CLAUDE.md`
- `records/dev_log.md`
- `records/daily/2026-07-22.md`
- `records/daily/2026-07-23.md`
- `records/handoff.md`

#### 실행한 명령
```bash
mkdir -p records
mv daily records/daily
mv dev_log.md records/dev_log.md
mv handoff.md records/handoff.md
```

#### 결과
- 세 종류의 작업 기록을 `records/` 아래에 분리해 저장했다.
- 이후 기록은 제목과 서술을 모두 한국어로 작성하도록 지침을 갱신했다.
- AGENTS.md와 CLAUDE.md에 이전 기록 경로 참조가 남아 있지 않음을 확인했다.

#### 수행하지 못한 검증
- 문서 및 경로 변경이므로 ROOT 컴파일 또는 분석 런타임 검증은 수행하지 않았다.

#### 남은 문제
- 없음.

#### 권장 다음 작업
- 다음 분석 작업부터 새 `records/` 경로만 사용한다.

#### Claude/User 참고
- 코드와 분석 산출물은 변경하지 않았다.

### 인계

#### 완료한 작업
- 현재 pp J/ψ subrange 결과, 기존 2D fit, BPH-10-002 및 HIN-16-025를 비교하는 forward b-fraction 그림과 CSV를 생성했다.
- phase-space 차이와 현재 fit 품질 경고를 그림과 분석 기록에 명시했다.

#### 변경 파일
- `pp_jpsi/drawing/b_frac/bFraction_fwd_y1p8_pp_comparison.C`
- `pp_jpsi/drawing/b_frac/bFraction_pp_fwd_y1p8_comparison.pdf`
- `pp_jpsi/drawing/b_frac/bFraction_pp_fwd_y1p8_comparison.png`
- `pp_jpsi/drawing/b_frac/bFraction_pp_fwd_y1p8_comparison_values.csv`
- `00_Project_Index.md`
- `analysis_notes.md`
- `todo.md`
- `notes/B_fraction.md`
- `records/dev_log.md`
- `records/daily/2026-07-23.md`
- `records/handoff.md`

#### 실행한 명령
```bash
/opt/conda/envs/root634/bin/root -l -b -q 'pp_jpsi/drawing/b_frac/bFraction_fwd_y1p8_pp_comparison.C+'
```

#### 결과
- ACLiC 컴파일과 실행이 성공했다.
- 현재 `3–4.5 GeV/c` 결과와 기존 2D fit의 차이는 통계오차 기준 `1.23σ`다.
- 생성된 PNG 렌더링을 육안 확인했다.

#### 수행하지 못한 검증
- 현재 subrange의 나머지 7개 계획 pT bin 결과가 없어 전체 추세 비교를 완료하지 못했다.
- 현재 및 기존 2D fit systematic uncertainty는 포함하지 않았다.

#### 남은 문제
- 현재 점의 ctau χ² p-value가 `3.67×10^-103`이다.
- `pp_jpsi/subrange_mass.C`의 stale `kReferenceDir` 때문에 병렬 mass-slice 출력이 실패한다.
- `run_pp_jpsi_chain.sh` summary가 내부 ROOT 실패를 잡지 못한다.

#### 권장 다음 작업
- 경로와 실패 status 전달을 수정한 뒤 한 bin을 재실행하고, fit 품질이 확보되면 전체 pT bin을 생성한다.

#### Claude/User 참고
- 현재 그림의 BPH는 7 TeV, `1.6 < |y| < 2.4`; HIN은 5.02 TeV, `|y| < 2.4` 자료다. 현재 `1.8 < |y| < 2.4` 결과와 phase space가 다르므로 시각적 reference로만 사용한다.


### 인계

#### 완료한 작업
- `bFraction_fwd_y1p8_pp_comparison.C`에서 `current.empty()` guard를 제거하고 전체 current subrange 8개 bin으로 비교 그림을 다시 생성했다.

#### 변경 파일
- `pp_jpsi/drawing/b_frac/bFraction_fwd_y1p8_pp_comparison.C`
- `pp_jpsi/drawing/b_frac/bFraction_pp_fwd_y1p8_comparison.pdf`
- `pp_jpsi/drawing/b_frac/bFraction_pp_fwd_y1p8_comparison.png`
- `pp_jpsi/drawing/b_frac/bFraction_pp_fwd_y1p8_comparison_values.csv`

#### 결과
- ROOT ACLiC 컴파일을 통과했다.

#### 남은 문제
- 일부 final ctau fit의 RooFit status가 0이 아니므로 convergence 검증이 필요하다.

### 인계

#### 완료한 작업
- `run_pp_jpsi_chain.sh`의 forward rapidity (1.8 < |y| < 2.4) pT binning을 legacy 2D Fit binning과 동일하게 변경했다.

#### 변경 사항d files
- `pp_jpsi/run_pp_jpsi_chain.sh`
- `records/dev_log.md`, `records/daily/2026-07-23.md`, `records/handoff.md`

#### 실행한 명령
```bash
bash -n pp_jpsi/run_pp_jpsi_chain.sh
```

#### 결과
- shell syntax 검증 통과. 새 binning은 10개 differential bin과 2개 integrated bin으로 구성된다.

#### 수행하지 못한 검증
- 실제 ROOT fit 체인 실행은 수행하지 않았다.

#### 남은 문제
- 새 binning으로 생성한 subrange 결과를 b-fraction 비교 플롯에 반영해야 한다.

#### 권장 다음 작업
- `pp_jpsi/run_pp_jpsi_chain.sh`를 실행한 뒤 산출 ROOT 파일과 comparison plot을 갱신한다.

#### Claude/User 참고
- 현재 설정은 2D Fit forward-rapidity bin-edge와 일치한다.

### 인계

#### 완료한 작업
- forward b-fraction comparison plot을 current subrange와 legacy 2D Fit의 동일한 10개 pT bin으로 갱신했다.

#### 변경 파일
- `pp_jpsi/drawing/b_frac/bFraction_fwd_y1p8_pp_comparison.C`
- `pp_jpsi/drawing/b_frac/bFraction_pp_fwd_y1p8_comparison.pdf`
- `pp_jpsi/drawing/b_frac/bFraction_pp_fwd_y1p8_comparison.png`
- `pp_jpsi/drawing/b_frac/bFraction_pp_fwd_y1p8_comparison_values.csv`
- `records/daily/2026-07-24.md`, `records/dev_log.md`, `records/handoff.md`

#### 실행한 명령
```bash
/opt/conda/envs/root634/bin/root -l -b -q 'pp_jpsi/drawing/b_frac/bFraction_fwd_y1p8_pp_comparison.C+'
```

#### 결과
- current 및 legacy 2D Fit point가 각각 10개로 생성되었고 PNG를 육안 확인했다.

#### 수행하지 못한 검증
- fit 결과 자체의 quality 재검증은 수행하지 않았다.

#### 남은 문제
- HIN systematic box 색상 설정에서 non-fatal ROOT color index 913 경고가 남아 있다.

#### 권장 다음 작업
- 필요 시 HIN systematic box 색상만 유효한 ROOT color index로 정리한다.

### 인계

#### 완료한 작업
- forward b-fraction 비교 그림에 `subrange / legacy 2D Fit` ratio pad와 unity line을 추가했다.

#### 변경 파일
- `pp_jpsi/drawing/b_frac/bFraction_fwd_y1p8_pp_comparison.C`
- `pp_jpsi/drawing/b_frac/bFraction_pp_fwd_y1p8_comparison.pdf`
- `pp_jpsi/drawing/b_frac/bFraction_pp_fwd_y1p8_comparison.png`
- `analysis_notes.md`, `todo.md`, `notes/B_fraction.md`
- `records/dev_log.md`, `records/daily/2026-07-24.md`, `records/handoff.md`

#### 결과
- current, legacy 및 ratio가 각각 10개 point로 생성됐다.
- ratio는 `0.852–0.950`이며 모든 bin에서 1보다 작다.

#### 수행하지 못한 검증
- 두 fit 결과 사이의 통계적 상관관계와 systematic uncertainty는 ratio error에 포함하지 않았다.

#### 권장 다음 작업
- subrange 결과의 일관된 하향 차이를 signal-yield extraction 및 ctau model 차이 관점에서 점검한다.


### 인계

#### 완료한 작업
- 현재 pp J/ψ 코드를 2026-06-26 원본 사본과 비교해 실제 변경분을 식별했다.
- PbPb 전용 설정을 유지하면서 공통 `NumCPU(nCPU)`, mass log-y 자동 범위, header include와 subrange 기준 경로 변경을 6개 PbPb 매크로에 이식했다.

#### 변경 파일
- `pb_jpsi/mass.C`
- `pb_jpsi/mc_mass.C`
- `pb_jpsi/ctau_pr.C`
- `pb_jpsi/ctau_np.C`
- `pb_jpsi/subrange_ctau.C`
- `pb_jpsi/subrange_mass.C`
- `records/dev_log.md`
- `records/daily/2026-07-24.md`
- `todo.md`
- `records/handoff.md`

#### 실행한 명령
```bash
/opt/conda/envs/root634/bin/root -l -b -q -e "gROOT->ProcessLine(\".L /data/hwan/RAA_subrange_fit/pb_jpsi/<macro>.C\");"
/opt/conda/envs/root634/bin/root -l -b -q -e "gSystem->mkdir(\"/tmp/raa_pb_compile_<macro>\", true); gSystem->SetBuildDir(\"/tmp/raa_pb_compile_<macro>\", true); gROOT->ProcessLine(\".L /data/hwan/RAA_subrange_fit/pb_jpsi/<macro>.C+\");"
```

#### 결과
- 수정한 6개 매크로의 ROOT interpreter load와 `/tmp` 분리 ACLiC 컴파일이 모두 exit code 0으로 끝났다.
- PbPb의 `cBin`, 입력 파일명, binning과 기존 fit strategy는 변경하지 않았다.

#### 수행하지 못한 검증
- 현재 코드가 참조하는 PbPb data/MC ROOT 입력 파일이 이 환경에서 확인되지 않아 실제 fit 실행과 출력 그림 확인은 수행하지 않았다.

#### 남은 문제
- PbPb 입력 ROOT 파일의 유효 경로를 확인해야 한다.
- 대표 bin에서 log-y 렌더링과 병렬 RooFit 실행 결과를 확인해야 한다.

#### 권장 다음 작업
- 입력 경로를 확정한 뒤 PbPb 대표 bin 하나로 전체 체인을 실행한다.

#### Claude/User 참고
- pp 전용 `1.8 < |y| < 2.4` binning과 `/data/Oniatree` pp 입력 경로는 PbPb에 이식하지 않았다.
- 최상위 디렉터리에 `.git`이 없어 원본 사본과의 `diff -u`로 변경 범위를 확인했다.


### 인계

#### 완료한 작업
- `pb_jpsi/run_pp_jpsi_chain.sh`의 active rapidity/pT binning을 pp와 동일한 `1.8 < |y| < 2.4`, 10개 differential + 2개 broad pT bin으로 변경했다.
- PbPb centrality는 모든 active bin에서 `0–90%`로 설정했다.
- helper, cookbook와 help 예시를 새 binning에 맞췄다.

#### 변경 파일
- `pb_jpsi/run_pp_jpsi_chain.sh`
- `decisions/Binning_decisions.md`
- `00_Project_Index.md`
- `analysis_notes.md`
- `notes/B_fraction.md`
- `records/dev_log.md`
- `records/daily/2026-07-24.md`
- `todo.md`
- `records/handoff.md`

#### 실행한 명령
```bash
bash -n pb_jpsi/run_pp_jpsi_chain.sh
cp /data/hwan/RAA_subrange_fit/pb_jpsi/run_pp_jpsi_chain.sh /tmp/raa_pb_binning_check.EucRgN/run_pp_jpsi_chain.sh
bash run_pp_jpsi_chain.sh --dry-run --only=mass --bin-jobs=1
```

#### 결과
- shell syntax 검증 통과
- 임시 디렉터리 전체 dry-run에서 `total_bins=12`, `failed_bins=0`, `killed_jobs=0`
- 모든 dry-run 호출이 `1.8 < |y| < 2.4`, `cBin 0–180` 및 pp와 동일한 pT edge를 사용했다.

#### 수행하지 못한 검증
- PbPb 입력 ROOT 파일 경로가 확인되지 않아 실제 fit은 실행하지 않았다.

#### 남은 문제
- 새 binning의 실제 mass/ctau fit 품질과 산출물을 확인해야 한다.
- centrality-dependent 결과가 필요하면 현재 active list와 분리된 bin configuration이 필요하다.

#### 권장 다음 작업
- `3.0–4.5 GeV/c`, `1.8 < |y| < 2.4`, centrality `0–90%` 대표 bin으로 actual run을 수행한다.

#### Claude/User 참고
- 기존 midrapidity, `1.6 < |y| < 1.8` 및 centrality-scan entries는 active production list에서 제거됐다.
- 변경 근거와 영향은 [[decisions/Binning_decisions]]에 기록했다.


### 인계

#### 완료한 작업
- Legacy J/ψ 2D Fit nominal driver와 같은 centrality bin `0–10%`, `10–30%`, `30–50%`, `50–90%`를 PbPb active list에 추가했다.
- 네 bin은 `3–40 GeV/c`, 현재 rapidity `1.8 < |y| < 2.4`에 적용된다.
- pp-matched `0–90%` 12개 bin은 그대로 유지했다.

#### 변경 파일
- `pb_jpsi/run_pp_jpsi_chain.sh`
- `decisions/Binning_decisions.md`
- `analysis_notes.md`
- `notes/B_fraction.md`
- `records/dev_log.md`
- `records/daily/2026-07-24.md`
- `todo.md`
- `records/handoff.md`

#### 실행한 명령
```bash
bash -n pb_jpsi/run_pp_jpsi_chain.sh
cp /data/hwan/RAA_subrange_fit/pb_jpsi/run_pp_jpsi_chain.sh /tmp/raa_pb_legacy_cent_check.8YATV6/run_pp_jpsi_chain.sh
bash run_pp_jpsi_chain.sh --dry-run --only=mass --bin-jobs=16
```

#### 결과
- 전체 dry-run `total_bins=16`, `failed_bins=0`, `killed_jobs=0`
- 추가된 호출은 `cBin 0–20`, `20–60`, `60–100`, `100–180`을 정확히 사용했다.
- `--help`의 새 centrality target 예시 렌더링을 확인했다.

#### 수행하지 못한 검증
- PbPb 입력 ROOT 경로가 확인되지 않아 actual fit은 실행하지 않았다.

#### 남은 문제
- 현재 rapidity는 `1.8–2.4`이며 Legacy 원본은 `1.6–2.4`이므로 비교 phase-space가 완전히 같지 않다.
- 새 centrality bin의 실제 mass/ctau fit 품질을 확인해야 한다.

#### 권장 다음 작업
- `--bin=3.0,40.0,1.8,2.4,0,10` 대표 bin으로 actual run을 수행한다.

#### Claude/User 참고
- Legacy 근거는 `/data/hwan/psi2S_RAA_PbPb2018/Macros/Jpsi/06final.sh:26-28`이다.
- current active list는 pp-matched 12개와 Legacy centrality 4개, 총 16개다.


### 인계

#### 완료한 작업
- Legacy centrality 4개를 제외한 pp-matched PbPb pT bin 12개 전체 chain fit 실행.

#### 변경 사항d files
- 코드 변경 없음. 실행 로그는 pb_jpsi/logs/run_pb_jpsi_chain_20260724_* 및 /tmp/pb_jpsi_pt_*.log.

#### 결과
- 12/12 bin rc=0; 오류/kill marker 없음.
- 10개 differential pT와 2개 pT-integrated bin 모두 완료.

#### 수행하지 못한 검증
- ROOT 산출물 내부 fit status와 물리량 정량 sanity check 미수행.

#### 권장 다음 작업
- 대표 bin에서 fit_result와 signal yield를 확인한다.

### 인계

#### 완료한 작업
- PbPb J/ψ `1.8 < |y| < 2.4`, centrality `0–90%`의 current subrange,
  legacy 2D Fit 및 HIN-16-025 reference b-fraction 비교 플롯을 생성했다.
- current/legacy 공통 10개 pT bin과 ratio pad를 포함했다.
- current ROOT 파일의 fit status, covariance quality와 p-value를 CSV에 기록했다.

#### 변경 파일
- `pb_jpsi/drawing/b_frac/bFraction_fwd_y1p8_PbPb_comparison.C`
- `pb_jpsi/drawing/b_frac/bFraction_PbPb_fwd_y1p8_comparison.pdf`
- `pb_jpsi/drawing/b_frac/bFraction_PbPb_fwd_y1p8_comparison.png`
- `pb_jpsi/drawing/b_frac/bFraction_PbPb_fwd_y1p8_comparison_values.csv`
- `00_Project_Index.md`
- `analysis_notes.md`
- `todo.md`
- `notes/B_fraction.md`
- `records/dev_log.md`
- `records/daily/2026-07-25.md`
- `records/handoff.md`

#### 실행한 명령
```bash
/opt/conda/envs/root634/bin/root -l -b -q 'pb_jpsi/drawing/b_frac/bFraction_fwd_y1p8_PbPb_comparison.C+'
pdfinfo pb_jpsi/drawing/b_frac/bFraction_PbPb_fwd_y1p8_comparison.pdf
```

#### 결과
- ACLiC 컴파일 및 plot 생성 성공
- current/legacy/ratio 각각 10개 point
- ratio `0.9157–1.2805`, 진단용 가중 평균 `1.0681 ± 0.0141`
- current fit status는 모두 0이다.
- 첫 pT bin covariance quality 2, 나머지는 3이다.
- `20–40 GeV/c` ctau p-value는 `0.0104`이다.
- PDF 1 page 및 PNG 렌더링을 확인했다.

#### 수행하지 못한 검증
- 최고 pT ctau pull의 직접 검토
- 두 fit 사이 통계 상관관계 및 systematic uncertainty
- HIN reference 원 출처의 uncertainty 구성과 centrality 표기 재검증

#### 남은 문제
- `20–40 GeV/c` p-value가 0.05보다 작다.
- `3–4.5 GeV/c` covariance quality가 2다.

#### 권장 다음 작업
- 두 bin의 fit diagnostics를 우선 검토한 뒤 comparison을 baseline으로 채택한다.

#### Claude/User 참고
- HIN point는 local legacy macro의 숫자를 그대로 사용했으며 직접 ratio에는
  사용하지 않았다.
- 최상위 디렉터리에 `.git`이 없어 Git 상태 점검은 불가능했다.
