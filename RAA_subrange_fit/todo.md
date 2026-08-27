# 할 일

### 날짜
2026-07-23 13:57

### 작업 맥락
`1.8 < |y| < 2.4` pp J/ψ b-fraction 비교 플롯을 만들면서 현재 subrange 산출물의 누락과 fit 품질 문제를 확인했다.

### 변경 사항
- 비교 플롯 생성 이후 필요한 검증 항목을 정리했다.

### 변경 이유
현재 한 점만 존재하고 그 점의 ctau χ² p-value가 극단적으로 작아, 추가 검증 없이 baseline 결과로 사용할 수 없기 때문이다.

### 결과
- [x] `pp_jpsi/subrange_mass.C`의 `kReferenceDir`를 현재 프로젝트 경로로 변경한다.
- [ ] `run_pp_jpsi_chain.sh`가 ROOT macro 내부 실패를 `failed_bins=0`으로 요약하는 상태 전달 문제를 수정한다.
- [x] legacy 2D Fit과 동일한 10개 pT bin의 subrange 결과를 생성하고 비교 플롯에 반영한다.
- [ ] `3.0–4.5 GeV/c` ctau fit의 `p=3.67×10^-103` 원인을 pull, fit range, template 및 signal-yield error 관점에서 확인한다.
- [ ] 현재 subrange 및 기존 2D fit의 systematic uncertainty를 비교 플롯에 반영할지 결정한다.

### 다음 작업
1. `run_pp_jpsi_chain.sh`의 실패 status propagation을 수정한다.
2. 한 bin으로 재실행해 mass-slice 출력 경로와 ctau fit 품질을 검증한다.
3. 전체 pT bin을 생성한 후 `bFraction_fwd_y1p8_pp_comparison.C`를 다시 실행한다.

### 날짜
2026-07-24 09:42

### 결과
- [x] 동일한 10개 bin의 current/legacy 비교와 ratio pad 생성
- [ ] subrange/legacy ratio가 전 bin에서 1보다 작은 원인을 fit 정의와 ctau template 관점에서 검증
- [ ] 두 fit 결과의 통계적 상관관계와 systematic uncertainty를 ratio에 반영할 방법 결정


### 날짜
2026-07-24 09:56

### 작업 맥락
pp 공통 코드 변경을 PbPb에 이식한 뒤 남은 런타임 검증을 정리했다.

### 결과
- [x] PbPb mass/ctau fit에 `NumCPU(nCPU)` 반영
- [x] PbPb mass 및 MC mass plot에 log-y 설정 반영
- [x] PbPb subrange `kReferenceDir`를 현재 프로젝트 경로로 변경
- [x] 수정한 PbPb 매크로 6개의 ROOT load 및 ACLiC 컴파일 검증
- [ ] PbPb data/MC 입력 ROOT 파일의 유효 경로 확인
- [ ] 대표 PbPb bin에서 전체 체인을 실행해 fit status, 출력 경로 및 log-y 렌더링 검증

### 다음 작업
1. 입력 ROOT 파일 위치를 확정한다.
2. 한 bin runtime test 후 필요하면 전체 PbPb bin을 재실행한다.


### 날짜
2026-07-24 10:06

### 작업 맥락
PbPb active production binning을 pp와 동일하게 변경한 뒤 필요한 실행 검증을 정리했다.

### 결과
- [x] PbPb rapidity를 `1.8 < |y| < 2.4`로 변경
- [x] PbPb pT bin을 pp와 동일한 10개 differential + 2개 broad bin으로 변경
- [x] 모든 active PbPb bin을 centrality `0–90%`로 설정
- [x] shell syntax 및 12-bin dry-run 검증
- [ ] 새 binning으로 대표 PbPb bin actual run
- [ ] fit status와 mass/ctau plot을 확인한 뒤 전체 12개 bin 생성
- [ ] centrality-dependent 결과가 필요하면 별도 bin list 정의

### 다음 작업
1. 입력 ROOT 경로를 확인한다.
2. `3.0 < pT < 4.5 GeV/c`, `1.8 < |y| < 2.4`, centrality `0–90%` 대표 bin을 실행한다.
3. 성공 시 전체 pp-matched PbPb bin을 실행한다.


### 날짜
2026-07-24 10:15

### 작업 맥락
Legacy 2D Fit centrality bin 추가 후 남은 검증을 정리했다.

### 결과
- [x] Legacy nominal `06final.sh` centrality edge 확인
- [x] `3–40 GeV/c`에 `0–10%`, `10–30%`, `30–50%`, `50–90%` 추가
- [x] `cBin 0–20`, `20–60`, `60–100`, `100–180` dry-run 호출 검증
- [x] 전체 16-bin shell syntax 및 dry-run 검증
- [ ] 대표 Legacy centrality bin actual fit
- [ ] Legacy rapidity `1.6–2.4`와 현재 `1.8–2.4` 차이 평가
- [ ] actual fit 성공 후 전체 16개 bin 생성

### 다음 작업
1. `3–40 GeV/c`, `1.8 < |y| < 2.4`, centrality `0–10%`를 대표 실행한다.
2. fit status와 mass/ctau 산출물을 확인한다.


### 2026-07-24 실행 완료 항목
- [x] pp-matched PbPb pT bin 12개 전체 chain 실행
- [x] 12개 bin 모두 rc=0 및 오류/kill marker 없음 확인
- [x] 10개 differential pT ROOT 파일의 b fraction, fit status, covariance quality 및 p-value 정량 확인

### 날짜
2026-07-25 00:35

### 작업 맥락
PbPb current subrange와 legacy 2D Fit의 pT별 b-fraction 비교 플롯을 생성한
뒤 남은 품질 및 불확도 검증을 정리했다.

### 결과
- [x] PbPb current/legacy 공통 10개 pT bin 비교
- [x] `subrange / legacy 2D Fit` ratio pad와 독립 통계오차 전파
- [x] PDF, PNG, CSV 생성 및 PNG 렌더링 확인
- [x] current 10개 fit status 0 확인
- [ ] `20–40 GeV/c` ctau p-value `0.0104`의 pull 및 model 원인 확인
- [ ] `3–4.5 GeV/c` covariance quality 2의 fit 안정성 확인
- [ ] current/legacy fit의 통계적 상관관계 및 systematic uncertainty 평가
- [ ] local HIN-16-025 reference의 원 출처 uncertainty와 centrality 표기 재검증

### 다음 작업
1. `20–40 GeV/c` ctau fit plot과 pull을 우선 확인한다.
2. first-bin covariance matrix와 minimizer diagnostics를 확인한다.
3. 필요하면 4개 centrality bin의 b-fraction 비교 플롯을 별도로 만든다.
