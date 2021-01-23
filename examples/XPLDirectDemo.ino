

/*
 * 
 * XPLDirect Demo, part one.  This reads the status of the beacon light from X-plane and illuminates the 
 * built-in LED on your arduino board if the beacon is lit.  This was developed on an Arduino Nano board and also
 * tested on a Mega.  It should work across the arduino spectrum without issue.

   To report problems, download updates and examples, suggest enhancements or get technical support:
   
      patreon:  www.patreon.com/curiosityworkshop
      email:    flightpro42@gmail.com
      YouTube:  https://youtube.com/channel/UCISdHdJIundC-OSVAEPzQIQ
 * 
 * 
 */

#include <arduino.h>

#include <XPLDirect.h>              //  include file for the X-plane direct interface
XPLDirect Xinterface(&Serial);      // create an instance of it

// Test Strings:        Use these to test your implementation with the serial monitor without loading xplane.  
// <a>                                                                to get name
// <f>                                                              request dataref needs
// <3001sim/cockpit2/switches/beacon_on>                            assign a dataref handle
// <e0010000001>                                                    to set beacon on
// <e0010000000>                                                    to set beacon off
      

long int beacon;                         // holds status of beacon light

void setup() 
{

  pinMode(LED_BUILTIN, OUTPUT);     // built in LED on arduino board for debug and demonstration purposes
   
  Serial.begin(XPLDIRECT_BAUDRATE); // start serial interface.  Baudrate is specified in the header, dont change   
  Xinterface.begin("Xplane Demo");               // needed for initialization.  Send a texual identifier of your device as a parameter.
 
  ErrorBlink(LED_BUILTIN, 5);       // let everyone know we are awake
   
 //Initialize serial and wait for port to open:
  while (!Serial)
  {
    ErrorBlink(LED_BUILTIN, 2);
    delay(300);
  }

 

  // Now register whatever datarefs we want to use.
  // Parameters are:  registerDataRef(  char *        dataref name, a c style string that identifies the dataref
  //                                    int           read mode, can be XPL_READ or XPL_WRITE or XPL_READWRITE
  //                                    unsigned int  update rate.  0 for constant or specify milliseconds to wait before update from xplane.  currently only implemented for sending values.
  //                                    int           resolution divider.  Use this to reduce traffic.  For instance x-plane sends engine RPM as a constantly changing float value.  Use a divider of 10 to reduce resolution to tens of RPMs ie 1523.34 RPM becomes 1520 RPM
  //                                    long int * or float *   the value that will be sent or received from xplane
  //                            
  //                                    optionally, the last value can be an array and element pointer for xplane datarefs that return arrays:
  //                                    long int *, int index   *or*   float *, int index
                  
 
  Xinterface.registerDataRef("sim/cockpit2/switches/beacon_on", XPL_READ, 100, 0, &beacon);    //  use beacon light dataref for reading

  digitalWrite(LED_BUILTIN, LOW);

}


void loop() 
{
  Xinterface.xloop();  //  needs to run every cycle

  if (beacon) digitalWrite(LED_BUILTIN, HIGH);        // if beacon is on set the builtin led on
  else        digitalWrite(LED_BUILTIN, LOW);

}

void ErrorBlink(int pin, int count)
{
  for (int i = 0; i< count; i++)
  { 
   digitalWrite(pin, HIGH);
   delay(200);
   digitalWrite(pin, LOW);
   delay(100);
  }
}
