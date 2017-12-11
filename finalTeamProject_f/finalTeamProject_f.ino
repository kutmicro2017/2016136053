#include <SevSeg.h>
#include <math.h>
//-------------온도 계산을 위한 상수---------------------------
const int B = 4275;               // B value of the thermistor
const int R0 = 100000;            // R0 = 100k
//-----------------------------------------------------------

//--------------------설정을 위한 상수------------------------
const int oneTouchInterval = 300; //1번 터치로 인정하는 시간
const int modeSetInterval = 3500; // 타이머 모드 설정 터치로 인정하는 시간 ~ modeSetInterval
const int powerSetInterval = 4000; //전원 설정 터치로 인정하는 시간 powerSetInterval ~ 
const int timeSetInterval = 5000; //타이머 설정 후 기다리는 시간
const int temDisplayTime = 5000; //온도 표시하는 시간
const int unitTime = 5; //1번 터치마다 올라가는 시간
const int MaxTime = 15 + unitTime; //설정 가능 최대 시간
//-----------------------------------------------------------

//-------------핀 번호---------------------------------------
const int pinTempSensor = A5; //온도 센서
const int touchSensor = 2; //터치 센서
const int led = 13;
//-----------------------------------------------------------

//------------------세븐세그먼트 디스플레이--------------------
SevSeg myDisplay;
byte numDigits = 4;
byte digitPins[] = {14, 15, 16, 17};
byte segmentPins[] = {23, 25, 27, 29, 31, 33, 35, 37};
//-----------------------------------------------------------

//-----------------모드 변경 변수-----------------------------
volatile bool state = false; //버튼이 눌렸는지 판단
volatile bool temMode = false; //온도 디스플레이 모드
volatile bool timerMode = false; //타이머 모드
volatile bool powerSet = false; //전원 on/off
volatile bool timeSet = false;
//-----------------------------------------------------------

//-------------------설정용 변수------------------------------
volatile int setTime = 0; //타이머 시간 설정
volatile unsigned long touchStart, touchEnd; //터치 간격 저장
volatile unsigned long settingStart, settingEnd; // 타이머 시간 설정 간격 조정
//-----------------------------------------------------------


void setup() {
  bool resistorsOnSegments = false;
  myDisplay.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, resistorsOnSegments);
  myDisplay.setBrightness(100);
  //---------------(이 위로는 세븐세그먼트 라이브러리 사용한 것)----------------------------
  pinMode(led, OUTPUT);
  attachInterrupt(0, readTouch, CHANGE); //터치센서가 눌렸다 떨어질 때 두 번 호출
}

void loop() {
  if (powerSet == true) {
    if (digitalRead(led) == HIGH ) { //전원이 켜있으면
      digitalWrite(led, LOW); //끔
    }
    else { //전원이 꺼있으면
      digitalWrite(led, HIGH); //켬
    }
    powerSet = false;
  }
  if (temMode) { //온도 표시
    int temperature = (int)checkTemp();
    unsigned long start, End;
    start = End = millis();
    while (End - start <= temDisplayTime) {
      myDisplay.setNumber(temperature);
      myDisplay.refreshDisplay();
      End = millis();
    }
    clearLEDs();
    temMode = false;
  }
  //--------------------타이머 모드 시작---------------------------------------
  if (timerMode) {
    int num;
    settingStart = settingEnd = millis();
    while (settingEnd - settingStart <= timeSetInterval) { //시간 설정 후 5초간 대기
      myDisplay.setNumber(setTime);
      myDisplay.refreshDisplay();
      settingEnd = millis();
    }
    timeSet = false;
    for (num = setTime * 60 ; num >= 0; num--) { //설정한 시간동안 반복
      unsigned long timerStart, timerEnd;
      if(num != setTime * 60 && num%60 == 0) setTime--;
      timerStart = timerEnd = millis();
      while (timerEnd - timerStart <= 999) {
        myDisplay.setNumber(setTime);
        myDisplay.refreshDisplay();
        timerEnd = millis();
      }
      if (!timerMode) { //타이머 모드가 false가 되면
        clearLEDs();
        setTime = 0;
        break;
      }
      if (temMode) { //온도 표시 설정 하면
        clearLEDs();
        int temperature = (int)checkTemp();
        unsigned long start, End;
        start = End = millis();
        while (End - start <= temDisplayTime) {
          myDisplay.setNumber(temperature);
          myDisplay.refreshDisplay();
          End = millis();
        }
        clearLEDs();
        temMode = false;
        num = num - (int)(temDisplayTime/1000);
      }
      if (powerSet == true) {
        if (digitalRead(led) == HIGH ) { //전원이 켜있으면
          digitalWrite(led, LOW); //끔
          clearLEDs();
          powerSet = false;
          timerMode  = false;
          setTime = 0;
          break;
        }
      }
    }
    if (num == -1) {
      digitalWrite(led, LOW);
      clearLEDs();
      setTime = 0;
      timerMode  = false;
    }
  }
  //--------------------타이머 모드 끝---------------------------------------
}

float checkTemp() { //온도 반환해주는 함수
  int a = analogRead(pinTempSensor);
  float R = 1023.0 / a - 1.0;
  R = R0 * R;
  float temperature = 1.0 / (log(R / R0) / B + 1 / 298.15) - 273.15; // convert to temperature via datasheet
  return temperature;
}

void clearLEDs() {   //세븐 세그먼트 전체 off
  digitalWrite(23, LOW);
  digitalWrite(25, LOW);
  digitalWrite(27, LOW);
  digitalWrite(29, LOW);
  digitalWrite(31, LOW);
  digitalWrite(33, LOW);
  digitalWrite(35, LOW);
  digitalWrite(37, LOW);
}

void readTouch() {
  if (digitalRead(touchSensor) == 1) { //터치 센서가 눌렸을 때
    touchStart = millis();
    state = true;
    return 0;
  }
  
  if (digitalRead(touchSensor) == LOW && state) { //손을 뗐을 때
    touchEnd = millis();
    if ((touchEnd - touchStart) <= oneTouchInterval && digitalRead(led) == HIGH) { //1번 터치
      if (timeSet == false) {
        temMode = true;
      }
      else if (timeSet == true) {
        setTime = (setTime + unitTime) % (MaxTime);
        state = false;
        settingStart = millis();
      }
      state = false;
    }
    else if ((touchEnd - touchStart) > oneTouchInterval && (touchEnd - touchStart) <= modeSetInterval && digitalRead(led) == HIGH) { //타이머 모드 터치
      if(timerMode == false){
        timerMode = true;
        timeSet = true;
      }
      else {
        timerMode = false;
      }
      state = false;
    }
    
    else if ((touchEnd - touchStart) > powerSetInterval) { //전원설정 터치
      powerSet = true;
      state = false;
    }
    return 0;
  }
}
