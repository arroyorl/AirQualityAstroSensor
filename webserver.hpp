//--------------------------------------------
//  HTTP POST
//--------------------------------------------
void sendHTTP (String httpPost){
  WiFiClient client;
  HTTPClient http;
  DebugLn(httpPost);
  http.begin(client, httpPost);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST("");
  DebugLn("HTTPPost return code: " + String(httpCode));    
  http.end();
}

//--------------------------------------------
//  HTTP GET
//--------------------------------------------
void sendHTTPGet (String httpGet){
  WiFiClient client;
  HTTPClient http;
  DebugLn(httpGet);
  http.begin(client, httpGet);
  int httpCode = http.GET();
  DebugLn("HTTPGet return code: " + String(httpCode));    
  http.end();
}

//--------------------------------------------
//  HTTPs GET
//--------------------------------------------
void sendHTTPsGet (String httpGet){
  HTTPClient https;

  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

  // fingerprint not needed
  client->setInsecure();
  // send data
  DebugLn("HTTPsGet: " + httpGet);
  if (https.begin(*client, httpGet)) {
    // https.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpCode = https.GET();
    String payload = https.getString();

    if (httpCode > 0) {
      // Sucess
      DebugLn("HTTPsGet return code: " + String(httpCode));    // this return 200 when success
    }
    else {
      WarningLn("HTTPsGet return code: " + String(httpCode) + "[" + https.errorToString(httpCode).c_str() + "]");
    }
    DebugLn(payload);     // this will get the response
    https.end();
  }
  else {
    DebugLn("HTTPsGet unable to connect");
  }

}

//--------------------------------------------
//  web server page returning raw data (json)
//--------------------------------------------
void handleRowData() {
  InfoLn("handleRowData");

  time(&now);                       // read the current time
  localtime_r(&now, &tm);           // update the structure tm with the current time

  httpUpdateResponse = "";
  getSensorData();
#ifdef  SEND_BY_MODULE
  String message =  "{ \"time\": \"" + String(now) + "\"" + 
                    ", \"ipaddress\": \"" + String(ipaddress) + "\"" + 
                    ", \"version\": \"" + String(FVERSION) + "\"" + 
# ifdef W_BME280    
                    ", " + bme280Json() + 
# endif
# ifdef W_GY906
                    ", " + gy906Json() + 
# endif
# ifdef  W_TSL2561
                    ", " + tsl2561Json() + 
# endif
# ifdef W_ML1145
                    ", " + ml1145Json() + 
# endif
# ifdef W_SDS011
                    ", " + sds011Json() +
# endif
# ifdef W_SQM
                    ", " + SQMJson() +
# endif
# ifdef W_VEML6075
                    ", " + veml6075Json() +
# endif
# ifdef W_RAIN
                    ", " + rainJson() +
# endif
# ifdef W_WIND
                    ", " + windJson() +
# endif
                    ", " + safeJson() +
                    " }"; 
  server.send(200, "text/html", message + "\r\n");

#else
  server.send(200, "text/html", prepareJsonData() + "\r\n");
#endif
}

#ifdef  W_BOLTWOOD
//-----------------------------------------------------
//  web server page returning Boltwood II format data
//-----------------------------------------------------
void handleBoltwood(){

  InfoLn("handleBoltwood");

  getSensorData();

  String data = boltwoodData();

  String UserAgent = server.header("User-Agent");
  DebugLn("User Agent:" + UserAgent);
  if (UserAgent.indexOf("curl") == -1) {
    //browswer is not curl, replace spaces by &nbsp;
    data.replace(" ","&nbsp;");
  }

  server.send(200, "text/html", data);

}
#endif

