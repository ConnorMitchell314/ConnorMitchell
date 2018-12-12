#include "ds_interface.h"

#define BUFFER_SIZE 10
#define BAUD_RATE 115200
#define SERIAL_TIMEOUT 500

DS_Interface::DS_Interface (int serial_rx, int serial_tx)
    :SoftwareSerial(serial_rx, serial_tx) 
{
    /* Our class is a subclass of the SoftwareSerial class. 
     * When we invoked the SoftwareSerial constructor after the “:”
     * above, we had done all the work needed. We could do more 
     * here but have nothing special to do, so we just have 
     * empty curly braces. This isn’t unusual in “ceremonial 
     * languages” such as c++ and java. Usually one leaves a comment
     * to show it wasn’t a mistake a la 
     * (“This page left blank intentionally”)
     */ 
}

DS_Interface::~DS_Interface () 
{
    /* It is necessary to define a destructor for the class, so that
     * the code will link correctly. However, at this time, with this
     * definition of the class, there is nothing to “destroy”, so it
     * is just a placeholder function, because in c++ the 
     * compiler/linker expect you to define what to do when an object
     * reaches end of life. In this case, we define it as doing  
     * “nothing”. But even “nothing” is a definition of the correct 
     * action.
     */
}

void DS_Interface::init()
{
    this->begin(BAUD_RATE);
    this->setTimeout(SERIAL_TIMEOUT);
}


char DS_Interface::readInputIfAvailable() 
{
  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);
  if (this->available() > 0) {
    this->readBytesUntil( '\n', buffer, BUFFER_SIZE);
  }
  return buffer[0];
}