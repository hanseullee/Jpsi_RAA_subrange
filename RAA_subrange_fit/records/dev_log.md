# 개발 기록

### 날짜
2026-08-14 00:00

### 작업 맥락
처음 사용하는 사람이 pp/PbPb J/ψ subrange-fit 체인을 재현할 수 있도록 실행 절차를 정리했다.

### 변경 사항
- 루트 `README.md`를 추가했다.
- ROOT 및 입력 파일 확인, dry-run, 대표 bin·전체 bin 실행, 단계별 실행, mass-slice 재실행, 로그 점검 방법을 기록했다.

### 결과
실행 안내 문서 작성과 링크 추가를 완료했다. 실제 fit은 실행하지 않았다.

### 다음 작업
1. 새 환경에서 대표 bin dry-run과 단일 bin 실행을 재확인한다.
2. 입력 ROOT 경로가 바뀌면 README와 관련 매크로를 함께 갱신한다.

# 개발 기록

### 날짜
2026-07-22 17:24

### 작업 맥락
`pp_jpsi/mass.C`의 mass-fit 그림 위쪽 패널을 로그 y축으로 표시할 때 패널 전체가 비어 보이는 문제를 점검했다.

### 변경 사항
`logYFlag`를 `pad1->SetLogy(logYFlag)`에 연결하고, `massplot` y축의 하한을 로그 모드에서 `max(1e-4, yMax*1e-4)`로 설정했다.

### 변경 이유
ROOT의 로그 축은 0을 하한으로 사용할 수 없다. 기존 `SetRangeUser(0.0, ...)` 설정이 log-y 렌더링을 무효화했다.

### 결과
`/opt/conda/envs/root634/bin/root -l -b -q -e 'gROOT->ProcessLine(".L /data/hwan/RAA_subrange_fit/pp_jpsi/mass.C+");'`로 ACLiC 컴파일을 통과했다. 실제 데이터 입력을 사용한 그림 생성은 이번 작업에서 실행하지 않았다.

### 다음 작업
1. 동일한 매크로 실행으로 출력 PNG/PDF에서 log-y 패널을 육안 확인한다.

### 날짜
2026-07-23 10:15

### 작업 맥락
`pp_jpsi/mass.C`의 상단 mass-fit 패널은 log-y를 사용한다. 660번 줄 부근의 기존 Y축 범위 코드는 이 매크로에 정의되지 않은 `ws`, `dsAB`, `myPlot_A` 객체를 참조하면서, 계산한 `Ydown`/`Yup`도 실제 축에 적용하지 않고 있었다.

### 변경 사항
표시 중인 `RooHist`의 양의 데이터 포인트 최소값과 error bar를 포함한 최대값을 직접 추출하도록 바꾸었다. log-y에서는 이 비율로 하단 10%, 상단 40%의 로그 공간 여백을 적용한 `yDown`/`yUp`을 `SetRangeUser`에 전달하고, linear 모드에서는 기존의 `0.0`~`1.65*yMax` 범위를 유지했다.

### 변경 이유
현재 매크로의 선언 및 플롯 객체에 맞는 Y축 범위를 사용하면서 로그 축의 양수 하한을 보장해야 한다.

### 결과
다음 명령으로 ACLiC 컴파일을 통과했다. 실제 입력 데이터로 출력 그림을 다시 생성해 여백을 육안 확인하는 작업은 수행하지 않았다.

```bash
/opt/conda/envs/root634/bin/root -l -b -q -e "gROOT->ProcessLine(\".L /data/hwan/RAA_subrange_fit/pp_jpsi/mass.C+\");"
```

### 다음 작업
1. 통상적인 `mass.C` 실행으로 해당 pT/rapidity bin의 PNG/PDF를 생성한다.
2. log-y 상단 여백과 최소 양의 bin 부근의 표시가 의도한 비율인지 육안 확인한다.

### 날짜
2026-07-23 10:27

