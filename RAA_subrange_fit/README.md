# RAA subrange fit 실행 안내

이 문서는 `/data/hwan/RAA_subrange_fit`에서 처음 작업하는 사람이 J/ψ mass fit, ctau fit 및 mass-subrange fit을 실행하기 위한 안내서다. 분석 코드는 `pp_jpsi/`와 `pb_jpsi/`로 나뉘며, 두 디렉터리의 실행 스크립트는 각각 독립적으로 실행한다.

## 1. 먼저 확인할 것

저장소와 ROOT 환경이 준비된 분석 노드에서 실행한다.

```bash
cd /data/hwan/RAA_subrange_fit
command -v root
/opt/conda/envs/root634/bin/root --version
```

현재 매크로에는 다음과 같은 공용 설정 및 입력 경로가 하드코딩되어 있다.

- ROOT 초기화 매크로: `/data/users/pjgwak/input_files/rootlogon.C`
- pp/PbPb 입력: `/data/Oniatree/skimmedFiles/`

입력 파일과 ROOT 초기화 파일을 읽을 수 없는 환경에서는 fit 전에 해당 경로를 현재 환경에 맞게 수정해야 한다. 입력 파일 존재 여부는 다음처럼 확인한다.

```bash
ls -lh /data/Oniatree/skimmedFiles/OniaRooDataSet_isMC0_JPsi_pp_y0.00_2.40_Effw0_Accw0_PtW1_TnP1_260303_root634.root
ls -lh /data/Oniatree/skimmedFiles/OniaRooDataSet_miniAOD_isMC0_JPsi_cent0_200_Effw0_Accw0_PtW0_TnP0_230721.root
```

## 2. 체인의 구성

기본 실행은 bin 하나에 대해 다음 순서로 진행된다.

```text
mc_mass.C → mass.C → ctau_pr.C → ctau_np.C
          → subrange_mass.C → subrange_ctau.C
```

- `mc_mass.C`: MC mass shape
- `mass.C`: 데이터 mass fit
- `ctau_pr.C`, `ctau_np.C`: prompt/nonprompt ctau 모델
- `subrange_mass.C`: ctau subrange별 mass fit과 `sliceSummary`
- `subrange_ctau.C`: 최종 ctau fit과 subrange 결과

ROOT 파일은 각 분석 디렉터리의 `roots/`, 그림은 `figs/`, 실행 로그는 `logs/` 아래에 저장된다.

## 3. dry-run으로 시작하기

실제 fit을 실행하지 않고 호출 명령과 bin 수만 확인하려면 먼저 dry-run을 사용한다.

```bash
cd /data/hwan/RAA_subrange_fit/pp_jpsi
bash run_pp_jpsi_chain.sh --dry-run --only=subrange --bin=3.0,4.5,1.8,2.4

cd /data/hwan/RAA_subrange_fit/pb_jpsi
bash run_pb_jpsi_chain.sh --dry-run --only=subrange --bin=3.0,4.5,1.8,2.4
```

`--dry-run`은 파일을 만들거나 fit을 실행하지 않는다. 처음에는 대표 bin 하나로 pT, rapidity 및 PbPb centrality가 의도한 값인지 확인한다.

## 4. pp J/ψ 실행

현재 pp 활성 목록은 `1.8 < |y| < 2.4`에서 differential bin `3–4.5`, `4.5–5.5`, `5.5–6.5`, `6.5–7.5`, `7.5–8.5`, `8.5–9.5`, `9.5–11`, `11–15`, `15–20`, `20–40 GeV/c`와 broad bin `3–6.5`, `6.5–40 GeV/c`다.

대표 bin 하나를 실행한다.

```bash
cd /data/hwan/RAA_subrange_fit/pp_jpsi
bash run_pp_jpsi_chain.sh --bin=3.0,4.5,1.8,2.4 --bin-jobs=1
```

앞 단계 결과가 있고 subrange만 다시 실행할 때:

```bash
bash run_pp_jpsi_chain.sh --only=subrange --bin=3.0,4.5,1.8,2.4
```

특정 단계만 실행할 때:

