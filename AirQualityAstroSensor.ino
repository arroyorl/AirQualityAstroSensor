/*
 *  This sketch runs an IoT for Air Quality and wind speed MQTT device.
 *  During the setup the device will act as AP mode so that you can set the
 *  Wifi connection, then it will try to connect to the WiFi. 
 *  - While serving the webserver it will also post
 *    the data to MQTT broker.
 */
///////////////////////////////////////////////////////////////
//           History                                         //
///////////////////////////////////////////////////////////////
//  0.1 first version using sds011
//      code based on measure.ino from Dirk O. Kaar <dok@dok-net.net>
//
//  0.1a updated sendDataWunderground()
//      eco mode working
//      added ADC_MODE(ADC_VCC) for read internal VCC
//      reviewed code for getBattLevel() (VCCINT)
//
//  0.2 added DHT22 to read Temp & Humid to be used in 
//      humidityCompensation funcion (based on hackAir github)
//
//  0.3 Moved SDS011 to GPIO12 and 14
//      included CCS811 gas sensor
//
//  0.4 Added Grove Multichannel Gas Sensor
//
//  0.4.1 added Weatherunderground PM2.5, PM10, NO2 and CO data
//
//  0.5 adjusted Vcc & batery level
//      added nominalBatt as nominal Batt (maximum) level
//      changed GetBattLevel to use nominalBatt
//
//  1.0 Added support for ThingSpeak
//
//  1.1 Using HTTP Get to submit ThingSpeak data
//
//  1.2 Back to ThingSpeak Library
//      error correction (when MQTT broker is down)
//
//  1.3 don't send Grove data if not defined
//
//  1.4 send dew point data to ThingSpeak (field 8)
//
//  1.5 activate/deactivate Grove sensor by configuration
//      show Vcc instead of Batt level in mainPage
//
//  1.5.1 reset device when temperature reading is NAN
//      send local IP by MQTT message
//
//  1.5.2 Initialize sensors after initialize WiFi
//      initialize DHT22 before SDS011  
//
//  1.5.3 Reset device every 24 hours
//
//  2.0 Added annemometer
//      changed DHT22 by BME280
//      removed echo mode
//
//  2.1 send windspeed data every n 5 secs. intervals
//      added maxwindspeed (max in send interval)
//
//  3.0 added TSL2561 luminosity sensor
//
//  4.0 added rain sensor
//      removed report of Vcc/battery (analog input is used for rain detector)
//
//  4.1 changed rain send frequency to 30 secs.
//      corrected an error in JSON message on handleRawData function
//
//  5.0 changed HTTPS access method to avoid fingerprint
//      removed fingerprint update web page
//
//  5.1 TSL2561 set to manual gain (1x) and 101 ms integration time
//
//  5.2 updated (corrected) formula for wind speed calculation
//      Wind speed as per https://pop.fsck.pl/hardware/wh-sp-ws01.html
//      is 0.34 * pulses/sec (in m/s)
//
//  5.2.1 TSL2561, changed integration time to 13 ms
//
//  5.2.2 integration time of TSL2561 changed to 402 ms and set on auto-gain
//
//  5.3 updated (corrected back) formula for wind speed calculation
//      as per https://cdn.sparkfun.com/assets/d/1/e/0/6/DS-15901-Weather_Meter.pdf
//      is 2.4 * pulses/sec (in Km/h)
//
//  5.3.1 test version for calibrate anemometer
//
//  5.3.2 MQTT messages adequared to v6.0
//
//  6.0 full reorganization of code, creating a file for each sensor
//      MQTT messages in JSON format
//      added GY-906 for cloud detection
//
//  6.1 Changed SDS011 library to SdsDustSensor 
//      (https://github.com/lewapek/sds-dust-sensors-arduino-library/tree/master)
//
//  6.2 added module for luminosity sensor ML1145
//      solved ThingSpeak issue (breaks and reset system) when code is set first time
//
//  6.3 mqtt now send one message for each sensor module (SEND_BY_MODULE) or
//        still sent an unique message with most sensor data
//      solved issue of wind sending 'nan' on first data submitted
//
//  6.4 Added skyTem() function for adjust Sky temperature
//      added cloudIndex() functio calculating cloud %
//
//  6.5 added SQM with TSL2591 
//      added VEML6075 (UV sensor)
//
//  6.6 changed web pages
//        removed initpage
//        / - root page (Usage)
//        /setup  - set up all communication parameters (SSID/PSSWD, MQTT)
//        /parameters - defines sensor parameters
//        /limits - defines limits for "safe" status
//      modified mqtt to take full topic from settings.data.mqtttopic, instead of create as
//        settings.data.mqtttopic / settings.data.name
//        if topic contains "%modname%", this is replaced by settings.data.name
//
//  6.7 split parameters by sensor
//      added 'K' parameters for GY906 skyTempAdj() function in parameters configuration .
//      added safe/unsafe limits parameters in parameters page
//      added safe.hpp for safe flag data submission
//
//  6.8 added boltwoodData() function which returns a Boltwood II format string with all weather data
//      added /boltwood web page, returning data in Boltwood II format
//      increased buffer size for MQTT to allow send all sensors data if SEND_BY_MODULE is not defined
//      changed wind.hpp to submit win measures on a complete period (numsampleswind * 5 secs.)
//
//  6.8.1 don't change spaces by &nbsp; in handlerBoltwood() if browser is 'curl'
//
//  6.9 replaced sqm-tsl2591 library by estandard Adafruit_TSL2591 and calculate SQM in sketch
//      sqm-tsl2591 library enter in loop when sensor is saturated
//
//  6.9.1 replaced formula for SQM mpsas calculation
//          from http://www.unihedron.com/projects/darksky/magconv.php
//        added settings.data.uvadjust as coefficient for UVIndex calculation in VELM6075
//
//  6.10 changed parameters and modes in VEML6075 module
//       changed #ifdef W_SQM by W_GY906 in parameters (the section corresponds to GY906)      
//
//  6.11 defined rainabove in settings to control if rainD is true when A0 reading (rainA) is
//        above or below threshold.
//       this is needed for graphite rain sensors 
//
//  6.11.1 included code in safe.hpp to take in account data.rainabove variable
//
//  6.12  Removed reset action when GY906 is not found and changed by a variable (gy906Active) 
//          which controls if get_gy906data() gets the data or returns inmediatelly
//
//  6.12.1  adjusted parameter for UV index
//
//  6.13  replaced Rdebug.h by rdebug.hpp (uses remote debug library: https://github.com/karol-brejna-i/RemoteDebug)
//        sending data to WU with http instead of https (https doesn't work well after include rdebug.hpp)
//        changed some Debug/DebugLn by Info/InfoLn, Warning/WarningLn and Error/ErrorLn for better debug
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>  

