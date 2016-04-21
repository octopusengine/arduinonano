/*
2015/11 - ver 05
ricardo@crcibernetica.com
http://crcibernetica.com
*/

#include <SoftwareSerial.h>
#include <Nextion.h>

SoftwareSerial nextion(2, 3);// Nextion TX to pin 2 and RX to pin 3 of Arduino

Nextion myNextion(nextion, 9600); //create a Nextion object named myNextion using the nextion serial port @ 9600bps

boolean button1State;
boolean button2State;
int cnt=0;
String serText = " ";

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  Serial.print("test-nextion");
  myNextion.init(); // send the initialization commands for Page 0

  //myNextion.setComponentText("t0", "Hello");
  nexTxt("t0", "3DWARF printer"); //ok moje
  delay(300);
  //int value = 1234;
  //myNextion.setComponentText("t1", String(value));

  serText = "t1.txt=";
  serText = serText + "\""+"test display"+"\"";
  nexCom(serText.c_str());  
  delay(1000);

  nexTxt("t2", "Arduino ok"); //ok moje
}

void nexTxt(String component, String txt){
  //boolean Nextion::setComponentText(String component, String txt){
  String componentText = component + ".txt=\"" + txt + "\"";//Set Component text
   nexCom(componentText.c_str());
  //return ack();
}//end set_component_txt */ 


void nexCom(const char* cmd){
   //Serial.print(cmd);
   //Serial.print(0xFF);
   //Serial.print(char(255));
    nextion.print(cmd);
    nextion.write(0xFF);
    nextion.write(0xFF);
    nextion.write(0xFF); 
}//end sendCommand 



void loop() {
  cnt++;
  //Serial.println(cnt);
  //myNextion.setComponentText("t2", String(cnt)); //---ok puvodni
  
  ///nexTxt("t2", String(cnt)); //ok moje
  
  //Serial.print(">");
  /*
  while (Serial.available()) {    
    //char inChar = (char)Serial.read();
    //Serial.print(inChar);
    //byte inChar = Serial.read();
    byte ctu = Serial.read();
    if (ctu == 'e')
      //if (Serial.read() == 'e')
      //if (inChar=="e") 
      {
      byte ctupom = Serial.read(); //druhy
      Serial.print(ctupom, DEC); //HEX 01/05
      ctupom = Serial.read(); //treti rozlisovaci
      Serial.print(ctupom, DEC);
      if (ctupom == 1) {digitalWrite(13, HIGH);}
      else if (ctupom == 5) {digitalWrite(13, LOW);}
      //  Serial.println("butt1"); delay(1000); }
      Serial.print(".");
      
    
    //else {Serial.print(ctu, HEX);digitalWrite(13, LOW);  }
    
    //inputString += inChar;
    //if (inChar == '\n') 
    //stringComplete = true;
    }
    }
    */
  
  
  String message = myNextion.listen(); //check for message
  //if (message != "") {
    
    //Serial.print(message);
    //Serial.print(".");
  //}
  
  if (message == "65 0 5 0 ffff ffff ffff") {
    myNextion.buttonToggle(button1State, "b0", 0, 2);
    myNextion.setComponentText("t1", "butt id5");
    digitalWrite(13, LOW);
  }
  if (message == "65 0 1 0 ffff ffff ffff") {
    myNextion.buttonToggle(button2State, "b1", 0, 2);
    myNextion.setComponentText("t1", "butt id1");
    digitalWrite(13, HIGH);
  }
  
  if (message == "65 0 3 0 ffff ffff ffff") { //if button "b0" is Released
    myNextion.sendCommand("b0.picc=2"); //set "b0" image to 2
    myNextion.sendCommand("ref b0"); //refresh
  }

  if (Serial.available()) {
    //Serial.print("s.av");
    // wait a bit for the entire message to arrive
    delay(100);
    ///serText = "t1.txt=";
    ///serText = serText + "\"";
        
    // read all the available characters
    serText = "";
    while (Serial.available() > 0) {
      // display each character to the LCD
      //lcd.write(Serial.read());
      //Serial.print(Serial.read());
      serText = serText + char(Serial.read());
    }
    //serText = serText + "\"";
    Serial.println(serText);
    nexCom(serText.c_str());
    
  }
  
  
  delay(200);

}
