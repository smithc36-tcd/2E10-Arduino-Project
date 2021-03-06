import controlP5.*;
import processing.net.*;

ControlP5 cp5;
Client myClient;
String data;



void setup() {
  size(600,600);
  cp5 = new ControlP5(this);
  myClient=new Client(this,"192.168.1.104" ,5200);
  
  PFont p = createFont("Helvetica",10);
  //ControlFont myFont = new ControlFont(p);
  
  
  cp5.addButton("StartBuggy")
  .setValue(0)
  .setPosition(100,100)
  .setSize(200,19)
  .setFont(p);
  
    cp5.addButton("StopBuggy")
  .setValue(0)
  .setPosition(100,150)
  .setSize(200,19)
  .setFont(p);


}
void draw() {
}
public void StartBuggy(int theValue) {
  if (myClient.active()){
    myClient.write("a");
     println("Start Button Pressed");
  }
}
public void StopBuggy(int theValue) {
  if (myClient.active()){
    myClient.write("d");
     println("Stop Button Pressed");
  }
}
