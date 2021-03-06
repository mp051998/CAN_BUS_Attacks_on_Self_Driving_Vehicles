
#include <SPI.h>              //Library for using SPI Communication 
#include <mcp2515.h>          //Library for using CAN Communication

#include <Wire.h> 

struct can_frame canMsg; 
MCP2515 mcp2515(10);                 // SPI CS Pin 10 

void setup() {
  delay(1000);
  SPI.begin();                       //Begins SPI communication
  Serial.begin(9600);                //Begins Serial Communication at 9600 baudrate 
  
  mcp2515.reset();      
  mcp2515.setBitrate(CAN_500KBPS,MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz 
  mcp2515.setNormalMode();                  //Sets CAN at normal mode
}

void loop() {
  //Sniff messages
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK){
    Serial.println(canMsg.can_id);
    Serial.println(canMsg.data[0]);
  }

  //Send message
  /*
  canMsg.can_id = 0x036;    // Replace this with the desired can id
  canMsg.data[0] = 11;
  mcp2515.sendMessage(&canMsg);
  */
}                    
