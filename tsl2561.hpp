/*************************************************
          TSL 2561 luminosity sensor
*************************************************/
#include <Adafruit_Sensor.h>  // Adafruit Unified Sensor v 1.1.14
#include <Adafruit_TSL2561_U.h>  // Adafruit TSL 2561 v 1.1.2

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

float       lux;
bool        TSL2561active = false;

bool TSL2561setup()
{
  if(!tsl.begin())
  {
    // TSL2561 not detected
    DebugLn("No TSL2561 detected ... Check wiring or I2C ADDR !");
    return false;
  }

  // Display sensor details
  sensor_t sensor;
  tsl.getSensor(&sensor);
  DebugLn("***** Inicializing TSL2561 *****");
  DebugLn ("Sensor:       " + String(sensor.name));
  DebugLn ("Driver Ver:   " + String(sensor.version));
  DebugLn ("Unique ID:    " + String(sensor.sensor_id));
  DebugLn ("Max Value:    " + String(sensor.max_value) + " lux");
  DebugLn ("Min Value:    " + String(sensor.min_value) + " lux");
  DebugLn ("Resolution:   " + String(sensor.resolution) + " lux");  
  delay(500);

  /* You can also manually set the gain or enable auto-gain support */
  // tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
  // tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  
  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */

  /* Update these values depending on what you've set above! */  
  DebugLn ("***** Initialized TSL 2561 *****");

  return true;
}

float getTSL2561lux() 
{
  /* Get a new sensor event */ 
  sensors_event_t event;
  sensor_t sensor;

  tsl.getSensor(&sensor);

  InfoLn ("Reading TSL2561 data");
  DebugLn ("Max Value:    " + String(sensor.max_value) + " lux");
  DebugLn ("Min Value:    " + String(sensor.min_value) + " lux");
  DebugLn ("Resolution:   " + String(sensor.resolution) + " lux");  

  tsl.getEvent(&event);

  return event.light;
 
}

void getTSL2561data() {

  // get luminostity from TSL2561
  if (TSL2561active) {
    lux = getTSL2561lux();
  }

}

String tsl2561Json()
{

  String jsonMsg =  String("\"tsl2561\": ") + 
                    "{ \"lux\":" + String(lux) +
                      "}";
  
  return jsonMsg;

}

void sendTSL2561data()
{

#ifdef  W_MQTT
  mqttSendSensorData(tsl2561Json());
#endif
}
