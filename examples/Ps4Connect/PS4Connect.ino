#include <FR.h>

void disconnect() {
  Serial.print("BYE");
}

void setup()
{
    Serial.begin(9600);
    safetySetup(disconnect, ps4);
    PS4.begin("03:03:03:03:03:03");
    Serial.println("Ready.");
}

void loop()
{
  safetyLoop();
  if (PS4.isConnected()){
    Serial.println("Connected!");
  }
}
