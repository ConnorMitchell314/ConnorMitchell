#ifndef DS_INTERFACE_H_INCLUDED
#define DS_INTERFACE_H_INCLUDED

#include <SoftwareSerial.h>

/*
 * DS_Interface stands for “Drivers Station Interface”
 * This class allows you to retrieve keystrokes that originate
 * on a python-based keyboard drivers station on a laptop, over 
 * wifi, assuming you have a nodeMCU set up properly to be 
 * the wifi or “radio link”, and with serial wires and shared ground 
 * wire connections to the arduino UNO
 * see: https://docs.google.com/drawings/d/13E9H3TtKm9N3-c0XWWdpt-mgodKqjUlVzRVLXEZ_MkY/edit?usp=sharing 
 *  for wiring details.
 */
class DS_Interface : public SoftwareSerial {
    public:
        // Create an instance in the overall scope above 
        // your code for Setup() & Loop()
        DS_Interface (int serial_rx, int serial_tx);
        virtual ~DS_Interface ();

        /*
         * init - a class method to call the setup on the pins 
         * specified in the constructor call for rx and tx
         * Call this from the Arduino Setup() function.
         */ 

        void init();

        /* 
         * readInputIfAvailable
         *  Each time this is called it gathers 1 ascii keystroke 
         *  that was sent from the driver's station
         *  OR
         *  it returns a char == '\0' which means there was no 
         *  keystroke available from the driver's station Note:
         * ‘\0’ == 0 so it’s easy to do an “if” on it.
         *  The driver's station only sends when the keyboard 
         *  provides a new keystroke
         *  There is no encoding for non-ascii keys such as 
         *  “arrow keys” and “F1” keys. Using those won’t work.
         */

        char readInputIfAvailable();
};

#endif // DS_INTERFACE_H_INCLUDED