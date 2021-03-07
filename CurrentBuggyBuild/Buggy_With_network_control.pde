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
double timer;

int lastValueOfLIREye;
int lastValueOfRIREye;

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

void loop() { 
  read_from_client();

  while(buggy_line_follow_bool){
     Buggy_line_follow();
     read_from_client();
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
    }else{
    analogWrite(PosLeftMotor, Off);// No power 
  }

  if( digitalRead( REYE ) == LOW && !US_Close){ // if left eye is LOW send high signal to motor input A4 and to A3
    analogWrite(PosRightMotor, Value);
  }else{
    analogWrite(PosRightMotor, Off); // no power 
  }
  
  if(lastValueOfLIREye != digitalRead(LEYE)){
    lastValueOfLIREye = digitalRead(LEYE);
    Report_IR_L(lastValueOfLIREye);
    //Serial.println("Left eye change");
  }

  if(lastValueOfRIREye != digitalRead(REYE)){
    lastValueOfRIREye = digitalRead(REYE);
    Report_IR_R(lastValueOfRIREye);
    //Serial.println("Right eye change");
  }
   read_from_client();
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

  timer = micros();
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
  //if (client.connected()) {
    if(digitalRead(LEYE) == HIGH) {
      server.write("u");
      //client.stop();
      Serial.println("u");
    }
    if(digitalRead(LEYE) == LOW) {
      server.write("i");
      //client.stop();
      Serial.println("i");
    }
  }
//}
// ==========================================================================================
void Report_IR_R(int lastIRValue){
client = server.available();
 //if (client.connected()) {
    if(lastIRValue == HIGH) {
      server.write("o");
      //client.stop();
      Serial.println("o");
    }
    if(lastIRValue == LOW) {
      server.write("p");
      //client.stop();
      Serial.println("p");
    }
 // }
}
// ==========================================================================================



/*void write_to_client(){
  client = server.available();
  if (client.connected()) {
    if(digitalRead(LEYE) == HIGH){
      char LEYEON = 'u';
      server.write(LEYEON);
      Serial.print(LEYEON);
    }
    if(digitalRead(LEYE) == LOW){
       char LEYEOFF = 'i';   
      server.write(LEYEOFF);
      Serial.print(LEYEOFF);
    }
    if(digitalRead(REYE) == HIGH){
      char REYEON = 'o';
      server.write(REYEON);
      Serial.print(REYEON);
    }
    if(digitalRead(REYE) == LOW){
      char REYEOFF = 'p';
      server.write(REYEOFF);
      Serial.print(REYEOFF);
    }
}*/
