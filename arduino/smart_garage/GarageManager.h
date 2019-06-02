#ifndef __GARAGE_MANAGER__
#define __GARAGE_MANAGER__

#include "Task.h"
#include "LedExt.h"
#include "Pir.h"
#include "ButtonImpl.h"
//Task for managing the garage
class GarageManager: public Task {

  public:
    GarageManager();
    void init(int period);
    void tick();
  private:
    LedExt* ledRed;
    Pir* pir;
    Button* closeButton;
    enum { EMPTY, OPENING, DETECTING, PARKING, CLOSING, FULL} state;
    //Variable used to check time expired (for pir relevation)
    unsigned long lastTime;
    int currentIntensity;
    //Function to wait for the string "open"
    void waitForOpen();
    //Function to open Garage
    void openGarage();
    //Function to close Garage
    void closeGarage();
    //Function to detect presence thanks to the Pir
    void detect();
    //Function to check if user want to force close the garage
    void checkForceClose();
};
#endif

