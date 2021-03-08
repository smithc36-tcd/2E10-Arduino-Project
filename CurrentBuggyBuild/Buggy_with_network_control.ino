#include <WiFiNINA.h> 

char ssid[] = "TP-LINK_179E";
char pass[] = "56702460"; // defines SSID and pass for wifi 
WiFiServer server(5200); //Creates a server that listens for incoming connections on the specified port.(5200)
WiFiClient client; // Creates a client that can connect to to a specified internet IP address


//=========================== function declarations ========================
void Buggy_line_follow(); // main function driving the buggy/line follwing 
void read_from_client(); //Reads from processing app
void stopbuggy();//stops buggy 
int US_sensor();//US sensor function 
void Report_IR_L(int lastIRValue);
void Report_IR_R(int lastIRValue);
void buggy_status();

// ========================== global variable declarations =====================
const int Value = 255;
const int Off = 0;//defines values for analog writing to pins (0-255)

const int LEYE = 2; // sets name for left eye 
const int REYE = 10; // sets name for right eye 

const int US_TRIG = 9;//name for Ultrasonic sensor
const int US_ECHO = 8; 

const int PosLeftMotor = 5;
const int NegLeftMotor = 6;
const int NegRightMotor = 11;
const int PosRightMotor = 12;// sets names for the motor pins 

bool buggy_line_follow_bool = false;
bool LeftOn = false;
bool RightOn = false; // Booleans for buggy_status();
bool US_Close = false; // Boolean for Ultrasonic sensor detection 

int increment = 10; // increment for Ultrasonic sensor 
int buggy_status_switch = 1;
int prevStatus;
int lastValueOfLIREye;
int lastValueOfRIREye; //polls for eyes and buggy 
// ============================ SetUp ==============================================================
void setup() {
  Serial.begin(9600); // default serial monitor speed 
  
  pinMode( LEYE, INPUT );
  pinMode( REYE, INPUT );// Sets eye pins as inputs 
  pinMode(US_ECHO, INPUT);//sets echo as input
  pinMode(US_TRIG, OUTPUT); // Sets trigger as input
  pinMode(PosLeftMotor, OUTPUT);
  pinMode(NegLeftMotor, OUTPUT);
  pinMode(PosRightMotor, OUTPUT);
  pinMode(NegRightMotor, OUTPUT);// sets the pins connected the H-Bridge as outputs 
  
  WiFi.begin(ssid, pass); // takes SSID and PASS defined above and connects to WiFi
  IPAddress ip = WiFi.localIP(); //sets WiFi.localIP() returns the IP address of the board
  Serial.print("IP Address:");
  Serial.println(ip); 
  server.begin(); //arduino server set up and prints IP address for server
}
// ========================== Loop ================================================================
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
 
  
  if( digitalRead( LEYE ) == LOW && !US_Close ){ // if no black line detected and there is no objects in front of the buggy it will start left motor
    analogWrite(PosLeftMotor, Value); //full power (255)  to motor 
    LeftOn = true; // bool for checking cuurent buggy status in buggy_status();
    }else{
    analogWrite(PosLeftMotor, Off);// No power (0) to motor 
    LeftOn = false; // bool for checking cuurent buggy status in buggy_status();
  }

  if( digitalRead( REYE ) == LOW && !US_Close){ // if no black line detected and there is no objects in front of the buggy it will start left motor
    analogWrite(PosRightMotor, Value); //full power (255)  to motor 
    RightOn = true; // bool for checking cuurent buggy status in buggy_status();
  }else{
    analogWrite(PosRightMotor, Off); // no power (0) to motor
    RightOn = false; // bool for checking cuurent buggy status in buggy_status();
  }
  
  if(lastValueOfLIREye != digitalRead(LEYE)){ // checks if IR eye stats has changed 
    lastValueOfLIREye = digitalRead(LEYE); //reads value of IR eyes
    Report_IR_L(lastValueOfLIREye); //reports value of IR eye to the client 
  }

  if(lastValueOfRIREye != digitalRead(REYE)){// checks if IR eye stats has changed
    lastValueOfRIREye = digitalRead(REYE); //reads values of IR eyes 
    Report_IR_R(lastValueOfRIREye);//reports value of IR eye to the client
  }

  
   read_from_client();//monitors the client to check start/stop commands 
   buggy_status(); //Everyloop the buggy reports its current status and the status of the IR eyes
}

// ==========================================================================================
void stopbuggy(){// stops buggy 
  
  analogWrite(PosLeftMotor, Off);
  analogWrite(PosRightMotor, Off); // sets the motors value to 0
}
// ==========================================================================================
int US_sensor(){ // calculates the distance of objects on front of the buggy and returns it as an integer 
  
  digitalWrite( US_TRIG, LOW );
  delayMicroseconds(2);
  digitalWrite( US_TRIG, HIGH );
  delayMicroseconds( 10 );
  digitalWrite( US_TRIG, LOW ); // trigger pin pulses and the time to return is measured 

  long duration = pulseIn( US_ECHO, HIGH ); // pulse in measures the time take for the signal to return to the sensor 
  int distance = duration/58; // calculates the distance on fron in CM.

    return distance;
}
// ==========================================================================================
void read_from_client(){ // Reads from the client to detect whether the start/stop command was recieved 

  client = server.available();
  if (client.connected()) {
  char c = client.read(); // reads char from client 

        if (c == 'a') {
          buggy_line_follow_bool = true; //begin line following 
        }
        if (c == 'd') {
          buggy_line_follow_bool = false; // stop lines following 
          stopbuggy(); // stops buggy 
        }
  }
}
// ==========================================================================================
void Report_IR_L(int lastIRValue){ // reports current status of the  Left IR eye to the client 
client = server.available();
    if(digitalRead(LEYE) == HIGH) {
      server.write("u"); // writes character 'u' to all clients connected to server 
    }
    if(digitalRead(LEYE) == LOW) {
      server.write("i"); // writes character 'i' to all clients connected to server

    }
}
// ==========================================================================================
void Report_IR_R(int lastIRValue){ // reports current status of the Right IR eye to the client
client = server.available();
    if(lastIRValue == HIGH) {
      server.write("o"); // writes character 'o' to all clients connected to server 
    }
    if(lastIRValue == LOW) {
      server.write("p"); //writes character 'p' to all clients connected to server 
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
  
      switch(buggy_status_switch){ // switch statement to write to the server and declare the current buggy status 
  case 1: server.write("c");break; // Case 1: The buggy is moving forward
  case 2: server.write("v");break; // Case 2: The buggy is turning right
  case 3: server.write("b");break; // Case 3: The buggy is turning left
  case 4: server.write("n");break; // Case 4: The buggy is stopped
  }
  
  }
  prevStatus = buggy_status_switch; //updates poll
}

// ==========================================================================================
