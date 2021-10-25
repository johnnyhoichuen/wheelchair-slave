
#include <Arduino.h>
#include "Wire.h"

#define SLAVE_ADDR 0b0001100 // 7bits

volatile byte receivedByte = 0;
volatile byte lastReceivedByte = 0;

volatile byte rrInfo[2] = {0};
volatile byte wrInfo[4] = {0};

byte rrResponse[11][4] = {
  {0x28, 0x00, 0x5A, 0x5A},
  {0x2C, 0x00, 0x08, 0x03},
  {0x30, 0x00, 0x07, 0x88},
  {0x34, 0x00, 0x00, 0x00},
  {0x38, 0x00, 0x00, 0x70},
  {0x3C, 0x00, 0x00, 0x00},
  {0x40, 0x00, 0x00, 0x83},
  {0x44, 0x00, 0x00, 0x79},
  {0x48, 0x00, 0x00, 0x00},
  {0x4C, 0x00, 0x00, 0x78},
  {0x50, 0x00, 0x00, 0x00}
};

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
  if (receivedByte != 0) {
    switch (receivedByte)
    {
      case 0x36:
        // return status
        Wire.write(0x21);
        break;
      case 0x46:

        // * TEMP *
        // put fw command in here for now

        Wire.write(0x01);
        Wire.write(0x63);
        Wire.write(0x99); // avg
        Wire.write(0x3C); // avg
        Wire.write(0x4A); // avg


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
        Wire.write(0xFF);
        break;
    }

    receivedByte = 0;
  }

  if (rrInfo[0] != 0) {
    Serial.println("n");
    for (int i = 0; i < sizeof(rrResponse)/sizeof(rrResponse[0]); i++) {
      if (rrInfo[1] == rrResponse[i][0]) {
        Wire.write(rrResponse[i][1]);
        Wire.write(rrResponse[i][2]);
        Wire.write(rrResponse[i][3]);
      }
    }

    // reset rrInfo
    for (int i = 0; i < sizeof(rrInfo)/sizeof(rrInfo[0]); i++) {
      rrInfo[i] = 0;
    }
  } else {
    Serial.println("y");
  }

  if (wrInfo[0] != 0) {
    for (int i = 0; i < sizeof(wrInfo)/sizeof(wrInfo[0]); i++) {
      Wire.write(wrInfo[i]);
    }

    // reset wrInfo
    for (int i = 0; i < sizeof(wrInfo)/sizeof(wrInfo[0]); i++) {
      wrInfo[i] = 0;
    }
  }
}

// try not to use serial here
void receiveEvent(int number)
{
  if (number == 1) {
    receivedByte = Wire.read();
  } else if (number == 2) { // RR
    for (int i = 0; i < number; i++) {
      rrInfo[i] = Wire.read();
    }
  } else if (number == 4) { // WR
    for (int i = 0; i < number; i++) {
      wrInfo[i] = Wire.read();
    }
  }
}











// #include <Arduino.h>
// #include "Wire.h"

// #define SLAVE_ADDR 0b0001100 // 7bits

// int receivedByte = 0;
// int lastReceivedByte = 0;

// void receiveEvent(int number);

// void setup()
// {
//   Wire.begin(SLAVE_ADDR);
//   Wire.onReceive(receiveEvent);
//   // Wire.onRequest(requestHandler);
//   // Wire.send();
//   // Wire.write();

//   Serial.begin(115200);

//   Serial.println("Slave setup completed");
// }

// void loop()
// {
//   Serial.println("slave looping");

//   switch (receivedByte)
//   {
//   case 0x36:
//     // return status
//     Wire.write(0x21);
//     break;
//   case 0x46:
//     Serial.println("RM cmd received");
//     // return data of XY-axis
//     // each set of data from 1 axis contains 2 bytes

//     // Wire.write() // X1
//     // Wire.write() // X2
//     // Wire.write() // Y1
//     // Wire.write() // Y2

//     break;
//   case 0x50:

//     // how should I connect with the target address???

//     Serial.println("RR cmd received, not finished");
//     break;
//   case 0x60:

//     // how should I connect with the target address???

//     Serial.println("WR cmd received, not finished");
//     // return status

//     // first 2 WR send 0x00
//     // 3rd WR send 0x36

//     // Wire.
//     break;
//   case 0x80:
//     Serial.println("EX cmd received");
//     // return status
//     Wire.write(0x00);
//     break;
//   case 0xD0:
//     Serial.println("MR cmd received");
//     // return status
//     Wire.write(0x00);
//     break;

//   default:
//     Serial.println("Command not recognised");
//     break;
//   }
// }

// // // try not to use serial here
// // void requestHandler()
// // {
// //   if (Wire.available())
// //   {
// //     receivedByte = Wire.read();
// //   } else {
// //     // check why it's not available
// //   }
// // }

// // try not to use serial here
// void receiveEvent(int number)
// {
//   if (Wire.available())
//   {
//     receivedByte = Wire.read();
//   }
//   else
//   {
//     // check why it's not available
//   }
// }