#include <SdsDustSensor.h> // v 1.5.1

#define SDS_PIN_RX GPIO14  // D5
#define SDS_PIN_TX GPIO12  // D6

SdsDustSensor sds(SDS_PIN_RX, SDS_PIN_TX);

//Global Variables
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
float       pm25f;
float       pm10f;
bool        is_SDS_running = false;
bool        send_SDS = false;
unsigned long timeSDS;

#ifdef  W_MQTT
// function prototype
void mqtt_send(String subtopic, String message, bool retain);
#endif


/**************************************************************************/
/*    humidityCompensation function
/*    based on same name function in hackAir.cpp 
/*    (https://github.com/hackair-project/hackAir-Arduino)
/**************************************************************************/
void  humidityCompensation() 
{
  pm25f = pm25f / (1.0 + 0.48756 * pow((humidity / 100.0), 8.60068));
  pm10f = pm10f / (1.0 + 0.81559 * pow((humidity / 100.0), 5.83411));
}

/**************************************************************************/
/*     Start SDS
/**************************************************************************/
void start_SDS() {
    DebugLn("Start wakeup SDS011");

    WorkingStateResult state = sds.wakeup(); 
    is_SDS_running = state.isWorking();
    timeSDS = millis();
}

/**************************************************************************/
/*     Stop SDS
/**************************************************************************/
void stop_SDS() {
    DebugLn("Start sleep SDS011");

    WorkingStateResult state = sds.sleep();
    is_SDS_running = state.isWorking();
}

/**************************************************************************/
/*     sds011 setup function 
/**************************************************************************/
void SDS011setup()
{

  sds.begin();

  DebugLn(sds.queryFirmwareVersion().toString()); // prints firmware version
  DebugLn(sds.setQueryReportingMode().toString()); // ensures sensor is in 'query' reporting mode

}

/**************************************************************************/
/*     handleSDS function
/**************************************************************************/
void handleSDS(){

  if( is_SDS_running ) {
    // run SDS for 30 secs. before get result
    PmResult pm = sds.queryPm();
    if (pm.isOk()) {
      pm25f = pm.pm25;
      pm10f = pm.pm10;
      DebugLn("PM10raw: " + String(pm10f));
      DebugLn("PM2.5raw: " + String(pm25f));
      humidityCompensation();
      DebugLn("PM10: " + String(pm10f));
      DebugLn("PM2.5: " + String(pm25f));
      stop_SDS();
      send_SDS = true;
    }
    else if( (millis() - timeSDS) > 30000 ) {
      // check if SDS has been in read cycle more than 30 secs.
      Debug("Could not read values from sensor, reason: ");
      DebugLn(pm.statusToString());
      stop_SDS();
    }
  }
}

/**************************************************************************/
/*     prepare sds011 JSON data
/**************************************************************************/
String sds011Json()
{

  String jsonMsg =  String("\"sds011\": ") + 
                        "{ \"pm10\":" + String(pm10f) + 
                        ", \"pm25\":" + String(pm25f) + 
                        ", \"running\":" + String(is_SDS_running) + 
                      "}";
  
  return jsonMsg;

}

/**************************************************************************/
/*     sds011 send data to MQTT broker
/**************************************************************************/
void sendSDS011data()
{

#ifdef  W_MQTT
  mqttSendSensorData(sds011Json());
#endif
}
