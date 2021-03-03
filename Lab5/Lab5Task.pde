import controlP5.*;
import processing.net.*;

ControlP5 cp5;
Client myClient;
String data;


void setup() {
  size(600,600);
  cp5 = new ControlP5(this);
  myClient=new Client(this,"192.168.1.103" ,5200);
  cp5.addButton("StartR")
  .setValue(0)
  .setPosition(100,100)
  .setSize(200,19);
  
    cp5.addButton("StartL")
  .setValue(0)
  .setPosition(100,150)
  .setSize(200,19);
  
      cp5.addButton("Forward")
  .setValue(0)
  .setPosition(100,200)
  .setSize(200,19);
  
      cp5.addButton("Backward")
  .setValue(0)
  .setPosition(100,250)
  .setSize(200,19);
}
void draw() {
}
public void StartR(int theValue) {
  if (myClient.active()){
    myClient.write("a");
     println("Button Pressed");
  }
}
public void StartL(int theValue) {
  if (myClient.active()){
    myClient.write("d");
     println("Button Pressed");
  }
}
public void Forward(int theValue) {
  if (myClient.active()){
    myClient.write("w");
     println("Button Pressed");
  }
}
public void Backward(int theValue) {
  if (myClient.active()){
    myClient.write("s");
     println("Button Pressed");
  }
}
