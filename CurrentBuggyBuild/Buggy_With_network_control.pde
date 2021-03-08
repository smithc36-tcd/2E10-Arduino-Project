import controlP5.*;
import processing.net.*;

ControlP5 cp5;
Client myClient;
char ServerChar; // character recieved from the server which dictates the reporting output 
boolean LEYE = false;
boolean REYE = false;
int buggy_status = 4; //buggy status 

void setup() {
  size(600,600); // sets size of window 
  cp5 = new ControlP5(this);
  myClient=new Client(this,"192.168.1.105" ,5200); // sets client using the Arduino IP address and port number 5200
  
  PFont p = createFont("Helvetica",16); // sets fonts using in buttons 
  
  cp5.addButton("StartBuggy")
  .setValue(0)
  .setPosition(200,100)
  .setSize(200,40)
  .setFont(p); // creates button for starting 
  
    cp5.addButton("StopBuggy")
  .setValue(0)
  .setPosition(200,150)
  .setSize(200,40)
  .setFont(p); // creates button for stopping 
}
void draw() {
PFont p = createFont("Helvetica",20);
background(255); //sets background colour 
fill(0);
rect(200,35, 200, 50, 10);
rect(150, 70, 300, 150, 10);


fill(255);
text("Controls", 300, 50);
textAlign(CENTER,CENTER);
textFont(p);

fill(0);

text("Buggy Reporting", 300, 280);
text("Eyes:", 220, 337);
text("Status:", 220, 450);
noFill();
strokeWeight(2);
rect(150, 300, 300, 200,10); // sets up graphical User interface 

if(myClient.active()){ // Reads from the server and if it reads a meaningful character it will update the related output
    ServerChar = myClient.readChar();  
     if(ServerChar ==  'u'){LEYE = true;}
     if(ServerChar ==  'i'){LEYE = false;}//if left eye is high or low 
     if(ServerChar ==  'o'){REYE = true; }
     if(ServerChar ==  'p'){REYE = false;}//if right eye is high or low 
     
     if(ServerChar == 'c'){buggy_status = 1;}//Buggy moving 
     if(ServerChar == 'v'){buggy_status = 2;}//buggy turning right
     if(ServerChar == 'b'){buggy_status = 3;}//buggy turning left
     if(ServerChar == 'n'){buggy_status = 4;}// buggy stopped
}
  if(LEYE){text("Left Eye High", 350, 325);}//Updates IR status to the UI
  if(!LEYE){text("Left Eye Low", 350, 325);}
  if(REYE){text("Right Eye High", 350, 350);}
  if(!REYE){text("Right Eye Low", 350, 350);}
  
  switch(buggy_status){ // switch statement which updates the stats of the buggy on the UI
    case 1: text("Moving!", 350, 450);break;
    case 2: text("Turning Right!", 350, 450);break;
    case 3: text("Turning Left!", 350, 450);break;
    case 4: text("Stopped!", 350, 450);break;
  }
}
public void StartBuggy(int theValue) {
  if (myClient.active()){
    myClient.write("a");
     println("Start Buggy Button Pressed"); // if button is pressed it will send the character 'a' to the server which starts the buggy 
  }
}
public void StopBuggy(int theValue) {
  if (myClient.active()){
    myClient.write("d");
     println("Stop Buggy Button Pressed"); // if button is pressed it will send the character 'd' to the server which stops the buggy 
  }
}
