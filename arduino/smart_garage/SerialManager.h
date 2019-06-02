#ifndef __SERIAL_MANAGER__
#define __SERIAL_MANAGER__

#include "Arduino.h"
#include "Task.h"

//Task for reading serial messages
class SerialManager: public Task {

  public:
    SerialManager();
    void init(int period);
    void tick();
  private:
  //Method to read message from serial
    void serialRead();

};

#endif
