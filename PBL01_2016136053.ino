// 4거리 3색신호등,,Led가 12개.. 각 번호의 디지털 핀에 연걸
#define LED1_R 12
#define LED1_G 11
#define LED1_Y 10 //상
#define LED2_R 26
#define LED2_G 28
#define LED2_Y 30//하
#define LED3_R 6
#define LED3_G 5
#define LED3_Y 4//좌
#define LED4_R 39
#define LED4_G 41
#define LED4_Y 43 //우
#define buttonA 2
const long interval = 60000; //60초
volatile bool state = true; //스위치가 눌렸는지를 판단하는 변수

void setup() {
  
  pinMode(LED1_R, OUTPUT);
  pinMode(LED1_G, OUTPUT);
  pinMode(LED1_Y, OUTPUT);
  pinMode(LED2_R, OUTPUT);
  pinMode(LED2_G, OUTPUT);
  pinMode(LED2_Y, OUTPUT);
  pinMode(LED3_R, OUTPUT);
  pinMode(LED3_G, OUTPUT);
  pinMode(LED3_Y, OUTPUT);
  pinMode(LED4_R, OUTPUT);
  pinMode(LED4_G, OUTPUT);
  pinMode(LED4_Y, OUTPUT);
  pinMode(buttonA, INPUT_PULLUP); //기본값이 HIGH
  attachInterrupt(0,tTof, FALLING); //인터럽트0 은 핀 2번, 따라서 스위치를 핀2에 연결, 스위치가 눌리면 실행
}                               //인터럽트 핀의 값이 LOW에서 HIGH로 바뀔 때 함수 실행

void loop() {
  
  // 사거리 신호등을 상 1 하 2 좌 3 우 4 라 하면 1,2가 같이 움직이고 3,4가 같이 움직임
  //빨강(1분 이내 delay) ->녹색(1분 이내) -> 노랑(10초동안 1초간격으로 점멸) -> 빨강 -> 무한루프
  //보행자 버튼 (3초 후에 노랑 -> 빨강)
digitalWrite(LED1_R, HIGH); digitalWrite(LED2_R, HIGH); //상하 신호등이 빨간색
digitalWrite(LED3_G, HIGH); digitalWrite(LED4_G, HIGH); //좌우 신호등이 초록색
delay(60000); //1분간 대기
digitalWrite(LED3_G,LOW); digitalWrite(LED4_G, LOW); //신호를 끈다
for(int delaytime=0; delaytime <10; delaytime++){
digitalWrite(LED3_Y,HIGH); digitalWrite(LED4_Y, HIGH); //노란 신호가 들어옴
delay(500);  
digitalWrite(LED3_Y,LOW); digitalWrite(LED4_Y, LOW);
delay(500); // 0.5+0.5 해서 1초간 깜빡인다
  } 
  
digitalWrite(LED1_R, LOW); digitalWrite(LED2_R, LOW);
digitalWrite(LED1_G, HIGH); digitalWrite(LED2_G, HIGH); //상하 신호등이 초록색
digitalWrite(LED3_R, HIGH); digitalWrite(LED4_R, HIGH); //좌우 신호등이 빨간색
unsigned long previousMillis = millis();
while(millis() - previousMillis <= interval) { //현재 시각과 이전 시각의 차이가 60초가 될 때까지 반복
  //시간 지연 동안에 다른 동작을 해야하므로 delay함수는 사용 못함
 if(state == false) { //스위치가 눌렸으면
   delay(3000); //3초간 대기
    digitalWrite(LED1_G,LOW); digitalWrite(LED2_G, LOW); // 초록불을 꺼준다
    for(int delaytime=0; delaytime <10; delaytime++){ //10초간
    digitalWrite(LED1_Y,HIGH); digitalWrite(LED2_Y, HIGH); //노란 신호가 들어옴
    delay(500);
    digitalWrite(LED1_Y,LOW); digitalWrite(LED2_Y, LOW); //노란 신호를 끔
    delay(500); //1초 간격으로 깜빡임
    } 
    digitalWrite(LED1_R,HIGH); digitalWrite(LED2_R, HIGH); //빨간 신호를 켠다
    state = true; //동작이 끝났으니 변수를 원래 상태로 돌려줌
    delay(30000);
    digitalWrite(LED1_R,LOW); digitalWrite(LED2_R, LOW);
    digitalWrite(LED1_G, HIGH); digitalWrite(LED2_G, HIGH); //다시 초록 신호를 켜준다
    previousMillis = millis() - 34000; //빨간 신호를 켜서 딜레이 된 만큼 지연 시간을 늘려줌
 }
}
digitalWrite(LED1_G, LOW); digitalWrite(LED2_G, LOW); //신호를 끈다
for(int delaytime=0; delaytime <10; delaytime++){
digitalWrite(LED1_Y,HIGH); digitalWrite(LED2_Y, HIGH); //노란 신호가 들어옴
delay(500);
digitalWrite(LED1_Y,LOW); digitalWrite(LED2_Y, LOW); //노란 신호를 끔
delay(500);
  }
//다시 상하는 빨간색 좌우는 초록색으로 반복
digitalWrite(LED3_R, LOW); digitalWrite(LED4_R, LOW);
}

void tTof(){ //스위치가 눌렸을 때 실행되는 함수
 if(digitalRead(LED1_G) == HIGH) state = false; //스위치가 눌리면 state의 값을 바꿔준다
}

//스위치를 누르면 인터럽트를 통해 상태를 바꿔주고 루프문에서 바뀐 상태를 가지고 조건문 실행
