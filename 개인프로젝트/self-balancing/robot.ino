#include <Wire.h>                                            
#include <SoftwareSerial.h>
SoftwareSerial serial_SW_wireless_module(8, 9);


int gyro_address = 0x68;                                     //MPU-6050 I2C address (0x68 or 0x69)
int acc_calibration_value = -300;                            //Enter the accelerometer calibration value

//Various settings
float pid_p_gain = 9;                                      
float pid_i_gain = 1.0;                                      
float pid_d_gain = 15;                                       
float turning_speed = 30;                                    
float max_target_speed = 150;                              


//Declaring global variables
byte start, received_byte, low_bat;

int left_motor, throttle_left_motor, throttle_counter_left_motor, throttle_left_motor_memory;
int right_motor, throttle_right_motor, throttle_counter_right_motor, throttle_right_motor_memory;
int battery_voltage;
int receive_counter;
int gyro_pitch_data_raw, gyro_yaw_data_raw, accelerometer_data_raw;

long gyro_yaw_calibration_value, gyro_pitch_calibration_value;

unsigned long loop_timer;

float angle_gyro, angle_acc, angle, self_balance_pid_setpoint;
float pid_error_temp, pid_i_mem, pid_setpoint, gyro_input, pid_output, pid_last_d_error;
float pid_output_left, pid_output_right;


void setup(){
  serial_SW_wireless_module.begin(38400);                                   
  Serial.begin(19200);                                                       
  Wire.begin();                                                           
  TWBR = 12;                                                                //Set the I2C clock speed to 400kHz

  //subroutine every 20us
  TCCR2A = 0;                                                              
  TCCR2B = 0;                                                              
  TIMSK2 |= (1 << OCIE2A);                                                  //Set the interupt enable bit OCIE2A in the TIMSK2 register
  TCCR2B |= (1 << CS21);                                                    //Set the CS21 bit in the TCCRB register to set the prescaler to 8
  OCR2A = 39;                                                               //The compare register is set to 39 => 20us / (1s / (16.000.000MHz / 8)) - 1
  TCCR2A |= (1 << WGM21);                                                   //Set counter 2 to CTC (clear timer on compare) mode
  

  Wire.beginTransmission(gyro_address);                                     
  Wire.write(0x6B);                                                        
  Wire.write(0x00);                                                       
  Wire.endTransmission();                               
  
  //Set the full scale of the gyro to +/- 250 degrees per second
  Wire.beginTransmission(gyro_address);                                     
  Wire.write(0x1B);                                                        
  Wire.write(0x00);                                                         //Set the register bits as 00000000 (250dps full scale)
  Wire.endTransmission();  
  
  //Set the full scale of the accelerometer to +/- 4g.
  Wire.beginTransmission(gyro_address);                                    
  Wire.write(0x1C);                                                         //We want to write to the ACCEL_CONFIG register (1A hex)
  Wire.write(0x08);                                                         //Set the register bits as 00001000 (+/- 4g full scale range)
  Wire.endTransmission();                                         
  
  //Set some filtering to improve the raw data.
  Wire.beginTransmission(gyro_address);                                   
  Wire.write(0x1A);                                                         //We want to write to the CONFIG register (1A hex)
  Wire.write(0x03);                                                         //Set the register bits as 00000011 (Set Digital Low Pass Filter to ~43Hz)
  Wire.endTransmission();                                                  

  pinMode(2, OUTPUT);                                                    
  pinMode(3, OUTPUT);                                                     
  pinMode(4, OUTPUT);                                                
  pinMode(5, OUTPUT);                                                     
  pinMode(13, OUTPUT);                                         

  for(receive_counter = 0; receive_counter < 500; receive_counter++){     
    if(receive_counter % 15 == 0)digitalWrite(13, !digitalRead(13));      
    Wire.beginTransmission(gyro_address);                                  
    Wire.write(0x43);                                                       //Start reading the Who_am_I register 75h
    Wire.endTransmission();                                                
    Wire.requestFrom(gyro_address, 4);                                      //Request 4 bytes from the gyro
    gyro_yaw_calibration_value += Wire.read()<<8|Wire.read();               //Combine the two bytes to make one integer
    gyro_pitch_calibration_value += Wire.read()<<8|Wire.read();             //Combine the two bytes to make one integer
    delayMicroseconds(3700);                                                
  }
  gyro_pitch_calibration_value /= 500;                                     
  gyro_yaw_calibration_value /= 500;                           

  loop_timer = micros() + 4000;                                             //Set the loop_timer variable at the next end loop time

}