### 작업 맥락
작업 기록을 루트와 `daily/`에 분산해 두면 분석 코드와 기록 파일의 구분이 불명확하고, 다음 작업자가 최신 기록 위치를 일관되게 찾기 어렵다.

### 변경 사항
기존 `daily/`를 `records/daily/`로, 루트의 `dev_log.md`와 `handoff.md`를 각각 `records/dev_log.md`, `records/handoff.md`로 이동했다. 기존 기록의 제목과 서술, AGENTS.md 및 CLAUDE.md의 기록 경로·양식을 한국어와 새 경로 기준으로 갱신했다.

### 변경 이유
일일 기록, 개발 기록, 인계 기록을 전용 디렉터리에 모으고 세 문서의 향후 작성 언어를 한국어로 통일하기 위해서다.

### 결과
기존 `daily/`, `dev_log.md`, `handoff.md` 경로를 참조하는 Markdown 문서가 남아 있지 않음을 `rg`로 확인했다. 코드와 분석 산출물은 변경하지 않았다.

### 다음 작업
1. 이후 작업부터 `records/dev_log.md`, `records/daily/YYYY-MM-DD.md`, `records/handoff.md`에 한국어로 기록한다.

### 날짜
2026-07-23 13:57

### 작업 맥락
현재 저장소의 pp J/ψ subrange b-fraction과 `/data/hwan/psi2S_RAA_PbPb2018/`의 기존 2D fit, BPH-10-002, HIN-16-025 값을 `1.8 < |y| < 2.4` 관점에서 한 그림에 비교했다.

### 변경 사항
`pp_jpsi/drawing/b_frac/bFraction_fwd_y1p8_pp_comparison.C`를 추가했다. 매크로는 현재 subrange ROOT의 `TParameter<double>` `bFraction`/`bFractionErr`, 기존 2D fit ROOT의 `TH1D` `2DfitResults`, 코드에 출처와 함께 고정한 두 HEPData 표를 읽어 PDF, PNG 및 CSV를 저장한다.

### 변경 이유
서로 다른 분석 방법과 published reference의 pT 의존성을 같은 축에서 재현 가능하게 비교하고, phase-space 차이와 systematic 포함 여부를 명시하기 위해서다.

### 결과
다음 명령으로 ACLiC 컴파일과 실행을 완료했다.

```bash
/opt/conda/envs/root634/bin/root -l -b -q 'pp_jpsi/drawing/b_frac/bFraction_fwd_y1p8_pp_comparison.C+'
```

생성 파일:

- `pp_jpsi/drawing/b_frac/bFraction_pp_fwd_y1p8_comparison.pdf`
- `pp_jpsi/drawing/b_frac/bFraction_pp_fwd_y1p8_comparison.png`
- `pp_jpsi/drawing/b_frac/bFraction_pp_fwd_y1p8_comparison_values.csv`

PNG는 896×752 RGB로 생성됐고 렌더링을 육안 확인했다. 현재 subrange 결과는 `3–4.5 GeV/c` 한 점만 읽혔다. 나머지 bin은 `subrange_mass.C`의 `kReferenceDir`가 `/data/users/pjgwak/work/daily_code_tracker/2026/06/26_run2_Raa_subrange_fit/pp_jpsi`를 가리켜 병렬 slice log를 만들지 못했다. 체인 summary가 이 실패에도 `failed_bins=0`을 기록하는 문제도 확인했다.

### 다음 작업
1. `kReferenceDir`를 현재 macro 또는 작업 디렉터리에서 결정하도록 수정한다.
2. ROOT macro 실패가 shell exit status에 반영되도록 체인을 수정한다.
3. 누락 bin을 재실행하고 비교 그림을 갱신한다.


### 날짜
2026-07-23 14:40

### 작업 맥락
1.8 < |y| < 2.4의 모든 current subrange 결과가 생성된 뒤, 비교 매크로가 결과가 비어 있을 때만 current graph와 범례를 숨기던 조건을 제거했다.

