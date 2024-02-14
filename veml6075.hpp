/*************************************************
          VEML6075 UV sensor
          based on Adafruit library example
*************************************************/

#include <Wire.h>
#include "Adafruit_VEML6075.h"

Adafruit_VEML6075 uv = Adafruit_VEML6075();

float uva;
float uvb;
float uvi;
bool  veml6075Active = false;


bool veml6075setup(){

  if (! uv.begin()) {
    Serial.println("No VEML6075 detected, check wiring");
    veml6075Active = false;
    return false;
  }

  // Set the integration constant
  uv.setIntegrationTime(VEML6075_100MS);
  // Get the integration constant and print it!
  Serial.print("Integration time set to ");
  switch (uv.getIntegrationTime()) {
    case VEML6075_50MS: Debug("50"); break;
    case VEML6075_100MS: Debug("100"); break;
    case VEML6075_200MS: Debug("200"); break;
    case VEML6075_400MS: Debug("400"); break;
    case VEML6075_800MS: Debug("800"); break;
  }
  DebugLn(" ms");

  // Set the high dynamic mode
  uv.setHighDynamic(true);
  // Get the mode
  if (uv.getHighDynamic()) {
    DebugLn("High dynamic reading mode");
  } else {
    DebugLn("Normal dynamic reading mode");
  }

  // Set the mode
  uv.setForcedMode(false);
  // Get the mode
  if (uv.getForcedMode()) {
    DebugLn("Forced reading mode");
  } else {
    DebugLn("Continuous reading mode");
  }

  // Set the calibration coefficients
  uv.setCoefficients(2.22, 1.33,  // UVA_A and UVA_B coefficients
                     2.95, 1.74,  // UVB_C and UVB_D coefficients
                     0.001461, 0.002591); // UVA and UVB responses

    veml6075Active = true;
    return true;

}

void get_veml6075data(){

  if (veml6075Active) {
    uva = uv.readUVA();
    uvb = uv.readUVB();
    uvi = uv.readUVI();

    DebugLn("Reading VEML6075 data: raw UVA: " + String(uva) + ", raw UVB: " + String(uvb) + ", UV index: " + String(uvi));
  }

}

String veml6075Json()
{

  String jsonMsg =  String("\"veml6075\": ") + 
                      "{ \"uva\":" + String(uva) + 
                      ", \"uvb\":" + String(uvb) + 
                      ", \"UVindex\":" + String(uvi,0) + 
                      "}";
  
  return jsonMsg;

}

void sendVEML6075data()
{

#ifdef  W_MQTT
  mqttSendSensorData(veml6075Json());
#endif
}

