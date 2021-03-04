#include <WiFiNINA.h>

char ssid[] = "TP-LINK_179E";
char pass[] = "56702460";
WiFiServer server(5200);
char command;

void Buggy_line_follow();
void read_from_client();

const int LEYE = 2; // sets name for left eye 
const int REYE = 10; // sets name for right eye 

const int US_TRIG = 9;//name for Ultrasonic sensor
const int US_ECHO = 8; 

const int PosLeftMotor = 5;
const int NegLeftMotor = 6;
const int NegRightMotor = 11;
const int PosRightMotor = 12;

bool buggy_line_follow_bool = false;

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
  }
}


void Buggy_line_follow(){
 int distance;
  long duration;
  int Value = 255;
  int Off = 0;
  bool US_Close = false;

  digitalWrite( US_TRIG, LOW );
  delayMicroseconds(2);
  digitalWrite( US_TRIG, HIGH );
  delayMicroseconds( 10 );
  digitalWrite( US_TRIG, LOW );

  duration = pulseIn( US_ECHO, HIGH );

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

  if( digitalRead( REYE ) == LOW && !US_Close ){ // if left eye is LOW send high signal to motor input A4 and to A3
    analogWrite(PosRightMotor, Value);
  }else{
    analogWrite(PosRightMotor, Off); // no power 
  }
}

void read_from_client(){
  WiFiClient client = server.available();
  if (client.connected()) {
  char c = client.read();

        if (c == 'a') {
          buggy_line_follow_bool = true;
        }
        if (c == 'd') {
          buggy_line_follow_bool = false;
        }
  }
}
