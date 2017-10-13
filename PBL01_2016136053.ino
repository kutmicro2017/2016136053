// 4거리 3색신호등,,Led가 12개.. 각 번호의 디지털 핀에 연걸
#define LED1_R 12
#define LED1_Y 11
#define LED1_G 10 //상
#define LED2_R 26
#define LED2_Y 28
#define LED2_G 30//하
#define LED3_R 6
#define LED3_Y 5
#define LED3_G 4//좌
#define LED4_R 39
#define LED4_Y 41
#define LED4_G 43 //우
#define buttonA 2

#define DelayTime 40000 //40초 초록, 빨간 불이 켜있는 시간
#define YellowBlinkTime 10000 //10초 노란불이 깜빡이는 시간
#define PedestrianDelayTime 3000 //3초 보행자 신호로 넘어갈 때 기다리는 시간
#define PedestrianMovingTime 20000 //20초 보행자 신호가 켜있는 시간

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
}                               //인터럽트 핀의 값이 HIGH에서 LOW로 바뀔 때 함수 실행

void loop() {
  // 사거리 신호등을 상 1 하 2 좌 3 우 4 라 하면 1번 초록불 -> 2번 -> 3번 -> 4번 -> 반복
  //녹색 -> 노랑 -> 빨강 -> 녹색->무한루프
  //신호가 초록일때 보행자 버튼 (3초 후에 노랑 -> 빨강)
digitalWrite(LED2_R, HIGH); digitalWrite(LED3_R, HIGH); digitalWrite(LED4_R, HIGH); 
digitalWrite(LED1_R, LOW); digitalWrite(LED1_G, HIGH); //위쪽 신호등만 초록색
delay(DelayTime); //40초간 켜줌
digitalWrite(LED1_G, LOW);
for(int delaytime=0; delaytime <(YellowBlinkTime/1000); delaytime++){// 초록 -> 빨강 될때 황색으로 경고
digitalWrite(LED1_Y, HIGH);
delay(500);  
digitalWrite(LED1_Y, LOW);
delay(500); 
  } 
digitalWrite(LED1_G, LOW);
// ---------------------------------------------------1번(상)만 초록
digitalWrite(LED1_R, HIGH); 
digitalWrite(LED2_R, LOW); digitalWrite(LED2_G, HIGH); //아래쪽 신호등만 초록색
for(int count=0; count<1000;count++){
 if(state == false) { //스위치가 눌렸으면
    delay(PedestrianDelayTime); //3초간 대기
    digitalWrite(LED2_G, LOW); //초록불을 꺼준다
    for(int delaytime=0; delaytime <10; delaytime++){ //10초간
    digitalWrite(LED2_Y, HIGH); //노란 신호가 들어옴
    delay(500);
    digitalWrite(LED2_Y, LOW); //노란 신호를 끔
    delay(500); //1초 간격으로 깜빡임
    } 
    digitalWrite(LED2_R, HIGH); //빨간 신호를 켠다
    delay(PedestrianMovingTime);
    state = true; //동작이 끝났으니 변수를 원래 상태로 돌려줌
    digitalWrite(LED2_R, LOW);
    digitalWrite(LED2_G, HIGH); //다시 초록 신호를 켜준다
 }
 if(count == (DelayTime/1000)) break;
 delay(1000);
}
digitalWrite(LED2_G, LOW);
for(int delaytime=0; delaytime <(YellowBlinkTime/1000); delaytime++){
digitalWrite(LED2_Y, HIGH);
delay(500);  
digitalWrite(LED2_Y, LOW);
delay(500); // 초록 -> 빨강 될때 황색으로 경고
  } 
digitalWrite(LED2_G, LOW);
// ---------------------------------------------------2번(하) 초록 보행자신호 넣어주기
digitalWrite(LED2_R, HIGH); 
digitalWrite(LED3_R, LOW); digitalWrite(LED3_G, HIGH); //위쪽 신호등만 초록색
delay(DelayTime); //40초간 켜줌
digitalWrite(LED3_G, LOW);
for(int delaytime=0; delaytime <(YellowBlinkTime/1000); delaytime++){
digitalWrite(LED3_Y, HIGH);
delay(500);  
digitalWrite(LED3_Y, LOW);
delay(500); // 초록 -> 빨강 될때 황색으로 경고
}
digitalWrite(LED3_G, LOW);
// ---------------------------------------------------3번(좌) 초록
digitalWrite(LED3_R, HIGH); 
digitalWrite(LED4_R, LOW); digitalWrite(LED4_G, HIGH); //위쪽 신호등만 초록색
delay(DelayTime); //40초간 켜줌
digitalWrite(LED4_G, LOW);
for(int delaytime=0; delaytime <(YellowBlinkTime/1000); delaytime++){
digitalWrite(LED4_Y, HIGH);
delay(500);  
digitalWrite(LED4_Y, LOW);
delay(500); // 초록 -> 빨강 될때 황색으로 경고
}
digitalWrite(LED4_G, LOW);
// ---------------------------------------------------4번(우) 초록
}

void tTof(){ //스위치가 눌렸을 때 실행되는 함수
 if(digitalRead(LED2_G) == HIGH) state = false; //스위치가 눌리면 state의 값을 바꿔준다
}

//스위치를 누르면 인터럽트를 통해 상태를 바꿔주고 루프문에서 바뀐 상태를 가지고 조건문 실행
