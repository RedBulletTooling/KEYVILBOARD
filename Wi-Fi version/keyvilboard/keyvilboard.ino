#include <NeoSWSerial.h> // https://github.com/SlashDevin/NeoSWSerial
#include "C_USBhost.h"
#include <Keyboard.h>

#define BAUD_RATE 9600
#define BAUD_RATE_USB_HOST_BOARD 115200

NeoSWSerial Esp8266(8, 9);
C_USBhost USBhost = C_USBhost(Serial1, /*debug_state*/false);

String bufferStr = "";
String last = "";
char letter;
char buffalo[315];
int x = 0;
int z = 0;

uint8_t defaultDelay = 0;

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

void create_command(char command, int y) {
  Serial.println(x);
  Serial.println(y);
  buffalo[x] += command;
  x++;

  if(y == 1){
    Serial.print(buffalo);
    for (int x=0; x < sizeof(buffalo); x++){
      bufferStr += buffalo[x];
      Serial.println(bufferStr);
    }
    memset(buffalo, 0, sizeof(buffalo));
    z = 0;
    x = 0;
  }

  if(bufferStr.length() > 0){

    bufferStr.replace("<","");
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
    Serial.println("done");
  }
  
}

void setup() {
  Esp8266.begin(BAUD_RATE);
  Esp8266.setTimeout(1000);
  Serial.begin(BAUD_RATE);
  USBhost.Begin(BAUD_RATE_USB_HOST_BOARD);                    // begin serial communication with USB host board in order to receive key bytes from the keyboard connected to it
  USBhost.SetMode('6');
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
    letter = Esp8266.read();
    Serial.write(letter);
    if(letter == '<') {
      z = 1;
    }
    create_command(letter, z);
  }
  
}
