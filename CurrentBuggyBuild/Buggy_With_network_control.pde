import controlP5.*;
import processing.net.*;

ControlP5 cp5;
Client myClient;
String data;
color LEYE = #000000;
color REYE = #07F515;


void setup() {
  size(600,600);
  cp5 = new ControlP5(this);
  myClient=new Client(this,"192.168.1.101" ,5200);
  
  PFont p = createFont("Helvetica",10);
  //ControlFont myFont = new ControlFont(p);
  
  
  cp5.addButton("StartBuggy")
  .setValue(0)
  .setPosition(100,100)
  .setSize(200,25)
  .setFont(p);
  
    cp5.addButton("StopBuggy")
  .setValue(0)
  .setPosition(100,150)
  .setSize(200,25)
  .setFont(p);
 
 //cp5.addToggle("Lefteye").setValue(0).setPosition(400, 100).setColor(LEYE);
}
void draw() {
  fill(LEYE);
  circle(400, 100, 25);
  fill(REYE);
  circle(450, 100, 25);
  //IRCheck();
//println("reading");
  
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
    String ServerChar = myClient.readString();
    if( ServerChar == "u"){
    LEYE = #07F515;
    draw();
    }
    if( ServerChar == "i"){
    LEYE = #000000;
    draw();
    }
    if( ServerChar == "o"){
    REYE = #07F515;
    draw();
    }
    if( ServerChar == "p"){
    LEYE = #000000;
    draw();
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
