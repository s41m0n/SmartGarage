#include "Arduino.h"
#include "ParkAssistant.h"

extern bool requireParkAssistant;
extern String inputString;
extern bool requireForceClose;

//Simple function to map a floating value from OriginalRange to a ChosenRange
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//Initialize all variables
ParkAssistant::ParkAssistant() {
  this->distance = 0.0;
  ledDist1 = new LedExt(LDIST1, 0);
  ledDist2 = new LedExt(LDIST2, 0);
  touchButton = new ButtonImpl(TOUCH);
  proxSensor = new Sonar(PROX_ECHO, PROX_TRIG);
  this->lastDistance = 0;
  this->lastTouching = 0;
  this->lastStop = 0;
}

//Initialize the task
void ParkAssistant::init(int period) {
  Task::init(period);
  state = PAUSE;
}

//Return true if the current distance is lower than the closure distance
bool ParkAssistant::checkDistance() {
  return this->distance <= DIST_CLOSE;
}

//Method to detect if the user has send a Stop request
bool ParkAssistant::hasStopped() {
  if (inputString == STR_STOP) {
    //check if the distance is respected
    if (this->checkDistance()) {
      Serial.println("ParkAssistant->Stop Request OK");
      Serial.flush();
      return true;
    } else {
      Serial.println("ParkAssistant->Stop Request REJECTED (TOO FAR)");
      Serial.flush();
    }
  }
  return false;
}

//Function to print the distance
void ParkAssistant::printDistance() {
  if (millis() - lastDistance > PRINT_TIME) {
    Serial.print("Distance: ");
    Serial.flush();
    Serial.println(this->distance);
    Serial.flush();
    this->lastDistance = millis();
  }
}

//Function to set leds.
void ParkAssistant::setLedDist() {
  if (this->distance <= DIST_MAX && this->distance > DIST_MED) {
    ledDist1->switchOn();
    ledDist2->switchOff();
    this->currentIntensity1 = mapfloat(this->distance, DIST_MED, DIST_MAX, 255, 0);
    this->currentIntensity2 = 0;
  } else if (this->distance <= DIST_MED && this->distance >= DIST_MIN) {
    ledDist1->switchOn();
    ledDist2->switchOn();
    this->currentIntensity1 = 255;
    this->currentIntensity2 = mapfloat(this->distance, DIST_MIN, DIST_MED, 255, 0);
  } else if (this->distance > DIST_MAX) {
    ledDist1->switchOff();
    ledDist2->switchOff();
    this->currentIntensity1 = 0;
    this->currentIntensity2 = 0;
  } else {
    ledDist1->switchOn();
    ledDist2->switchOn();
    this->currentIntensity1 = 255;
    this->currentIntensity2 = 255;
  }
  ledDist1->setIntensity(this->currentIntensity1);
  ledDist2->setIntensity(this->currentIntensity2);
}

//Function to detect if the car is touching
void ParkAssistant::checkTouching() {
  if (touchButton->isPressed() && this->distance < DIST_MIN) {
    if (millis() - this->lastTouching > PRINT_TIME) {
      Serial.println("ParkAssistant->Touching");
      Serial.flush();
      this->lastTouching = millis();
    }
  }
}

void ParkAssistant::tick() {
  switch (state) {
    //During pause state simply it waits that garage manager active it.
    case PAUSE: {
        if (requireParkAssistant) {
          this->state = RUNNING;
        }
        break;
      }
    //Running state
    case RUNNING: {
        //Read and print distance
        this->distance = proxSensor->getDistance();
        this->printDistance();
        //if stopped, switch all of and turn back to the PAUSE state
        //else set leds, check if touching and notify if the car can stop
        if (!this->hasStopped()) {
          this->setLedDist();
          this->checkTouching();
          if (this->distance < DIST_MIN) {
            if (millis() - this->lastStop > PRINT_TIME) {
              Serial.println("ParkAssistant->Ok can stop");
              Serial.flush();
              this->lastStop = millis();
            }
          }
        } else {
          ledDist1->switchOff();
          ledDist2->switchOff();
          requireParkAssistant = false;
          this->state = PAUSE;
        }
        //If the Garage has detected a force close request, check if the car is too far or if the garage can close
        if (requireForceClose) {
          if (this->checkDistance()) {
            Serial.println("ParkAssistant->Force Close OK");
            Serial.flush();
            ledDist1->switchOff();
            ledDist2->switchOff();
            requireParkAssistant = false;
            this->state = PAUSE;
          } else {
            Serial.println("ParkAssistant->Force Close REJECTED (TOO FAR)");
            Serial.flush();
          }
          requireForceClose = false;
        }
        break;
      }
    default: {
        break;
      }
  }
}
