#include "SerialManager.h"

extern bool serialWriteMode;
extern String inputString;
extern String outputString;

SerialManager::SerialManager() {
}

void SerialManager::init(int period) {
  Task::init(period);
}

//Method to read message from serial, when it finishes the string is cleared
//(But as I created a finished sequential state machine, before the string is cleared at least 1 task
//has read it)
void SerialManager::serialRead() {
  if (Serial.available()) {
    inputString += (char)Serial.read();
  }
  else {
    inputString = "";
  }
}

void SerialManager::tick() {
  this->serialRead();
}
