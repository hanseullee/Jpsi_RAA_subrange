# CLAUDE.md

## 목적

이 repository는 Codex와 Claude Code가 함께 사용하는 charmonium heavy-ion 분석 프로젝트이다.
Claude Code는 코드 리뷰, 분석 전략 검토, 오류 원인 분석, 구현 및 연구 기록 정리를 수행한다.

이 repository는 Syncthing으로 Obsidian과 동기화되므로, 작업 결과를 코드뿐 아니라 `notes/`, `records/daily/`, `decisions/` 구조에도 일관되게 반영한다.

## 최우선 규칙

- `AGENTS.md`를 이 프로젝트의 공통 운영 규칙으로 따른다.
- 기존 작업과 미커밋 변경을 확인하기 전에 파일을 덮어쓰지 않는다.
- 코드 변경 이유와 검증 결과를 Markdown 기록에 남긴다.
- 물리적 해석, 분석 선택, 구현 상세를 구분한다.
- 실행하지 않은 테스트를 완료했다고 기록하지 않는다.

## 작업 시작 전 확인

먼저 다음 명령을 실행한다.

```bash
git status
git diff --stat
```

필요하면 다음도 확인한다.

```bash
git diff
git log -5 --oneline
find . -name '*.sync-conflict-*' -print
```

다음 문서를 순서대로 읽는다.

1. `AGENTS.md`
2. `00_Project_Index.md`
3. `records/handoff.md`
4. `todo.md`
5. `analysis_notes.md`
6. `records/dev_log.md`
7. 현재 작업과 관련된 `notes/<topic>.md`
8. 관련 `decisions/*_decisions.md`
9. 최신 `records/daily/YYYY-MM-DD.md`

파일이 없으면 오류로 중단하지 말고, 현재 구조를 확인한 뒤 필요한 경우 생성한다. `records/dev_log.md`, `records/daily/`, `records/handoff.md`는 제목과 서술을 모두 한국어로 작성한다.

## 기록 구조

| 목적 | 파일 또는 폴더 |
|---|---|
| 코드 변경·오류·환경·명령 | `records/dev_log.md` |
| 분석 전략·물리 해석·결과 비교 | `analysis_notes.md` |
| 다음 작업 | `todo.md` |
| 주제별 누적 지식 | `notes/` |
| 날짜별 작업 요약 | `records/daily/` |
| 분석·구현 결정의 근거 | `decisions/` |
| agent 인계(한국어) | `records/handoff.md` |
| Obsidian 탐색 허브 | `00_Project_Index.md` |

## Claude Code 작업 방식

### 코드 리뷰

- selection, weight, denominator/numerator, branch mapping 및 bin boundary를 우선 점검한다.
- prompt/nonprompt, pp/PbPb, mid/forward, pT/centrality 분기가 대칭적으로 구현됐는지 확인한다.
- histogram `Fill`, `Divide`, `Write`의 분자·분모·weight 일관성을 확인한다.
- ROOT branch가 실제 입력 tree에 존재하는지 확인하고 입력 포맷 차이를 고려한다.
- 기존 결과가 바뀔 수 있는 수정은 예상 영향과 검증 방법을 먼저 기록한다.

### 분석 전략 검토

- 가정, 선택지, 장점·단점, 예상 bias 및 필요한 validation을 구분한다.
- acceptance, efficiency, pT reweighting, ctau selection, fit model 및 systematic의 상관관계를 확인한다.
- 물리 결과의 차이를 코드 오류, 분석 정의, 입력 차이, 통계적 fluctuation으로 나눠 검토한다.
- 중요한 선택이 확정되면 관련 `decisions/` 문서를 갱신한다.

### 코드 수정

- 변경 범위를 최소화하고 기존 호출 인터페이스 및 산출물 키를 가능한 한 보존한다.
- 큰 리팩토링은 기능 변경과 구조 변경을 분리한다.
- 패치 후 최소한 syntax 또는 compile 검증을 수행한다.
- ROOT 환경이 없으면 정적 검토만 수행했음을 명시하고 `todo.md`에 런타임 검증을 남긴다.

### 문서 갱신

작업이 끝나면 필요한 범위에서 다음을 갱신한다.

