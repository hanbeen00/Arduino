#include<Servo.h> 

Servo servo1;     
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;
Servo servo7;
Servo servo8;

void setup() { 
  servo1.attach(2); 
  servo2.attach(3);
  servo3.attach(4);
  servo4.attach(5);

  
  servo5.attach(6); 
  servo6.attach(7);
  servo7.attach(8);
  servo8.attach(9);
}

void loop() {
  servo1.write(40); 
  servo2.write(60);  
  servo3.write(30);
  servo4.write(60); 

  servo5.write(140); 
  servo6.write(120);  
  servo7.write(150);
  servo8.write(120); 
}
