/*************************************************
          TSL 2591 luminosity sensor + SQM
          based on SQM_example by Gabe Shaughnessy
*************************************************/
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "SQM_TSL2591.h"

SQM_TSL2591 sqm = SQM_TSL2591(2591);

unsigned int    sqmFull;
unsigned int    sqmIr;
unsigned int    sqmVis;
float           sqmMpsas;
float           sqmDmpsas; 
float           sqmLux;
bool            sqmActive = false;


bool SQMsetup()
{
  if(!sqm.begin())
  {
    // TSL2591 not detected
    DebugLn("No TSL2591 detected ... Check wiring or I2C ADDR !");
    sqmActive = false;
    return false;
  }

  // set initial parameters
    sqm.config.gain = TSL2591_GAIN_LOW;
    sqm.config.time = TSL2591_INTEGRATIONTIME_200MS;
    sqm.configSensor();
    sqm.showConfig();  // show integration time and gain
    sqm.setCalibrationOffset(0.0);
 

  // Display sensor details
  sensor_t sensor;
  sqm.getSensor(&sensor);
  DebugLn("***** Inicializing SQM-TSL2591 *****");
  DebugLn("Integration:  " + String(sqm.integrationValue) + " ms");
  DebugLn("Gain:         " + String(sqm.gainValue) +"x");
  DebugLn("Sensor:       " + String(sensor.name));
  DebugLn("Driver Ver:   " + String(sensor.version));
  DebugLn("Unique ID:    " + String(sensor.sensor_id));
  DebugLn("Max Value:    " + String(sensor.max_value) + " lux");
  DebugLn("Min Value:    " + String(sensor.min_value) + " lux");
  DebugLn("Resolution:   " + String(sensor.resolution) + " lux");  
  delay(100);

  /* Update these values depending on what you've set above! */  
  DebugLn ("***** Initialized SQM-TSL2591 *****");

  sqmActive = true;
  return true;
}

void getSQMdata() {

  if (sqmActive) {
    sqm.takeReading();

    sqmFull = sqm.full;
    sqmIr = sqm.ir;
    sqmVis = sqm.vis;
    sqmMpsas = sqm.mpsas;
    sqmDmpsas = sqm.dmpsas; 
    sqmLux = sqm.calculateLux(sqmFull, sqmIr);

    Debug ("Reading TSL2591 data: ");
    Debug("full: " + String(sqmFull));
    Debug(", ir: " + String(sqmIr));
    Debug(", vis: " + String(sqmVis));
    Debug(", lux: " + String(sqmLux));
    DebugLn(", mpsas: " + String(sqmMpsas) + " +/- " + String(sqmDmpsas));
  }

}

String SQMJson()
{

  String jsonMsg =  String("\"SQM\": ") + 
                      "{ \"full\":" + String(sqmFull) + 
                      ", \"ir\":" + String(sqmIr) + 
                      ", \"visible\":" + String(sqmVis) + 
                      ", \"lux\":" + String(sqmLux) + 
                      ", \"mpsas\":" + String(sqmMpsas) + 
                      ", \"dmpsas\":" + String(sqmDmpsas) +    
                      "}";
  
  return jsonMsg;

}

void sendSQMdata()
{

#ifdef  W_MQTT
  mqttSendSensorData(SQMJson());
#endif
}

