#include <WiFiNINA.h>

char ssid[] = "TP-LINK_179E";
char pass[] = "56702460";
WiFiServer server(5200);
WiFiClient client; // Different elements of wifi set up 

void Buggy_line_follow(); // main function driving the buggy/line follwing 
void read_from_client(); //Reads from processing app
void stopbuggy();//stops buggy 
int US_sensor();//US sensor function 
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
bool US_Close = false;
int increment = 10;
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
  server.begin(); //arduino server set up and prints IP address for server
}
// ==========================================================================================
void loop() { 
  read_from_client();
  buggy_status();

  while(buggy_line_follow_bool){ // if button is pressed on client buggy will start 
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

if(increment > 50){ // every 50 iterations it will update the distance value and check if there is an obstacle 
 distance = US_sensor();

 if(distance < 10){
    US_Close = true;
 }else{
    US_Close = false; 
    }
 increment=0;
}
increment++;
 
  
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
void stopbuggy(){// stops buggy 
  
  analogWrite(PosLeftMotor, Off);
  analogWrite(PosRightMotor, Off);
}
// ==========================================================================================
int US_sensor(){
  
  digitalWrite( US_TRIG, LOW );
  delayMicroseconds(2);
  digitalWrite( US_TRIG, HIGH );
  delayMicroseconds( 10 );
  digitalWrite( US_TRIG, LOW );

  long duration = pulseIn( US_ECHO, HIGH ); // pulse in measures the time take for the signal to return to the sensor 
  int distance = duration/58;

    return distance;
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
if(!LeftOn && !RightOn){buggy_status_switch = 4;} // identifies which state the buggy is in based on booleans related to the motors 

  if(buggy_status_switch != prevStatus){//poll to check if buggy status has changed 
  
      switch(buggy_status_switch){
  case 1: server.write("c");break; // Case 1: The buggy is moving forward
  case 2: server.write("v");break; // Case 2: The buggy is turning right
  case 3: server.write("b");break; // Case 3: The buggy is turning left
  case 4: server.write("n");break; // Case 4: The buggy is stopped
  }
  
  }
  prevStatus = buggy_status_switch;
}

// ==========================================================================================
