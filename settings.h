/*  Copyright (C) 2016 Buxtronix and Alexander Pruss

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include <EEPROM.h>

#ifndef SETTINGS_H 
#define SETTINGS_H 

#define MAGIC_LENGTH 4
#define SSID_LENGTH 32
#define PSK_LENGTH 64
#define NAME_LENGTH 32
#define ID_LENGTH 16
#define KEY_LENGTH 16
#define TIMEZONE_LENGTH 128
#define SERVER_LENGTH 256

#define MAGIC "AQ6\0"

/* Configuration of NTP */
#define NTP_SERVER "europe.pool.ntp.org"           
#define TIMEZONE "CET-1CEST,M3.5.0/02,M10.5.0/03"  

char my_default_ssid[SSID_LENGTH] = "";
char my_default_psk[PSK_LENGTH] = "";

#define CLOCK_NAME "ESP-AIRQUAL"

class Settings {
  public:
  struct {
    char    magic[MAGIC_LENGTH];               // magic = "AQ6"
    char    ssid[SSID_LENGTH];                 // SSID
    char    psk[PSK_LENGTH];                   // PASSWORD
    char    name[NAME_LENGTH];                 // device name
    int     poolinterval;                      // sensor reading interval

    char    mqttbroker[SERVER_LENGTH];         // Address of MQTT broker server
    char    mqtttopic[NAME_LENGTH];            // MQTT topic for Jeedom
    char    mqttuser[NAME_LENGTH];             // MQTT account user
    char    mqttpassword[NAME_LENGTH];         // MQTT account password
    int     mqttport;                          // port of MQTT broker

    char    timezone[TIMEZONE_LENGTH];         // TIMEZONE definition
    char    ntpserver[TIMEZONE_LENGTH];        // NTP server

    char    stationID[ID_LENGTH];              // Wunderground Station ID
    char    stationKey[KEY_LENGTH];            // Wunderground Station Key

    unsigned long ThingSpeakChannel;           // ThingSpeak channel number
    char    ThingSpeakKey[NAME_LENGTH];        // ThingSpeak API Write Key

    float   tempadjust;                        // Calibracion de temperatura
    float   altitude;                          // altitude over sea level 
    float   humiditythreshold;                    // limit for humidity safe

    int     rainthreshold;                     // Threshold of rain sensor

    int     numsampleswind;                    // num samples (5 secs/each) to send wind data
    float   windthreshold;                     // threshold for win speed safe

    float   cloudytemp;                        // sky temperature above it is considereded cloudy
    float   k1, k2, k3, k4, k5, k6, k7;        // 'K' parameters for GY906 skyTempAdj() function
    float   cloudthreshold;                    // limit for cloud index safe

    float   uvadjust;                          // UV adjust coeficient
    int     minIR;                             // minimum IR read (ML1145)
    int     minVis;                            // minimum Visible read (ML1145)

    float   sqmthreshold;                      // SQM threshold safe
    float   luxthreshold;                      // luminosity safe threshold
} data;
    
    Settings() {};
 
    void Load() {           // Carga toda la EEPROM en el struct
      EEPROM.begin(sizeof(data));
      EEPROM.get(0,data);  
      EEPROM.end();
     // Verify magic; // para saber si es la primera vez y carga los valores por defecto
     if (String(data.magic)!=MAGIC){
      data.magic[0]=0;
      data.ssid[0]=0;
      data.psk[0]=0;
      data.name[0]=0;

      data.mqttbroker[0]=0;
      strcpy(data.mqtttopic, "jeedom/%modname%");
      data.mqttuser[0]=0;
      data.mqttpassword[0]=0;
      data.mqttport=1883;

      strcpy(data.timezone,TIMEZONE);
      strcpy(data.ntpserver,NTP_SERVER);

      data.poolinterval=300;    // 300 seconds

      data.stationID[0]=0;
      data.stationKey[0]=0;

      data.ThingSpeakChannel=0;
      data.ThingSpeakKey[0]=0;

      data.tempadjust=0.0;
      data.altitude=0.0;
      data.humiditythreshold=80.0;

      data.rainthreshold=600;

      data.numsampleswind=12;
      data.windthreshold=30.0;

      data.cloudytemp=-5.0;
      data.k1=33.0;
      data.k2=0.0;
      data.k3=8.0;
      data.k4=100.0;
      data.k5=100.0;
      data.k6=0.0;
      data.k7=0.0;
      data.cloudthreshold=30.0;

      data.uvadjust=1.0;
      data.minIR=1000;
      data.minVis=1000;

      data.sqmthreshold=12.0;
      data.luxthreshold=30.0;

      Save();
     }
    };
      
    void Save() {
      EEPROM.begin(sizeof(data));
      EEPROM.put(0,data);
      EEPROM.commit();
      EEPROM.end();
    };
};
  #endif
