#include <Arduino.h>
#include "Wire.h"

#define SLAVE_ADDR 0b0001100 // 7bits

int receivedByte = 0;
int lastReceivedByte = 0;

void setup()
{
  Wire.begin(SLAVE_ADDR);
  Wire.onReceive(receiveHandler);
  // Wire.onRequest(requestHandler);
  // Wire.send();
  // Wire.write();

  Serial.begin(115200);

  Serial.println("Slave setup completed");
}

void loop()
{
  Serial.println("slave looping");

  switch (receivedByte)
  {
  case 0x36:
    // return status
    Wire.write(0x21);
    break;
  case 0x46:
    Serial.println("RM cmd received");
    // return data of XY-axis
    // each set of data from 1 axis contains 2 bytes

    // Wire.write() // X1
    // Wire.write() // X2
    // Wire.write() // Y1
    // Wire.write() // Y2

    break;
  case 0x50:

    // how should I connect with the target address???

    Serial.println("RR cmd received, not finished");
    break;
  case 0x60:

    // how should I connect with the target address???

    Serial.println("WR cmd received, not finished");
    // return status

    // first 2 WR send 0x00
    // 3rd WR send 0x36

    // Wire.
    break;
  case 0x80:
    Serial.println("EX cmd received");
    // return status
    Wire.write(0x00);
    break;
  case 0xD0:
    Serial.println("MR cmd received");
    // return status
    Wire.write(0x00);
    break;

  default:
    Serial.println("Command not recognised");
    break;
  }
}

// // try not to use serial here
// void requestHandler()
// {
//   if (Wire.available())
//   {
//     receivedByte = Wire.read();
//   } else {
//     // check why it's not available
//   }
// }

// try not to use serial here
void receiveHandler()
{
  if (Wire.available())
  {
    receivedByte = Wire.read();
  }
  else
  {
    // check why it's not available
  }
}