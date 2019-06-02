#include "Scheduler.h"
#include "GarageManager.h"
#include "ParkAssistant.h"
#include "SerialManager.h"
#include "Settings.h"
//Three global variable to communicate between Tasks
//Variable for force closing
bool requireForceClose = false;
//Variable for park assistant activation
bool requireParkAssistant = false;
//Variable that contains the input string(open/stop from java)
String inputString = "";

Scheduler sched;

void setup() {
  Serial.begin(9600);
  
  sched.init(SCHEDULER_TIME);

  Task* garageManager = new GarageManager();
  Task* parkAssistant = new ParkAssistant();
  Task* serialManager = new SerialManager();
  
  garageManager->init(TASK_GARAGE_MANAGER_TIME);
  parkAssistant->init(TASK_PARK_ASSISTANT_TIME);
  serialManager->init(TASK_SERIAL_MANAGER_TIME);
  
  sched.addTask(garageManager);
  sched.addTask(parkAssistant);
  sched.addTask(serialManager);
}

void loop() {
  sched.run();
}
