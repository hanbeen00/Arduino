#include <SoftwareSerial.h>

// RX, TX
SoftwareSerial BTSerial(9,8);
int ENpin = 7;
const int X = A0, Y = A1;
byte send_byte;

void setup() {
  pinMode(ENpin, OUTPUT);
  digitalWrite(ENpin, HIGH);

  // PC → 시리얼 통신
  Serial.begin(9600);
  
  // 아두이노보드 → 블루투스 통신
  BTSerial.begin(38400);
  
  Serial.println("ATcommand :");
}

void loop() {
  if(BTSerial.available()) {
    Serial.write(BTSerial.read());    
  }


  /*char ty = '0';
  if(FY > 810) ty = '4';
  else if(FY < 210) ty = '2';
  if(ty != '0') {
    BTSerial.write(ty);
  }*/

  int FY = analogRead(A1);
  int FX = analogRead(A0);
  char tx = '0';
  send_byte = B00000000; 
  if(FX > 810){
     tx = '1';
     send_byte |= B00000001;
  }
  else if(FX < 210){
     tx = '3';
     send_byte |= B00000010;
  }
  else if(FY>810){ 
    tx ='4';
    send_byte |= B00001000; 
  }
  else if(FY < 210){
    tx = '2';
    send_byte |= B00000100;
  }
  else{
    tx='0';
  }
  //BTSerial.write(tx);
  BTSerial.write(send_byte);


  delay(80);
}
