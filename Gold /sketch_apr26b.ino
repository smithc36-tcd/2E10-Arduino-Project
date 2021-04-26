#include <PID_v1.h>
#include <Arduino_LSM6DS3.h>
#include <Math.h>

const int RightPos = 9; // sets name for H-bridge input A1 
const int RightNeg = 10; // sets name for H-brigde input A2 
const int LeftNeg = 11; // sets name for H-bridge input A3 
const int LeftPos = 12; // Sets name for H-brigde input A4 
const int Motorspeedpin = 6;

volatile float sample, gyro_sample, GYangle = 0,PrevGYangle = 0,finalangle = 0, delta_angle, accAngle; //volatile as they may be altered by the ISR 
volatile unsigned long current_time, previous_time = 0, delta_time;
float GYx, GYy, GYz;
float GYxoff= 1.19, GYyoff = -2.52, GYzoff= -0.034; // Offsets for Gyroscope   
float Accx, Accy, Accz;
float Accxoff = 0.034142, Accyoff = -0.005264, Acczoff = -0.008972;// offsets for accerometer
volatile int Motorspeed;
bool directionbool;

//=======================================================================================

double Input = 0, Output = 0;
double Kp = 53, Ki =25, Kd = 0.3, Setpoint;
PID anglePID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);//creates a PID object 


//=======================================================================================
void setup() {
 Serial.begin(9600);
 float accAnglesum =0, accAngleAvg;

 IMU.begin();


  Setpoint = 0;// creates set point 
  anglePID.SetMode(AUTOMATIC); // 
  anglePID.SetOutputLimits(-255,255); // sets max and min boundaries for the PID Output 

  pinMode(RightNeg, OUTPUT);
  pinMode(RightPos, OUTPUT);
  pinMode(LeftNeg, OUTPUT);
  pinMode(LeftPos, OUTPUT);

}
//=======================================================================================
void loop() {

  if (IMU.gyroscopeAvailable() && IMU.accelerationAvailable()) {
    IMU.readGyroscope(GYx,GYy,GYz);

      GYx = GYx - GYxoff; // reads IMU data and preforms offset 
      
    IMU.readAcceleration(Accx,Accy,Accz);

      Accy = Accy - Accyoff;
      //Accz = Accz - Acczoff; // reads IMU data and preforms offset 
    }else{}

    angle();  
    
  Motorspeed = abs(Output);
  analogWrite(Motorspeedpin, Motorspeed); // sets motor speed

  if(Output >= 0){
    directionbool = true;} // decides which direction to correct the angle 
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
   digitalWrite(LeftNeg, HIGH);// motor control 
 }
}

void angle(){
  current_time = millis();
  delta_time = current_time - previous_time;
  delta_angle = GYx * delta_time/1000;
  GYangle = GYangle + delta_angle; 
  previous_time = current_time; // preforms numeric integration on the Gyroscope readings to calculate current angle

  accAngle = (atan2(Accy, Accz)*RAD_TO_DEG); // uses the math library and trig to calculate angle using the accelerometer readings 

  Input = (0.98)*(GYangle) + (0.02)*(accAngle); // complementary filter, removes the noises from the Accelerometer data and reduces drift 
  anglePID.Compute();


}
