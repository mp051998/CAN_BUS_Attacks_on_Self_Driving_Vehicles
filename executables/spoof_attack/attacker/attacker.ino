#include <SPI.h>          //Library for using SPI Communication 
#include <mcp2515.h>      //Library for using CAN Communication

struct can_frame canMsg;
MCP2515 mcp2515(10); // chip select CS pin 10

void setup() {
  SPI.begin();
  // put your setup code here, to run once:
    mcp2515.reset();
    mcp2515.setBitrate(CAN_500KBPS,MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz
    mcp2515.setNormalMode();
  
    canMsg.can_id  = 0x036;           //CAN id as 0x036
    canMsg.can_dlc = 1;               //CAN data length as 1 
}

void loop() {
  
  // put your main code here, to run repeatedly:
  canMsg.data[0] = 50;        //Update pot value in [0]
Serial.println(50);
  mcp2515.sendMessage(&canMsg);     //Sends the CAN message
 ;
}
