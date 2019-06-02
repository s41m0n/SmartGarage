#ifndef __PARK_ASSISTANT__
#define __PARK_ASSISTANT__

#include "Task.h"
#include "ButtonImpl.h"
#include "LedExt.h"
#include "Sonar.h"
#include "Settings.h"

class ParkAssistant: public Task {

public:
  ParkAssistant();  
  void init(int period);  
  void tick();
private:
  enum { RUNNING, PAUSE} state;
  //Car distance
  float distance;
  //Variable to save last time it prints the distance(because I don't want Arduino spam)
  unsigned long lastDistance;
  //Variable to save last time it says touching(because I don't want Arduino spam)
  unsigned long lastTouching;
  //Variable to save last time it says ok can stop(because I don't want Arduino spam)
  unsigned long lastStop;
  int currentIntensity1;
  int currentIntensity2;
  LedExt* ledDist1;
  LedExt* ledDist2;
  Button* touchButton;
  Sonar*  proxSensor;
  //Function to print the distance of the car
  void printDistance();
  //Function to set leds intensity
  void setLedDist();
  //Function to check if the car is touching
  void checkTouching();
  //Function to detect if the car has stopped
  bool hasStopped();
  //Function that return true if the current distance is lower than DIST_CLOSE
  bool checkDistance();
};

#endif
