#include <PID_v1.h>
#include <SAMDTimerInterrupt.h>
#include <SAMD_ISR_Timer.h>
#include <Arduino_LSM6DS3.h>
#include <Math.h>

const int RightPos = 9; // sets name for H-bridge input A1 
const int RightNeg = 10; // sets name for H-brigde input A2
const int LeftNeg = 11; // sets name for H-bridge input A3 
const int LeftPos = 12; // Sets name for H-brigde input A4 
const int Motorspeedpin = 6;

volatile float sample, gyro_sample, GYangle = 0,PrevGYangle = 0,finalangle = 0, delta_angle, accAngle;
volatile unsigned long current_time, previous_time = 0, delta_time;
float GYx, GYy, GYz;
float GYxoff= 1.355, GYyoff = -2.52, GYzoff= -0.034;
float Accx, Accy, Accz;
float Accxoff = 0.034142, Accyoff = -0.005264, Acczoff = -0.008972;
int Motorspeed;
bool directionbool;

SAMDTimer ITimer0(TIMER_TC3);
#define TIMER0_INTERVAL_MS        25      // 1s = 1000ms

double Input, Output;
double Kp = 45, Ki = 35 , Kd = 0.8, Setpoint;
PID anglePID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void TimerHandler0(void){
  current_time = millis();
  delta_time = current_time - previous_time;
  delta_angle = GYx * delta_time/1000;
  GYangle = PrevGYangle + delta_angle; 
  PrevGYangle = GYangle;
  previous_time = current_time;

  accAngle = (atan2(Accy, Accz)*RAD_TO_DEG);

  finalangle = (0.98)*(GYangle) + (0.02)*(accAngle);
  Input = finalangle;
  anglePID.Compute();

  //Serial.print(finalangle);
  //Serial.print('\t');
  Serial.print(accAngle);
  Serial.print('\t');
  Serial.println(finalangle);
  
}

void setup() {
 Serial.begin(9600);

 IMU.begin();
  if (IMU.gyroscopeAvailable() && IMU.accelerationAvailable()) {
    IMU.readGyroscope(GYx, GYy, GYz);

      GYx = GYx - GYxoff;
      
    IMU.readAcceleration(Accx,Accy,Accz);

      Accy = Accy - Accyoff;
      //Accz = Accz - Acczoff;
    }
     accAngle = atan2(Accy, Accz)*RAD_TO_DEG;

   GYangle = 0;

  Setpoint = 0;// creates set point 
  anglePID.SetMode(AUTOMATIC); // 
  anglePID.SetOutputLimits(-255,255); // sets max and min boundaries for the PID Output 

  pinMode(RightNeg, OUTPUT);
  pinMode(RightPos, OUTPUT);
  pinMode(LeftNeg, OUTPUT);
  pinMode(LeftPos, OUTPUT);

   if (ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0))
    Serial.println("Starting  ITimer0 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer0. Select another freq. or timer");
}

void loop() {

  if (IMU.gyroscopeAvailable() && IMU.accelerationAvailable()) {
    IMU.readGyroscope(GYx, GYy, GYz);

      GYx = GYx - GYxoff;
      
    IMU.readAcceleration(Accx,Accy,Accz);

      Accy = Accy - Accyoff;
      //Accz = Accz - Acczoff;
    }
  Motorspeed = abs(Output);
  analogWrite(Motorspeedpin, Motorspeed);

  if(Output > 0){
    directionbool = true;}
   else{
    directionbool = false;}
 
 if(!directionbool){
  digitalWrite(RightNeg, LOW);
  digitalWrite(RightPos, HIGH);
  
  digitalWrite(LeftNeg, LOW);
  digitalWrite(LeftPos, HIGH);
  
 }else{

   digitalWrite(RightPos, LOW);
   digitalWrite(RightNeg, HIGH);

   digitalWrite(LeftPos, LOW);
   digitalWrite(LeftNeg, HIGH);
   
 }
 

}
