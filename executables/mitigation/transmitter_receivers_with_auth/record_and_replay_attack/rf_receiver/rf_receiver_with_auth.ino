#define datain A0                                 // Initialize Arduino Analog Pin A0 as datain Pin
#include <Base64.h>
#include <SPI.h>          //Library for using SPI Communication 
#include <mcp2515.h>      //Library for using CAN Communication

int numbr;
char* x[30]={}; 
char* y[30]={}; 
char* z[30]={}; 
unsigned int temp = 0;                    // variable used to store received data
const unsigned int upperThreshold = 600;  // upper threshold value
const unsigned int lowerThreshold = 50;   // lower threshold value

struct can_frame canMsg;
MCP2515 mcp2515(10); // chip select CS pin 10

void setup()
{
  Serial.begin(9600);
  canMsg.can_id  = 0x037;           //CAN id as 0x037
  canMsg.can_dlc = 1;               //CAN data length as 1 byte
  mcp2515.reset();      
  mcp2515.setBitrate(CAN_500KBPS,MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz 
  mcp2515.setNormalMode();                  //Sets CAN at normal mode
}

void loop(){
  if (canMsg.can_id==0x038){       // only messages from the monitering node is evaluated here
    if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK){ //canMsg contains received message
      // encoding begins here
      char input[] = "New Key";
      int inputLen = sizeof(input);
      int encodedLen = base64_enc_len(inputLen);
      char encoded[encodedLen];
      base64_encode(encoded, input, inputLen);
      char* y[30]= {encoded};  //// encoding ends here
      canMsg.data[0] = y[30];    
      mcp2515.sendMessage(&canMsg); // sending the encrypted message
      char x = canMsg.data[0];    
      char input2[10] = {x} ;               //  decoding starts here   
      int input2Len = sizeof(input2);
      int decodedLen = base64_dec_len(input2, input2Len);
      char decoded[decodedLen];
      base64_decode(decoded, input2, input2Len);
      Serial.print(input2); Serial.print(" = "); Serial.println(decoded);  // decoding ends here and printing the decoded message        
      char* z[30]= {decoded};
      if (z[30]=="New Key"){   //comparing with the the initial key that we generated above
          Serial.print("authentication complete");
      }
    }
  }
  else{
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
}
