#include <XBOXRECV.h>
#include <SPI.h>

uint8_t buf[8] = { 0 };	/* Keyboard report buffer */

uint8_t current_key = 0;

void setup();
void loop();

USB Usb;
XBOXRECV Xbox(&Usb);

void releaseKey() 
{
  buf[0] = 0;
  buf[2] = 0;
  Serial.write(buf, 8); // Release key  
}

void setup() 
{
  Serial.begin(9600);

  #if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  #endif

  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start \r\n"));
    while (1); //Halt if we failed to find the serial connection
  }

  Serial.print(F("\r\nuControl connection established.\r\n"));

}


void loop() 
{
  Usb.Task();

      
      if(Xbox.getAnalogHat(LeftHatX) > 7500 ) { 
        buf[2] = 4; 
        Serial.write(buf, 8); 
        current_key = 4;
        } else {
        buf[2] = current_key; 
        }

        if(Xbox.getAnalogHat(LeftHatX) < -7500 ) { 
        buf[2] = 7; 
        Serial.write(buf, 8); 
        current_key = 7;
        } else {
        buf[2] = current_key; 
        }

        if(Xbox.getAnalogHat(LeftHatY) > 7500 ) { 
        buf[2] = 26; 
        Serial.write(buf, 8); 
        current_key = 26;
        } else {
        buf[2] = current_key; 
        }

        if(Xbox.getAnalogHat(LeftHatY) < -7500 ) { 
        buf[2] = 22; 
        Serial.write(buf, 8); 
        current_key = 22;
        } else {
        buf[2] = current_key; 
        }


      if((Xbox.getAnalogHat(LeftHatY) > -7500 ) && (Xbox.getAnalogHat(LeftHatY) < 7500 ) && (Xbox.getAnalogHat(LeftHatX) > -7500 ) && (Xbox.getAnalogHat(LeftHatX) < 7500 )){
        buf[2] = 0; 
        Serial.write(buf, 8);

      } else {
        buf[2] = current_key; 
        Serial.write(buf, 8);
      }
      //if (Xbox.getAnalogHat(LeftHatX) < -7500) { buf[2] = 7; Serial.write(buf, 8); } else releaseKey();
      

      //if(Xbox.getAnalogHat(LeftHatY) > 7500) { buf[2] = 26; Serial.write(buf, 8); } else releaseKey();
      //if(Xbox.getAnalogHat(LeftHatY) < -7500) { buf[2] = 22; Serial.write(buf, 8); } else releaseKey();

/*

      if(Xbox.getAnalogHat(RightHatX) > 7500) {  buf[2] = 13; Serial.write(buf, 8); releaseKey(); }
      if(Xbox.getAnalogHat(RightHatX) < -7500) {  buf[2] = 15; Serial.write(buf, 8); releaseKey(); }

      if(Xbox.getAnalogHat(RightHatY) > 7500) {  buf[2] = 12; Serial.write(buf, 8); releaseKey(); }
      if(Xbox.getAnalogHat(RightHatY) < -7500) {  buf[2] = 14; Serial.write(buf, 8); releaseKey();	}


*/


      //buf[2] = 0;
      //Serial.write(buf, 8);
}
