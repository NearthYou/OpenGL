## SDL 게임 프로젝트
- OpenGL 및 SDL 라이브러리를 활용한 게임 구조 학습 및 저수준 시스템 구현 경험 확보
- Snake, Tank, Physics Code 폴더에 코드, x64/ 폴더에 실행 파일 존재

## 공통 기술
- SDL 기반 게임 루프, 키보드·마우스 입력, 충돌 처리, 상태 전이 구현

### 1. Snake Game
- 방향키 이동, 먹이 먹기, 충돌 시 게임 종료
- 상태 전이 기반 진행 구조

### 2. Tank Game
- 방향키 이동 및 방향대로 포탄 발사
- 포탄 수 제한, 주워야 재장전
- 탱크에서 내려 도보 이동 가능

### 3. Parabola
- 마우스 드래그로 발사 방향·힘 설정
- 중력, 반발력, 마찰력 적용
- 예측 궤적 시각화 및 구조물 충돌 반응
