
const int LEYE = 2; // sets name for left eye 
const int REYE = 10; // sets name for right eye 

const int US_TRIG = 9;//name for Ultrasonic sensor
const int US_ECHO = 8; 

const int AOne = 5; // sets name for H-bridge input A1 
const int ATwo = 6; // sets name for H-brigde input A2
const int AThree = 12; // sets name for H-bridge input A3 
const int AFour = 11; // Sets name for H-brigde input A4 

void setup() {
  Serial.begin(9600); // Sets serial speed to default of 9600

  pinMode( LEYE, INPUT );
  pinMode( REYE, INPUT );// Sets eye pins as inputs 
  pinMode(US_ECHO, INPUT);//sets echo as input
  pinMode(US_TRIG, OUTPUT); // Sets trigger as input 
  pinMode( AOne, OUTPUT ); // +Left Motor 
  pinMode( ATwo, OUTPUT ); // -Left Motor 
  pinMode( AThree, OUTPUT ); // +Right motpr 
  pinMode( AFour, OUTPUT ); // - Right motor // Sets motorcontol pins to outputs
}

void loop() {

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
    //analogWrite(AOne, Off);
    //analogWrite(ATwo, Off);
    //analogWrite(AThree, Off);
    //analogWrite(AFour, Off);
    Serial.println("less than 10");
  }else{
    US_Close = false; 
    }
  
  if( digitalRead( LEYE ) == LOW && !US_Close ){ // if left eye is LOW send high signal to motor input A1 and HIGH to A2
    analogWrite(AOne, Value);
    }else{
    analogWrite(AOne, Off);// No power 
  }

  if( digitalRead( REYE ) == LOW && !US_Close ){ // if left eye is LOW send high signal to motor input A4 and to A3
    analogWrite(AFour, Value);
  }else{
    analogWrite(AFour, Off); // no power 
  }
  //delay(1);
}
