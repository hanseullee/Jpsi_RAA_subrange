# AGENTS.md

## 목적

이 프로젝트는 연구 코드와 분석 기록을 함께 관리한다.
Codex는 코드 생성·수정뿐 아니라 작업 맥락, 분석 판단, 검증 결과와 남은 문제를 재현 가능한 Markdown 기록으로 남긴다.

이 repository는 Syncthing을 통해 Obsidian Vault로도 사용된다. 따라서 연구 기록은 단순 로그가 아니라 서로 연결되는 프로젝트 지식 구조로 관리한다.

## 기본 원칙

- 코드 변경만 하지 말고, 무엇을 왜 변경했는지 함께 기록한다.
- 중요한 분석 아이디어, 가정, 선택, 결과 및 실패 원인은 반드시 문서에 남긴다.
- 사람이 나중에 다시 읽어도 입력, 조건, 명령, 결과를 재현할 수 있어야 한다.
- 물리적 해석과 구현 세부사항을 명확히 구분한다.
- 기존 기록을 삭제하거나 과도하게 축약하지 않는다.
- 불확실한 내용은 확정된 결론처럼 기록하지 않고, 가정·미검증·추가 확인 필요 여부를 표시한다.

## 프로젝트 기록 구조

### 핵심 허브

- `00_Project_Index.md`
  - Obsidian 프로젝트 홈
  - 핵심 파일, 주제별 노트, 결정 기록, 최근 daily note 링크를 관리한다.

### 원본 누적 기록

- `records/dev_log.md`
  - 코드 수정, 디버깅, 환경설정, 실행 명령, 입력·출력 파일, 에러 및 해결 기록
  - 반드시 한국어로 작성한다.
- `analysis_notes.md`
  - 분석 전략, 물리적 해석, 비교 결과, 가정, 검증 논리 및 주요 질문
- `todo.md`
  - 미완료 작업, 검증 필요 항목 및 다음 실행 단계

### Obsidian 구조화 기록

- `notes/`
  - 주제별 장기 노트
  - 예: `Efficiency.md`, `Acceptance.md`, `pT_reweighting.md`, `Ctau_selection.md`, `Systematics.md`, `RAA.md`, `Flow_v2.md`, `Fits_and_yields.md`
- `records/daily/`
  - 날짜별 작업 일지
  - 파일명: `records/daily/YYYY-MM-DD.md`
  - 반드시 한국어로 작성한다.
- `decisions/`
  - 분석 기준 또는 구현 방식을 선택한 이유와 영향을 기록
  - 기존 주제별 decision register가 있으면 우선 갱신하고, 독립적으로 중요한 결정은 날짜·주제별 파일을 만들 수 있다.
- `records/handoff.md`
  - Codex, Claude Code 또는 사용자가 다음 작업을 이어받기 위한 최신 인계 기록
  - 반드시 한국어로 작성한다.

## 기록 위치 결정 규칙

하나의 작업이 여러 성격을 가지면 필요한 파일을 함께 갱신한다.

| 내용 | 기록 위치 |
|---|---|
| 코드 변경, 에러, 환경, 실행 명령 | `records/dev_log.md` |
| 물리 해석, 분석 전략, 결과 비교 | `analysis_notes.md` |
| 남은 작업 | `todo.md` |
| 특정 분석 주제의 누적 지식 | `notes/<topic>.md` |
| 그날 수행한 작업 요약 | `records/daily/YYYY-MM-DD.md` |
| 중요한 선택과 근거 | `decisions/` |
| 다음 agent를 위한 인계 | `records/handoff.md` |
| 탐색 링크와 현재 상태 | `00_Project_Index.md` |

## 반드시 기록해야 하는 상황

1. 새로운 분석 아이디어나 가정을 제안했을 때
2. fit strategy, selection, binning, weight, systematic variation을 변경했을 때
3. acceptance, efficiency, ctau 또는 prompt/nonprompt 정의를 변경했을 때
4. 에러 원인과 해결 방법을 확인했을 때
5. 실행 명령, 입력 파일, 출력 파일명 또는 histogram key가 변경됐을 때
6. 기존 결과와 차이가 발생했거나 결과 해석이 바뀌었을 때
7. TODO 또는 다음 검증 단계가 정해졌을 때
8. 다른 agent가 알아야 할 미완료 상태가 생겼을 때

## 기본 기록 형식

