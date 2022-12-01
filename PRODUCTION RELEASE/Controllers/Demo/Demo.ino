/* Controller Libraries */
#include "Includes.h"
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
#include <ControllerBT.h>
#include <usbhub.h>



/* MACROS */
#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

#define FACTORYRESET_ENABLE         1
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define MODE_LED_BEHAVIOUR          "MODE"


#undef DEBUG



struct {
    int8_t x;
    int8_t y;
    uint8_t buttons;
    uint8_t rfu;
    int8_t z;
    int8_t rx;
} joyReport;

USB Usb;
BTD Btd(&Usb);
ControllerBT Controller(&Btd, PAIR);
PS3BT PS3(&Btd, PAIR);
PS5BT PS5(&Btd, PAIR);
SWITCHPROBT SWITCHPRO(&Btd, PAIR);
XBOX360WIFI XBOX360(&Btd, PAIR);
XBOXONESBT XBOXONES(&Btd, PAIR);
WIIBT WII(&Btd, PAIR);

/* Global Variables */
int state = 0;
/* For tilt sensors and touchpad on the Controller/PS5 dualshock/dualsense */
bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;

/*Initialize the SPI bluetooth module to receive the configuration */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_SCK, BLUEFRUIT_SPI_MISO, BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

void setup() 
{
  timerSetup();

  //Wait for a serial connection
  #if !defined(__MIPSEL__)
  while (!Serial);
  #endif

  Serial.begin(115200);
  delay(200);

  //Initialize USB libraries
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1);
  }
  Serial.println(F("\r\nuControl Bluetooth Library Started"));
  delay(100);

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

ISR(TIMER1_COMPA_vect){//timer1 reads from the bluetooth module once per second


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
  if(str.equals("PS3")) state = 2;
  if(str.equals("XBX")) state = 3;
  if(str.equals("WII")) state = 4;
  if(str.equals("SWP")) state = 5;
  if(str.equals("XBB")) state = 6;
  if(str.equals("SWP")) state = 5;

  switch(state){

    case 0:
      Controller = connect(&Btd, PS5);
      break;
    case 1:
      Controller = connect(&Btd, PS4);
      break;
    case 2:
      Controller = connect(&Btd, PS3);
      break;
    case 3:
      Controller = connect(&Btd, XBOXONES);
      break;
    case 4:
      Controller = connect(&Btd, WII);
      break;
    case 5:
      Controller = connect(&Btd, SWITCHPRO);
      break;
    case 6:
      Controller = connect(&Btd, XBOX360);
      break;
    default: 
      Controller = nullptr;
      break;
  }


}
// Send an HID report to the USB interface
void sendJoyReport(struct joyReport_t *report)
{
    Serial.write((uint8_t *)report, sizeof(joyReport_t));
}

void sendJoyReport(struct joyReport_t *report, int size){
  Serial.write((uint8_t *)&joyReport, size);

}

// turn a button on
void setButton(joyReport_t *joy, uint8_t button)
{
    uint8_t index = button/8;
    uint8_t bit = button - 8*index;

    joy->button[index] |= 1 << bit;
}

// turn a button off
void clearButton(joyReport_t *joy, uint8_t button)
{
    uint8_t index = button/8;
    uint8_t bit = button - 8*index;

    joy->button[index] &= ~(1 << bit);
}

uint8_t button=0;	// current button
bool press = true;	// turn buttons on?


void loop() 
{

  joyReport.buttons = 0;
  joyReport.rfu = 0;
  joyReport.x = 0;
  joyReport.y = 0;
  joyReport.z = 0;
  joyReport.rx = 0;

 

  Usb.Task();
  if (Controller.connected()) {

      
 

      if(Controller.getAnalogHat(LeftHatX) > 137 || Controller.getAnalogHat(LeftHatX) < 117) joyReport.x = map(Controller.getAnalogHat(LeftHatX),0,255,-128,127);
      if(Controller.getAnalogHat(LeftHatY) > 137 || Controller.getAnalogHat(LeftHatY) < 117) joyReport.y = map(Controller.getAnalogHat(LeftHatY),0,255,-128,127);
      if(Controller.getAnalogHat(RightHatX) > 137 || Controller.getAnalogHat(RightHatX) < 117) joyReport.z = map(Controller.getAnalogHat(RightHatX),0,255,-128,127);
      if(Controller.getAnalogHat(RightHatY) > 137 || Controller.getAnalogHat(RightHatY) < 117) joyReport.rx = map(Controller.getAnalogHat(RightHatY),0,255,-128,127);


        if (Controller.getButtonPress(CROSS)) { joyReport.buttons = 1; } 
        else if (Controller.getButtonPress(DOWN)) { joyReport.buttons = 2; } else { joyReport.buttons = 0; }


        Serial.write((uint8_t *)&joyReport, 6);
        delay(3);

        if (Controller.getAnalogButton(L2) != oldL2Value || Controller.getAnalogButton(R2) != oldR2Value) // Only write value if it's different
        Controller.setRumbleOn(Controller.getAnalogButton(L2), Controller.getAnalogButton(R2));
        oldL2Value = Controller.getAnalogButton(L2);
        oldR2Value = Controller.getAnalogButton(R2);

        if (Controller.getButtonClick(PS)) {
        Controller.disconnect();
        }

        //Serial.print("X: ");
        //Serial.println(joyReport.x);
          //Serial.println(Controller.getAnalogHat(LeftHatX));
        //Serial.print("Y: ");
        //Serial.println(joyReport.y);
          //Serial.println(Controller.getAnalogHat(LeftHatY));

        //joyReport.axis[2] = 0;//Controller.getAnalogHat(RightHatX);
        //joyReport.axis[3] = 0;//Controller.getAnalogHat(RightHatY);
        
    // }
   }
  
  //for(int i = 0; i < NUM_BUTTONS; i++){
  //  clearButton(&joyReport,i);
  //}

 // setButton(&joyReport,1);

    //  for(int i = 0; i < NUM_BUTTONS; i++){
   // setButton(&joyReport,i);
 // }
 // joyReport.button[0]=uint8_t(3);

 // if(send) sendJoyReport(&joyReport);
  
  //joyReport.axis[0] += 10;
  //joyReport.axis[1] += 20;

  //sendJoyReport(&joyReport);

}