#include <time.h> 

#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>   // v 1.9.12

#define FVERSION  "v6.13"
#define PNAME "ESP8266 Air Quality"

///////////////////////////////////////////////////
// Comment for final release
#define RDEBUG
///////////////////////////////////////////////////

// modules included
#define W_MQTT        // MQTT module
#define W_WU          // send data to WeatherUnderground
#define W_TS          // send data to ThingSpeak
#define W_BME280      // Temperature, humidity and pressure BME280 sensor
#define W_SDS011      // SDS011 air quality sensor
// #define W_TSL2561     // luminosity sensor
#define W_RAIN        // rain sensor
#define W_WIND        // anemomether
#define W_GY906       // cloud detector (GY-906 / MLX90614)
// #define W_ML1145      // UV-luminosity sensor (UV calculated)
#define W_BOLTWOOD    // Boltwood II format data submission
#define W_SQM         // Sky quality (mpsas) and luminosity
#define W_VEML6075    // UV index


#define SEND_BY_MODULE  // send a MQTT message for each module
                        // if not defined will send a message integrating all data
                        // except for rain and wind for which separate messages are sent

#include "rdebug.hpp"
#include "settings.h"

#define GPIO0 0
#define GPIO1 1
#define GPIO2 2
#define GPIO3 3
#define GPIO4 4
#define GPIO5 5
#define GPIO6 6
#define GPIO7 7
#define GPIO8 8
#define GPIO9 9
#define GPIO10 10
#define GPIO11 11
#define GPIO12 12
#define GPIO13 13
#define GPIO14 14
#define GPIO15 15
#define GPIO16 16

