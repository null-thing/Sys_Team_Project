### check list
**만약 문장으로 받는거 안되면 그냥 번호로 받아서 하자** <br>
- 걍 번호로 받자 ㅋㅋ...
- [ ] 소켓통신 구현함?
- [ ] 앞에서 오는 정보들을 받아낼수 있는가?
- [ ] 사진을 저장할 수 있는가?
- 도어 인풋 페이지
- [x] 1. 도어개수를 잘 묻고 입력한 도어개수값을 잘 받는가?
- [x] 1-1. 10진수 아닌 값이나 너무 큰값 받을경우 처리하는가?
- 도어 인터페이스 메인페이지
- [x] 2. 입력한 도어개수에 맞춰서 도어 인터페이스를 구현했는가?
- [x] 3. 각 도어의 텍스트 input을 읽었는가?
- [x] 4. 각 도어의 인터페이스 위치에 텍스트 input값을 넣었는가? (input값 업는 놈은 not found처리 했는가?)
- [x] 5. 일정 간격마다 업데이트 되는가?
- [x] 6. 히스토리를 물어보는 요청을 받을수 있는가?
- [ ] 6-1. 읽은 파일들을 히스토리에 집어 넣을 수 있는가? (일정 수를 넘으면 과거 값이 지워지고 새로운 값이 들어가는 대충 circular queue형태를 띄는가?)
- 도어 히스토리 페이지
- [x] 7. 히스토리 요청에따라 어떤 도어를 열지를 알수 있는가?
- [x] 8. 원하는 도어의 히스토리를 보여줄수 있는가?
- [x] 9. 히스토리 페이지에서 다시 도어 인터페이스 페이지로 넘어갈수 있는가?
- [ ] 10. 히스토리 페이지가 열려있는 동안에도 계속해서 door 정보도 받아지는가?
<br>
- [ ] ex. 문 이름 지정 가능하게 가능?, 기본 cli에서 원하는 문 삭제, 추가 가능?
- [ ] ex. cli글자들의 색을 바꿀수 있는가?
- [ ] ex. cli에서 받아오는 입력들로 컴퓨터에 alert창을 띄울수 있는가?(띠롱 하고 울리면서 나오는거)

### 지금까지 구현해놓은 것들
1. 켰을떄 door num개수 물어보는 화면
2. door 개수 받아서 현재 상황 5초마다 갱신해주는 화면 (원하는 도어 개수만큼 모니터링 해줌)
3. 

### to do list (앞으로 해야하는 것들)
1. 통신해서 정보들 받아오는거 구현하기(받아오면 그냥 txt파일로 만들고 이후에 그거 읽어서 띄워줄려고 그냥, 이게 확실히 편함)
2. 사진을 따로 다른 폴더에 저장하는거 구현하기 (문이 열린 경우에만)
* 나머지는 그렇다 치는데 이거 2개가 어려워서 최대한 빨리 해결해야함
3. 메인 cli에 다양한 기능들 추가
    - cli 글자 색 바꿀수 있는지
    - cli 입력 바뀌면 경고창을 띄울수 있는지
    - 원하는 도어만 볼수 있는지 (도어 추가, 삭제기능)

## function introduction

### start_door_num - 구현완.
- start section
- receive total door num that user wants

### current_door_cli - history txt 파일에 정보 넣는거만 하면됨
- main section
- monitoring door situation per two seconds
- if user want to see history, receive history command

### history_reader - 이게 켜져있는순간동안 cli가 계속해서 업데이트 하면서 history에 값은 계속 추가하도록 해야함, 일단 나와있는 파일을 읽는것은 가능
- parameter: door number (int)
- return: none
- function: read {n}door history, show history on the cli 
- if {n}door history not found, print history not found and break
- remain infinitely if there is no quit signal