```bash
bash run_pp_jpsi_chain.sh --only=mass --bin=3.0,4.5,1.8,2.4
bash run_pp_jpsi_chain.sh --only=ctau_pr --bin=3.0,4.5,1.8,2.4
bash run_pp_jpsi_chain.sh --only=subrange_mass --bin=3.0,4.5,1.8,2.4
bash run_pp_jpsi_chain.sh --only=subrange_ctau --bin=3.0,4.5,1.8,2.4
```

전체 pp bin은 다음처럼 실행한다. 메모리와 CPU 상황을 모르면 `--bin-jobs=1` 또는 `4`부터 사용한다.

```bash
bash run_pp_jpsi_chain.sh --parallel-bins --bin-jobs=4
```

## 5. PbPb J/ψ 실행

PbPb는 pp-matched 기본 bin `1.8 < |y| < 2.4`, centrality `0–90%`를 사용한다. `3–40 GeV/c`에는 Legacy 2D Fit 비교용 `0–10%`, `10–30%`, `30–50%`, `50–90%` bin도 있다.

대표 pp-matched bin:

```bash
cd /data/hwan/RAA_subrange_fit/pb_jpsi
bash run_pp_jpsi_chain.sh --bin=3.0,4.5,1.8,2.4 --bin-jobs=1
```

centrality subrange는 pT-integrated `3–40 GeV/c` bin에 percent 단위로 지정한다.

```bash
bash run_pp_jpsi_chain.sh --bin=3.0,40.0,1.8,2.4,0,10 --bin-jobs=1
```

전체 활성 목록은 먼저 dry-run으로 확인한다.

```bash
bash run_pp_jpsi_chain.sh --dry-run --parallel-bins --bin-jobs=4
bash run_pp_jpsi_chain.sh --parallel-bins --bin-jobs=4
```

## 6. mass-slice 재실행

특정 subrange mass slice를 다시 실행할 수 있다.

```bash
cd /data/hwan/RAA_subrange_fit/pp_jpsi
bash run_pp_jpsi_chain.sh --subrange-slices=100 --bin=3.0,4.5,1.8,2.4
bash run_pp_jpsi_chain.sh --subrange-slices=98,100-102 --bin=3.0,4.5,1.8,2.4
bash run_pp_jpsi_chain.sh --only=subrange_ctau --bin=3.0,4.5,1.8,2.4
```

저장된 mass slice와 `sliceSummary`를 재사용해 최종 ctau만 다시 fit하려면:

```bash
bash run_pp_jpsi_chain.sh --reuse-mass-slices --bin=3.0,4.5,1.8,2.4
```

PbPb에서는 필요하면 `--center-veto=X`를 추가할 수 있다. 분석 정의를 바꿀 수 있으므로 실행 명령과 결과를 반드시 기록한다.

## 7. 결과와 로그 확인

```bash
cd /data/hwan/RAA_subrange_fit/pp_jpsi
ls -lt logs | head
tail -n 80 logs/run_pp_jpsi_chain_*/summary.log
```

다음 항목을 확인한다.

- summary의 `failed_bins`와 `killed_jobs`
- 각 bin 로그의 ROOT 오류, 입력 파일 오류 및 fit status
- `roots/.../mass/`, `roots/.../ctau_pr/`, `roots/.../ctau_np/`, `roots/.../subrange/` 산출물
- 최종 fit의 covariance quality, pull 및 chi-square/p-value
- 필요할 때 `figs/` 아래 PDF 생성 여부

summary에 실패가 없더라도 개별 bin 로그와 ROOT 파일을 함께 확인한다. fit status 0만으로 fit 품질이 보장되는 것은 아니다.

전체 옵션:

```bash
bash /data/hwan/RAA_subrange_fit/pp_jpsi/run_pp_jpsi_chain.sh --help
bash /data/hwan/RAA_subrange_fit/pb_jpsi/run_pp_jpsi_chain.sh --help
```

## 8. 작업 기록

실행 명령, 입력 파일, bin 조건, 출력 파일, 오류와 검증 결과는 `records/dev_log.md`에 한국어로 남긴다. 분석적 해석은 `analysis_notes.md`, 남은 검증은 `todo.md`, 다음 작업자에게 필요한 상태는 `records/handoff.md`에 기록한다. 중요한 binning·fit model·selection 변경은 관련 `decisions/` 문서도 갱신한다.

ROOT 실행이 불가능한 환경에서는 “코드 검토”와 “실제 fit 런타임 검증”을 구분해 기록한다.