### 변경 사항
`bFraction_fwd_y1p8_pp_comparison.C`에서 `current.empty()` guard 네 곳을 삭제했다. current graph, 범례, 첫 점 p-value 표기 및 stdout 요약은 이제 항상 실행된다.

### 결과
ROOT ACLiC 컴파일과 PDF/PNG/CSV 재생성을 완료했다. current subrange 8개 differential bin을 읽었다.

### 다음 작업
1. final ctau fit status가 0이 아닌 bin을 별도 재fit 또는 minimizer 검증한다.

### 날짜
2026-07-23 14:46

### 작업 맥락
pp J/psi subrange-fit 체인의 forward rapidity (1.8 < |y| < 2.4) pT 구간을 기존 2D Fit 결과와 직접 비교할 수 있도록 정렬했다.

### 변경 사항
`pp_jpsi/run_pp_jpsi_chain.sh`의 `bin_groups`에서 forward rapidity pT bin을 `3.0-4.5, 4.5-5.5, 5.5-6.5, 6.5-7.5, 7.5-8.5, 8.5-9.5, 9.5-11.0, 11.0-15.0, 15.0-20.0, 20.0-40.0 GeV/c`로 변경하고, integrated `3.0-6.5`, `6.5-40.0 GeV/c`도 유지했다.

### 변경 이유
legacy pp J/psi 2D Fit의 1.8 < |y| < 2.4 binning과 동일하게 맞춰 b fraction 비교가 bin-edge 차이 없이 가능하도록 했다.

### 결과
`bash -n pp_jpsi/run_pp_jpsi_chain.sh`를 통과했다. 물리 fit 실행 및 새 ROOT 산출물 생성은 수행하지 않았다.

### 다음 작업
1. 수정된 체인으로 forward rapidity subrange fit을 실행한다.
2. 생성 결과를 2D Fit b fraction 비교 플롯에 반영한다.

### 날짜
2026-07-24 09:35

### 작업 맥락
forward rapidity pp J/psi의 current subrange b fraction을 legacy 2D Fit과 동일한 pT bin에서 비교하도록 그림을 갱신했다.

### 변경 사항
`pp_jpsi/drawing/b_frac/bFraction_fwd_y1p8_pp_comparison.C`의 current edge를 `3.0-4.5, 4.5-5.5, 5.5-6.5, 6.5-7.5, 7.5-8.5, 8.5-9.5, 9.5-11.0, 11.0-15.0, 15.0-20.0, 20.0-40.0 GeV/c`로 변경하고 PDF/PNG/CSV를 다시 생성했다.

### 결과
ROOT ACLiC 컴파일 및 실행이 성공했고, current 및 legacy 2D Fit 각각 10개 point를 읽었다. PNG를 육안 확인했다. `TColor::GetColorTransparent` color index 913 경고는 남아 있으나 파일 생성은 완료됐다.

### 다음 작업
1. 필요 시 HIN systematic box의 color index 경고를 정리한다.

### 날짜
2026-07-24 09:42

### 작업 맥락
동일한 pT bin의 current subrange와 legacy 2D Fit을 정량 비교할 수 있도록 ratio pad를 추가했다.

### 변경 사항
`bFraction_fwd_y1p8_pp_comparison.C`에 bin-edge 매칭, `subrange / legacy 2D Fit` 계산, 독립 통계오차 전파, two-pad canvas 및 unity line을 구현했다.

### 결과
ROOT ACLiC 컴파일과 실행이 성공했다. current, legacy, ratio가 각각 10개 point이며 PDF/PNG를 재생성하고 PNG를 육안 확인했다.

### 다음 작업
1. fit 간 통계적 상관관계를 ratio uncertainty에 반영할지 검토한다.


### 날짜
2026-07-24 09:56

