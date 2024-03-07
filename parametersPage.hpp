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

const char PARAM_FIRST_page[] PROGMEM = R"=====(
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
<form method="post" action="/parametersform">
<table style="width:700px">
<tr><td colspan=3 class=heading>General parameters</td></tr>
<tr><td>Pooling Interval:</td><td><input type=text name="poolint" value="@@POOLINT@@"> <small>sec (10 - 3600)</small></td><td></td></tr>
)=====";

const char PARAM_BME280_page[] PROGMEM = R"=====(
<tr><td>Altitude:</td><td><input type=text name="altitude" value="@@ALTITUDE@@"><small> m (0 - 8000)</small></td><td></td></tr>
<tr><td>Temp. adj.:</td><td><input type=text name="tempadjust" value="@@TEMPADJUST@@"><small> &#186;C (-5 - +5)</small></td><td></td></tr>
)=====";

const char PARAM_WIND_page[] PROGMEM = R"=====(
<tr><td>Num Samples WindSpeed:</td><td><input type=text name="numsampleswind" value="@@NUMSAMPLESWIND@@"><small> 5 secs. each (=@@WINDINTERVAL@@ secs.)</small></td><td></td></tr>
)=====";

const char PARAM_UV_page[] PROGMEM = R"=====(
<tr><td>UV adjust:</td><td><input type=text name="uvadjust" value="@@UVADJUST@@"> </td><td></td></tr>
)=====";

const char PARAM_GY906_page[] PROGMEM = R"=====(
<table style="width:700px">
<tr><td colspan=4 class=heading>Sky temperature parameters</td></tr>
<tr><td>Clear sky temp.:</td>
  <td colspan=2><input type=text size=6 name="cloudytemp" value="@@CLOUDYTEMP@@">
      <small>&nbsp;&nbsp;<label align="right" style="width:50px; display:inline-block; background:@@COLSKYTEMP@@"> @@SKYTEMP@@ </label>&nbsp;&#186;C </small></td></tr>
<tr><td>K1 <input type=text size="6" name="k1" value="@@K1@@"></td>
    <td>K2 <input type=text size="6" name="k2" value="@@K2@@"></td>
    <td>K3 <input type=text size="6" name="k3" value="@@K3@@"></td>
	<td>K4 <input type=text size="6" name="k4" value="@@K5@@"></td>
</tr>
<tr><td>K5 <input type=text size="6" name="k5" value="@@K4@@"></td>
    <td>K6 <input type=text size="6" name="k6" value="@@K6@@"></td>
 	<td>K7 <input type=text size="6" name="k7" value="@@K7@@"></td>
    <td></td>
</tr>
</table>
)=====";


const char PARAM_SAFE_page[] PROGMEM = R"=====(
<table style="width:700px">
<tr><td colspan=3 class=heading>Safe status parameters</td></tr>
<tr><td style="width:150px">Humidity:</td>
  <td  style="width:120px">&lt;&nbsp;<input type=text size=10 name="humiditythreshold" value="@@HUMIDITYTHRESHOLD@@"> </td>
  <td><small>&nbsp;<label align="right" style="width:70px; display:inline-block; background:@@COLHUMID@@"> @@HUMIDITY@@ </label>&nbsp;% </small></td></tr>
<tr><td>Wind: </td>
  <td>&lt;&nbsp;<input type=text size=10 name="windthreshold" value="@@WINDTHRESHOLD@@"> </td>
  <td><small>&nbsp;<label align="right" style="width:70px; display:inline-block; background:@@COLWIND@@"> @@MAXWINDSPEED@@ </label>&nbsp;km/h </small></td></tr>
<tr><td>Rain threshold:</td>
  <td>
  	<select id="rainabove" name="rainabove">
      <option value=">" @@SELABOVE@@>&gt;</option>
      <option value="<" @@SELBELOW@@>&lt;</option>
  	</select>
    &nbsp;<input type=text size=6 name="rainthreshold" value="@@RAINTHRESHOLD@@"> </td>
  <td><small>&nbsp;<label align="right" style="width:70px; display:inline-block; background:@@COLRAIN@@"> @@RAINA@@ </label></small></td></tr>

