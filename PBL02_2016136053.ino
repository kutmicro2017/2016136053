#define RedLED_1 12
#define YellowLED_1 11
#define GreenLED_1 10 //상하 연동 LED
#define RedLED_2 9
#define YellowLED_2 8
#define GreenLED_2 7 //좌우 연동 LED
#define buttonA 2 //스위치

#define DelayTime 10000 //10초
#define YellowBlinkTime 3000 //3초
#define PedestrianDelayTime 3000 //3초
#define PedestrianMovingTime 10000 //10초

volatile bool state = true; //스위치가 눌렸는지를 판단하는 변수

int walkSignNumber[10][8][4] = {

  { //0
    { 0, 1, 1, 0 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 0, 1, 1, 0 }
  },

  { //1
    { 0, 0, 1, 0 },
    { 0, 1, 1, 0 },
    { 1, 1, 1, 0 },
    { 0, 1, 1, 0 },
    { 0, 1, 1, 0 },
    { 0, 1, 1, 0 },
    { 0, 1, 1, 0 },
    { 1, 1, 1, 1 }
  },

  { //2
    { 0, 1, 1, 0 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 0, 0, 0, 1 },
    { 0, 0, 1, 0 },
    { 0, 1, 0, 0 },
    { 1, 0, 0, 0 },
    { 1, 1, 1, 1 }
  },

  { //3
    { 0, 1, 1, 0 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 0, 0, 0, 1 },
    { 0, 1, 1, 1 },
    { 0, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 0, 1, 1, 0 }
  },

  { //4
    { 0, 0, 1, 0 },
    { 0, 1, 1, 0 },
    { 1, 0, 1, 0 },
    { 1, 1, 1, 1 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 }
  },

  { //5
    { 1, 1, 1, 1 },
    { 1, 0, 0, 0 },
    { 1, 0, 0, 0 },
    { 1, 1, 1, 0 },
    { 0, 0, 0, 1 },
    { 0, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 0, 1, 1, 0 }
  },

  { //6
    { 0, 1, 1, 1 },
    { 1, 0, 0, 0 },
    { 1, 0, 0, 0 },
    { 1, 1, 1, 0 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 0, 1, 1, 0 }
  },

  { //7
    { 1, 1, 1, 1 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 0, 0, 0, 1 },
    { 0, 0, 1, 0 },
    { 0, 1, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 1, 0, 0 }
  },

  { //8
    { 0, 1, 1, 0 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 0, 1, 1, 0 },
    { 0, 1, 1, 0 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 0, 1, 1, 0 }
  },

  { //9
    { 0, 1, 1, 0 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 0, 1, 1, 1 },
    { 0, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 0, 1, 1, 0 }
  }
};

int signImage[2][8][8] = {
  { //stop
    { 0, 0, 0, 1, 1, 0, 0, 0},
    { 0, 0, 1, 0, 0, 1, 0, 0},
    { 0, 0, 0, 1, 1, 0, 0, 0},
    { 0, 0, 1, 1, 1, 1, 0, 0},
    { 0, 1, 0, 1, 1, 0, 1, 0},
    { 0, 0, 0, 1, 1, 0, 0, 0},
    { 0, 0, 1, 0, 0, 1, 0, 0},
    { 0, 0, 1, 0, 0, 1, 0, 0}
  },

  { //go
    { 0, 0, 0, 1, 1, 0, 0, 0},
    { 0, 0, 1, 0, 0, 1, 0, 0},
    { 0, 0, 0, 1, 1, 0, 0, 0},
    { 1, 0, 1, 1, 1, 1, 1, 0},
    { 0, 1, 0, 1, 1, 0, 0, 1},
    { 0, 0, 1, 0, 0, 1, 0, 0},
    { 0, 1, 0, 0, 0, 0, 1, 0},
    { 0, 0, 1, 0, 0, 0, 0, 1}
  }

};

int walkPedMatrixCol[] = { 40, 42, 44, 46, 48, 5, 4, 3  };
int walkPedMatrixRow[] = { 41, 43, 45, 47, 49, 14, 15, 16 }; //숫자 나타내는 도트매트릭스

int drowSignalCol[] = { 22, 24, 26, 28, 30, 32, 34, 36 };
int drowSignalRow[] = { 23, 25, 27, 29, 31, 33, 35, 37 }; //아이콘 나타내는 도트매트릭스
//보행자 신호등은 상하 신호등이 빨간색일 때만 다운카운트

//col에 0v row에 5v가 들어가야 켜짐

void YellowBlink(int pin); //노란불 점멸
void CleanMatrix();
void printNumMatrix(int num);
void printSignalMatrix(int num);
void CleanSignalMatrix();
void tTof() { //스위치가 눌렸을 때 실행되는 함수
  if (digitalRead(GreenLED_1) == HIGH || digitalRead(RedLED_1) == HIGH ) state = false; //스위치가 눌리면 state의 값을 바꿔준다
}

void setup() {
  pinMode(RedLED_1, OUTPUT);
  pinMode(YellowLED_1, OUTPUT);
  pinMode(GreenLED_1, OUTPUT);
  pinMode(RedLED_2, OUTPUT);
  pinMode(YellowLED_2, OUTPUT);
  pinMode(GreenLED_2, OUTPUT);

  for (int i = 0; i < 8; i++) { //핀모드 설정
    pinMode(walkPedMatrixRow[i], OUTPUT);
    pinMode(walkPedMatrixCol[i], OUTPUT);
  }
  for (int i = 0; i < 8; i++) { //핀모드 설정
    pinMode(drowSignalRow[i], OUTPUT);
    pinMode(drowSignalCol[i], OUTPUT);
  }
  pinMode(buttonA, INPUT_PULLUP);
  attachInterrupt(0, tTof, FALLING);
}

