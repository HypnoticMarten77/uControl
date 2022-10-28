/* Normal Arduino Libraries */
#include <Arduino.h>
#include <SPI.h>
#include <String.h>

/* Adafruit BLE Module Libraries */
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "BluefruitConfig.h"

/* Host Shield Libraries */
#include <PS4BT.h>
#include <usbhub.h>
// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

/* MACROS */
#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

#define FACTORYRESET_ENABLE         1
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define MODE_LED_BEHAVIOUR          "MODE"

/* Global Variables */
int state = 0;
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_SCK, BLUEFRUIT_SPI_MISO, BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

USB Usb;
BTD Btd(&Usb);
PS4BT PS4(&Btd, PAIR);
boolean toggle1 = 0;

// After that you can simply create the instance like so and then press the PS button on the device
//PS4BT PS4(&Btd);

bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;


void timerSetup(){

  pinMode(24, OUTPUT);
  cli();

  //set timer1 interrupt at 1Hz
    TCCR1A = 0;// set entire TCCR1A register to 0
    TCCR1B = 0;// same for TCCR1B
    TCNT1  = 0;//initialize counter value to 0
    // set compare match register for 1hz increments
    OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    // Set CS10 and CS12 bits for 1024 prescaler
    TCCR1B |= (1 << CS12) | (1 << CS10);  
    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);

  sei();
  
}

ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)
//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
  if (toggle1){
    digitalWrite(24,HIGH);
    toggle1 = 0;
  }
  else{
    digitalWrite(24,LOW);
    toggle1 = 1;
  }

  ble.println("AT+BLEUARTRX");
  ble.readline(1,false);
  if (strcmp(ble.buffer, "OK") == 0) { return; }
    
  /* Read data that was found in the buffer on interrupt */
  //Serial.print(F("[Recv] "));
  String str = ble.buffer;
  //Serial.println(str);
  ble.waitForOK();


        
  if(str.equals("PS5")) state = 0;
  if(str.equals("PS4")) state = 1;

  Serial.print(F("State: "));
  Serial.println(state);


}


void setup(void)
{

  timerSetup();

  Serial.begin(115200);

  #if !defined(__MIPSEL__)
    while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  #endif

  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1);
  }
  Serial.println(F("\r\nPS4 Bluetooth Library Started"));

  delay(1000);

  ble.begin(VERBOSE_MODE); //Disable verbose mode for production release (useful for debug)
  ble.echo(false); //Disable echoing commands
  ble.verbose(false); //Disable verbose mode after init

  Serial.println("Waiting for connection...");

  /* Wait for connection */
  while (! ble.isConnected()) {
    delay(100);
  }

  Serial.println("uControl Connected.");
  delay(100);

  
}



void loop(void)
{


  
  Usb.Task();
 
  if (PS4.connected()) {
      if (PS4.getAnalogHat(LeftHatX) > 137 || PS4.getAnalogHat(LeftHatX) < 117 || PS4.getAnalogHat(LeftHatY) > 137 || PS4.getAnalogHat(LeftHatY) < 117 || PS4.getAnalogHat(RightHatX) > 137 || PS4.getAnalogHat(RightHatX) < 117 || PS4.getAnalogHat(RightHatY) > 137 || PS4.getAnalogHat(RightHatY) < 117) {
        Serial.print(F("\r\nLeftHatX: "));
        Serial.print(PS4.getAnalogHat(LeftHatX));
        Serial.print(F("\tLeftHatY: "));
        Serial.print(PS4.getAnalogHat(LeftHatY));
        Serial.print(F("\tRightHatX: "));
        Serial.print(PS4.getAnalogHat(RightHatX));
        Serial.print(F("\tRightHatY: "));
        Serial.print(PS4.getAnalogHat(RightHatY));
      }

      if (PS4.getAnalogButton(L2) || PS4.getAnalogButton(R2)) { // These are the only analog buttons on the PS4 controller
        Serial.print(F("\r\nL2: "));
        Serial.print(PS4.getAnalogButton(L2));
        Serial.print(F("\tR2: "));
        Serial.print(PS4.getAnalogButton(R2));
      }
      if (PS4.getAnalogButton(L2) != oldL2Value || PS4.getAnalogButton(R2) != oldR2Value) // Only write value if it's different
        PS4.setRumbleOn(PS4.getAnalogButton(L2), PS4.getAnalogButton(R2));
      oldL2Value = PS4.getAnalogButton(L2);
      oldR2Value = PS4.getAnalogButton(R2);

      if (PS4.getButtonClick(PS)) {
        Serial.print(F("\r\nPS"));
        PS4.disconnect();
      }
      else {
        if (PS4.getButtonClick(TRIANGLE)) {
          Serial.print(F("\r\nTriangle"));
          PS4.setRumbleOn(RumbleLow);
        }
        if (PS4.getButtonClick(CIRCLE)) {
          Serial.print(F("\r\nCircle"));
          PS4.setRumbleOn(RumbleHigh);
        }
        if (PS4.getButtonClick(CROSS)) {
          Serial.print(F("\r\nCross"));
          PS4.setLedFlash(10, 10); // Set it to blink rapidly
        }
        if (PS4.getButtonClick(SQUARE)) {
          Serial.print(F("\r\nSquare"));
          PS4.setLedFlash(0, 0); // Turn off blinking
        }

        if (PS4.getButtonClick(UP)) {
          Serial.print(F("\r\nUp"));
          PS4.setLed(Red);
        } if (PS4.getButtonClick(RIGHT)) {
          Serial.print(F("\r\nRight"));
          PS4.setLed(Blue);
        } if (PS4.getButtonClick(DOWN)) {
          Serial.print(F("\r\nDown"));
          PS4.setLed(Yellow);
        } if (PS4.getButtonClick(LEFT)) {
          Serial.print(F("\r\nLeft"));
          PS4.setLed(Green);
        }

        if (PS4.getButtonClick(L1))
          Serial.print(F("\r\nL1"));
        if (PS4.getButtonClick(L3))
          Serial.print(F("\r\nL3"));
        if (PS4.getButtonClick(R1))
          Serial.print(F("\r\nR1"));
        if (PS4.getButtonClick(R3))
          Serial.print(F("\r\nR3"));

        if (PS4.getButtonClick(SHARE))
          Serial.print(F("\r\nShare"));
        if (PS4.getButtonClick(OPTIONS)) {
          Serial.print(F("\r\nOptions"));
          printAngle = !printAngle;
        }
        if (PS4.getButtonClick(TOUCHPAD)) {
          Serial.print(F("\r\nTouchpad"));
          printTouch = !printTouch;
        }

        if (printAngle) { // Print angle calculated using the accelerometer only
          Serial.print(F("\r\nPitch: "));
          Serial.print(PS4.getAngle(Pitch));
          Serial.print(F("\tRoll: "));
          Serial.print(PS4.getAngle(Roll));
        }

        if (printTouch) { // Print the x, y coordinates of the touchpad
          if (PS4.isTouching(0) || PS4.isTouching(1)) // Print newline and carriage return if any of the fingers are touching the touchpad
            Serial.print(F("\r\n"));
          for (uint8_t i = 0; i < 2; i++) { // The touchpad track two fingers
            if (PS4.isTouching(i)) { // Print the position of the finger if it is touching the touchpad
              Serial.print(F("X")); Serial.print(i + 1); Serial.print(F(": "));
              Serial.print(PS4.getX(i));
              Serial.print(F("\tY")); Serial.print(i + 1); Serial.print(F(": "));
              Serial.print(PS4.getY(i));
              Serial.print(F("\t"));
            }
          }
        }
      }
    }

 

  
}


