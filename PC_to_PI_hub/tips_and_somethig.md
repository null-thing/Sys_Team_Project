# This program is server in Both socket communication !!!
## Communicate with this program as client

* 보내는 정보 : port 8011
- 문이 열림/닫힘
- 불이 켜짐/꺼짐
- 사진

1. 몇 개의 door가 연결되었는지 : 받고 나서 int로 캐스트할것.
2. 보낼 스트링의 길이  : 받고 나서 int로 캐스트할것.

3. """
(door 고유번호)\n
(문열림)\n
(불 켜짐 여부)\n
(사진 : string buffered?)\0
"""

- 1번을 받는다
- 2번에 받은 것으로 버퍼 사이즈 지정
- 3를 1번에서 받은 int를 기반으로 반복하여 파싱.
- 이후 쭉 반복


* 받는 정보 : Port 8010
- 센서로부터의 정보
- 광도 센서 정보
- 사진