
#define datain A0                                 // Initialize Arduino Analog Pin A0 as datain Pin

#include <SPI.h>      
#include <mcp2515.h>

struct can_frame canMsg;
MCP2515 mcp2515(10);

unsigned int temp = 0;                    // variable used to store received data
const unsigned int upperThreshold = 600;  // upper threshold value
const unsigned int lowerThreshold = 50;   // lower threshold value

void setup() {
  Serial.begin(9600);
  SPI.begin();               //Begins SPI communication
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS,MCP_8MHZ);  //Sets CAN at speed 500KBPS and Clock 8MHz
  mcp2515.setNormalMode();

  canMsg.can_id = 0x036;
  canMsg.can_dlc = 1;     //CAN data length as 1 byte
}

void loop() {
  temp=analogRead(datain);    // listen for data on Analog pin 0
  Serial.println(temp);
  if(temp > lowerThreshold && temp < upperThreshold) {   // If a HIGH signal is received, turn LED ON
    Serial.println("Sending unlock message");
    canMsg.data[0] = 99;
    mcp2515.sendMessage(&canMsg);
    delay(200);
  }
  else if(temp>upperThreshold) {  // If a LOW signal is received, turn LED OFF
    //digitalWrite(ledPin, LOW);
    Serial.println("Sending lock message");
    canMsg.data[0] = 11;
    mcp2515.sendMessage(&canMsg);
    delay(200);
  }
}