1. `records/dev_log.md`: 구현, 명령, 오류, 결과
2. `analysis_notes.md`: 분석적 의미와 해석
3. `todo.md`: 남은 실행 및 검증
4. `notes/<topic>.md`: 해당 주제의 현재 상태
5. `records/daily/YYYY-MM-DD.md`: 오늘 수행한 작업 요약
6. `decisions/`: 중요한 선택과 근거
7. `records/handoff.md`: 다음 agent가 바로 이어갈 수 있는 정보
8. `00_Project_Index.md`: 새 주제 문서 또는 decision 링크가 생긴 경우

## Obsidian 문서 작성 규칙

### 주제 노트

주제 노트는 단순 로그를 복사하지 말고 현재 상태를 중심으로 정리한다.

권장 섹션:

```markdown
# Topic

## 목적 또는 정의
## 현재 설정
## 핵심 결과
## 물리적 해석
## 관련 코드 및 입력
## 미해결 문제
## 관련 결정
## 날짜별 상세 기록
```

### Daily note

동일 날짜 파일이 있으면 이어서 작성한다.

```markdown
# YYYY-MM-DD

## 수행한 작업
## 변경 파일
## 실행 명령
## 결과
## 문제 및 미검증 사항
## 다음 작업
## 관련 노트
```

### Decision 기록

다음 항목은 decision으로 남긴다.

- baseline 또는 production version 선택
- pT weight source와 적용 방식
- binning, selection, ctau cut
- acceptance/efficiency 정의
- fit model 및 systematic variation
- fallback 사용 또는 제거

Decision은 “무엇을 했는가”보다 “왜 선택했는가”를 중심으로 작성한다.

## Multi-agent 협업

- Codex 또는 사용자의 기존 변경을 발견하면 `git diff`로 먼저 확인한다.
- 같은 파일의 대규모 수정을 피한다.
- 다른 agent가 남긴 `records/handoff.md`의 미완료 검증을 우선 확인한다.
- 작업 도중 새 문제가 발견되면 관련 TODO와 handoff를 즉시 갱신한다.
- 다른 agent가 사용할 수 있도록 함수명, 경로, 명령 및 출력 키를 정확하게 기록한다.
- Syncthing conflict 파일이 있으면 원본을 비교한 뒤 사용자 기록이 손실되지 않게 병합한다.

## 작업 종료 체크리스트

- [ ] `git diff --stat`로 최종 변경 범위 확인
- [ ] 의도하지 않은 파일 변경 여부 확인
- [ ] 가능한 syntax/compile/runtime 테스트 수행
- [ ] 결과 파일 또는 histogram key 확인
- [ ] 미수행 검증을 명시
- [ ] `records/dev_log.md` 또는 `analysis_notes.md` 갱신
- [ ] `todo.md` 갱신
- [ ] 관련 `notes/`, `records/daily/`, `decisions/` 갱신
- [ ] `records/handoff.md` 갱신

## 인계 형식

```markdown
### 인계

#### 완료한 작업
- 완료한 작업

#### 변경 파일
- 수정한 파일 목록

#### 실행한 명령
```bash
# 실행 명령
```

#### 결과
- 검증된 결과

#### 수행하지 못한 검증
- 미수행 검증과 이유

#### 남은 문제
- 남은 문제

#### 권장 다음 작업
- 다음 권장 작업

#### Codex/User 참고
- 다음 작업자가 반드시 알아야 할 사항
```

## 연구 분석 체크 포인트

### Efficiency 및 Acceptance

- gen/reco selection의 일치 여부
- numerator/denominator phase space
- pT, rapidity, centrality 및 state 분기
- weighted/no-weighted 정의
- integrated 결과의 spectrum dependence

### pT reweighting

- 입력 ROOT 파일과 `TF1` 또는 histogram key
- mid/forward 함수 분리
- pp/PbPb 및 prompt/nonprompt 입력 구분
- weight 중복 적용 여부
- missing function fallback 동작

### Ctau 및 Prompt/Nonprompt

- pT bin별 ctau cut 정의
- reco/gen matching
- ctau efficiency 적용 위치
- prompt-enriched와 true prompt의 구분

### Fits, RAA 및 v2

- fit model과 alternative model
- yield 및 uncertainty 전달
- acceptance×efficiency 입력 버전
- pp reference와 PbPb normalization
- scalar-product/Event Plane 정의 및 Q-vector branch

## 출력 형식

사용자에게 보고할 때는 다음 순서를 우선한다.

1. 핵심 결과
2. 변경한 파일
3. 수행한 검증
4. 미확인 사항
5. 다음 작업

불필요한 서술을 줄이고 파일명, 함수명, 명령, 파라미터 및 결과 위치를 명확히 적는다.
