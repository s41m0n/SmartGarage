#include "Arduino.h"
#include "GarageManager.h"
#include "Settings.h"

extern bool requireForceClose;
extern bool requireParkAssistant;
extern String inputString;

//Initialize all variables
GarageManager::GarageManager() {
  this-> currentIntensity = FADE_STEP;
  this->lastTime = 0;
  ledRed = new LedExt(LR, 0);
  pir = new Pir(PIR);
  closeButton = new ButtonImpl(CLOSE);
}

//Initialize the task
void GarageManager::init(int period) {
  Task::init(period);
  this->state = EMPTY;
}

//If user send the message to open, Garage says hello and then change state
void GarageManager::waitForOpen() {
  if (inputString == STR_OPEN) {
    Serial.println("GarageManager->Open request received");
    Serial.flush();
    this->state = OPENING;
    ledRed->switchOn();
  }
}

//In this function it is setted the intensity of the red led.
//The step to simulate the 2 seconds are precalculated (see Settings.h)
void GarageManager::openGarage() {
  this->currentIntensity += FADE_STEP;
  if (this->currentIntensity > 255) {
    Serial.println("@Opened@");
    Serial.flush();
    this->state = DETECTING;
    this->currentIntensity = 255;
    this->lastTime = millis();
  } else {
    ledRed->setIntensity(currentIntensity);
  }
}

//In this function the garage is closed in 2 seconds.
void GarageManager::closeGarage() {
  this->currentIntensity -= FADE_STEP;
  if (this->currentIntensity < 0) {
    Serial.println("@Closed@");
    Serial.flush();
    this->state = EMPTY;
    ledRed->switchOff();
    this->currentIntensity = 0;
  } else {
    ledRed->setIntensity(currentIntensity);
  }
}

//In this function is detected the car. If not present, Garage will close and
//turn back to the first state.
void GarageManager::detect() {
  if (millis() - this->lastTime < PIR_RELEVATION_TIME) {
    if (pir->isDetected()) {
      Serial.println("Garage->Welcome home Simone");
      Serial.flush();
      this->state = PARKING;
      requireParkAssistant = true;
    }
  } else {
    Serial.println("GarageManager->No car detected, bye");
    Serial.flush();
    this->state = CLOSING;
  }
}

//Function to check is user want to force close the garage
void GarageManager::checkForceClose() {
  if (closeButton->isPressed()) {
    requireForceClose = true;
  }
}

void GarageManager::tick() {
  switch (this->state) {
    //State to wait an open request
    case EMPTY: {
        this->waitForOpen();
        break;
      }
    //In this state the garage is opening (and can detect a force close request)
    case OPENING: {
        this->checkForceClose();
        if (requireForceClose) {
          Serial.println("GarageManager->Force Close OK");
          Serial.flush();
          this->state = CLOSING;
        } else {
          this->openGarage();
        }
        break;
      }
    //In this state the garage is detecting the car (and can detect a force close request too)
    case DETECTING: {
        this->checkForceClose();
        if (requireForceClose) {
          Serial.println("GarageManager->Force Close OK");
          Serial.flush();
          this->state = CLOSING;
        } else {
          this->detect();
        }
        break;
      }
    //In this state the garage is closing
    case CLOSING: {
        requireForceClose = false;
        this->closeGarage();
        break;
      }
    //In this state the park assistant is working, the garage just check if it has finished
    //or if user want to force close the garage
    case PARKING: {
        this->checkForceClose();
        if (!requireParkAssistant) {
          this->state = CLOSING;
        }
        break;
      }
    default: {
        break;
      }
  }
}
