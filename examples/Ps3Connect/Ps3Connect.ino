#include <FR.h>

void disconnect() {
  Serial.print("BYE");
}

void setup()
{
    Serial.begin(9600);
    safetySetup(disconnect, ps3);
    Ps3.begin("03:03:03:03:03:03");
    Serial.println("Ready.");
}

void loop()
{
  safetyLoop();
  if (Ps3.isConnected()){
    Serial.println("Connected!");
  }
}