#define LEDON LOW
#define LEDOFF HIGH
#define SETUP_PIN GPIO0
#define WAKEUP_PIN GPIO16

// see board names in C:\Users\ricardo\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\3.1.2\boards.txt
// Define Red and green LEDs
  #ifdef ARDUINO_ESP8266_NODEMCU_ESP12E
# define RED_LED    GPIO2   // D4
# define GREEN_LED  GPIO10  // D12 - SD3
#endif
#ifdef ARDUINO_ESP8266_GENERIC
# define RED_LED    GPIO12 //    antes GPIO9
# define GREEN_LED  GPIO10
#endif

//Struct to store setting in EEPROM
Settings    settings;

//Global Variables
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
String      data;
String      httpUpdateResponse;
const char* ap_ssid = "ESP-AIRQUAL";
const char* ap_password = ""; //"12345678";
int         ap_setup_done = 0;
rst_info*   myResetInfo;
String      ipaddress;
unsigned long        lastTime; //The millis counter to see when a second rolls by

// global variables for time
time_t      now;                    // this are the seconds since Epoch (1970) - UTC
tm          tm;                     // the structure tm holds time information in a more convenient way
char        hhmm[6];                // variable to store time in format HH:MM

// MQTT functions
#ifdef W_MQTT
#   include  "mqtt.h"
#endif

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Include functions for sensors
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#include "bme280.hpp"           // temperature, humidity, pression
#include "tsl2561.hpp"          // luminosity
#include "gy906.hpp"            // sky temperature, clouds
#include "sds011.hpp"           // pm2.5 & pm10
#include "rain.hpp"             // rain
#include "wind.hpp"             // wind speed
#include "ml1145.hpp"           // UV index (calculated), luminosity
#include "sqm-tsl2591.hpp"      // SQM (TSL2591)
#include "veml6075.hpp"         // UVA, UVB and UV index
#include "safe.hpp"             // check safe conditions
#include "boltwood.hpp"         // prepare data in Boltwood II format
#include "wunderground.hpp"     
#include "thingspeak.hpp"

// function prototypes
String  prepareJsonData();
void    getSensorData();
int     setupSTA();

ESP8266WebServer server(80);
// root web page -> usage
#include "rootPage.hpp"
// Web page for setup configuration
#include "setupPage.hpp"
// web page for parameters configuration
#include "parametersPage.hpp"
// Web server for respond to rawdata web requestes and send data to wonderground
#include "webserver.hpp"

/////////////////////////////////////////
// blink LED
/////////////////////////////////////////
void blinkLed(unsigned long time){
  digitalWrite(RED_LED, LEDON);
  delay(time);
  digitalWrite(RED_LED, LEDOFF);
  delay(time);
}

/////////////////////////////////////////
// setup WiFi configuration
/////////////////////////////////////////
void firstSetup(){
  InfoLn("First Setup ->");
  InfoLn(String(settings.data.magic));
  InfoLn("Setting up AP");
  WiFi.mode(WIFI_AP);             //Only Access point
  WiFi.softAP(ap_ssid, NULL, 8);  //Start HOTspot removing password will disable security
  delay(50);
  server.on("/", handleSetup);
  server.on("/setupform", handleSetupForm);
  InfoLn("Server Begin");
  server.begin(); 
  delay(100);
  do {
    server.handleClient(); 
    blinkLed(200);
    Info(".");
  }
  while (!ap_setup_done);
    
  settings.data.magic[0] = MAGIC[0];
  settings.data.magic[1] = MAGIC[1];
  settings.data.magic[2] = MAGIC[2];
  settings.data.magic[3] = MAGIC[3];
  server.stop();
  WiFi.disconnect();
  settings.Save();
  InfoLn("First Setup Done");
}


