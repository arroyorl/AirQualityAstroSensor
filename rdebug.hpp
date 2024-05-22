////////
// Library: RemoteDebug2 - debug over WiFi - for Esp8266 (NodeMCU) or ESP32
// 
// This module uses RemoteDebug2 library, which is a fork of not supported (as it seems, last update on May 9, 2019) RemoteDebug library by Joao Lopes. 
// The API is fully compatible. 
//
// Example of use:
//
//        if (Debug.isActive(Debug.<level>)) { // <--- This is very important to reduce overheads and work of debug levels
//            Debug.printf("bla bla bla: %d %s\n", number, str);
//            Debug.println("bla bla bla");
//        }
//
// Or short way (prefered if only one debug at time)
//
//		debugA("This is a any (always showed) - var %d", var);
//
//		debugV("This is a verbose - var %d", var);
//		debugD("This is a debug - var %d", var);
//		debugI("This is a information - var %d", var);
//		debugW("This is a warning - var %d", var);
//		debugE("This is a error - var %d", var);
//
//		debugV("This is println");
//
//
///////
/***************************************************************************************************************
*   v1.0  Initial version
*
*   v2.0  Added Verbose/VerboseLn, Info/InfoLn, Warning/WarningLn and Error/ErrorLn 
*
***************************************************************************************************************/   

////// Defines
#define USE_LIB_WEBSOCKET     // Use standard WebSockets library (https://github.com/Links2004/arduinoWebSockets)

// Host name (please change it)

#ifdef PNAME
# define HOST_NAME PNAME
#else
# define HOST_NAME "rdebug"
#endif


// Remote debug over WiFi - not recommended for production, only for development
#include "RemoteDebug.h"        // https://github.com/karol-brejna-i/RemoteDebug

RemoteDebug RDebug;


#define DEFAULT_SPEED 115200
#define WELCOME "Rdebug: Welcome!"

/////////////////////////////////////////////////////////////
//   DebugSerialStart  - start serial port for debug
/////////////////////////////////////////////////////////////
void DebugSerialStart() {

#if defined(DEBUG) || defined(RDEBUG)
  Serial.begin(DEFAULT_SPEED);
  while (!Serial);
  Serial.println("*** Initialized serial debug ");  
#endif

}

/////////////////////////////////////////////////////////////
//   DebugRemoteStart  - start remote debug
/////////////////////////////////////////////////////////////
void DebugRemoteStart() {
#ifdef  RDEBUG
  if (WiFi.status() == WL_CONNECTED ) {
    RDebug.begin(HOST_NAME); // Initialize the WiFi server
    RDebug.setResetCmdEnabled(true); // Enable the reset command
//    RDebug.showProfiler(true); // Profiler (Good to measure times, to optimize codes)
    RDebug.showColors(true); // Colors
    Serial.println("*** Initialized remote debug ");  
  }
  #endif
}

/////////////////////////////////////////////////////////////
//   Debug
/////////////////////////////////////////////////////////////
void Debug(String s) {

#if defined(DEBUG) || defined(RDEBUG)
  Serial.print(s);
#endif

#ifdef RDEBUG
  if (RDebug.isActive(RDebug.DEBUG)) { 
      RDebug.print(s);
  }
#endif
}

void Debug(int value) {
  Debug(String(value));
}

/////////////////////////////////////////////////////////////
//   DebugLn
/////////////////////////////////////////////////////////////
void DebugLn(String s) {
#if defined(DEBUG) || defined(RDEBUG)
  Serial.println(s);
#endif

#ifdef RDEBUG
  if (RDebug.isActive(RDebug.DEBUG)) { 
      RDebug.println(s);
  }
#endif

}

void DebugLn(int value) {
  DebugLn(String(value));
}

/////////////////////////////////////////////////////////////
//   Verbose
/////////////////////////////////////////////////////////////
void Verbose(String s) {

#if defined(DEBUG) || defined(RDEBUG)
  Serial.print(s);
#endif

#ifdef RDEBUG
  if (RDebug.isActive(RDebug.VERBOSE)) { 
      RDebug.print(s);
  }
#endif
}

void Verbose(int value) {
  Verbose(String(value));
}

/////////////////////////////////////////////////////////////
//   VerboseLn
/////////////////////////////////////////////////////////////
void VerboseLn(String s) {
#if defined(DEBUG) || defined(RDEBUG)
  Serial.println(s);
#endif

#ifdef RDEBUG
  if (RDebug.isActive(RDebug.VERBOSE)) { 
      RDebug.println(s);
  }
#endif

}

void VerboseLn(int value) {
  VerboseLn(String(value));
}

/////////////////////////////////////////////////////////////
//   Info
/////////////////////////////////////////////////////////////
void Info(String s) {

#if defined(DEBUG) || defined(RDEBUG)
  Serial.print(s);
#endif

#ifdef RDEBUG
  if (RDebug.isActive(RDebug.INFO)) { 
      RDebug.print(s);
  }
#endif
}

void Info(int value) {
  Info(String(value));
}

/////////////////////////////////////////////////////////////
//   InfoLn
/////////////////////////////////////////////////////////////
void InfoLn(String s) {
#if defined(DEBUG) || defined(RDEBUG)
  Serial.println(s);
#endif

#ifdef RDEBUG
  if (RDebug.isActive(RDebug.INFO)) { 
      RDebug.println(s);
  }
#endif

}

void InfoLn(int value) {
  InfoLn(String(value));
}

/////////////////////////////////////////////////////////////
//   Warning
/////////////////////////////////////////////////////////////
void Warning(String s) {

#if defined(DEBUG) || defined(RDEBUG)
  Serial.print(s);
#endif

#ifdef RDEBUG
  if (RDebug.isActive(RDebug.WARNING)) { 
      RDebug.print(s);
  }
#endif
}

void Warning(int value) {
  Warning(String(value));
}

/////////////////////////////////////////////////////////////
//   WarningLn
/////////////////////////////////////////////////////////////
void WarningLn(String s) {
#if defined(DEBUG) || defined(RDEBUG)
  Serial.println(s);
#endif

#ifdef RDEBUG
  if (RDebug.isActive(RDebug.WARNING)) { 
      RDebug.println(s);
  }
#endif

}

void WarningLn(int value) {
  WarningLn(String(value));
}


/////////////////////////////////////////////////////////////
//   Error
/////////////////////////////////////////////////////////////
void Error(String s) {

#if defined(DEBUG) || defined(RDEBUG)
  Serial.print(s);
#endif

#ifdef RDEBUG
  if (RDebug.isActive(RDebug.ERROR)) { 
      RDebug.print(s);
  }
#endif
}

void Error(int value) {
  Error(String(value));
}

/////////////////////////////////////////////////////////////
//   ErrorLn
/////////////////////////////////////////////////////////////
void ErrorLn(String s) {
#if defined(DEBUG) || defined(RDEBUG)
  Serial.println(s);
#endif

#ifdef RDEBUG
  if (RDebug.isActive(RDebug.ERROR)) { 
      RDebug.println(s);
  }
#endif

}

void ErrorLn(int value) {
  ErrorLn(String(value));
}


/////////////////////////////////////////////////////////////
//   HandleDebug
/////////////////////////////////////////////////////////////
void HandleDebug() {

#ifdef RDEBUG
  RDebug.handle();
#endif

}