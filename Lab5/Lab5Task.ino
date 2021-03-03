#include <WiFiNINA.h>

char ssid[] = "TP-LINK_179E";
char pass[] = "56702460";
WiFiServer server(5200);
 char command;

const int PosLeftMotor = 5; 
const int NegLeftMotor = 6;
const int NegRightMotor = 11; 
const int PosRightMotor = 12;


void setup() {
Serial.begin(9600);
pinMode (PosLeftMotor, OUTPUT);
pinMode (NegLeftMotor, OUTPUT);
pinMode(PosRightMotor, OUTPUT); 
pinMode(NegRightMotor, OUTPUT);
WiFi.begin(ssid, pass);
IPAddress ip = WiFi.localIP();
Serial.print("IP Address:");
Serial.println(ip);
server.begin();
}

void loop() {
  WiFiClient client = server.available();
    if (client.connected()) {
      //Serial.println("Client Connected");
      char c = client.read();
        if(c == 'a'){
          //Serial.println("Starting Right Motor");
      turn_r();
         }
           if(c == 'd'){
          //Serial.println("Starting Left Motor");
      turn_l();
         }
         if(c == 'w'){
          //Serial.println("Starting Forward");
      forward();
         }
         if(c == 's'){
          //Serial.println("Starting Backward");
      backward();
         }
    }
}

void turn_r(){
  digitalWrite(PosLeftMotor, HIGH);
  digitalWrite(NegRightMotor,HIGH);
  delay(500);
  digitalWrite(PosLeftMotor,LOW);
  digitalWrite(NegRightMotor,LOW);
}
void turn_l(){
  digitalWrite(PosRightMotor, HIGH);
  digitalWrite(NegLeftMotor, HIGH);
  delay(500);
  digitalWrite(PosRightMotor,LOW);
  digitalWrite(NegLeftMotor, LOW);
}
void forward(){
  digitalWrite(PosRightMotor, HIGH);
  digitalWrite(PosLeftMotor, HIGH);
  delay(500);
  digitalWrite(PosRightMotor,LOW);
  digitalWrite(PosLeftMotor, LOW);
}
void backward(){
  digitalWrite(NegRightMotor, HIGH);
  digitalWrite(NegLeftMotor, HIGH);
  delay(500);
  digitalWrite(NegRightMotor,LOW);
  digitalWrite(NegLeftMotor, LOW);
}
