#define SENSORTYPE BME280
#define SENSORNAME "BME280"
#include <Wire.h>
#include <Adafruit_Sensor.h>    // 1.1.14
#include <Adafruit_BME280.h>    // 2.2.4

Adafruit_BME280 bme; 

float       temperature=0;
float       humidity=0;
float       dewpoint=0;
float       pressure=0;
float       sealevelpressure=0;
float       altitudecalc=0;

bool        bme280active = false;

float       tempf;
float       dewpointf;
float       baromin;

#define NUM_SAMPLES_BME 10

/**************************************************************************/
/*     calculates dew point 
/*     reference: http://wahiduddin.net/calc/density_algorithms.htm
/*
/**************************************************************************/
float computeDewPoint(float celsius, float humidity)
{
        double RATIO = 373.15 / (273.15 + celsius);  // RATIO was originally named A0, possibly confusing in Arduino context
        double SUM = -7.90298 * (RATIO - 1);
        SUM += 5.02808 * log10(RATIO);
        SUM += -1.3816e-7 * (pow(10, (11.344 * (1 - 1/RATIO ))) - 1) ;
        SUM += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1) ;
        SUM += log10(1013.246);
        double VP = pow(10, SUM - 3) * humidity;
        double T = log(VP/0.61078);   // temp var
        return (241.88 * T) / (17.558 - T);
}

/**************************************************************************/
/*     BME280 setup function 
/**************************************************************************/
void BME280setup(){
  // BME280, SDA=4 (D2), SCL=5 (D1)
  bme280active = bme.begin(0x76);          
  DebugLn("-> BME begin: " + String(bme280active));
}

/**************************************************************************/
/*     BME280 read data function 
/**************************************************************************/
void getBME280data() {

float runtemp=0;
float runhumid=0;
float runpress=0;

  if (bme280active) {
    DebugLn("get BME280 data");

    //////////////////////////////// BME280 ////////////////////////////////////////////////
    for(int i = 0 ; i < NUM_SAMPLES_BME; i++){
      runtemp += bme.readTemperature();
      runhumid += bme.readHumidity();
      runpress += bme.readPressure();
    }

    float nSamples = NUM_SAMPLES_BME;
    temperature = (runtemp / nSamples) + settings.data.tempadjust;
    tempf = (temperature * 9.0) / 5.0 + 32.0;
    humidity = runhumid / nSamples;
    dewpoint = computeDewPoint(temperature, humidity);
    dewpointf = (dewpoint * 9.0) / 5.0 + 32.0;
    pressure = (runpress / nSamples) / 100.0F;
    sealevelpressure = bme.seaLevelForAltitude(settings.data.altitude, pressure);
    baromin = sealevelpressure * 29.92 / 1013.25;
    DebugLn("BME280 data read !");
    ////////////////////////////////////////////////////////////////////////////////

    if (isnan(humidity) || isnan(temperature) || isnan(pressure)) {
        DebugLn("Failed to read from sensor!");
    }

    
    DebugLn("Temp: "+String(temperature));
    DebugLn("Hum: "+String(humidity));
    DebugLn("DewPoint: "+String(dewpoint));
    DebugLn("Pres: " + String(pressure));
    DebugLn("SeaLevelPres: "+String(sealevelpressure));
  }
  else {
    DebugLn("************ BME280 not active *************");
  }
}

String bme280Json()
{

  String jsonMsg =  String("\"bme280\": ") + 
                      "{ \"temperature\":" + String(temperature) + 
                      ", \"humidity\":" + String(humidity) + 
                      ", \"dewpoint\":" + String(dewpoint) +
                      ", \"pressure\":" + String(pressure) + 
                      ", \"sealevelpressure\":" + String(sealevelpressure) + 
                      "}";
  
  return jsonMsg;

}

void sendBME280data()
{

#ifdef  W_MQTT
  mqttSendSensorData(bme280Json());
#endif
}