### 작업 맥락
현재 `pp_jpsi` 코드를 2026-06-26 원본 사본과 비교해 이후의 실제 수정분을 추출하고, PbPb 전용 입력·centrality·binning·fit model은 유지한 채 공통 구현 변경을 `pb_jpsi`에 이식했다. 최상위 경로에는 `.git`이 없어 `git status`와 `git diff`는 사용할 수 없었으며, 원본 사본과의 `diff -u`로 변경 범위를 검증했다.

### 변경 사항
- `pb_jpsi/mass.C`: 공통 header를 추가해 `nCPU`를 사용하고, 최초 fit과 retry에 `NumCPU(nCPU)`를 적용했다. 상단 mass plot을 log-y로 전환하고 양의 데이터 최소값과 error를 포함한 최대값으로 표시 범위를 자동 계산하도록 했다.
- `pb_jpsi/mc_mass.C`: mass plot을 log-y로 전환하고 최초 fit에 `NumCPU(nCPU)`를 적용했다.
- `pb_jpsi/ctau_pr.C`, `pb_jpsi/ctau_np.C`: 공통 header를 추가하고 최초 fit과 retry에 `NumCPU(nCPU)`를 적용했다.
- `pb_jpsi/subrange_mass.C`, `pb_jpsi/subrange_ctau.C`: `kReferenceDir`를 `/data/hwan/RAA_subrange_fit/pb_jpsi`로 갱신했다.
- pp 전용 `/data/Oniatree` 입력 경로, `1.8 < |y| < 2.4` binning 및 pp 모델 분기는 PbPb에 적용하지 않았다. PbPb의 `cBin`, 기존 입력 파일명과 fit strategy를 유지했다.
- 표준 패치 도구는 `bwrap: loopback: Failed RTM_NEWADDR: Operation not permitted`로 실행되지 않아, 확정된 exact-string 치환으로 변경을 적용했다.

### 변경 이유
pp에서 검증한 병렬 RooFit 설정, log-y 표시 및 현재 작업 경로 수정을 PbPb 체인에도 일관되게 반영하되 충돌계별 분석 정의를 보존하기 위해서다.

### 결과
다음 형식의 ROOT interpreter load를 6개 매크로에 각각 실행했고 모두 exit code 0을 확인했다.

```bash
/opt/conda/envs/root634/bin/root -l -b -q -e "gROOT->ProcessLine(\".L /data/hwan/RAA_subrange_fit/pb_jpsi/<macro>.C\");"
/opt/conda/envs/root634/bin/root -l -b -q -e "gSystem->mkdir(\"/tmp/raa_pb_compile_<macro>\", true); gSystem->SetBuildDir(\"/tmp/raa_pb_compile_<macro>\", true); gROOT->ProcessLine(\".L /data/hwan/RAA_subrange_fit/pb_jpsi/<macro>.C+\");"
```

검증 대상은 `mass.C`, `mc_mass.C`, `ctau_pr.C`, `ctau_np.C`, `subrange_ctau.C`, `subrange_mass.C`이다. 이어서 각 매크로의 ACLiC build directory를 `/tmp/raa_pb_compile_<macro>`로 분리해 컴파일했고, 6개 모두 shared library 생성과 exit code 0을 확인했다. 저장소 안의 기존 컴파일 산출물은 변경하지 않았다. 현재 PbPb 코드가 가리키는 data/MC ROOT 입력 파일은 이 환경에서 확인되지 않아 실제 fit 실행과 출력 그림 검증은 수행하지 않았다.

### 다음 작업
1. PbPb 입력 ROOT 파일 경로를 확인한다.
2. 대표 centrality/rapidity/pT bin 하나로 전체 체인을 실행해 fit status, 출력 경로와 log-y 렌더링을 검증한다.


### 날짜
2026-07-24 10:06

### 작업 맥락
사용자 요청에 따라 `pb_jpsi/run_pp_jpsi_chain.sh`의 active rapidity/pT binning을 `pp_jpsi/run_pp_jpsi_chain.sh`와 동일하게 변경했다. PbPb 체인의 필수 centrality 필드는 pp-matched 각 bin에 `0–90%`로 설정했다.