void loop() {
  digitalWrite(GreenLED_1, HIGH); //상하 신호등이 초록색
  digitalWrite(RedLED_2, HIGH); //좌우 신호등이 빨간색
  
  unsigned long startTime, endTime;
  startTime = endTime = millis();
  while (endTime - startTime < DelayTime) {
    printNumMatrix(DelayTime / 1000);
    printSignalMatrix(0);
    if (state == false) { //스위치가 눌렸으면
      state = true;
      unsigned int pedSignStart, pedSignEnd;
      
      for (int num = 3; num >= 0; num--) {
        pedSignStart = pedSignEnd = millis();
      while (pedSignEnd - pedSignStart < 1000 ) {
        printNumMatrix(DelayTime / 1000);
        printSignalMatrix(0);
        pedSignEnd = millis();
      }
      }
      digitalWrite(GreenLED_1, LOW); //초록불을 꺼준다
      YellowBlink(YellowLED_1);
      digitalWrite(RedLED_1, HIGH); //상하 신호등 빨간색
      digitalWrite(RedLED_2, LOW); digitalWrite(GreenLED_2, HIGH); // 좌우는 초록색
      for (int num = PedestrianMovingTime / 1000; num >= 0; num--) {
        pedSignStart = pedSignEnd = millis();
        while (pedSignEnd - pedSignStart < 1000) {
          printNumMatrix(num);
          printSignalMatrix(1);
          pedSignEnd = millis();
        }
      }
      digitalWrite(GreenLED_2, LOW);
      YellowBlink(YellowLED_2);
      digitalWrite(RedLED_1, LOW); digitalWrite(GreenLED_1, HIGH); //상하 신호 다시 초록색
      digitalWrite(RedLED_2, HIGH); //좌우 신호 다시 빨간색
      startTime = millis(); //차량 신호의 길이를 0초부터 다시 시작
    }
    endTime = millis();
  }
  digitalWrite(GreenLED_1, LOW);
  YellowBlink(YellowLED_1);
  digitalWrite(RedLED_1, HIGH); //상하 신호등이 빨간색
  digitalWrite(RedLED_2, LOW); digitalWrite(GreenLED_2, HIGH); //좌우 신호등이 초록색

  for (int num = (DelayTime / 1000); num >= 0; num--) {
    unsigned long startTime, endTime;
    startTime = endTime = millis();
    while (endTime - startTime < 1000) {
      printNumMatrix(num);
      printSignalMatrix(1);
      if(state == false){
        state = true;
        num = DelayTime/1000;
      }
      endTime = millis();
    }
  }
  digitalWrite(GreenLED_2, LOW);
  YellowBlink(YellowLED_2);
  digitalWrite(RedLED_1, LOW); //상하 빨간불 꺼줌
}

//------------------------------------------------------각종 함수

void YellowBlink(int pin) {
  unsigned long startTime, endTime;
  startTime = endTime = millis();
  while (endTime - startTime <= YellowBlinkTime) {
    printNumMatrix(DelayTime / 1000);
    printSignalMatrix(0);
    unsigned long blinkStart, blinkEnd;
    blinkStart = blinkEnd = millis();
    while (blinkEnd - blinkStart < 500) {
      printNumMatrix(DelayTime / 1000);
      printSignalMatrix(0);
      digitalWrite(pin, HIGH);
      blinkEnd = millis();
    }
    blinkStart = blinkEnd = millis();
    while (blinkEnd - blinkStart < 500) {
      printNumMatrix(DelayTime / 1000);
      printSignalMatrix(0);
      digitalWrite(pin, LOW);
      blinkEnd = millis();
    }
    endTime = millis();
  }
}


void CleanMatrix() { //도트를 모두 끈다
  for (int i = 0; i < 8; i++) {
    digitalWrite(walkPedMatrixRow[i], LOW);
    digitalWrite(walkPedMatrixCol[i], HIGH);
  }
}

void CleanSignalMatrix() {//도트를 모두 끈다
  for (int i = 0; i < 8; i++) {
    digitalWrite(drowSignalRow[i], LOW);
    digitalWrite(drowSignalCol[i], HIGH);
  }
}

void printNumMatrix(int num) { //남은 시간 출력
  for (int r = 0; r < 8; r++) {
    CleanMatrix();
    digitalWrite(walkPedMatrixRow[r], HIGH);
    for (int c = 0; c < 4; ++c) { //10의 자리
      if (walkSignNumber[num / 10][r][c]) digitalWrite(walkPedMatrixCol[c], LOW);
    }
    for (int c = 0; c < 4; ++c) { //1의 자리
      if (walkSignNumber[num % 10][r][c]) digitalWrite(walkPedMatrixCol[c + 4], LOW);
    }
    delay(1); //시간지연을 조금이라도 줘야 도트가 깨끗하게 보임
    CleanMatrix();
  }
}

void printSignalMatrix(int num) { //정지(0)인지 통행 가능(1)인지 아이콘 출력
  for (int r = 0; r < 8; r++) {
    CleanSignalMatrix();
    digitalWrite(drowSignalRow[r], HIGH);
    for (int c = 0; c < 8; ++c) {
      if (signImage[num][r][c]) digitalWrite(drowSignalCol[c], LOW);
    }
    delay(1);
    CleanSignalMatrix();
  }
}
