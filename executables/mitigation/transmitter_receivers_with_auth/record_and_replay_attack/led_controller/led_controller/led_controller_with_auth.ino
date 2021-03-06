
#include <Base64.h>
#include <SPI.h>          //Library for using SPI Communication 
#include <mcp2515.h>      //Library for using CAN Communication
#include <Wire.h> 

#define ledPin 7
unsigned int temp = 0;             // variable used to store received data
const unsigned int unlock = 99;   // unlock value
const unsigned int lock = 11;     // lock value
char* x[30]={}; 
char* y[30]={}; 
char* z[30]={}; 

struct can_frame canMsg;
MCP2515 mcp2515(10); // chip select CS pin 10

void setup()
{
  pinMode(ledPin, OUTPUT);      // ledPin/Pin 12 acts as Output Pin
  Serial.begin(9600);
  canMsg.can_id  = 0x036;           //CAN id as 0x036
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
}
