#include <Arduino.h>
#include <SPI.h>
#include <String.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

 #define FACTORYRESET_ENABLE         1
    #define MINIMUM_FIRMWARE_VERSION    "0.6.6"
    #define MODE_LED_BEHAVIOUR          "MODE"

int state = 0;


Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_SCK, BLUEFRUIT_SPI_MISO, BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

void setup(void)
{
  while (!Serial);

  Serial.begin(115200);
 
  ble.begin(VERBOSE_MODE);

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  ble.verbose(false);  // debug info is a little annoying after this point!

  Serial.println("Waiting for connection...");

  /* Wait for connection */
  while (! ble.isConnected()) {
    delay(100);
  }

  Serial.println("uControl Connected.");
  delay(100);
  //attachInterrupt(digitalPinToInterrupt(20), onInterrupt, RISING);
  
}

void onInterrupt(){
  /* Check for data in the buffer */
    ble.println("AT+BLEUARTRX");
    ble.readline();
    if (strcmp(ble.buffer, "OK") == 0) { return; }
    
    /* Read data that was found in the buffer on interrupt */
    Serial.print(F("[Recv] "));
    
    Serial.println(ble.buffer);
    ble.waitForOK();


}


void loop(void)
{

    ble.println("AT+BLEUARTRX");
    ble.readline();
    if (strcmp(ble.buffer, "OK") == 0) { return; }
    
    /* Read data that was found in the buffer on interrupt */
    Serial.print(F("[Recv] "));
    String str = ble.buffer;
    Serial.println(str);
    ble.waitForOK();


        
    if(str.equals("PS5")) state = 0;
    if(str.equals("PS4")) state = 1;


    Serial.println(state);
  
 

  
}