### 변경 사항
- `bin_groups`를 `1.8 < |y| < 2.4`의 pp와 동일한 10개 differential pT bin 및 2개 broad pT bin으로 교체했다.
- 기존 `|y| < 1.6`, `1.6 < |y| < 2.4` bin과 centrality-scan entries를 active list에서 제거했다.
- `is_pt_integrated_bin()`을 새 broad bin `3–6.5`, `6.5–40 GeV/c`에 맞게 갱신했다.
- command cookbook와 `--help` 예시를 새 대표 bin `3.0,4.5,1.8,2.4`로 갱신했다.
- 편집 중 exact-string 치환에서 shell positional parameter가 빈 문자열로 보간된 문제를 diff 검토에서 발견했고, 실행 전에 `$1`–`$4` 참조를 복구했다.

### 변경 이유
pp/PbPb bin edge를 일치시켜 직접 비교 가능한 산출물을 만들고, 스크립트 도움말과 target-bin 판정도 새 production list와 일관되게 유지하기 위해서다.

### 결과
다음 검증을 수행했다.

```bash
bash -n pb_jpsi/run_pp_jpsi_chain.sh
cp /data/hwan/RAA_subrange_fit/pb_jpsi/run_pp_jpsi_chain.sh /tmp/raa_pb_binning_check.EucRgN/run_pp_jpsi_chain.sh
bash run_pp_jpsi_chain.sh --dry-run --only=mass --bin-jobs=1
```

- shell syntax 검증 통과
- 저장소 밖 임시 디렉터리의 전체 dry-run에서 `total_bins=12`, `failed_bins=0`, `killed_jobs=0`
- 12개 mass 호출 모두 `y=1.8–2.4`, `cBin=0–180`과 pp 동일 pT edge를 사용함을 확인
- 실제 ROOT fit은 수행하지 않았다.

### 다음 작업
1. PbPb 입력 ROOT 경로를 확인한다.
2. 대표 bin에서 실제 체인을 실행해 fit status와 출력 파일을 검증한다.


### 날짜
2026-07-24 10:15

### 작업 맥락
현재 pp-matched PbPb production list에 Legacy J/ψ 2D Fit과 같은 centrality bin을 추가했다. 기준 파일은 `/data/hwan/psi2S_RAA_PbPb2018/Macros/Jpsi/06final.sh`이며 active loop의 `Final2DFit.C(3.,40,1.6,2.4,cLow,cHigh)` 호출을 사용했다.

### 변경 사항
- `pb_jpsi/run_pp_jpsi_chain.sh`의 `bin_groups`에 `3–40 GeV/c`, `0–10%`, `10–30%`, `30–50%`, `50–90%` 네 항목을 추가했다.
- percent centrality는 script에서 각각 `cBin 0–20`, `20–60`, `60–100`, `100–180`으로 변환된다.
- `is_pt_integrated_bin()`이 `3–40 GeV/c` centrality subrange를 허용하도록 갱신했다.
- cookbook와 `--help`에 `--bin=3.0,40.0,1.8,2.4,0,10` 예시를 추가했다.
- help 치환 과정에서 `$0`이 Perl 실행 인자 `-e`로 보간된 문제를 diff에서 발견해 실제 검증 전에 `$(basename "$0")`로 복구했다.

### 변경 이유
Legacy 2D Fit과 centrality-dependent 결과를 동일 edge에서 비교하기 위해서다. 현재 분석 rapidity는 pp와 맞춘 `1.8–2.4`를 유지하므로 Legacy 원본 `1.6–2.4`와 rapidity 하한은 다르다.

### 결과
```bash
bash -n pb_jpsi/run_pp_jpsi_chain.sh
cp /data/hwan/RAA_subrange_fit/pb_jpsi/run_pp_jpsi_chain.sh /tmp/raa_pb_legacy_cent_check.8YATV6/run_pp_jpsi_chain.sh
bash run_pp_jpsi_chain.sh --dry-run --only=mass --bin-jobs=16
```

