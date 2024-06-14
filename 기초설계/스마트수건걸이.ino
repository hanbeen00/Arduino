#include <DHT.h> // 온습도센서 라이브러리 사용
#define DHTPIN A1 // DHT 센서의 핀 번호를 A1으로 설정
#define DHTTYPE DHT11 // 사용되는 DHT 센서 타입을 DHT11으로 설정
DHT dht(DHTPIN, DHTTYPE); // DHT 객체를 생성하고 핀과 타입을 설정

#include <Servo.h> // 서보모터 라이브러리 사용
Servo myservo; // 서보모터 객체를 myservo로 생성

#include <Wire.h> // I2C 통신을 위한 라이브러리
#include <LiquidCrystal_I2C.h> // LCD 1602 I2C용 라이브러리
LiquidCrystal_I2C lcd(0x3F, 16, 2); // LCD 객체 생성, 주소 0x3F 또는 0x27, 크기 16x2

int relaypin = 6; // 릴레이 핀 번호를 6으로 설정
int relaypinx = 7; // 릴레이 핀 번호를 7로 설정
int trig = 13; // 초음파 센서 트리거 핀 번호를 13으로 설정
int echo = 12; // 초음파 센서 에코 핀 번호를 12로 설정

void setup() {
  pinMode(relaypin, OUTPUT); // 릴레이 핀을 출력 모드로 설정
  pinMode(trig, OUTPUT); // 트리거 핀을 출력 모드로 설정
  pinMode(echo, INPUT); // 에코 핀을 입력 모드로 설정
  myservo.attach(9); // 서보모터를 9번 핀에 연결
  Serial.begin(9600); // 시리얼 통신 속도를 9600으로 설정
}

void loop() {
  int h = dht.readHumidity(); // 습도 값을 읽어서 변수 h에 저장
  int t = dht.readTemperature(); // 온도 값을 읽어서 변수 t에 저장
  long duration, distance; // 거리 측정을 위한 변수 선언

  // 트리거 핀으로 10us 동안 펄스 출력
  digitalWrite(trig, LOW); // 트리거 핀 LOW로 설정
  delayMicroseconds(2); // 2마이크로초 지연
  digitalWrite(trig, HIGH); // 트리거 핀 HIGH로 설정
  delayMicroseconds(10); // 10마이크로초 지연
  digitalWrite(trig, LOW); // 트리거 핀 LOW로 설정

  // 에코 핀에서 펄스 지속 시간 읽기
  duration = pulseIn(echo, HIGH);  
  distance = duration * 170 / 1000 / 10; // 거리 계산 (cm 단위)
  
  // 시리얼 모니터에 거리 출력
  Serial.print("거리: ");
  Serial.print(distance); 
  Serial.println("cm");

  if (distance >= 30) { // 거리가 30cm 이상일 때
    Serial.print("humidity:"); 
    Serial.println(h); // 시리얼 모니터에 습도 출력
    
    lcd.init(); // LCD 초기화
    lcd.backlight(); // 백라이트 켜기
    lcd.setCursor(0, 0); // 첫 번째 줄 첫 번째 위치로 커서 이동
    lcd.print("Working!"); // "Working!" 출력
    lcd.setCursor(0, 1); // 두 번째 줄 첫 번째 위치로 커서 이동
    lcd.print("humidity: "); // "humidity: " 출력
    lcd.print(h); // 습도 값 출력
    
    digitalWrite(relaypinx, LOW); // 릴레이 핀 LOW로 설정 (LED 바)
    myservo.write(180); // 서보모터를 180도로 회전 (역방향 최고 속도)

    if (h < 50) { // 습도가 50% 미만일 때
      digitalWrite(relaypin, HIGH); // 릴레이 핀 HIGH로 설정
    } else {
      digitalWrite(relaypin, LOW); // 릴레이 핀 LOW로 설정
    }
  } else { // 거리가 30cm 미만일 때
    lcd.init(); // LCD 초기화
    lcd.backlight(); // 백라이트 켜기
    lcd.setCursor(0, 0); // 첫 번째 줄 첫 번째 위치로 커서 이동
    lcd.print("Pause!"); // "Pause!" 출력
    myservo.write(90); // 서보모터 정지 (90도)
    digitalWrite(relaypin, HIGH); // 릴레이 핀 HIGH로 설정
  }
  
  delay(500); // 0.5초 지연
}
