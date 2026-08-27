# 분석 기록

### 날짜
2026-07-23 13:57

### 작업 맥락
현재 pp J/ψ mass-subrange+ctau 방법으로 얻은 `1.8 < |y| < 2.4` nonprompt fraction을 기존 pp 2D fit 및 CMS BPH-10-002/HIN-16-025 결과와 비교했다. 비교 자료는 충돌 에너지와 rapidity 범위가 모두 같지는 않으므로 동일 phase-space의 직접 결합 또는 정량 적합으로 해석하지 않는다.

### 변경 사항
- 현재 결과는 `pp_jpsi/roots/y1.80_2.40/subrange/`에서 읽었다.
- 기존 2D fit은 `/data/hwan/psi2S_RAA_PbPb2018/Macros/pp_Jpsi/roots/2DFit_No_Weight/Final/`의 `2DfitResults` histogram 1번 bin에서 읽었다.
- BPH-10-002는 7 TeV, `1.6 < |y| < 2.4` HEPData table 7 값을 사용했다.
- HIN-16-025는 5.02 TeV, `|y| < 2.4` HEPData table 1의 pp 값을 사용했다.

### 변경 이유
현재 subrange 방법과 기존 2D fit의 일관성을 확인하고, 두 방법의 pT 의존성이 기존 CMS 측정의 범위와 추세 안에 있는지 한 그림에서 점검하기 위해서다.

### 결과
- 현재 성공한 subrange 산출물은 `3.0 < pT < 4.5 GeV/c` 한 점뿐이며 `f_B = 0.1528189 ± 0.0021446 (stat.)`이다.
- 같은 bin의 기존 2D fit은 `f_B = 0.1501129 ± 0.0004970 (stat.)`이다.
- 두 중심값 차이는 `0.0027061`, 통계오차만 합친 pull은 `1.23σ`, 비는 `1.0180`이다.
- 그러나 현재 subrange ctau fit의 저장된 χ² p-value는 `3.67×10^-103`이다. fit status 0 및 covariance quality 3과 별개로 분포 적합도가 매우 나쁘므로 이 점은 최종 물리 결과가 아니라 검증이 필요한 중간 산출물이다.
- BPH와 HIN 자료는 각각 rapidity 및 에너지가 현재 결과와 달라 시각적 reference로만 사용한다.

### 다음 작업
1. 현재 subrange ctau model, fit range, signal-yield uncertainty를 점검해 매우 작은 p-value의 원인을 찾는다.
2. 누락된 7개 pT bin의 subrange 산출물을 재생성한 뒤 같은 매크로를 다시 실행한다.
3. systematic uncertainty가 확정되면 현재 및 기존 2D-fit 점에도 systematic box를 추가한다.

## 관련 노트

- [[notes/B_fraction]]
- [[records/daily/2026-07-23]]

### 날짜
2026-07-24 09:42

### 작업 맥락
동일한 10개 pT bin에서 current subrange와 legacy 2D Fit b fraction을 직접 비교하고 아래쪽 ratio pad를 추가했다.

### 변경 사항
- ratio를 `f_B(subrange) / f_B(legacy 2D Fit)`으로 정의했다.
- 통계오차는 두 결과를 독립이라고 가정하여 표준 오차 전파로 계산했다.

### 결과
- 10개 ratio는 `0.852–0.950` 범위이며 모두 1보다 작다.
- bin 간 상관을 무시한 통계오차 가중 평균은 `0.8882 ± 0.0033`이다.
- 동일 데이터에서 나온 두 방법은 상관되어 있을 수 있으므로 ratio error와 가중 평균은 진단용이다.

### 다음 작업
1. 전 pT 구간의 일관된 하향 차이가 signal-yield extraction, ctau template 또는 2D Fit 정의 차이에서 생기는지 검증한다.
2. 두 방법의 공통 systematic과 통계적 상관관계를 평가하기 전에는 ratio의 유의도를 최종 결론으로 해석하지 않는다.


### 날짜
2026-07-24 10:06

### 작업 맥락
PbPb J/ψ subrange-fit production binning을 현재 pp J/ψ active binning과 동일한 phase-space로 맞췄다. 목적은 pp와 PbPb 결과를 rapidity/pT edge 재조합 없이 직접 비교할 수 있게 하는 것이다.

### 변경 사항
- rapidity를 `1.8 < |y| < 2.4`로 통일했다.
- differential pT bin을 `3–4.5`, `4.5–5.5`, `5.5–6.5`, `6.5–7.5`, `7.5–8.5`, `8.5–9.5`, `9.5–11`, `11–15`, `15–20`, `20–40 GeV/c`로 설정했다.
- broad pT bin `3–6.5`, `6.5–40 GeV/c`를 포함했다.
- pp에는 centrality 축이 없으므로 PbPb의 모든 active bin은 `0–90%` (`cBin 0–180`)로 통합했다.

### 변경 이유
기존 PbPb list의 `|y| < 1.6`, `1.6 < |y| < 2.4` 및 서로 다른 pT edge는 pp 결과와 직접 bin-by-bin 비교할 때 phase-space 차이를 만든다. pp active list와 일대일로 대응시키면 이후 RAA 또는 B-fraction 비교에서 bin mismatch를 제거할 수 있다.

