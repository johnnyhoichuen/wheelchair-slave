
#include <Arduino.h>
#include "Wire.h"

#define SLAVE_ADDR 0b0001100 // 7bits

volatile byte receivedByte = 0;
volatile byte lastReceivedByte = 0;

volatile byte rrInfo[2] = {0};
volatile byte wrInfo[4] = {0};

const byte rrResponse[11][4] = {
    {0x28, 0x00, 0x5A, 0x5A}, // rmb to change 0x01 back to 0x00
    {0x2C, 0x00, 0x08, 0x03}, // rmb to change 0x02 back to 0x00
    {0x30, 0x00, 0x07, 0x88},
    {0x34, 0x00, 0x00, 0x00},
    {0x38, 0x00, 0x00, 0x70},
    {0x3C, 0x00, 0x00, 0x00},
    {0x40, 0x00, 0x00, 0x83},
    {0x44, 0x00, 0x00, 0x79},
    {0x48, 0x00, 0x00, 0x00},
    {0x4C, 0x00, 0x00, 0x78},
    {0x50, 0x00, 0x00, 0x00}};

const byte wrResponse[3][2] = {
    {0x00, 0x00},
    {0x04, 0x00},
    {0x08, 0x36},
};

// 
static byte rmBuffer[5];

// 
//static byte rrBuffer[3];

void receiveEvent(int number);

void setup()
{
  Wire.begin(SLAVE_ADDR);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestHandler);

  Serial.begin(115200);

  Serial.println("Slave setup completed");
}

void loop()
{
}

// try not to use serial here
void requestHandler()
{
  if (receivedByte != 0)
  {
    switch (receivedByte)
    {
    case 0x36:
      // return status
      Wire.write(0x21);
      break;
    case 0x46:

//      Wire.write(0x77); // random number

      // * TEMP *
      // put fw command in here for now

      rmBuffer[0] = 0x01;
      rmBuffer[1] = 0x63;
      rmBuffer[2] = 0x99;
      rmBuffer[3] = 0x3C;
      rmBuffer[4] = 0x4A;
      
      Wire.write(rmBuffer, 5);
        
      //        Wire.write(0x01);
      //        Wire.write(0x63);
      //        Wire.write(0x99); // avg
      //        Wire.write(0x3C); // avg
      //        Wire.write(0x4A); // avg

      break;
    case 0x80:
      // return status
      Wire.write(0x00);
      break;
    case 0xD0:
      // return status
      Wire.write(0x00);
      break;

    default:
      // Serial.println("Command not recognised");
      //        Wire.write(0xFF);
      break;
    }

    receivedByte = 0;
  }

  if (rrInfo[0] != 0)
  {
    for (int i = 0; i < sizeof(rrResponse) / sizeof(rrResponse[0]); i++)
    {
      if (rrInfo[1] == rrResponse[i][0])
      {
        static uint8_t rrBuffer[3];
        rrBuffer[0] = rrResponse[i][1];
        rrBuffer[1] = rrResponse[i][2];
        rrBuffer[2] = rrResponse[i][3];

        Wire.write(rrBuffer, 3);
        
//        Wire.write(rrResponse[i][1]);
//        Wire.write(rrResponse[i][2]);
//        Wire.write(rrResponse[i][3]);

//        Serial.println("rrR data:");
////        Serial.println(rrResponse[i][1], HEX);
////        Serial.println(rrResponse[i][2], HEX);
////        Serial.println(rrResponse[i][3], HEX);
//        for (int i = 0; i < sizeof(buffer)/sizeof(buffer[0]); i++) {
//          Serial.println(buffer[i], HEX);
//        }
//        Serial.println("rrR sent");
        
      }
    }

    // reset rrInfo after using it
    for (int i = 0; i < sizeof(rrInfo) / sizeof(rrInfo[0]); i++)
    {
      rrInfo[i] = 0;
    }
  }

  // this part is checked correct
  if (wrInfo[0] != 0)
  {
    for (int i = 0; i < sizeof(wrResponse)/sizeof(wrResponse[0]); i++) {
      if (wrInfo[3] == wrResponse[i][0]) {
        Wire.write(wrResponse[i][1]);
      }
    }

    // reset wrInfo after using it
    for (int i = 0; i < sizeof(wrInfo) / sizeof(wrInfo[0]); i++)
    {
      wrInfo[i] = 0;
    }
  }
}

// try not to use serial here
void receiveEvent(int number)
{
  if (number == 1)
  {
    receivedByte = Wire.read();
  }
  else if (number == 2)
  { // RR
    // received info is checked correct
    for (int i = 0; i < number; i++)
    {
      rrInfo[i] = Wire.read();
    }
  }
  else if (number == 4)
  { // WR
    // received info is checked correct
    for (int i = 0; i < number; i++)
    {
      wrInfo[i] = Wire.read();
    }
  }
}