/*************************************************
          safe / unsafe conditions check

  v2.0 included code to take in account data.rainabove variable

*************************************************/

bool safePrev = false;
bool safeNow  = false;

bool isSafe(){

  bool res = true;

#ifdef  W_BME280
  if (skyTemp > settings.data.cloudytemp)
    res = false;
  else if (humidity > settings.data.humiditythreshold)
    res = false;
#endif
#ifdef  W_WIND
  if (maxwindspeed > settings.data.windthreshold)
    res = false;
#endif
#ifdef  W_RAIN
  if (settings.data.rainabove) {
    if (rainA > settings.data.rainthreshold)
      res = false;
  }
  else {
    if (rainA < settings.data.rainthreshold)
      res = false;
  }
  DebugLn("rainabove: " + String(settings.data.rainabove) + ", threshold: " + String(settings.data.rainthreshold));
#endif
#ifdef  W_GY906
  if (cloudI > settings.data.cloudthreshold)
    res = false;
#endif
#ifdef  W_SQM
  if (sqmMpsas < settings.data.sqmthreshold)
    res = false;
  else if (sqmLux > settings.data.luxthreshold)
    res = false;
#endif

  safePrev = safeNow;   // store previous safe condition
  safeNow = res;        // store current safe condition
  return res;
}

String safeJson()
{

  String jsonMsg =  String("\"safe\": ") + String(isSafe());
  return jsonMsg;

}

void sendSafedata()
{

#ifdef  W_MQTT
  mqttSendSensorData(safeJson());
#endif
}