<tr><td>Cloud index:</td>
  <td>&lt;&nbsp;<input type=text size=10 name="cloudthreshold" value="@@CLOUDTHRESHOLD@@"> </td>
  <td><small>&nbsp;<label align="right" style="width:70px; display:inline-block; background:@@COLCLOUD@@"> @@CLOUDINDEX@@ </label>&nbsp;%</small></td></tr>
<tr><td>SQM:</td>
  <td>&gt;&nbsp;<input type=text size=10 name="sqmthreshold" value="@@SQMTHRESHOLD@@"> </td>
  <td><small>&nbsp;<label align="right" style="width:70px; display:inline-block; background:@@COLSQM@@"> @@SQMMPSAS@@ </label>&nbsp;mpsas</small></td></tr>
<tr><td>Light:</td>
  <td>&lt;&nbsp;<input type=text size=10 name="luxthreshold" value="@@LUXTHRESHOLD@@"> </td>
  <td><small>&nbsp;<label align="right" style="width:70px; display:inline-block; background:@@COLLUX@@"> @@LUX@@ </label>&nbsp;lux</small></td></tr>
</table>
)=====";


const char PARAM_WU_page[] PROGMEM = R"=====(
<table style="width:700px">
<tr><td colspan=3 class=heading>Wunderground access</td></tr>
<tr><td>Station ID:</td><td><input type=text size=35 name="stationid" value="@@STATIONID@@"></td></tr>
<tr><td>Station Key:</td><td><input type=text size=35 name="stationkey" value="@@STATIONKEY@@"></td></tr>
)=====";

const char PARAM_TS_page[] PROGMEM = R"=====(
<tr><td colspan=3 class=heading>ThingSpeak Setup</td></tr>
<tr><td>ThingSpeak channel:</td><td><input type=text size=35 name="tschannel" value="@@TSCHANNEL@@"></td></tr>
<tr><td>ThingSpeak API key:</td><td><input type=text size=35 name="tskey" value="@@TSKEY@@"></td></tr>
)=====";

const char PARAM_LAST_page[] PROGMEM = R"=====(
</table>
<p/>
<input type="submit" value="Update">
</form>
<div class="update">@@UPDATERESPONSE@@</div>
</html>
)=====";

