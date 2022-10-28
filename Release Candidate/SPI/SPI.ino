
#include <SPI.h>
#include <SoftSPI.h>

// Create a new SPI port with:
// Pin 2 = MOSI,
// Pin 1 = MISO,
// Pin 0 = SCK
SoftSPI mySPI(4, 3, 2);

void READ_DATA(){
  digitalWrite(5, LOW);
  static uint8_t v = 5;
  uint8_t in = mySPI.transfer(v);
  in = mySPI.transfer(v);
  in = mySPI.transfer(v);

  in = mySPI.transfer(v);
  if(in){
    in = mySPI.transfer(v);
    in = mySPI.transfer(v);
    in = mySPI.transfer(v);
    Serial.print(" Got value: ");
    Serial.print(in, HEX);
    Serial.print("\n");

  }


  digitalWrite(5, HIGH);
}


void setup() {

  attachInterrupt(digitalPinToInterrupt(18), READ_DATA, RISING);
  digitalWrite(5, LOW);
  pinMode(18,INPUT);
  pinMode(5,OUTPUT);
  mySPI.begin();
  Serial.begin(115200);
}

void loop() {
  
  //static uint8_t v = 5;
  //uint8_t in = mySPI.transfer(v);
  //Serial.print(" Got value: ");
  //Serial.print(in, HEX);
  //Serial.println(v == in ? " PASS" : " FAIL");
  //delay(1000);

  
}