`records/dev_log.md`, `analysis_notes.md`, `todo.md`에 누적 기록을 추가할 때는 아래 형식을 사용한다. `records/dev_log.md`, `records/daily/`, `records/handoff.md`의 서술과 제목은 모두 한국어로 작성한다.

```markdown
### 날짜
YYYY-MM-DD HH:MM

### 작업 맥락
현재 작업의 배경과 대상 파일을 2~3문장으로 설명한다.

### 변경 사항
변경하거나 수행한 내용을 파일명, 함수명, 옵션과 함께 구체적으로 적는다.

### 변경 이유
왜 이 변경 또는 검증이 필요한지 적는다.

### 결과
실행 결과, 확인된 사실, 미검증 사항을 구분해 적는다.

### 다음 작업
1. 다음 작업
2. 추가 검증
3. 필요 시 비교 또는 결정
```

## 주제별 노트 작성 규칙

`notes/<topic>.md`는 날짜순 로그가 아니라 해당 주제의 현재 지식을 정리하는 문서다.

권장 구조:

```markdown
# Topic

## 목적 또는 정의

## 현재 분석 설정

## 주요 결과 및 관찰

## 물리적 해석

## 구현 및 관련 코드

## 미해결 문제

## 관련 결정

## 날짜별 상세 기록
```

작업 후 새로운 결과가 기존 주제 노트와 관련되면 다음을 수행한다.

1. 주제 노트의 핵심 결과 또는 설정을 갱신한다.
2. 상세 경과는 `records/daily/YYYY-MM-DD.md` 또는 기존 원본 로그에 남긴다.
3. Obsidian 링크를 사용한다.

예:

```markdown
- [[notes/Efficiency]]
- [[notes/pT_reweighting]]
- [[decisions/Efficiency_decisions]]
- [[records/daily/2026-06-16]]
```

## 일일 기록 작성 규칙

중요한 작업을 수행한 날에는 `records/daily/YYYY-MM-DD.md`를 생성하거나 갱신한다.

포함할 내용:

- 오늘 수행한 작업
- 수정한 파일
- 실행한 명령
- 주요 결과
- 문제점 또는 미검증 사항
- 다음 작업
- 관련 주제 및 decision 링크

동일 날짜 파일이 이미 있으면 새 파일을 만들지 말고 기존 파일에 이어서 추가한다.

## Decision 기록 규칙

다음과 같이 나중에 “왜 이렇게 했는가?”가 중요해질 선택은 `decisions/`에 기록한다.

- pT weight 적용 여부와 입력 파일 선택
- acceptance/efficiency 정의
- ctau cut 및 prompt/nonprompt 분리 방법
- binning 또는 rapidity/centrality 구간
- fit model 및 systematic variation
- baseline 산출물 또는 운영 버전 채택
- fallback 사용 여부

Decision 기록에는 최소한 다음을 포함한다.

```markdown
## 결정

## 배경

## 검토한 대안

## 선택 근거

## 결과에 미치는 영향

## 검증 상태

## 관련 노트
```

단순 리팩토링이나 일회성 디버깅은 decision으로 만들지 않고 `records/dev_log.md`에 기록한다.

## 연구 프로젝트 맞춤 규칙

- Charmonium, J/psi, psi(2S), prompt/nonprompt, RAA, double ratio, v2 관련 해석은 `analysis_notes.md` 및 관련 `notes/`에 남긴다.
- ROOT macro, Python, Bash, batch job, CRAB3, branch schema 및 환경 문제는 `records/dev_log.md`에 남긴다.
- systematic uncertainty, fit model variation, sideband, acceptance, efficiency, ctau 관련 변경은 반드시 기록한다.
- 결과가 통계적 fluctuation인지, 분석 정의 차이인지, 코드 오류인지 구분해 기술한다.
- ROOT 실행이 불가능한 환경에서는 “코드 검토 완료”와 “런타임 검증 완료”를 구분한다.
- 수치나 histogram 결과를 비교할 때 입력 파일, state, rapidity, pT, centrality 및 weight 옵션을 함께 남긴다.

## Codex 행동 지침