void handleParameters() {
  DebugLn("handleParameters");

 String s = FPSTR(PARAM_FIRST_page);
#ifdef  W_BME280
  s += FPSTR(PARAM_BME280_page);
#endif
#ifdef  W_WIND
  s += FPSTR(PARAM_WIND_page);
#endif
#if defined(W_ML1145) || defined(W_VEML6075)
  s += FPSTR(PARAM_UV_page);
#endif
#ifdef  W_GY906
  s += FPSTR(PARAM_GY906_page);
#endif

  s += FPSTR(PARAM_SAFE_page);

#ifdef  W_WU
  s += FPSTR(PARAM_WU_page);
#endif
#ifdef  W_TS
  s += FPSTR(PARAM_TS_page);
#endif

  s += FPSTR(PARAM_LAST_page);

  s.replace("@@PNAME@@", PNAME);
  s.replace("@@VERSION@@",FVERSION);

  // main parameters
  s.replace("@@POOLINT@@",String(settings.data.poolinterval));
  s.replace("@@ALTITUDE@@",String(settings.data.altitude,1));
  s.replace("@@TEMPADJUST@@",String(settings.data.tempadjust));
  s.replace("@@NUMSAMPLESWIND@@",String(settings.data.numsampleswind));
  s.replace("@@WINDINTERVAL@@",String(settings.data.numsampleswind * 5));
  s.replace("@@UVADJUST@@",String(settings.data.uvadjust,4));

#define COLRED    "#FF4500"   // orange red
#define COLGREEN  "#7FFF00"   //Chartreuse (ligth green)

#ifdef  W_GY906
  s.replace("@@CLOUDYTEMP@@",String(settings.data.cloudytemp,1));
  s.replace("@@SKYTEMP@@",String(skyTemp,1));
  if (skyTemp < settings.data.cloudytemp)
    s.replace("@@COLSKYTEMP@@", COLGREEN);
  else
    s.replace("@@COLSKYTEMP@@", COLRED);
  s.replace("@@K1@@",String(settings.data.k1,2));
  s.replace("@@K2@@",String(settings.data.k2,2));
  s.replace("@@K3@@",String(settings.data.k3,2));
  s.replace("@@K4@@",String(settings.data.k4,2));
  s.replace("@@K5@@",String(settings.data.k5,2));
  s.replace("@@K6@@",String(settings.data.k6,2));
  s.replace("@@K7@@",String(settings.data.k7,2));
#endif

  // unsafe status settings
  s.replace("@@HUMIDITYTHRESHOLD@@",String(settings.data.humiditythreshold,1));
  s.replace("@@WINDTHRESHOLD@@",String(settings.data.windthreshold,1));
  s.replace("@@RAINTHRESHOLD@@",String(settings.data.rainthreshold));
  if (settings.data.rainabove) {
    s.replace("@@SELABOVE@@", "selected");
    s.replace("@@SELBELOW@@", "");
  }
  else {
    s.replace("@@SELABOVE@@", "");
    s.replace("@@SELBELOW@@", "selected");
  }
  s.replace("@@CLOUDTHRESHOLD@@",String(settings.data.cloudthreshold));
  s.replace("@@SQMTHRESHOLD@@",String(settings.data.sqmthreshold));
  s.replace("@@LUXTHRESHOLD@@",String(settings.data.luxthreshold));

  // unsafe parameters actual data
  s.replace("@@HUMIDITY@@",String(humidity,1));
  if (humidity < settings.data.humiditythreshold)
    s.replace("@@COLHUMID@@", COLGREEN);
  else
    s.replace("@@COLHUMID@@", COLRED);
  s.replace("@@MAXWINDSPEED@@",String(maxwindspeed,1));
  if (maxwindspeed < settings.data.windthreshold)
    s.replace("@@COLWIND@@", COLGREEN);
  else
    s.replace("@@COLWIND@@", COLRED);
  s.replace("@@RAINA@@",String(rainA));
  if (settings.data.rainabove) {
    if (rainA < settings.data.rainthreshold)
      s.replace("@@COLRAIN@@", COLGREEN);
    else
      s.replace("@@COLRAIN@@", COLRED);
  }
  else {
    if (rainA > settings.data.rainthreshold)
      s.replace("@@COLRAIN@@", COLGREEN);
    else
      s.replace("@@COLRAIN@@", COLRED);
  }
  s.replace("@@CLOUDINDEX@@",String(cloudI,1));
  if (cloudI < settings.data.cloudthreshold)
    s.replace("@@COLCLOUD@@", COLGREEN);
  else
    s.replace("@@COLCLOUD@@", COLRED);

  s.replace("@@SQMMPSAS@@",String(sqmMpsas,1));
  if (sqmMpsas > settings.data.sqmthreshold)
    s.replace("@@COLSQM@@", COLGREEN);
  else
    s.replace("@@COLSQM@@", COLRED);
  s.replace("@@LUX@@",String(sqmLux,1));
  if (sqmLux < settings.data.luxthreshold)
    s.replace("@@COLLUX@@", COLGREEN);
  else
    s.replace("@@COLLUX@@", COLRED);

#ifdef W_WU
  // Wunderground parameters
  s.replace("@@STATIONID@@",settings.data.stationID);
  s.replace("@@STATIONKEY@@",settings.data.stationKey);
#endif
#ifdef W_TS
  // ThinkSpeak parameters
  s.replace("@@TSCHANNEL@@",String(settings.data.ThingSpeakChannel));
  s.replace("@@TSKEY@@",settings.data.ThingSpeakKey);
#endif

  s.replace("@@UPDATERESPONSE@@", httpUpdateResponse);
  
  time(&now);                       // read the current time
  localtime_r(&now, &tm);           // update the structure tm with the current time
  strftime(hhmm,6,"%H:%M",&tm);     // format time (HH:MM)
  s.replace("@@TIMENOW@@",String(hhmm));

  httpUpdateResponse = "";
  server.send(200, "text/html", s);
}

