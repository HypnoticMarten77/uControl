/* uControl Firmware - Written by Mason Anderson */

/* INCLUDE LIBRARIES */
//#include <XBOXONE.h>
#include <XBOXRECV.h>
//#include <spi4teensy3.h> //LIKELY TO REMOVE LATER
#include <SPI.h>

/* DEFINE MACROS */
#undef DEBUG 1 /* [0] Do not debug serial outputs -- [1] Print serial debug */
#define DEBUG_BUTTONS 0
#define NUM_BUTTONS	40
#define NUM_AXES	8	       // 8 axes, X, Y, Z, etc
#ifdef dobogusinclude

#endif


bool send_report = false;
USB Usb;
XBOXRECV Xbox(&Usb);
//XBOXONE Xbox(&Usb);

typedef struct uControlReport {
    int16_t axis[NUM_AXES];
    uint8_t button[(NUM_BUTTONS+7)/8]; // 8 buttons per byte
} uControlReport;

uControlReport report;


void setup(void);
void loop(void);
void setButton(uControlReport *uControl, uint8_t button);
void clearButton(uControlReport *uControl, uint8_t button);
void sendreport(uControlReport *report);


void setup() 
{
    Serial.begin(115200);

  #if !defined(__MIPSEL__)
    while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  #endif

  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start \r\n"));
    while (1); //Halt if we failed to find the serial connection
  }
  Serial.print(F("\r\nuControl connection established.\r\n"));

  
    delay(200);

    for (uint8_t ind=0; ind<8; ind++) {
	    report.axis[ind] = 0;
    }

    for (uint8_t ind=0; ind<sizeof(report.button); ind++) {
        report.button[ind] = 0;
    }

    
}

// Send an HID report to the USB interface
void sendreport(struct uControlReport *report)
{
#ifndef DEBUG
    Serial.write((uint8_t *)report, sizeof(uControlReport));

#else
    // Serial dump for human readable outputs
    for (uint8_t ind=0; ind<NUM_AXES; ind++) {
      Serial.print("axis[");
      Serial.print(ind);
      Serial.print("]= ");
      Serial.print(report->axis[ind]);
      Serial.print(" ");
    }
    Serial.println();
    
    #if DEBUG_BUTTONS == 1
    for (uint8_t ind=0; ind<NUM_BUTTONS/8; ind++) {
      Serial.print("button[");
      Serial.print(ind);
      Serial.print("]= ");
      Serial.print(report->button[ind], HEX);
      Serial.print(" ");
    }
    Serial.println();
    #endif

#endif
}

// Press button 

void setButton(uControlReport *uControl, uint8_t button)
{
    uint8_t index = button/8;
    uint8_t bit = button - 8*index;

    uControl->button[index] |= 1 << bit;
}

// Release button
void clearButton(uControlReport *uControl, uint8_t button)
{
    uint8_t index = button/8;
    uint8_t bit = button - 8*index;

    uControl->button[index] &= ~(1 << bit);
}



void loop() 
{
  Usb.Task();

    send_report = false;

    if(Xbox.XboxReceiverConnected){
    /* Reset all the unused axis */
   // for(int i = 4; i < 8; i++){
     // report.axis[i] = 0;
   // }

   

  /* Send the correct axis based on the input */
    if(Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500){
      report.axis[0] = int(Xbox.getAnalogHat(LeftHatX));
      send_report = true;
    }
    if(Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500){
      report.axis[1] = int(Xbox.getAnalogHat(LeftHatY));
      send_report = true;
    }

    if(Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500){
      report.axis[2] = int(Xbox.getAnalogHat(RightHatX));
      send_report = true;
    }
    if(Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500){
      report.axis[3] = int(Xbox.getAnalogHat(RightHatY));
      send_report = true;
    }


     /* Clear all the buttons */
   // for(int i = 0; i < 40; i++){
   //   report.button[i] = 0;
   // }

 /* Clear all the buttons */
    for(int i = 0; i < 40; i++){
      clearButton(&report,i);
    }

    if (send_report) sendreport(&report);

    }

/*
    //if(Xbox.getButtonPress(A)) setButton(&report, 0); else clearButton(&report, 0);
    for (uint8_t ind=0; ind<sizeof(report.button); ind++) {
        report.button[ind] = 0;
    }
*/
    //sendreport(&report);

    //button++;
    //if (button >= 40) {
    //   button = 0;
    //   press = !press;
    //}
    //delay(2);
}
