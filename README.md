# AirQualityAstroSensor
Extension of Air Quality Sensor Extended adding an cloud sensor based on MLX90614, a TSL2591 and a VEML6075.

Sensor uses an a SD011 for air quality, a BME280 for Temperature and humitity measurement, a MLX90614 to measure sky temperature ad calculate cloud coverage, a TSL2591 to get luminosity and calculate sky quality (SQM measured in mpsas), a VEML6075 to get UV index and an anemomether and a rain sensor

All data is transmitted over MQTT.

The MLX90614 sensor reads sky temperature (object temperature) and applies a correction factor factor based on a formula from indi-duino Meteo: [https://github.com/LabAixBidouille/Indi/blob/master/3rdparty/indi-duino/devices/Firmwares/indiduinoMETEO/indiduinoMETEO.ino](https://github.com/LabAixBidouille/Indi/blob/master/3rdparty/indi-duino/devices/Firmwares/indiduinoMETEO/indiduinoMETEO.ino) , also calculates a cloud coverage percentage based on a formula from same program.

Sky quality measure is based on SQM_example by Gabe Shaughnessy on library [https://github.com/gshau/SQM_TSL2591](https://github.com/gshau/SQM_TSL2591)

The program sends an MQTT message for each sensor reading in JSON format:
**{"time": unix_time, "sensor_name": {"sensor_data1": value, "sensor_data2": value, ...}}**

Also send a message for astronomy safe conditions with sensor name "safe" and value 1 or 0, and sends a message in Boltwood II format with the weather data and flags.

> [!NOTE]
> On first run, the program creates an access point with name "ESP-AIRQUAL" creating a web server accessible on http://192.168.4.1 which allow to configure SSID/password, node name, and MQTT configuration. Once configured these parameters, the program connects to the configured WiFi and starts data capturing and submission. This AP can also been activated after a reset if the SETUP_PIN (GPIO 0) is down during a lapse of 5 seconds after reset.

on normal operation, the program presents 4 web pages on the device IP address:
	**http://ip_address/setup** - the setup page mentioned above
	**http://ip_address/parameters** - which allows to configure several parameters of the program
	**http://ip_address/data** - which returns all sensors data in JSON format
	**http://ip_address/boltwood** - returning data in Boltwood II format

## Configurable parameters are:
**Pooling interval:** num of seconds elapsed between sensors data reading and submission
**Altitude:** altitude in meters. it is calculated to calculate sea level pressure
**Temp. Adj.:** temperature offset.
**Num samples WindSpeed:** number of samples to take on wind speed calculation periods of 5 secs. The program calculates the maximum wind speed during the period and average of wind speed and submits the data.

## Weather Underground access parameters:
Station ID and Station Key, see [https://www.wunderground.com/pws/installation-guide](https://www.wunderground.com/pws/installation-guide)

## Thing Speak setup:
see [https://es.mathworks.com/help/thingspeak/users-and-channels.html](https://es.mathworks.com/help/thingspeak/users-and-channels.html)

## Conditional compilation:
depending of which sensors are installed, you may conditional compile the program. In the .ino file there are several defines starting with W_module_name, commenting thse defines, will disable the use of the corresponding sensor. Notice that the corresponding .hpp file is always included, but not used.

> [!NOTE]
> If you comment or delete the #define W_MQTT, no MQTT messages are sent, although it still apperas on **/setup** page

**SEND_BY_MODULE** define: is defined, send an MQTT separate message for each sensor. If not defined, send only one JSON message including all sensors on it. Despite it is defined or not, the rain and wind info is always submitted when captured (30 secs for rain, Num_samples_windspeed * 5 secs for wind)

> [!IMPORTANT]
> check all defines regarding GPIOs for leds, wing gauge and I2C bus.

## Leds behavior:
On start RED and GREEN leds are on for 5 secs. If SETUP_PIN (GPIO 0) is down at end of this period, both leds are off and the program enters in AP mode for configuration, otherwise goes to normal operation mode (STA mode).

Initial setup (AP mode), RED led blinks quickly (200 ms).

During WiFi conecction (setup STA), RED led bilnks slowly (500 ms), after the program is connected to WiFi, RED led is off and GREEN led is on during 3 secs.

On normal operation, both leds are off.