/////////////////////////////////////////
// connect to WiFi
/////////////////////////////////////////
int setupSTA()
{
  int timeOut=0;

  for (int retry=0; retry<=3; retry++) {
    WiFi.disconnect();
    WiFi.hostname("ESP-" + String(settings.data.name)) ;
    WiFi.mode(WIFI_STA);
    InfoLn("Connecting to "+String(settings.data.ssid));
    InfoLn("Connecting to "+String(settings.data.psk));
    
    if (String(settings.data.psk).length()) {
      WiFi.begin(String(settings.data.ssid), String(settings.data.psk));
    } else {
      WiFi.begin(String(settings.data.ssid));
    }
    timeOut=0;
    retry=0;    
     while (WiFi.status() != WL_CONNECTED) {
      if (timeOut < 10){          // if not timeout, keep trying
        delay(100);
        Debug(String(WiFi.status()));
        blinkLed(500);
        timeOut ++;
      } else{
        timeOut = 0;
        DebugLn("-Wifi connection timeout");
        blinkLed(500);
        retry++;
        if (retry == 3) { 
          return 0;
        }
      }
    }
    InfoLn(" Connected");

    // Print the IP address
    ipaddress = WiFi.localIP().toString();
    InfoLn(ipaddress); 
    InfoLn(WiFi.hostname().c_str());
    break; 
    }
    return 1;
}

/////////////////////////////////////////
// get data from sensors
/////////////////////////////////////////
void  getSensorData(){

#ifdef W_BME280
  getBME280data();
#endif
#ifdef W_TSL2561
  getTSL2561data();
#endif
#ifdef W_RAIN
  readRainSensor();
#endif
#ifdef W_GY906
  get_gy906data();
#endif
#ifdef W_ML1145
  getML1145data();
#endif
#ifdef W_SQM
  getSQMdata();
#endif
#ifdef W_VEML6075
  get_veml6075data();
#endif

}

/////////////////////////////////////////
// prepare all sensor data in JSON format
/////////////////////////////////////////
String prepareJsonData() {

  ipaddress = WiFi.localIP().toString();

  time(&now);  // read the current time

  String res =  "{ \"time\": \"" + String(now) + "\"" + 
                ", \"ipaddress\": \"" + String(ipaddress) + "\"" + 
                ", \"version\": \"" + String(FVERSION) + "\"" + 
                ",\"sensors\":" +
                  "{ \"temperature\":" + String(temperature) + 
                  ", \"humidity\":" + String(humidity) + 
                  ", \"dewpoint\":" + String(dewpoint) +
                  ", \"pressure\":" + String(pressure) + 
                  ", \"sealevelpressure\":" + String(sealevelpressure) + 
                  ", \"pm10\":" + String(pm10f) + 
                  ", \"pm25\":" + String(pm25f) + 
                  ", \"running\":" + String(is_SDS_running) + 
                  ", \"lux\":" + String(lux) +
                  ", \"rainD\":" + String(rainD) + 
                  ", \"rainA\":" + String(rainA) + 
                  ", \"windspeed\":" + String(windspeed) +
                  ", \"maxwindspeed\":" + String(maxwindspeed) +
                  ", \"skyT\":" + String(skyTemp) + 
                  ", \"ambientT\":" + String(ambientTemp) + 
                  ", \"cloudIndex\":" + String(cloudI) +    
                  ", \"clouds\":" + String(clouds) +    
                  ", \"UVindex\":" + String(UVindex) +
                  ", \"sqmfull\":" + String(sqmFull) +
                  ", \"sqmIR\":" + String(sqmIr) +
                  ", \"sqmVis\":" + String(sqmVis) +
                  ", \"mpsas\":" + String(sqmMpsas) +
                  ", \"dmpsas\":" + String(sqmDmpsas) +
                  ", \"UVA\":" + String(uva) +
                  ", \"UVB\":" + String(uvb) +
                  ", \"vemlIVIndex\":" + String(uvi) +
                  "}" +
                "}";

  return res;
}

