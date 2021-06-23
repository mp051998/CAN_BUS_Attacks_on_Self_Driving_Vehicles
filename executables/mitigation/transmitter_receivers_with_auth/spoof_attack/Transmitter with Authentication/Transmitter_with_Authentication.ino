
#include <Base64.h>
#include <SPI.h>          //Library for using SPI Communication 
#include <mcp2515.h>      //Library for using CAN Communication
#include <Wire.h> 
#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04
#define potPin A0
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
int potValue=0;
char* x[30]={}; 
char* y[30]={}; 
char* z[30]={}; 

struct can_frame canMsg;
MCP2515 mcp2515(10); // chip select CS pin 10

void setup()
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUTs
  Serial.begin(9600);
  canMsg.can_id  = 0x034;           //CAN id as 0x034
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
        // Clears the trigPin condition
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        // Reads the echoPin, returns the sound wave travel time in microseconds
        duration = pulseIn(echoPin, HIGH);
        // Calculating the distance
        distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
        // Displays the distance on the Serial Monitor
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" cm");
      
        canMsg.data[0] = distance;        //Update pot value in [0]
        mcp2515.sendMessage(&canMsg);     //Sends the CAN message
        delay(200);
      }
    }
  }
}
