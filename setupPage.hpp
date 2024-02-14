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

const char SETUP_page[] PROGMEM = R"=====(
<head>
 <meta name="viewport" content="initial-scale=1">
 <style>
 body {font-family: helvetica,arial,sans-serif;}
 table {border-collapse: collapse; border: 1px solid black;}
 td {padding: 0.25em;}
 .title { font-size: 2em; font-weight: bold;}
 .name {padding: 0.5em;}
 .heading {font-weight: bold; background: #c0c0c0; padding: 0.5em;} 
 .update {color: #dd3333; font-size: 0.75em;}
 </style>
</head>
<div class=title>@@PNAME@@</div>
<div class=name>Firmware: @@VERSION@@&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp<small> @@TIMENOW@@</small></div>
<form method="post" action="/setupform">
<table>
<tr><td colspan=2 class=heading>WiFi Setup</td></tr>
<tr><td>SSID:</td><td><input type=text name="ssid" value="@@SSID@@"></td></tr>
<tr><td>PSK:</td><td><input type=password name="psk" value="@@PSK@@"></td></tr>
<tr><td>Name:</td><td><input type=text name="modname" value="@@MODNAME@@"></td></tr>
<tr><td colspan=2>Update Wifi config:<input type=checkbox name=update_wifi value=1 @@WIFICHECK@@></td></tr>

<tr><td colspan=2 class=heading>MQTT Setup</td></tr>
<tr><td>MQTT broker:</td><td><input type=text size=35 name="mqttbroker" value="@@MQTTBROKER@@"></td></tr>
<tr><td>MQTT port:</td><td><input type=text size=35 name="mqttport" value="@@MQTTPORT@@"></td></tr>
<tr><td>MQTT user:</td><td><input type=text size=35 name="mqttuser" value="@@MQTTUSER@@"></td></tr>
<tr><td>MQTT passwd:</td><td><input type=text size=35 name="mqttpasswd" value="@@MQTTPASSWD@@"></td></tr>
<tr><td>MQTT topic:</td><td><input type=text size=35 name="mqtttopic" value="@@MQTTTOPIC@@"></td></tr>

<tr><td colspan=2 class=heading>Real time clock</td></tr>
<tr><td>NTP server:</td><td><input type=text size=35 name="ntpserver" value="@@NTPSERVER@@"> </td></tr>
<tr><td>Time zone:</td><td><input type=text size=35 name="timezone" value="@@TIMEZONE@@"> </td></tr>

</table>
<p/>
<input type="submit" value="Update">
</form>
<div class="update">@@UPDATERESPONSE@@</div>
</html>
)=====";

void handleSetup() {
  DebugLn("handleSetup");

 String s = FPSTR(SETUP_page);
  s.replace("@@PNAME@@", PNAME);
  s.replace("@@VERSION@@",FVERSION);

  s.replace("@@SSID@@", settings.data.ssid);
  s.replace("@@PSK@@", settings.data.psk);
  s.replace("@@MODNAME@@", settings.data.name);
  if (ap_setup_done == 0) 
    s.replace("@@WIFICHECK@@", "checked");   // check update Wifi checkbox if first setup
  else
    s.replace("@@WIFICHECK@@", "");         // uncheck update Wifi checkbox if first setup
  s.replace("@@MQTTBROKER@@",settings.data.mqttbroker);
  s.replace("@@MQTTPORT@@",String(settings.data.mqttport));
  s.replace("@@MQTTUSER@@",settings.data.mqttuser);
  s.replace("@@MQTTPASSWD@@",settings.data.mqttpassword);
  s.replace("@@MQTTTOPIC@@",settings.data.mqtttopic);
  s.replace("@@NTPSERVER@@",settings.data.ntpserver);
  s.replace("@@TIMEZONE@@",settings.data.timezone);
  s.replace("@@UPDATERESPONSE@@", httpUpdateResponse);
  
  time(&now);                       // read the current time
  localtime_r(&now, &tm);           // update the structure tm with the current time
  strftime(hhmm,6,"%H:%M",&tm);     // format time (HH:MM)
  s.replace("@@TIMENOW@@",String(hhmm));

  httpUpdateResponse = "";
  server.send(200, "text/html", s);
}

void handleSetupForm() {
  DebugLn("handleSetupForm");

  DebugLn("WiFi mode "+ String(WiFi.status()));
  String update_wifi = server.arg("update_wifi");
  DebugLn("update_wifi: " + update_wifi );
  String t_ssid = server.arg("ssid");
  String t_psk = server.arg("psk");
  String t_name = server.arg("modname");
  String(t_name).replace("+", " ");
  if (update_wifi == "1") {
    t_ssid.toCharArray(settings.data.ssid,SSID_LENGTH);
    t_psk.toCharArray(settings.data.psk,PSK_LENGTH);
    t_name.toCharArray(settings.data.name,NAME_LENGTH);
  }

  String brokerprev = String(settings.data.mqttbroker);
  int portprev = settings.data.mqttport;
  String topicprev = settings.data.mqtttopic;
  String userprev = settings.data.mqttuser;
  String passwprev = settings.data.mqttpassword;
  String aux;
  aux = server.arg("mqttbroker");
  aux.toCharArray(settings.data.mqttbroker,128);
  aux = server.arg("mqttuser");
  aux.toCharArray(settings.data.mqttuser,30);
  aux = server.arg("mqttpasswd");
  aux.toCharArray(settings.data.mqttpassword,30);
  aux = server.arg("mqtttopic");
  aux.toCharArray(settings.data.mqtttopic,30);
  aux = server.arg("mqttport");
  if (aux.length()) {
    settings.data.mqttport=aux.toInt();
  }

  aux = server.arg("ntpserver");
  aux.toCharArray(settings.data.ntpserver,128);
  aux = server.arg("timezone");
  aux.toCharArray(settings.data.timezone,128);

  if (ap_setup_done > 0) {
    // initial setup was done
    configTime(settings.data.timezone, settings.data.ntpserver);
    time(&now);                       // read the current time
    localtime_r(&now, &tm);           // update the structure tm with the current time
    strftime(hhmm,6,"%H:%M",&tm);     // format time (HH:MM)

    Debug("-> Time sync to ");
    DebugLn(ctime(&now));

    if (update_wifi != "") {
      delay(500);
      setupSTA();             // connect to Wifi
    }

#ifdef W_MQTT
    if (String(settings.data.mqttbroker) != brokerprev 
        || settings.data.mqttport != portprev 
        || String(settings.data.mqtttopic) != topicprev
        || String(settings.data.mqttuser) != userprev 
        || String(settings.data.mqttpassword) != passwprev) {
      // MQTT broker or port has changed. restart mqtt connection
      mqtt_disconnect();
      mqtt_init();
    }
#endif
  }

  httpUpdateResponse = "The configuration was updated.";
  ap_setup_done = 1;
  server.sendHeader("Location", "/setup");
  server.send(302, "text/plain", "Moved");
  settings.Save();

}