/////////////////////////////////////////
// send sensors data
/////////////////////////////////////////
void sendData() {
  InfoLn("sendData");

  // get Temperature, humidity and Pressure
  getSensorData();
  DebugLn("Temp: "+String(temperature)+" ºC, Humid: "+String(humidity)+"%");

  if ( isnan(temperature) ) {
    // temperature value is nan, reset device
    WarningLn("Failed temp reading [nan], reseting device");
    delay(2000);
    ESP.restart();
  }


#ifdef W_MQTT
# ifdef SEND_BY_MODULE
  // send one MQTT message for each module data
#   ifdef W_BME280
      sendBME280data();
#   endif
#   ifdef W_GY906
      sendGY906data();
#   endif
#   ifdef W_TSL2561
      sendTSL2561data();
#   endif
#   ifdef W_ML1145
      sendML1145data();
#   endif
#   ifdef W_SQM
      sendSQMdata();
#   endif
#   ifdef W_VEML6075
      sendVEML6075data();
#   endif
      sendSafedata();
#   ifdef W_BOLTWOOD  
      sendBoltwood();
#   endif
# else
  mqtt_send("data", prepareJsonData(), false);
# endif
#endif
  delay(100);
#ifdef  W_WU
  sendDataWunderground();
#endif
  delay(100);
#ifdef  W_TS
  ts_send(settings.data.ThingSpeakChannel, settings.data.ThingSpeakKey);
#endif

}

/////////////////////////////////////////
// module setup
/////////////////////////////////////////
void setup() {
  DebugSerialStart();
  InfoLn("Start ->");

  myResetInfo = ESP.getResetInfoPtr();
  InfoLn("myResetInfo->reason "+String(myResetInfo->reason)); // reason is uint32
                                                                 // 0 = power down
                                                                 // 6 = reset button
                                                                 // 5 = restart from deepsleep


  //************** load settings **********************************
  settings.Load();

  //************** setup LED and DHT pins, and switch OFF LEDs **************
  DebugLn("Setup ->");
  pinMode(RED_LED, FUNCTION_3);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, FUNCTION_3);
  pinMode(GREEN_LED, OUTPUT);

  // ******** initiallize LEDs and wait for setup pin *****************
  DebugLn("-> Check if SETUP_PIN is low");
  digitalWrite(RED_LED, LEDON);
  digitalWrite(GREEN_LED, LEDON);
  // Wait up to 5s for SETUP_PIN to go low to enter AP/setup mode.
  pinMode(SETUP_PIN, INPUT);      //Configure setup pin as input
  digitalWrite(SETUP_PIN, HIGH);  //Enable internal pooling
  delay(5000);  
  DebugLn("Magic ->"+String(settings.data.magic));

  // if NO MAGIC Or SETUP PIN enter hard config...
  if (String(settings.data.magic) != MAGIC||!digitalRead(SETUP_PIN)){
    digitalWrite(GREEN_LED, LEDOFF);
    digitalWrite(RED_LED, LEDON);
    InfoLn("First Setup ->");
    firstSetup();
  }
  else {
    // initial setup is done
    ap_setup_done = 1;
  }

  // NO SETUP, switch off both LEDs
  digitalWrite(GREEN_LED, LEDOFF);
  digitalWrite(RED_LED, LEDOFF);

  //********* initialize sensors *****************
#ifdef  W_BME280
  BME280setup();
#endif
#ifdef W_TSL2561
  // TSL2561, SDA=4 (D2), SCL=5 (D1)
  TSL2561active = TSL2561setup();
#endif
#ifdef W_RAIN
  setupRain();
#endif
#ifdef W_WIND
  setupWind();
#endif
#ifdef W_GY906
  gy906setup();
#endif
#ifdef W_ML1145
  ML1145setup();
#endif
#ifdef W_SQM
  SQMsetup();
#endif
#ifdef W_VEML6075
  veml6075setup();
#endif

#ifdef  W_SDS011
  SDS011setup();
  delay(100); 
  // initiate SDS011 capture 
  start_SDS();
#endif


  // ********** set timezone (do it before WiFi setup) *******************
  configTime(settings.data.timezone, settings.data.ntpserver);

  // *********** setup STA mode and connect to WiFi ************
  if (setupSTA() == 0) { // SetupSTA mode
    WarningLn("Wifi no connected, wait 60 sec. and restart");
    delay(60*1000);
    ESP.restart();
  }

  // ********** initialize remote Debug *******************
  DebugRemoteStart();

  // ********** initialize OTA *******************
  ArduinoOTA.begin();

