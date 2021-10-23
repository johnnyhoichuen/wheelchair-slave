#include <Arduino.h>
#include "Wire.h"

#define SLAVE_ADDR 0b0001100 // 7bits

void setup()
{
  Wire.begin(SLAVE_ADDR);
  // Wire.onReceive(receiveHandler);
  Wire.onRequest(requestHandler);
  Wire.send();
  Wire.write();

  Serial.begin(115200);

  Serial.println("Slave setup completed");
}

void loop()
{

  Serial.println("slave looping");
}

// try not to use serial here
void requestHandler()
{
}

// try not to use serial here
void receiveHandler()
{
  // do nth for now
}