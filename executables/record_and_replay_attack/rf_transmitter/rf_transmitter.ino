#define dataout 12                // Initialize Arduino Pin 12 as dataout Pin

#include <SPI.h>

int count = 0;

void setup() {
  pinMode(dataout, OUTPUT);      // dataout/Pin 12 acts as Output Pin
  Serial.begin(9600);
  
  SPI.begin();
  //LOCK
  //digitalWrite(dataout, HIGH);             // Pin 12 is High for Data Transmission
  //UNLOCK
}

void loop() {
  /*
   * this node will send a lock or unlock signal ONCE 
   * depending on what is uncommented.
   */
  if (count < 1){
    //LOCK
    /*
    Serial.println("Sending LOCK");
    digitalWrite(dataout, HIGH);
    */

    //UNLOCK
    Serial.println("Sending UNLOCK");
    digitalWrite(dataout, LOW);
    count++;
  }
}