#ifdef W_MQTT
  // ********* initialize MQTT ******************
  if (strlen(settings.data.mqttbroker) > 0 ) {
    // MQTT broker defined, initialize MQTT
    mqtt_init();
    delay(100);
  }
#endif
#ifdef  W_TS
  ThingSpeaksetup();
#endif

  //********** switch ON GREEN LED, initialize web servers and switch OFF LEDs
  digitalWrite(GREEN_LED, LEDON);
  digitalWrite(RED_LED, LEDOFF);
  DebugLn("-> Initiate WebServer");
  server.on("/",handleRoot);
  server.onNotFound(handleRoot);
  server.on("/parameters",handleParameters);
  server.on("/setup", handleSetup);
  server.on("/data",handleRowData);
  server.on("/setupform", handleSetupForm);
  server.on("/parametersform", handleParametersForm);
  server.collectHeaders("User-Agent","Content-Type");
#ifdef  W_BOLTWOOD
  server.on("/boltwood", handleBoltwood);
#endif
  delay(100);
  server.begin(); 
  delay(3000);
  digitalWrite(GREEN_LED, LEDOFF);
  digitalWrite(RED_LED, LEDOFF);  

  // ********* initialize counters
  lastTime = millis();

  // read current time'
  time(&now);                       // read the current time
  Info("-> Time sync to ");
  InfoLn(ctime(&now));

#ifdef W_SDS011
  //send SDS011 first data if available
  handleSDS();
  if (send_SDS) {
    send_SDS = false;
    // send sensor data to MQTT
    sendSDS011data();
  }
#endif

  // send data (first interval)
  sendData();

  InfoLn("-> End Setup");

  // RemoteDebug handle
  HandleDebug();
}

/////////////////////////////////////////
// main loop
/////////////////////////////////////////
void loop() {

  // handle http client while waiting
  server.handleClient();    
  
  // handle OTA
  ArduinoOTA.handle();

  // handle MQTT
  handleMQTT();

  // RemoteDebug handle
  HandleDebug();

  // normal loop

  if ( (millis() - lastTime) > (settings.data.poolinterval * 1000) ) {
      // check if has been elapsed poolinterval seconds
      lastTime = millis();
#     ifdef  W_SDS011
      // initiate SDS011 capture
      start_SDS();
#     else
      sendData();
#     endif
  }
  else {
    if (send_SDS) {
      send_SDS = false;
      // send sensor data to MQTT
#     ifdef  W_SDS011
      sendData();
      sendSDS011data();
#     endif
    }
  }

# ifdef  W_SDS011
  // check if SDS has complete read cycle
  handleSDS();
# endif

#ifdef  W_RAIN
  // send rain data every 30 seconds
  if( (millis() - lastSecondR) >= 30000)
  {
    lastSecondR = millis();
    sendRainData();
#ifdef  W_BOLTWOOD
    isSafe();
    if(safeNow != safePrev) {
      sendBoltwood;
    }
#endif
  }
#endif

#ifdef W_WIND
  // calculate wind speed every 5 seconds
  if(millis() - lastSecondW >= 5000)
  {
      lastSecondW = millis();

      //Get wind speed 
      windspeed = get_wind_speed(); //Wind speed in Km/h
      // DebugLn("windspeed: " + String(windspeed));
      sumwindspeed += windspeed;
      maxwindspeed = max(windspeed, maxwindspeed);
      numsampleswind++;
      if (numsampleswind == settings.data.numsampleswind) {
        windspeed = (sumwindspeed / (float)numsampleswind);  
        windspeedNow = windspeed;
        maxwindspeedNow = maxwindspeed;
        sendWindData();
        sumwindspeed = 0.0;
        maxwindspeed = 0.0;
        numsampleswind = 0;
#ifdef  W_BOLTWOOD
        isSafe();
        if(safeNow != safePrev) {
          sendBoltwood;
        }
#endif
      }
  }
#endif

  // reset every 24 h 
  if (millis() > (24*60*60*1000ul)) {
    ESP.restart();
  }
}
