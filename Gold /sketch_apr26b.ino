#include <Arduino_LSM6DS3.h>

float GYx, GYy, GYz, Accx, Accy, Accz;
float GYxoff= 1.19, GYyoff = -2.52, GYzoff= -0.034;
float Accxoff = 0.034142, Accyoff = -0.005264, Acczoff = -0.008972;// offsets for accerometer

void setup() {
   Serial.begin(9600);
   IMU.begin();
}

void loop() {
 if (IMU.gyroscopeAvailable() && IMU.accelerationAvailable()) {
    IMU.readGyroscope(GYx,GYy,GYz);

      GYx = GYx - GYxoff; // reads IMU data and preforms offset 
      
    IMU.readAcceleration(Accx,Accy,Accz);

      Accy = Accy - Accyoff;
      //Accz = Accz - Acczoff; // reads IMU data and preforms offset 
    }else{}
}
