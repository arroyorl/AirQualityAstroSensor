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

const char ROOT_page[] PROGMEM = R"=====(
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

<body>
<p style="font-family:'Courier New'">
This the "@@PNAME@@" module<br>
<small>Firmware: @@VERSION@@</small> <br>
<br>
Usage:<br>
<small>
&nbsp&nbsp&nbsp&nbsp&nbsp      <a href="/setup">http://@@IPADDRESS@@/setup</a> for communications setup <br>
&nbsp&nbsp&nbsp&nbsp&nbsp      <a href="/parameters">http://@@IPADDRESS@@/parameters</a> for sensor parameters <br>
&nbsp&nbsp&nbsp&nbsp&nbsp      <a href="/data">http://@@IPADDRESS@@/data</a> for raw data in JSON format <br>
&nbsp&nbsp&nbsp&nbsp&nbsp      <a href="/boltwood">http://@@IPADDRESS@@/boltwood</a> returns weather data in Boltwood II format (if Boltwood module is included)<br>
</small>
</p>
</body>

</html>
)=====";

void handleRoot() {
  DebugLn("handleRoot");

 String s = FPSTR(ROOT_page);

  s.replace("@@PNAME@@", PNAME);
  s.replace("@@VERSION@@",FVERSION);
  s.replace("@@IPADDRESS@@", ipaddress);
  
  time(&now);                       // read the current time
  localtime_r(&now, &tm);           // update the structure tm with the current time
  strftime(hhmm,6,"%H:%M",&tm);     // format time (HH:MM)
  s.replace("@@TIMENOW@@",String(hhmm));

  httpUpdateResponse = "";
  server.send(200, "text/html", s);
}

