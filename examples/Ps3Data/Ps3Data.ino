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
    if(stateIsConnected){

        if( Ps3.data.button.cross ){
            Serial.println("Pressing the cross button");
        }

        if( Ps3.data.button.square ){
            Serial.println("Pressing the square button");
        }

        if( Ps3.data.button.triangle ){
            Serial.println("Pressing the triangle button");
        }

        if( Ps3.data.button.circle ){
            Serial.println("Pressing the circle button");
        }

    }
}
