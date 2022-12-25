#include "SoftwareSerial.h"

SoftwareSerial bluetoothSerial(2, 3); // TX, RX

String bluetoothIsReady;

String commandState = "";
String commandValue = "";

void initBluetooth() {
  bluetoothSerial.begin(9600);
  Serial.begin(9600);
  
  while(!Serial);

  sendCommand("AT");
  sendCommand("AT+ROLE0");
  sendCommand("AT+UUIDFFE0");
  sendCommand("AT+CHARFFE1");
  sendCommand("AT+NAMEPADEL_SHOOTER"); 
}

void updateSerial() {
  signalColor(0, 0, 255);
    
  if (bluetoothSerial.available()) {
    String command = bluetoothSerial.readString();
    Serial.println(commandState); 

    if(command.startsWith("state:")) {
      int index = command.indexOf(':');
      
      commandValue = "";
      commandState = command.substring(index, command.length());
    }

    if(command.startsWith("set:")) {
      int index = command.indexOf(':');

      commandValue = command.substring(index, command.length());
    }
  }

// Enable if we need to send back information...
//  if (Serial.available()) {
//    bluetoothSerial.println(Serial.read());     
//  }
}

bool isCommandSet() {
  return commandState != "" && commandValue != "";  
}

String getCommandState() {
  return commandState;  
}

String getCommandValue() {
  return commandValue;  
}

void sendCommand(const char * command) {
  Serial.print("Command send :");
  Serial.println(command);
  bluetoothSerial.println(command);
  //wait some time
  delay(100);

  char reply[100];
  int i = 0;
  while (bluetoothSerial.available()) {
    reply[i] = bluetoothSerial.read();
    i += 1;
  }
  //end the string
  reply[i] = '\0';
  Serial.print(reply);
  Serial.println("\nReply end");                 
  delay(100);
}
