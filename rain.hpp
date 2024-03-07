/************************************************************
        RAIN sensor

  v1.0  Initial version

  v2.0  added settings.data.rainabove variable, it define if rains when analogic reading is above
        or below the threshold.
          with a FC-37 printed circuit board sensor, rains if the analog read is below the threshold
          with a graphite sensor with 1 MOhm resitor to +3.3 v, rains if analog read is above threshold

*************************************************************/

#define RAIN_ANALOG A0     // A0

int         rainA;
bool        rainD;
long        lastSecondR; // millis counter for rain 

#ifdef  W_MQTT
// function prototype
void mqtt_send(String subtopic, String message, bool retain);
#endif

void setupRain(){
  lastSecondR = millis();
}

void readRainSensor()
{
  // Read rain sensor on analog input
  rainA = analogRead(RAIN_ANALOG);
  if (settings.data.rainabove) {
    rainD = ( rainA > settings.data.rainthreshold );
  }
  else {
    rainD = ( rainA < settings.data.rainthreshold );
  }
  
}

String rainJson()
{

  String jsonMsg =  String("\"rain\": ") + 
                      "{ \"rainD\":" + String(rainD) + 
                      ", \"rainA\":" + String(rainA) + 
                      "}";
  
  return jsonMsg;

}

void sendRainData()
{
  // read rain sensor and send data
  readRainSensor();

#ifdef  W_MQTT
  mqttSendSensorData(rainJson());
#endif

}