### 결과
- active PbPb job은 10개 differential bin과 2개 broad bin, 총 12개다.
- 기존 PbPb centrality subrange scan과 midrapidity bin은 active list에서 제외됐다.
- shell syntax 및 mass-only dry-run은 통과했지만 실제 PbPb fit 결과는 아직 생성하지 않았다.

### 다음 작업
1. 유효한 PbPb 입력 ROOT 경로를 확인한다.
2. 대표 bin을 실행해 `1.8 < |y| < 2.4`에서 mass/ctau model과 fit 품질을 검증한다.
3. 필요하면 centrality-dependent 결과용 bin list를 별도 variation으로 정의한다.

## 관련 노트

- [[notes/B_fraction]]
- [[decisions/Binning_decisions]]
- [[records/daily/2026-07-24]]

### 날짜
2026-07-24 10:15

### 작업 맥락
pp-matched PbPb binning에 Legacy J/ψ 2D Fit과 동일한 centrality 구간을 추가했다. Legacy 기준은 `/data/hwan/psi2S_RAA_PbPb2018/Macros/Jpsi/06final.sh`의 nominal active loop에서 확인했다.

### 변경 사항
- Legacy `cBin` edge `0–20`, `20–60`, `60–100`, `100–180`을 percent 단위 `0–10%`, `10–30%`, `30–50%`, `50–90%`로 변환했다.
- Legacy와 동일한 `3 < pT < 40 GeV/c`에 네 centrality bin을 적용했다.
- rapidity는 현재 pp-matched 분석 기준인 `1.8 < |y| < 2.4`를 사용했다. Legacy 원본의 rapidity는 `1.6 < |y| < 2.4`이므로 완전히 동일한 phase-space는 아니다.

### 변경 이유
centrality-dependent PbPb 결과를 Legacy 2D Fit과 같은 구간에서 직접 비교하면서, 앞서 정렬한 pp/PbPb rapidity 기준을 유지하기 위해서다.

### 결과
- active production list는 pp-matched `0–90%` 12개 bin과 Legacy centrality 4개 bin, 총 16개가 됐다.
- Legacy centrality edge와 pT 범위는 일치하지만 rapidity 하한은 `1.6`에서 `1.8`로 변경되어 있다. 비교 시 이 차이를 명시해야 한다.
- actual fit 결과는 아직 생성하지 않았다.

### 다음 작업
1. 대표 centrality bin에서 actual fit을 실행한다.
2. Legacy 2D Fit과 비교할 때 rapidity 하한 차이의 영향을 평가한다.
3. 16개 bin 전체 fit 품질을 확인한다.

## 관련 노트

- [[notes/B_fraction]]
- [[decisions/Binning_decisions]]
- [[records/daily/2026-07-24]]

### 날짜
2026-07-25 00:35

### 작업 맥락
PbPb J/ψ `1.8 < |y| < 2.4`, centrality `0–90%`의 current
mass-subrange+ctau b fraction을 동일한 10개 pT bin의 legacy 2D Fit 및
기존 HIN-16-025 reference와 비교했다.

### 변경 사항
- ratio를 `f_B(current subrange) / f_B(legacy 2D Fit)`으로 정의했다.
- current와 legacy의 통계오차를 독립으로 가정해 ratio 오차를 전파했다.
- HIN-16-025 reference는
  `/data/hwan/psi2S_RAA_PbPb2018/Macros/Jpsi_260601/draw_Bfraction_pbpb_y1p8_2p4.C`
  에 저장된 9개 값을 사용했다.

### 변경 이유
pp에서 수행한 방법 간 비교를 동일한 PbPb rapidity, pT 및 centrality
phase space에 적용해 두 fit strategy의 pT 의존성을 확인하기 위해서다.

### 결과
- current와 legacy가 각각 10개 공통 bin에서 읽혔다.
- ratio는 `0.9157–1.2805`이다. `3–6.5 GeV/c`에서는
  `1.197–1.281`, `9.5–20 GeV/c`에서는 `0.996–1.028`,
  `20–40 GeV/c`에서는 `0.9157`이다.
- bin 간 상관을 무시한 진단용 통계오차 가중 평균은
  `1.0681 ± 0.0141`이다.
- current ctau fit status는 10개 모두 0이다. 첫 bin의 covariance quality는
  2, 나머지는 3이다. `20–40 GeV/c`의 ctau χ² p-value는
  `0.0104`로 0.05보다 작다.
- 저pT 상승과 최고 pT 하강은 pp에서 관찰된 전 bin 하향 패턴과 다르다.
  공통 데이터 상관관계와 systematic uncertainty가 빠져 있으므로 방법 차이의
  유의도 또는 물리적 pT 의존성으로 확정하지 않는다.
- HIN reference의 uncertainty 구성과 centrality 표기는 local legacy macro
  이상으로 독립 검증하지 않았으므로 시각적 reference로만 사용한다.

### 다음 작업
1. `20–40 GeV/c` pull과 ctau fit range/template을 확인한다.
2. `3–4.5 GeV/c` covariance quality 2의 Hessian 안정성을 확인한다.
3. current/legacy 통계 상관관계와 systematic uncertainty를 평가한다.

## 관련 노트

- [[notes/B_fraction]]
- [[records/daily/2026-07-25]]