- shell syntax와 `--help` 렌더링 통과
- 전체 dry-run: `total_bins=16`, `failed_bins=0`, `killed_jobs=0`
- Legacy centrality 호출 확인: `mass(3.0,40.0,1.8,2.4,...,0,20)`, `(...,20,60)`, `(...,60,100)`, `(...,100,180)`
- 실제 ROOT fit은 수행하지 않았다.

### 다음 작업
1. 입력 ROOT 경로를 확인한다.
2. 네 centrality bin 중 하나를 actual run해 fit status와 출력 태그를 검증한다.


### 날짜
2026-07-24 10:28

### 작업 맥락
pp-matched PbPb pT bin 12개에 전체 fit chain을 실행했다. Legacy centrality 전용 4개는 제외했다.

### 변경 사항
- bin별 run_pp_jpsi_chain.sh 호출로 mc_mass, mass, ctau_pr, ctau_np, subrange_mass, subrange_ctau를 모두 실행했다.
- 4개 worker 병렬 실행 로그를 pb_jpsi/logs/run_pb_jpsi_chain_20260724_* 및 /tmp/pb_jpsi_pt_*.log에 저장했다.

### 결과
- 12/12 bin rc=0 종료, 오류/kill marker 없음.
- differential 10개와 pT-integrated 3–6.5, 6.5–40 GeV/c 모두 완료했다.

### 다음 작업
1. 생성 ROOT 파일의 fit status와 yield를 대표 bin에서 확인한다.
2. pT별 결과를 요약한다.

### 날짜
2026-07-25 00:35

### 작업 맥락
PbPb J/ψ current subrange b fraction을 동일한 pT bin의 legacy 2D Fit 및
기존 HIN-16-025 reference와 비교하는 전용 ROOT 매크로와 산출물을 만들었다.

### 변경 사항
- `pb_jpsi/drawing/b_frac/bFraction_fwd_y1p8_PbPb_comparison.C`를 추가했다.
- current 입력은
  `pb_jpsi/roots/y1.80_2.40/subrange/subrange_result_c0_180_y1.80_2.40_pt*.root`
  의 `bFraction`, `bFractionErr`, `ctauPvalue`, `ctauFitResult`를 읽는다.
- legacy 입력은
  `/data/hwan/psi2S_RAA_PbPb2018/Macros/Jpsi_260601/roots/2DFit_No_Weight/Final/`
  의 `2DfitResults` 1번 bin을 읽는다.
- current/legacy 공통 10개 pT bin, HIN reference, ratio pad, CSV 출력을 구현했다.
- ratio 오차는 두 fit의 통계오차를 독립으로 가정해 전파했다.

### 변경 이유
pp comparison과 같은 구성으로 PbPb의 두 fit method를 bin-by-bin 비교하고,
fit quality 정보까지 재현 가능한 표로 남기기 위해서다.

### 실행한 명령
```bash
/opt/conda/envs/root634/bin/root -l -b -q 'pb_jpsi/drawing/b_frac/bFraction_fwd_y1p8_PbPb_comparison.C+'
pdfinfo pb_jpsi/drawing/b_frac/bFraction_PbPb_fwd_y1p8_comparison.pdf
```

### 결과
- ACLiC 컴파일 및 실행이 성공했다.
- current, legacy, ratio가 각각 10개 point로 생성됐다.
- PDF 1 page, PNG `896×872`, CSV 29개 data row를 확인했다.
- PNG 렌더링에서 축, 범례, ratio와 unity line을 확인했다.
- current fit은 모두 status 0이며 첫 bin만 covariance quality 2, 나머지는
  3이다. `20–40 GeV/c` p-value는 `0.0104`이다.

### 다음 작업
1. `20–40 GeV/c` pull과 fit model을 검토한다.
2. first-bin covariance quality 2의 원인을 확인한다.
3. systematic 및 fit 간 상관관계를 ratio에 반영할지 결정한다.
