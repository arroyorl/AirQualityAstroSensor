/**
/ function for send data to WeatherUnderground
**/

// function prototipes
void sendHTTPsGet (String httpGet);
void sendHTTPGet (String httpGet);

void sendDataWunderground(){
float tempf;
float dewptf;
float baromin;

  if(strlen(settings.data.stationID) > 0 && strlen(settings.data.stationKey) > 0 ){
    // Send data to Wunderground
    InfoLn("--- Sending data to Wunderground ---");
    String  weatherData =  "http://rtupdate.wunderground.com/weatherstation/updateweatherstation.php?";
    weatherData += "ID=" + String(settings.data.stationID);
    weatherData += "&PASSWORD=" + String(settings.data.stationKey);
    weatherData += "&dateutc=now";
    weatherData += "&action=updateraw";
    tempf = (temperature * 9.0) / 5.0 + 32.0;
    weatherData += "&tempf=" + String (tempf);
    weatherData += "&humidity=" + String (humidity);
    dewptf = (dewpoint * 9.0) / 5.0 + 32.0;
    weatherData += "&dewptf=" + String(dewptf);
    baromin = sealevelpressure * 29.92 / 1013.25;
    weatherData += "&baromin=" + String(baromin);
  
  #ifdef W_SDS011
    // send PM10 and PM2.5 particles concentration
    weatherData += "&AqPM2.5=" + String (pm25f);
    weatherData += "&AqPM10=" + String (pm10f);
  #endif
  
    // send to Wunderground
    sendHTTPGet(weatherData);
  }
                
}