void loop(){
  
  char s;
  if (serial_SW_wireless_module.available()) {                          
    //s=serial_SW_wireless_module.read();
    //Serial.println(s);
    received_byte = serial_SW_wireless_module.read();                     
    //Serial.print((char)received_byte); // debug;
    receive_counter = 0;                                               
  }
  if(receive_counter <= 25)receive_counter ++;                              //The received byte will be valid for 25 program loops (100 milliseconds)
  else received_byte = 0x00;         
  //else Serial.println(s);


  //Angle calculations
  Wire.beginTransmission(gyro_address);                                    
  Wire.write(0x3F);                                                
  Wire.endTransmission();                                                  
  Wire.requestFrom(gyro_address, 2);                                        
  accelerometer_data_raw = Wire.read()<<8|Wire.read();                      
  accelerometer_data_raw += acc_calibration_value;                          //Add the accelerometer calibration value
  if(accelerometer_data_raw > 8200)accelerometer_data_raw = 8200;           //Prevent division by zero by limiting the acc data to +/-8200;
  if(accelerometer_data_raw < -8200)accelerometer_data_raw = -8200;         

  angle_acc = asin((float)accelerometer_data_raw/8200.0)* 57.296;           //Calculate the current angle according to the accelerometer

  if(start == 0 && angle_acc > -0.25&& angle_acc < 0.25){                   //If the accelerometer angle is almost 0
    angle_gyro = angle_acc;                                                 //Load the accelerometer angle in the angle_gyro variable
    start = 1;                                                              //Set the start variable to start the PID controller
  }
  
  Wire.beginTransmission(gyro_address);                                     
  Wire.write(0x43);                                                       
  Wire.endTransmission();                                                   //End the transmission
  Wire.requestFrom(gyro_address, 4);                                        //Request 4 bytes from the gyro
  gyro_yaw_data_raw = Wire.read()<<8|Wire.read();                           //Combine the two bytes to make one integer
  gyro_pitch_data_raw = Wire.read()<<8|Wire.read();                         
  
  gyro_pitch_data_raw -= gyro_pitch_calibration_value;                      //Add the gyro calibration value
  angle_gyro += gyro_pitch_data_raw * 0.000031;                             //Calculate the traveled during this loop angle and add this to the angle_gyro variable

  
  //MPU-6050 offset compensation
  gyro_yaw_data_raw -= gyro_yaw_calibration_value;                         //Add the gyro calibration value
  angle_gyro -= gyro_yaw_data_raw * 0.0000003;                            //Compensate the gyro offset when the robot is rotating

  angle_gyro = angle_gyro * 0.9996 + angle_acc * 0.0004;                    //Correct the drift of the gyro angle with the accelerometer angle


  //PID controller calculations
  pid_error_temp = angle_gyro - self_balance_pid_setpoint - pid_setpoint;
  if(pid_output > 10 || pid_output < -10)pid_error_temp += pid_output * 0.015 ;

  pid_i_mem += pid_i_gain * pid_error_temp;                                 //Calculate the I-controller value and add it to the pid_i_mem variable
  if(pid_i_mem > 400)pid_i_mem = 400;                                       //Limit the I-controller to the maximum controller output
  else if(pid_i_mem < -400)pid_i_mem = -400;
 
  //Calculate the PID output value
  pid_output = pid_p_gain * pid_error_temp + pid_i_mem + pid_d_gain * (pid_error_temp - pid_last_d_error);
  if(pid_output > 400)pid_output = 400;                                     //Limit the PI-controller to the maximum controller output
  else if(pid_output < -400)pid_output = -400;

  pid_last_d_error = pid_error_temp;                                        //Store the error for the next loop

  if(pid_output < 5 && pid_output > -5)pid_output = 0;                      //Create a dead-band to stop the motors when the robot is balanced

  if(angle_gyro > 30 || angle_gyro < -30 || start == 0){    //If the robot tips over or the start variable is zero
    pid_output = 0;                                                         //Set the PID controller output to 0 so the motors stop moving
    pid_i_mem = 0;                                                          //Reset the I-controller memory
    start = 0;                                                              //Set the start variable to 0
    self_balance_pid_setpoint = 0;                                          //Reset the self_balance_pid_setpoint variable
  }



  //Control calculations
  pid_output_left = pid_output;                                             
  pid_output_right = pid_output;                                 

  if((received_byte & B00000001)){                                            
    pid_output_left += turning_speed;                           
    pid_output_right -= turning_speed;                                   
    //Serial.println(s);
  }
  if((received_byte & B00000010)){                                        
    pid_output_left -= turning_speed;                                  
    pid_output_right += turning_speed;                                  
    //Serial.println(s);
  }

  if((received_byte & B00000100)){                                        
    if(pid_setpoint > -2.5)pid_setpoint -= 0.05;                          
    if(pid_output > max_target_speed * -1)pid_setpoint -= 0.005;          
    //Serial.println(s);
  }
  if((received_byte & B00001000)){                                          
    if(pid_setpoint < 2.5)pid_setpoint += 0.05;                    
    if(pid_output < max_target_speed)pid_setpoint += 0.005;         
    //Serial.println(s);
  }   

  if((!(received_byte & B00001100))){                                    
    if(pid_setpoint > 0.5)pid_setpoint -=0.05;                         
    else if(pid_setpoint < -0.5)pid_setpoint +=0.05;                       
    else pid_setpoint = 0;                                                 
  }
  
  if(pid_setpoint == 0){                                                    
    if(pid_output < 0)self_balance_pid_setpoint += 0.0015;                 
    if(pid_output > 0)self_balance_pid_setpoint -= 0.0015;             
  }


  
  //Motor pulse calculations
  //To compensate for the linear speed behaviour.
  
  if(pid_output_left > 0)pid_output_left = 405 - (1/(pid_output_left + 9)) * 5500;
  else if(pid_output_left < 0)pid_output_left = -405 - (1/(pid_output_left - 9)) * 5500;

  if(pid_output_right > 0)pid_output_right = 405 - (1/(pid_output_right + 9)) * 5500;
  else if(pid_output_right < 0)pid_output_right = -405 - (1/(pid_output_right - 9)) * 5500;

  //Calculate the needed pulse time for the left and right stepper motor controllers
  if(pid_output_left > 0)left_motor = 400 - pid_output_left;
  else if(pid_output_left < 0)left_motor = -400 - pid_output_left;
  else left_motor = 0;

  if(pid_output_right > 0)right_motor = 400 - pid_output_right;
  else if(pid_output_right < 0)right_motor = -400 - pid_output_right;
  else right_motor = 0;

  //Copy the pulse time to the throttle variables so the interrupt subroutine can use them
  throttle_left_motor = left_motor;
  throttle_right_motor = right_motor;

  
  //Loop time timer
  while(loop_timer > micros());
  loop_timer += 4000;
}