void handleParametersForm() {
String aux;

  DebugLn("handleParametersForm");

  String pint = server.arg("poolint");
  if (pint.length()) {
    settings.data.poolinterval=constrain(pint.toInt(),10,3600);
  }
  String altid = server.arg("altitude");
  if (altid.length()) {
    settings.data.altitude = altid.toFloat();
    settings.data.altitude=max(float(0),settings.data.altitude);
    settings.data.altitude=min(settings.data.altitude,float(8000));
  }
  String tadj = server.arg("tempadjust");
  if (tadj.length()) {
    settings.data.tempadjust = tadj.toFloat();
    settings.data.tempadjust=max(float(-5),settings.data.tempadjust);
    settings.data.tempadjust=min(settings.data.tempadjust,float(5));
  }
  aux = server.arg("numsampleswind");
  if (aux.length()) {
    settings.data.numsampleswind = aux.toInt();
  }
  aux = server.arg("uvadjust");
  if (aux.length()) {
    settings.data.uvadjust = aux.toFloat();
  }

  String cloudyT = server.arg("cloudytemp");
  if (cloudyT.length()) {
    settings.data.cloudytemp = cloudyT.toFloat();
    settings.data.cloudytemp=max(float(-50),settings.data.cloudytemp);
    settings.data.cloudytemp=min(settings.data.cloudytemp,float(20));
  }
  aux = server.arg("k1");
  if (aux.length()) {
    settings.data.k1 = aux.toFloat();
  }
  aux = server.arg("k2");
  if (aux.length()) {
    settings.data.k2 = aux.toFloat();
  }
  aux = server.arg("k3");
  if (aux.length()) {
    settings.data.k3 = aux.toFloat();
  }
  aux = server.arg("k4");
  if (aux.length()) {
    settings.data.k4 = aux.toFloat();
  }
  aux = server.arg("k5");
  if (aux.length()) {
    settings.data.k5 = aux.toFloat();
  }
  aux = server.arg("k6");
  if (aux.length()) {
    settings.data.k6 = aux.toFloat();
  }
  aux = server.arg("k7");
  if (aux.length()) {
    settings.data.k7 = aux.toFloat();
  }

  aux = server.arg("humiditythreshold");
  if (aux.length()) {
    settings.data.humiditythreshold= aux.toFloat();
  }
  aux = server.arg("windthreshold");
  if (aux.length()) {
    settings.data.windthreshold= aux.toFloat();
  }

  aux = server.arg("rainthreshold");
  if (aux.length()) {
    settings.data.rainthreshold= constrain(aux.toInt(),0,1023);
  }
  aux = server.arg("rainabove");
  if (aux == ">") { 
    settings.data.rainabove = true;
  }
  else {
    settings.data.rainabove = false;
  }

  aux = server.arg("cloudthreshold");
  if (aux.length()) {
    settings.data.cloudthreshold= aux.toFloat();
  }
  aux = server.arg("sqmthreshold");
  if (aux.length()) {
    settings.data.sqmthreshold= aux.toFloat();
  }
  aux = server.arg("luxthreshold");
  if (aux.length()) {
    settings.data.luxthreshold= aux.toFloat();
  }

#ifdef W_WU
  // WeatherUnderground settings
  aux = server.arg("stationid");
  aux.toCharArray(settings.data.stationID,128);
  aux = server.arg("stationkey");
  aux.toCharArray(settings.data.stationKey,128);
#endif

#ifdef  W_TS
  // ThingSpeak settings
  aux = server.arg("tschannel");
  if (aux.length()) {
    settings.data.ThingSpeakChannel=aux.toInt();
  }
  else {
    settings.data.ThingSpeakChannel=0;
  }
  aux = server.arg("tskey");
  aux.toCharArray(settings.data.ThingSpeakKey,30);
#endif

  httpUpdateResponse = "The configuration was updated.";
  server.sendHeader("Location", "/parameters");
  server.send(302, "text/plain", "Moved");
  settings.Save();

}
