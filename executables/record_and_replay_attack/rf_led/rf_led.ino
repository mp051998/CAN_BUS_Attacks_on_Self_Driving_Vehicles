
#include <SPI.h>              //Library for using SPI Communication 
#include <mcp2515.h>          //Library for using CAN Communication

#include <Wire.h> 

#define ledPin 7
unsigned int temp = 0;             // variable used to store received data
const unsigned int unlock = 99;   // unlock value
const unsigned int lock = 11;     // lock value

struct can_frame canMsg; 
MCP2515 mcp2515(10);    // SPI CS Pin 10 

void setup() {
  pinMode(ledPin, OUTPUT);      // ledPin/Pin 12 acts as Output Pin
  SPI.begin();                       //Begins SPI communication
  Serial.begin(9600);                //Begins Serial Communication at 9600 baudrate 
  
  mcp2515.reset();      
  mcp2515.setBitrate(CAN_500KBPS,MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz 
  mcp2515.setNormalMode();                  //Sets CAN at normal mode
}

void loop() {
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    if (canMsg.can_id==0x036) { // The RF Receiver's can_id is 0x036
      temp = canMsg.data[0]; 
      if (temp == unlock){      // If temp data is 99, then it is unlock
        Serial.println("Unlocking");
        // Make the LED blink twice
        digitalWrite(ledPin, HIGH);
        delay(750);
        digitalWrite(ledPin, LOW);
        delay(750);
        digitalWrite(ledPin, HIGH);
        delay(750);
        digitalWrite(ledPin, LOW);
      }
      else if(temp == lock){  // If them data is 11, then it denotes lock
        Serial.println("locking");
        // Make the LED flask for 2s
        digitalWrite(ledPin, HIGH);
        delay(2000);
        digitalWrite(ledPin, LOW);
      }
      Serial.println(temp);
      delay(500);
    }
  }
}
