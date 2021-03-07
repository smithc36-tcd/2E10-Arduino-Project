#include <WiFiNINA.h>

char ssid[] = "TP-LINK_179E";
char pass[] = "56702460";
WiFiServer server(5200);
WiFiClient client;
char command;

void Buggy_line_follow();
void read_from_client();
void stopbuggy();
long US_sensor();
void Report_IR_L(int lastIRValue);
void Report_IR_R(int lastIRValue);
void buggy_status();

const int Value = 255;
const int Off = 0;

const int LEYE = 2; // sets name for left eye 
const int REYE = 10; // sets name for right eye 

const int US_TRIG = 9;//name for Ultrasonic sensor
const int US_ECHO = 8; 

const int PosLeftMotor = 5;
const int NegLeftMotor = 6;
const int NegRightMotor = 11;
const int PosRightMotor = 12;

bool buggy_line_follow_bool = false;
bool LeftOn = false;
bool RightOn = false;
int buggy_status_switch = 1;
int prevStatus;

int lastValueOfLIREye;
int lastValueOfRIREye;
// ==========================================================================================
void setup() {
  Serial.begin(9600);
  
  pinMode( LEYE, INPUT );
  pinMode( REYE, INPUT );// Sets eye pins as inputs 
  pinMode(US_ECHO, INPUT);//sets echo as input
  pinMode(US_TRIG, OUTPUT); // Sets trigger as input
  pinMode(PosLeftMotor, OUTPUT);
  pinMode(NegLeftMotor, OUTPUT);
  pinMode(PosRightMotor, OUTPUT);
  pinMode(NegRightMotor, OUTPUT);
  
  WiFi.begin(ssid, pass);
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address:");
  Serial.println(ip);
  server.begin();
}
// ==========================================================================================
void loop() { 
  read_from_client();
  buggy_status();

  while(buggy_line_follow_bool){
     Buggy_line_follow();
     read_from_client();
     buggy_status();
     if(buggy_line_follow_bool == false){
      break;
     }
  }
}

// ==========================================================================================
void Buggy_line_follow(){
 int distance;
  long duration;
  bool US_Close = false;

 duration = US_sensor();

  distance = duration/58;

  if(distance < 10){
    US_Close = true;
    Serial.println("less than 10");
  }else{
    US_Close = false; 
    }
  
  if( digitalRead( LEYE ) == LOW && !US_Close ){ // if left eye is LOW send high signal to motor input A1 and HIGH to A2
    analogWrite(PosLeftMotor, Value);
    LeftOn = true;
    }else{
    analogWrite(PosLeftMotor, Off);// No power 
    LeftOn = false;
  }

  if( digitalRead( REYE ) == LOW && !US_Close){ // if left eye is LOW send high signal to motor input A4 and to A3
    analogWrite(PosRightMotor, Value);
    RightOn = true;
  }else{
    analogWrite(PosRightMotor, Off); // no power 
    RightOn = false;
  }
  
  if(lastValueOfLIREye != digitalRead(LEYE)){
    lastValueOfLIREye = digitalRead(LEYE);
    Report_IR_L(lastValueOfLIREye);
  }

  if(lastValueOfRIREye != digitalRead(REYE)){
    lastValueOfRIREye = digitalRead(REYE);
    Report_IR_R(lastValueOfRIREye);
  }

  
   read_from_client();
   buggy_status();
}

// ==========================================================================================
void stopbuggy(){
  
  analogWrite(PosLeftMotor, Off);
  analogWrite(PosRightMotor, Off);
}
// ==========================================================================================
long US_sensor(){
  
  digitalWrite( US_TRIG, LOW );
  delayMicroseconds(2);
  digitalWrite( US_TRIG, HIGH );
  delayMicroseconds( 10 );
  digitalWrite( US_TRIG, LOW );

  long duration = pulseIn( US_ECHO, HIGH );

    return duration;
}
// ==========================================================================================
void read_from_client(){

  client = server.available();
  if (client.connected()) {
  char c = client.read();

        if (c == 'a') {
          buggy_line_follow_bool = true;
        }
        if (c == 'd') {
          buggy_line_follow_bool = false;
          stopbuggy();
        }
  }
}
// ==========================================================================================
void Report_IR_L(int lastIRValue){
client = server.available();
    if(digitalRead(LEYE) == HIGH) {
      server.write("u");
    }
    if(digitalRead(LEYE) == LOW) {
      server.write("i");

    }
}
// ==========================================================================================
void Report_IR_R(int lastIRValue){
client = server.available();
    if(lastIRValue == HIGH) {
      server.write("o");
    }
    if(lastIRValue == LOW) {
      server.write("p");
    }
}
// ==========================================================================================
void buggy_status(){
client = server.available();

if(LeftOn && RightOn){buggy_status_switch = 1;}
if(LeftOn && !RightOn){buggy_status_switch = 2;}
if(!LeftOn && RightOn){buggy_status_switch = 3;}
if(!LeftOn && !RightOn){buggy_status_switch = 4;}

  if(buggy_status_switch != prevStatus){
  
      switch(buggy_status_switch){
  case 1: server.write("c");Serial.println("case 1");break;
  case 2: server.write("v");Serial.println("case 2");break;
  case 3: server.write("b");Serial.println("case 3");break;
  case 4: server.write("n");Serial.println("case 4");break;
  }
  
  }
  prevStatus = buggy_status_switch;
}

// ==========================================================================================
