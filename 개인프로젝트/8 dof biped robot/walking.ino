#include <Servo.h>

Servo s5,s6,s7,s8; 
Servo s1,s2,s3,s4;          // creates object to control the servo

float ss1,ss2,ss3,ss4,ss5,ss6,ss7,ss8;
float m1=40;
float m2=60;
float m3=30;
float m4=60;

float m5=140;
float m6=120;
float m7=150;
float m8=120;

float del= 50;

// 초기 상태
void stable()
{
   s1.write(m1);    s5.write(m5);
   s2.write(m2);    s6.write(m6);
   s3.write(m3);    s7.write(m7);
   s4.write(m4);    s8.write(m8);
  
}

void setup() {
  // put your setup code here, to run once:
  s1.attach(2); //왼쪽힙
  s2.attach(3); //왼쪽 무릎
  s3.attach(4); //왼쪽 발목 위
  s4.attach(5); //왼쪽 발목
  
  s5.attach(6); //오른쪽힙
  s6.attach(7); //오른쪽 무릎
  s7.attach(8); //오른쪽 발목 위
  s8.attach(9); //오른쪽 발목
}

void loop() {
  stable();
  delay(del);

  //오른다리 나가기
  // 1. 발목 꺾기
  for(          ss4=m4,           ss8=m8;
                ss4>=m4-15,       ss8>=m8-15; 
                ss4--,            ss8--)
  {
    s4.write(ss4); //왼 발목
    s8.write(ss8); //오른 발목
    delay(del);
  }
  
  // 2. 오른다리 앞으로 나가고 왼다리 뒤로 나가기
  for(        ss5=m5,            ss6=m6,            ss7=m7,            ss1=m1,           ss2=m2,             ss3=m3 ; 
              ss5<=m5+10,        ss6>=m6-20,        ss7>=m7-20,        ss1<=m1+20,      ss2>=m2-10,         ss3<=m3+10; 
              ss5=ss5+1,         ss6=ss6-2,        ss7=ss7-2,          ss1=ss1+2,        ss2--,              ss3++)
  {
    s5.write(ss5);  //오른 힙
    s6.write(ss6);  //오른 무릎
    s7.write(ss7);  //오른 발목위

    s1.write(ss1);  //왼 힙
    s2.write(ss2);  //왼 무릎
    s3.write(ss3);  //외 발목위
    delay(30);
  }

  // 3. 발목 원상태 및 오른다리 착지
  for(          ss4=m4-15,          ss6=m6-20,            ss7=m7-20,              ss8=m8-15 ;
                ss4<=m4,            ss6<=m6,             ss7<=m7+10,             ss8<=m8;
                ss4=ss4+1.5,        ss6=ss6+2,            ss7=ss7+3,            ss8=ss8+1.5)
  {
    s4.write(ss4); //왼 발목
    s6.write(ss6); //오른 무릎
    s7.write(ss7); //오른 발목위
    s8.write(ss8); //오른 발목
    delay(del);
  }

  // 4. 초기상태로 돌아가기(두다리 원상태)
  for(        ss5=m5+10,          ss6=m6,            ss7=m7+10,         ss1=m1+20,          ss2=m2-10,          ss3=m3+10 ; 
              ss5>=m5,            ss6<=m6,           ss7>=m7,           ss1>=m1,            ss2<=m2,            ss3>=m3; 
              ss5=ss5-1,          ss6=ss6+0,         ss7--,            ss1=ss1-2,          ss2++,              ss3--)
  {
    s1.write(ss1);  //왼 힙
    s2.write(ss2);  //왼 무릎
    s3.write(ss3);  //외 발목위


    s5.write(ss5);  //오른 힙
    s6.write(ss6);  // 오른 무릎
    s7.write(ss7);  //오른 발목위
    delay(del);
  }


  //왼다리 나가기
  // 1. 발목 꺾기
  for(    ss4=m4,               ss8=m8;
          ss4<=m4+15,           ss8<=m8+15; 
          ss4++,                ss8++)
  {
    s4.write(ss4); //왼 발목
    s8.write(ss8); //오른 발목
    delay(del);
  }

  // 2. 왼다리 앞으로 나가고 오른다리 뒤로 나가기
  for(       ss5=m5,        ss6=m6,         ss7=m7,         ss1=m1,           ss2=m2,           ss3=m3 ; 
             ss5>=m5-20,    ss6<=m6+10,     ss7>=m7-10,     ss1>=m1-10,       ss2<=m2+20,       ss3<=m3+20; 
             ss5=ss5-2,     ss6++,          ss7--,          ss1=ss1-1,        ss2=ss2+2,        ss3=ss3+2)
  {
    s5.write(ss5);  //오른 힙
    s6.write(ss6);  //오른 무릎
    s7.write(ss7);  //오른 발목위

    s1.write(ss1);  //왼 힙
    s2.write(ss2);  //왼 무릎
    s3.write(ss3);  //외 발목위
    delay(del);
  }

  // 3. 발목 원상태 및 왼다리 착지
  for(        ss8=m8+15,          ss2=m2+20,        ss3=m3+20,          ss4=m4+15 ;
              ss8>=m8,            ss2>=m2,          ss3>=m3-10,         ss4>=m4; 
              ss8=ss8-1.5,        ss2=ss2-2,        ss3=ss3-3,          ss4=ss4-1.5)
  {
    s8.write(ss8); //오른 발목
    s2.write(ss2); // 왼 무릎
    s3.write(ss3); //왼 발목위
    s4.write(ss4); //왼 발목
    delay(del);
  }

  // 4. 초기상태로 돌아가기(두다리 원상태)
  for(        ss5=m5-20,          ss6=m6+10,          ss7=m7-10,          ss1=m1-10,           ss2=m2,             ss3=m3-10 ; 
              ss5<=m5,            ss6>=m6,            ss7<=m7,            ss1<=m1,             ss2<=m2,            ss3<=m3; 
              ss5=ss5+2,          ss6--,              ss7++,              ss1=ss1+1,           ss2=ss2+0,          ss3++)
  {
    s5.write(ss5);  //오른 힙
    s6.write(ss6);  // 오른 무릎
    s7.write(ss7);  //오른 발목위

    s1.write(ss1);  //왼 힙
    s2.write(ss2);  // 왼 무릎
    s3.write(ss3);  //왼 발목위
    delay(del);
  }

  //잠시 정지
  stable();
  delay(del);

}

 
