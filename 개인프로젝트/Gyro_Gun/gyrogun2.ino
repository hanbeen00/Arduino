

#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Mouse.h>
#include <MouseTo.h>
#include <Keyboard.h>

#define BTN_PIN 4

MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;
int vx, vy;

#define PortX A0
#define PortY A1

#define YLimitExtremeW 896
#define XLimitExtremeA 896
#define YLimitExtremeS 128
#define XLimitExtremeD 128

#define YLimit2W 768
#define XLimit2A 768
#define YLimit2S 256
#define XLimit2D 256

#define smallerX >
#define biggerX <
#define smallerY <
#define biggerY >

void joystickMove(int X, int Y);

void setup() {
  Serial.begin(9600);
  Keyboard.begin();
}

void loop() {

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);


  int btn1 = digitalRead(BTN_PIN);
  int btn2 = digitalRead(8);
  Serial.println(btn1);
  Serial.print(analogRead(A1));  // X축 값이 표기됩니다.

  Serial.print("  ");  //숫자가 헷깔리지않토록 중간에 공백칸을 두었습니다.

  Serial.print(analogRead(A0));  // X축 값이 표기 됩니다.

  Serial.print("  ");  //숫자가 헷깔리지않토록 중간에 공백칸을 두었습니다.

  Serial.println(digitalRead(8));  // Z축(스위치) 값이 표기됩니다.


  int XValue = analogRead(PortX);
  int YValue = analogRead(PortY);

  /*joystickMove(XValue, YValue);


  if (0)
  {
    Serial.println(XValue);
    Serial.println(YValue);
    Serial.print('\n'); 

    delay(2000);
  }*/

  if (btn1 == 0) {

    //MouseTo.setTarget(960,540,0);
    // while (MouseTo.move() == false) {}
    //Keyboard.press(MOUSE_LEFT); //키 누름
    Mouse.press(MOUSE_LEFT);
  } else {
    Mouse.release(MOUSE_LEFT);
  }

  /*if (btn2 == 0) {
   //MouseTo.setTarget(960,540,0);
  //while (MouseTo.move() == false) {}
     Keyboard.press('r'); //키 누름
  }
  else{
    Keyboard.releaseAll(); //키 해제
  }*/


  vx = (gx + 550) / 200;   // "+300" because the x axis of gyroscope give values about -350 while it's not moving. Change this value if you get something different using the TEST code, chacking if there are values far from zero.
  vy = -(gz + 180) / 200;  // same here about "-100"
  Serial.println(vx);
  Serial.println(vy);
  Mouse.move(vy, vx);
  delay(20);
}







void joystickMove(int X, int Y) {
  static bool isWpressed = false;
  static bool isApressed = false;
  static bool isSpressed = false;
  static bool isDpressed = false;

  if (Y biggerY YLimitExtremeW)  // W pressed
  {
    if (!isWpressed) {
      Keyboard.press('w');
      isWpressed = true;
    }
  } else if (Y biggerY YLimit2W) {
    if (isWpressed) {
      Keyboard.release('w');
      isWpressed = false;
      Keyboard.print('w');
    }
  } else if (isWpressed) {
    Keyboard.release('w');
    isWpressed = false;
  }

  if (X smallerX XLimitExtremeA)  // A pressed
  {
    if (!isApressed) {
      Keyboard.press('a');
      isApressed = true;
    }
  } else if (X smallerX XLimit2A) {
    if (isApressed) {
      Keyboard.release('a');
      isApressed = false;
      Keyboard.print('a');
    }
  } else if (isApressed) {
    Keyboard.release('a');
    isApressed = false;
  }

  if (Y smallerY YLimitExtremeS)  // S pressed
  {
    if (!isSpressed) {
      Keyboard.press('s');
      isSpressed = true;
    }
  } else if (Y smallerY YLimit2S) {
    if (isSpressed) {
      Keyboard.release('s');
      isSpressed = false;
      Keyboard.print('s');
    }
  } else if (isSpressed) {
    Keyboard.release('s');
    isSpressed = false;
  }

  if (X biggerX XLimitExtremeD)  // D pressed
  {
    if (!isDpressed) {
      Keyboard.press('d');
      isDpressed = true;
    }
  } else if (X biggerX XLimit2D) {
    if (isDpressed) {
      Keyboard.release('d');
      isDpressed = false;
      Keyboard.print('d');
    }
  } else if (isDpressed) {
    Keyboard.release('d');
    isDpressed = false;
  }
}