- 작업을 시작하기 전에 저장소 상태와 최신 기록을 확인한다.
- 다른 agent의 미커밋 변경을 발견하면 먼저 내용을 확인하며 임의로 덮어쓰지 않는다.
- 큰 변경은 작은 단위로 나누고 각 단계에서 검증한다.
- 사용자가 중요한 분석 변경을 요청하면 코드와 문서를 함께 갱신한다.
- 새 주제가 아니라면 불필요하게 새 Markdown 파일을 만들지 말고 기존 주제 노트를 갱신한다.
- 코드 수정 후 가능한 범위에서 syntax, compile, runtime 또는 output sanity check를 수행한다.
- 실행할 수 없는 검증은 수행한 것처럼 쓰지 않는다.
- ROOT, Python, Bash 명령은 재실행 가능하도록 그대로 기록한다.
- 에러 메시지는 원인을 판별할 수 있는 핵심 부분을 남긴다.

## Multi-agent 협업 규칙

이 프로젝트는 Codex와 Claude Code가 같은 repository에서 작업할 수 있다.

### 작업 시작 전

반드시 다음을 확인한다.

```bash
git status
git diff --stat
```

그리고 다음 파일을 읽는다.

1. `AGENTS.md`
2. `CLAUDE.md`
3. `00_Project_Index.md`
4. `records/handoff.md`
5. `todo.md`
6. `analysis_notes.md`
7. `records/dev_log.md`
8. 작업과 관련된 `notes/` 및 `decisions/` 문서
9. 필요하면 최신 `records/daily/` 문서

다른 agent가 수정한 변경사항이 있으면 `git diff`로 확인한다. 충돌 가능성이 있는 파일은 임의로 덮어쓰지 않는다.

### 작업 중

- 같은 파일을 동시에 크게 수정하지 않는다.
- 다른 agent가 편집 중일 가능성이 있는 파일은 변경 범위를 최소화한다.
- 물리·분석 전략 변경과 단순 코드 수정의 기록 위치를 구분한다.
- 불확실한 분석 결정은 바로 baseline에 반영하지 말고 가정과 대안을 먼저 기록한다.
- 기존 `records/daily/`, `notes/`, `decisions/` 내용을 중복 생성하지 않는다.
- Syncthing 충돌 파일(`*.sync-conflict-*`)이 있으면 먼저 내용을 비교하고 해결한다.

### 작업 종료 전

다음을 확인하고 기록한다.

1. 변경한 파일
2. 실행한 명령
3. 확인한 결과
4. 수행하지 못한 검증
5. 남은 문제
6. 다음 권장 작업
7. 관련 주제 노트·daily·decision 갱신 여부

기록 위치:

- 코드/에러/환경: `records/dev_log.md`
- 분석/물리 해석: `analysis_notes.md`, `notes/`
- 남은 작업: `todo.md`
- 날짜별 요약: `records/daily/YYYY-MM-DD.md`
- 중요한 결정: `decisions/`
- 인계: `records/handoff.md`
- 탐색 구조가 바뀐 경우: `00_Project_Index.md`

## 인계 기록 형식

작업을 마치거나 중간에 멈출 때 `records/handoff.md`를 다음 형식으로 갱신한다.

```markdown
### 인계

#### 완료한 작업
- 완료한 작업

#### 변경 사항d files
- 수정한 파일

#### 실행한 명령
```bash
# 실행한 명령
```

#### 결과
- 확인된 결과

#### 수행하지 못한 검증
- 수행하지 못한 검증과 이유

#### 남은 문제
- 남은 문제

#### 권장 다음 작업
- 다음 권장 작업

#### Claude/User 참고
- 다음 작업자가 알아야 할 사항
```

## Git 및 Syncthing 주의사항

- 이 repository는 공용 Git 저장소이면서 개인 Obsidian Vault로 동기화될 수 있다.
- Obsidian 전용 파일이 Git에 올라가면 안 되는 환경에서는 `.git/info/exclude` 상태를 확인한다.
- 이미 Git이 추적 중인 파일은 `.git/info/exclude`로 숨겨지지 않는다.
- `.obsidian/workspace*.json`, Syncthing conflict 파일 및 로컬 임시 파일을 실수로 commit하지 않는다.
- 같은 Markdown 파일을 여러 장치 또는 agent에서 동시에 수정하지 않는다.
- 파일 수정 전후 Syncthing 상태가 안정화됐는지 확인하는 것이 좋다.

## 출력 스타일

- 불필요하게 장황하게 쓰지 않는다.
- 핵심 결과, 변경 파일, 검증 상태, 다음 작업 순서로 정리한다.
- 재현 가능한 파일명, 명령, 파라미터 및 출력 위치를 우선한다.
- 미확인 사항은 명확히 표시한다.
