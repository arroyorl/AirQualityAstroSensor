/*************************************************
                  Annemometer
*************************************************/
#define ANNEMOMETER_PIN GPIO13 // D7

float       windspeed;
float       maxwindspeed=0.0;
float       sumwindspeed=0.0;
float       windspeedNow;
float       maxwindspeedNow=0.0;
int         numsampleswind=0;

long        lastSecondW;

long          lastWindCheck = 0;
volatile long lastWindIRQ = 0;
volatile byte windClicks = 0;

#ifdef  W_MQTT
// function prototype
void mqtt_send(String subtopic, String message, bool retain);
#endif


//Interrupt routines (these are called by the hardware interrupts, not by the main code)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void IRAM_ATTR wspeedIRQ()
// Activated by the magnet in the anemometer (2 ticks per rotation)
{
    if (millis() - lastWindIRQ > 15) // Ignore switch-bounce glitches less than 15ms (99.5 MPH max reading) after the reed switch closes
    {
        lastWindIRQ = millis(); //Grab the current time
        windClicks++; //There is 1.492MPH for each click per second.
    }
}


//Returns the instataneous wind speed
float get_wind_speed()
{
    float deltaTime = millis() - lastWindCheck; // elapsed time

    deltaTime /= 1000.0; //Convert to seconds

    float WSpeed = (float)windClicks / deltaTime; // clics/sec

    windClicks = 0; //Reset and start watching for new wind
    lastWindCheck = millis();

    // DebugLn("winclics/sec: " + String(WSpeed));

    WSpeed *= 2.4; // 2.4 Km/h per click/sec.

    return(WSpeed);
}

void setupWind(){

  //*********** Annemometer initialization *************
  pinMode(ANNEMOMETER_PIN, FUNCTION_3);
  pinMode(ANNEMOMETER_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ANNEMOMETER_PIN), wspeedIRQ, FALLING);
  // reset value and counters
  sumwindspeed = 0.0;
  numsampleswind = 0;
  maxwindspeed = 0.0;

  lastSecondW = millis();

}

String windJson()
{

  String jsonMsg =  String("\"wind\": ") + 
                        "{ \"windspeed\":" + String(windspeedNow) + 
                        ", \"maxwindspeed\":" + String(maxwindspeedNow) + 
                      "}";
  
  return jsonMsg;

}

void sendWindData()
{

#ifdef  W_MQTT
  mqttSendSensorData(windJson());
#endif
}