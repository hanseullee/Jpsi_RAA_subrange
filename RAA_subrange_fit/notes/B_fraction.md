# B fraction

## 목적 또는 정의

J/ψ의 nonprompt fraction은

`f_B = N(nonprompt J/ψ) / [N(prompt J/ψ) + N(nonprompt J/ψ)]`

로 정의한다. 현재 비교의 주 phase space는 pp 및 PbPb 5.02 TeV,
`1.8 < |y| < 2.4`이며 PbPb pT 비교는 centrality `0–90%`이다.

## 현재 분석 설정

- 현재 방법: mass subrange별 signal yield를 구한 뒤 ctau 분포를 prompt/nonprompt template으로 fit
- 기존 방법: mass-ctau 2D fit
- 현재 및 legacy 2D Fit 공통 pT bin: `3–4.5`, `4.5–5.5`, `5.5–6.5`, `6.5–7.5`, `7.5–8.5`, `8.5–9.5`, `9.5–11`, `11–15`, `15–20`, `20–40 GeV/c`
- 비교 매크로: `pp_jpsi/drawing/b_frac/bFraction_fwd_y1p8_pp_comparison.C`
- PbPb 비교 매크로: `pb_jpsi/drawing/b_frac/bFraction_fwd_y1p8_PbPb_comparison.C`
- PbPb active production chain: `pb_jpsi/run_pp_jpsi_chain.sh`
- PbPb pp-matched 12개 rapidity/pT bin은 centrality `0–90%`를 사용한다.
- 추가 Legacy 2D Fit centrality bin은 `3–40 GeV/c`에서 `0–10%`, `10–30%`, `30–50%`, `50–90%`이다.

## 주요 결과 및 관찰

- current subrange와 legacy 2D Fit 결과가 공통 10개 pT bin에 존재한다.
- `subrange / legacy 2D Fit` 비는 `0.852–0.950`이며 모든 bin에서 1보다 작다.
- bin 간 상관을 무시한 통계오차 가중 평균은 `0.8882 ± 0.0033`이다. 이는 진단값이며 결합 물리 결과로 사용하지 않는다.
- 비교 그림 아래쪽 ratio pad에는 두 방법의 통계오차를 독립이라고 가정해 전파한 오차를 표시한다.
- PbPb current subrange와 legacy 2D Fit은 공통 10개 pT bin에서 각각 모두 읽혔다.
- PbPb `subrange / legacy 2D Fit`은 `0.916–1.281`이다. 저pT
  `3–6.5 GeV/c`에서는 `1.20–1.28`, `9.5–20 GeV/c`에서는 약
  `1.00–1.03`, `20–40 GeV/c`에서는 `0.916`이다.
- PbPb ratio의 진단용 통계오차 가중 평균은 `1.0681 ± 0.0141`이다.
- PbPb current 10개 ctau fit은 모두 status 0이다. `3–4.5 GeV/c`만
  covariance quality 2이고 나머지는 3이며, `20–40 GeV/c` p-value는
  `0.0104`로 추가 검증이 필요하다.

## 물리적 해석

pp의 동일한 10개 bin에서 subrange 결과는 legacy 2D Fit보다 일관되게 낮다. 이 패턴은 단일-bin 통계 fluctuation보다 두 방법 사이의 signal-yield extraction 또는 ctau modeling 정의 차이를 우선 점검해야 함을 시사한다. 다만 두 결과가 같은 데이터에서 산출되어 통계적으로 상관될 수 있으므로 현재 독립오차 ratio만으로 차이의 유의도를 확정하지 않는다. BPH-10-002는 7 TeV 및 `1.6 < |y| < 2.4`, HIN-16-025 pT 자료는 5.02 TeV 및 `|y| < 2.4`이므로 현재 phase space와의 차이를 포함한 시각적 reference로만 해석한다.

PbPb에서는 pp와 달리 방법 차이가 전 pT에서 같은 방향이 아니다. 저pT에서
subrange가 높고 중간 pT에서는 두 방법이 일치하며 최고 pT에서는 낮다. 같은
데이터를 사용한 fit 사이의 상관관계와 systematic uncertainty를 포함하지 않은
진단 결과이므로, pT 의존성을 물리 효과로 해석하기 전에 signal-yield extraction과
ctau template 차이를 확인해야 한다.

## 구현 및 관련 코드

- 현재 ROOT 입력: `pp_jpsi/roots/y1.80_2.40/subrange/subrange_result_y1.80_2.40_pt3.00_4.50.root`
- 기존 2D fit 입력: `/data/hwan/psi2S_RAA_PbPb2018/Macros/pp_Jpsi/roots/2DFit_No_Weight/Final/`
- BPH 입력: HEPData DOI `10.17182/hepdata.57532.v1/t7`
- HIN 입력: HEPData DOI `10.17182/hepdata.80816.v1/t1`
- 그림: `pp_jpsi/drawing/b_frac/bFraction_pp_fwd_y1p8_comparison.pdf`
- 수치표: `pp_jpsi/drawing/b_frac/bFraction_pp_fwd_y1p8_comparison_values.csv`
- PbPb 현재 ROOT 입력: `pb_jpsi/roots/y1.80_2.40/subrange/subrange_result_c0_180_y1.80_2.40_pt*.root`
- PbPb legacy 2D Fit 입력: `/data/hwan/psi2S_RAA_PbPb2018/Macros/Jpsi_260601/roots/2DFit_No_Weight/Final/`
- PbPb 그림: `pb_jpsi/drawing/b_frac/bFraction_PbPb_fwd_y1p8_comparison.pdf`
- PbPb 수치표: `pb_jpsi/drawing/b_frac/bFraction_PbPb_fwd_y1p8_comparison_values.csv`

## 미해결 문제

- 현재 및 기존 2D fit systematic uncertainty는 이번 그림에 포함하지 않았다.
- ratio의 통계오차는 두 fit 사이의 공통 데이터 및 상관관계를 포함하지 않는다.
- subrange가 전 bin에서 낮은 원인을 fit 정의, ctau template 및 공통 상관관계 관점에서 검증해야 한다.
- PbPb `20–40 GeV/c` ctau fit의 p-value `0.0104`와
  `3–4.5 GeV/c` covariance quality 2의 pull 및 fit 안정성을 확인해야 한다.
- PbPb ratio의 통계오차는 current/legacy fit 사이의 상관관계를 포함하지 않는다.
- PbPb HIN-16-025 수치는 local legacy plotting macro에서 가져왔으며 원 논문
  uncertainty 구성과 centrality 표기를 별도로 재검증해야 한다.

## 관련 결정

- [[decisions/Binning_decisions]]: pp-matched bin은 `0–90%`로 유지하고 Legacy `3–40 GeV/c` centrality 4개 bin을 추가했다.

## 날짜별 상세 기록

- [[records/daily/2026-07-23]]
- [[records/daily/2026-07-24]]
- [[records/daily/2026-07-25]]