ISR(TIMER2_COMPA_vect){
  //Left motor pulse calculations
  throttle_counter_left_motor ++;                                           //Increase the throttle_counter_left_motor variable by 1 every time this routine is executed
  if(throttle_counter_left_motor > throttle_left_motor_memory){             //If the number of loops is larger then the throttle_left_motor_memory variable
    throttle_counter_left_motor = 0;                                        //Reset the throttle_counter_left_motor variable
    throttle_left_motor_memory = throttle_left_motor;                       //Load the next throttle_left_motor variable
    if(throttle_left_motor_memory < 0){                                     //If the throttle_left_motor_memory is negative
      PORTD &= 0b11110111;                                                  //Set output 3 low to reverse the direction of the stepper controller
      throttle_left_motor_memory *= -1;                                     //Invert the throttle_left_motor_memory variable
    }
    else PORTD |= 0b00001000;                                               //Set output 3 high for a forward direction of the stepper motor
  }
  else if(throttle_counter_left_motor == 1)PORTD |= 0b00000100;             //Set output 2 high to create a pulse for the stepper controller
  else if(throttle_counter_left_motor == 2)PORTD &= 0b11111011;             //Set output 2 low because the pulse only has to last for 20us 
  
  //right motor pulse calculations
  throttle_counter_right_motor ++;                                          //Increase the throttle_counter_right_motor variable by 1 every time the routine is executed
  if(throttle_counter_right_motor > throttle_right_motor_memory){           //If the number of loops is larger then the throttle_right_motor_memory variable
    throttle_counter_right_motor = 0;                                       //Reset the throttle_counter_right_motor variable
    throttle_right_motor_memory = throttle_right_motor;                     //Load the next throttle_right_motor variable
    if(throttle_right_motor_memory < 0){                                    //If the throttle_right_motor_memory is negative
      PORTD |= 0b00100000;                                                  //Set output 5 low to reverse the direction of the stepper controller
      throttle_right_motor_memory *= -1;                                    //Invert the throttle_right_motor_memory variable
    }
    else PORTD &= 0b11011111;                                               //Set output 5 high for a forward direction of the stepper motor
  }
  else if(throttle_counter_right_motor == 1)PORTD |= 0b00010000;            //Set output 4 high to create a pulse for the stepper controller
  else if(throttle_counter_right_motor == 2)PORTD &= 0b11101111;            //Set output 4 low because the pulse only has to last for 20us
}
