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
  rainD = ( rainA < settings.data.rainthreshold );
  
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

