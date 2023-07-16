#include <DHT.h> //온습도센서 라이브러리 사용
#define DHTPIN A1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#include <Servo.h> // 서보모터 라이브러리 사용
Servo myservo;  // 서보모터 객체를 myservo로 생성

#include <Wire.h>                        // i2C 통신을 위한 라이브러리
#include <LiquidCrystal_I2C.h>        // LCD 1602 I2C용 라이브러리
LiquidCrystal_I2C lcd(0x3F,16,2);     // 접근주소: 0x3F or 0x27

int relaypin=6;
int relaypinx=7;
int trig=13; int echo=12;

void setup() {
  pinMode(relaypin, OUTPUT);
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
   myservo.attach(9);  // 서보모터 9번핀에 연결
  Serial.begin(9600); // 통신 속도 설정
}
void loop() {
  int h = dht. readHumidity();  int t = dht.readTemperature();
  long duration, distance;    // 거리 측정을 위한 변수 선언
   // 트리거 핀으로 10us 동안 펄스 출력
   digitalWrite(trig, LOW);        // Trig 핀 Low
   delayMicroseconds(2);            // 2us 딜레이*/
   digitalWrite(trig, HIGH);    // Trig 핀 High
   delayMicroseconds(10);            // 10us 딜레이
   digitalWrite(trig, LOW);        // Trig 핀 Low1
   duration = pulseIn(echo, HIGH);  
   distance = duration * 170 / 1000/10;
   Serial.print("거리: ");
   Serial.print(distance); // 거리를 시리얼 모니터에 출력
   Serial.println("cm");


  if(distance>= 30){
    Serial.print("humidity:"); Serial.println(h);
    lcd.init();                      // LCD 초기화
    // Print a message to the LCD.
    lcd.backlight();                // 백라이트 켜기
    lcd.setCursor(0,0);             // 1번째, 1라인
    lcd.print("Working!");
    lcd.setCursor(0,1);             // 1번째, 2라인
    lcd.print("humidity: ");
    lcd.print(h);
     digitalWrite(relaypinx,LOW); // led바 릴레이
    myservo.write(180); // 역방향으로 최고속도 회전

    if(h<50){
      digitalWrite(relaypin,HIGH);
    }
    else{
       digitalWrite(relaypin,LOW);  
    }
  }
  else{
    lcd.init();                      // LCD 초기화
    // Print a message to the LCD.
    lcd.backlight();                // 백라이트 켜기
    lcd.setCursor(0,0);             // 1번째, 1라인
    lcd.print("Pause!");
    myservo.write(90); // 정지
    digitalWrite(relaypin,HIGH);
  }
  delay(500); 
}
