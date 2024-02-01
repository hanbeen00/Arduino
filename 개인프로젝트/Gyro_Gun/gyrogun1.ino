/* Code to control the mouse pointer
 * through the movement of the head
 * Change values at vx and vy (+300 and -100 in my case)
 * using the TEST code to make your project work.
 * 
 * Gabry295
 */
#include <Mouse.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Mouse.h>
#include "Keyboard.h"

#define Relaypin 6
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

MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;
int vx, vy;

void setup() {
  Serial.begin(9600);
  Keyboard.begin();
  Wire.begin();
  pinMode(5, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(Relaypin, OUTPUT);
  mpu.initialize();
  if (!mpu.testConnection()) {
    while (1);
    }
}

void loop() {
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  int btn1 = digitalRead(4);
  int btn2 = digitalRead(7);
  int cnt=0;

  if (btn1 == 0) {
    Mouse.press(MOUSE_LEFT);
    digitalWrite(Relaypin,HIGH);
  }
  else{
    Mouse.release(MOUSE_LEFT);
    digitalWrite(Relaypin,LOW);
  }

   if (btn2 == 0) {
     Mouse.press(MOUSE_RIGHT);
     cnt++;
    //Keyboard.press('r');
  }
  else{
    if(cnt%2==0){
      Mouse.release(MOUSE_RIGHT);
      //Keyboard.release('r');
    }
  }

  vx = (gx+550)/200;  // "+300" because the x axis of gyroscope give values about -350 while it's not moving. Change this value if you get something different using the TEST code, chacking if there are values far from zero.
  vy = -(gz+200)/200; // same here about "-100"sww
  Serial.println(vy);
  if(digitalRead(5)==1){
    Mouse.move(vy, vx);
  }
  int XValue = analogRead(PortX);
  int YValue = analogRead(PortY);
  joystickMove(XValue, YValue);

  delay(20);
}






void joystickMove(int X, int Y)
{        
  static bool isWpressed = false;
  static bool isApressed = false;
  static bool isSpressed = false;
  static bool isDpressed = false;

  if (Y biggerY YLimitExtremeW) // W pressed
  {
    if (!isWpressed)
    {
      Keyboard.press('d');
      isWpressed = true;
    }
  }
  else if (Y biggerY YLimit2W)
  {
    if (isWpressed)
    {
      Keyboard.release('d');
      isWpressed = false;
      Keyboard.print('d');
    }
  }
  else if (isWpressed)
  {
    Keyboard.release('d');
    isWpressed = false;
  }

  if (X smallerX XLimitExtremeA) // A pressed
  {
    if (!isApressed)
    {
      Keyboard.press('w');
      isApressed = true;
    }
  }
  else if (X smallerX XLimit2A)
  {
    if (isApressed)
    {
      Keyboard.release('w');
      isApressed = false;
      Keyboard.print('w');
    }
  }
  else if (isApressed)
  {
    Keyboard.release('w');
    isApressed = false;
  }
  
  if (Y smallerY YLimitExtremeS) // S pressed
  {
    if (!isSpressed)
    {
      Keyboard.press('a');
      isSpressed = true;
    }
  }
  else if (Y smallerY YLimit2S)
  {
    if (isSpressed)
    {
      Keyboard.release('a');
      isSpressed = false;
      Keyboard.print('a');
    }
  }
  else if (isSpressed)
  {
    Keyboard.release('a');
    isSpressed = false;
  }
 
  if (X biggerX XLimitExtremeD) // D pressed
  {
    if (!isDpressed)
    {
      Keyboard.press('s');
      isDpressed = true;
    }
  }
  else if (X biggerX XLimit2D)
  {
    if (isDpressed)
    {
      Keyboard.release('s');
      isDpressed = false;
      Keyboard.print('s');
    }
  }
  else if (isDpressed)
  {
    Keyboard.release('s');
    isDpressed = false;
  }
}
