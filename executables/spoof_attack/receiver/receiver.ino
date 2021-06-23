
#include <SPI.h>              //Library for using SPI Communication 
#include <mcp2515.h>          //Library for using CAN Communication

#include <Wire.h> 

struct can_frame canMsg; 
MCP2515 mcp2515(10);                 // SPI CS Pin 10 

const int buzzer = 9; //buzzer to arduino pin 9

void setup() {
  delay(1000);
    
  SPI.begin();                       //Begins SPI communication
  
  Serial.begin(9600);                //Begins Serial Communication at 9600 baudrate 

  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
  
  mcp2515.reset();      

  mcp2515.setBitrate(CAN_500KBPS,MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz 
  mcp2515.setNormalMode();                  //Sets CAN at normal mode
}

void loop() 
{
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) // To receive data (Poll Read)
  {
    if(canMsg.can_id==0x036)
    {
     int x = canMsg.data[0];         
     Serial.println(x);
     if(x <= 25){
      tone(buzzer, 1000); // Send 1KHz sound signal...
      delay(500);        // ...for 1 sec
      noTone(buzzer);     // Stop sound...
     }
     else{
      delay(500);
     }
     }
    }
   
}                    
