  #include <Adafruit_Sensor.h>  // 1.1.14
#include <Adafruit_SI1145.h>  // 1.2.2

Adafruit_SI1145 uv_sensor = Adafruit_SI1145();

bool        si1145connected=false;
float       UVraw=0;
int         UVindex=0;
int         Visint=0;
int         IRint=0;
float       luxML=0;

#define NUM_SAMPLES_ML 50

/**************************************************************************/
/*     ML1145 setup function 
/**************************************************************************/
void ML1145setup(void) 
{

  /* Initialise the sensor */
  // SI1145 (UV), SDA=4 (D2), SCL=5(D1)
  if (! uv_sensor.begin()) {
    DebugLn("Didn't find Si1145");
    si1145connected=false;
  }
  else {
    si1145connected=true;
  }
  DebugLn("-> SI1145 begin");


}

// #define NO_COVER
#define ACRYLIC_GLASS_CLEAR

#ifdef NO_COVER
# define   VIS_COEFF     5.41    // No cover 
# define   IR_COEFF      -0.08   // No cover
# define   GAIN   1              // gain correction, empirically calculated
#endif
#ifdef ACRYLIC_GLASS_CLEAR
# define   VIS_COEFF     6.33     // Acrilic Glass Clear 
# define   IR_COEFF      -0.098   // Acrilic Glass Clear
# define   GAIN   0.96            // gain correction, Acrilic Glass Clear
#endif

///////////////////////////// ML1145/////////////////////////////////////////////
//
void getML1145data() {
// see https://www.silabs.com/documents/public/application-notes/AN523.pdf for LUX formula and cover coefficients
//     https://www.silabs.com/documents/public/data-sheets/Si1145-46-47.pdf for SI1145 datasheet
#define   VIS_DARK_READ 260
#define   IR_DARK_READ  252

int IRrunv=0;
int Visrunv=0;
int UVrun=0;
bool minsChange=false;

  if (!si1145connected) return;  // return if SI1145 not found

  for(int i = 0 ; i < NUM_SAMPLES_ML; i++){
    IRrunv += uv_sensor.readIR();
    Visrunv += uv_sensor.readVisible();
    UVrun += uv_sensor.readUV();
  }

  UVraw = UVrun / NUM_SAMPLES_ML / 100.0 ;
  DebugLn("UV raw: " + String(UVraw));
  UVindex = round(UVraw * settings.data.uvadjust);
  DebugLn("UV index: " + String(UVindex));

  IRint = IRrunv / NUM_SAMPLES_ML;
  if (IRint < settings.data.minIR) {
    settings.data.minIR = IRint;
    minsChange = true;
  }

  Visint = Visrunv / NUM_SAMPLES_ML;
  if (Visint < settings.data.minVis) {
    settings.data.minVis = Visint;
    minsChange = true;
  }

  // save settings if either Visible or IR dark values changes
  if (minsChange) {
    settings.Save();
    Debug("New Dark values, IR: ");
    Debug(settings.data.minIR);
    Debug(", Visible: ");
    DebugLn(settings.data.minVis);
  }
  
  luxML = ( (Visint - settings.data.minVis) * VIS_COEFF + (IRint - settings.data.minIR) * IR_COEFF ) * GAIN ;
  DebugLn("ML1145 data read !");

  DebugLn("IR int: " + String(IRint));
  DebugLn("Visible: " + String(Visint));
  DebugLn("luxML: " + String(luxML));
  
}

String ml1145Json()
{

  String jsonMsg =  String("\"ml1145\": ") + 
                      "{ \"UVindex\":" + String(UVindex) +
                      ", \"lux\":" + String(luxML) +
                      ", \"ir\":" + String(IRint) +
                      ", \"visible\":" + String(Visint) +
                      "}";
  
  return jsonMsg;

}

void sendML1145data()
{

#ifdef  W_MQTT
  mqttSendSensorData(ml1145Json());
#endif
}

