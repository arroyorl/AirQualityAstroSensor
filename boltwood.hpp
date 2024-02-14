/*************************************************************
* returns a String in Boltwood II format with all weather data
*************************************************************/

unsigned long lastBoltwood = 0;   // used by boltwoodData()
char boltwoodInfo[150] ="";

String boltwoodData() {
char  currDate[12];
char  currTime[10];

  struct tm * timeinfo;
  time(&now);
  timeinfo = localtime(&now);  

  strftime(currDate,12,"%F",timeinfo);
  strftime(currTime,10,"%T",timeinfo);

  bool wetFlag = humidity > settings.data.humiditythreshold;
  unsigned long secsElapsed = (millis() - lastBoltwood)/1000;
  lastBoltwood = millis();
  int cloudFlag;
  if (cloudI < settings.data.cloudthreshold)
    cloudFlag = 1;  // clear
  else if (skyTemp < 0)
    cloudFlag = 2;  // cloudy
  else
    cloudFlag = 3;  // overcast
  int windFlag = (maxwindspeed > settings.data.windthreshold ? 2 : 1);
  int rainFlag = (rainD ? 3 : 1);
  int darknessFlag = (sqmLux > settings.data.luxthreshold ? 3: 1);

  sprintf(boltwoodInfo, 
          "%10S %8s.00 C M %5.1f %5.1f %5.1f  %5.1f     %3d  %5.1f   000 %0d %0d %05d 00000.00000 %0d %0d %0d %0d 0 %0d",
          currDate, currTime, skyTemp, temperature, ambientTemp, (maxwindspeedNow / 1.609344), (int)humidity, dewpoint, (int)rainD, (int)wetFlag, secsElapsed, 
          cloudFlag, windFlag, rainFlag, darknessFlag, (safeNow ? 0 :1));

  DebugLn("Boltwood: " + String(boltwoodInfo));

  return boltwoodInfo;
}

String boltwoodJson()
{

  String jsonMsg =  String("\"boltwood\": \"") + String(boltwoodData()) + "\"";
  return jsonMsg;

}

void sendBoltwood()
{

#ifdef  W_MQTT
  mqttSendSensorData(boltwoodJson());
#endif
}


