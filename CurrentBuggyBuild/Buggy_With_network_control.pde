import controlP5.*;
import processing.net.*;

ControlP5 cp5;
Client myClient;
char ServerChar;
boolean LEYE = false;
boolean REYE = false;
int buggy_status = 4;

void setup() {
  size(600,600);
  cp5 = new ControlP5(this);
  myClient=new Client(this,"192.168.1.107" ,5200);
  
  
  PFont p = createFont("Helvetica",10);
  //ControlFont myFont = new ControlFont(p);
  
  
  cp5.addButton("StartBuggy")
  .setValue(0)
  .setPosition(200,100)
  .setSize(200,40)
  .setFont(p);
  
    cp5.addButton("StopBuggy")
  .setValue(0)
  .setPosition(200,150)
  .setSize(200,40)
  .setFont(p);
}
void draw() {
PFont p = createFont("Helvetica",20);
background(255);

text("Controls", 300, 50);
textAlign(CENTER,CENTER);
textFont(p);
fill(0);

text("Buggy Reporting", 300, 300);

if(myClient.active()){
    ServerChar = myClient.readChar();  
     if(ServerChar ==  'u'){LEYE = true;}
     if(ServerChar ==  'i'){LEYE = false;}
     if(ServerChar ==  'o'){REYE = true; }
     if(ServerChar ==  'p'){REYE = false;}
     
     if(ServerChar == 'c'){buggy_status = 1;}
     if(ServerChar == 'v'){buggy_status = 2;}
     if(ServerChar == 'b'){buggy_status = 3;}
     if(ServerChar == 'n'){buggy_status = 4;}
     
}
  if(LEYE){text("Left Eye High", 300, 325);}
  if(!LEYE){text("Left Eye Low", 300, 325);}
  if(REYE){text("Right Eye High", 300, 350);}
  if(!REYE){text("Right Eye Low", 300, 350);}
  
  switch(buggy_status){
    case 1: text("Moving!", 300, 400);break;
    case 2: text("Turning Right!", 300, 400);break;
    case 3: text("Turning Left!", 300, 400);break;
    case 4: text("Stopped!", 300, 400);break;
  }
  
    
     
}
public void StartBuggy(int theValue) {
  if (myClient.active()){
    myClient.write("a");
     println("Start Buggy Button Pressed");
  }
}
public void StopBuggy(int theValue) {
  if (myClient.active()){
    myClient.write("d");
     println("Stop Buggy Button Pressed");
  }
}
/*
void IRCheck(){
  if(myClient.active()){
    //println("here");
    ServerChar = myClient.readChar();
    if( myClient.readChar() == 'u'){
      ServerChar = 'u';
      println("ServerChar u");
    }
    if( myClient.readChar() == 'i'){
      ServerChar = 'i';
      println("ServerChar i");
    }
    if( myClient.readChar() == 'o'){
      ServerChar = 'o';
      println("ServerChar o");
    }
    if( myClient.readChar() == 'p'){
      ServerChar = 'p';
      println("ServerChar p");
    }
  }
}
*/
/*public void Lefteye (int theValue){
  if(myClient.active()){
    char ServerChar = char(myClient.read());
    if( ServerChar == 'u'){
      Toggle("Lefteye").setValue(1);
    }
  }
}*/
