#include <ThingSpeak.h>  // v 2.0.1

void ThingSpeaksetup(){

  // ********* initialize ThingSpeak *************
    ThingSpeak.begin(espClient);

}

/*************************************************
 * ThingSpeak send function
 * **********************************************/
void ts_send(unsigned long ts_channel, char* ts_key){
int result;

  if ( (ts_channel != 0) && (strlen(ts_key) > 0)) {
    DebugLn("ThingSpeak, loading fields...");
#ifdef W_SDS011
    ThingSpeak.setField(1, pm10f);
    ThingSpeak.setField(2, pm25f);
#endif
    ThingSpeak.setField(6, temperature);
    ThingSpeak.setField(7, humidity);
    ThingSpeak.setField(8, dewpoint);

    result = ThingSpeak.writeFields(ts_channel, ts_key);
    Debug("ThingSpeak write, channel = ");
    Debug(ts_channel);
    Debug(", Key= ");
    Debug(ts_key);
    Debug(", result = ");
    DebugLn(result);
  }
}
