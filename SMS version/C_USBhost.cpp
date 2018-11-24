
#include "C_USBhost.h"

const PROGMEM byte asciiMap[128] = {0,0,0,0,0,0,0,0,42,43,40,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,44,158,180,160,161,162,164,52,166,167,165,174,54,45,55,56,39,30,31,32,33,34,35,36,37,38,179,51,182,46,183,184,159,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,47,49,48,163,173,53,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,175,177,176,181,0};
const PROGMEM byte keyPadMap[16] = {85,87,0,86,99,84,98,89,90,91,92,93,94,95,96,97};
bool debug = false;

C_USBhost::C_USBhost(HardwareSerial& s):
  serial(s) {
  
}

C_USBhost::C_USBhost(HardwareSerial& s, bool debug_state):
  serial(s) {
  debug = debug_state;
}


void C_USBhost::Begin(unsigned long baud_rate){
  serial.begin(baud_rate);
}

void C_USBhost::SetBaudRate(char* baud_rate){
  serial.write("BAUD ");
  serial.write(baud_rate);
  serial.write("\r\n");

  delay(500); 

  char str[65]; byte i;
  while(serial.available()){
    str[i++] = serial.read();
  }

  if(debug){
    Serial.print(F("USB host board, changing baud rate response: ")); Serial.println(str);
  }
  
  if(strstr(str, "Baud Rate Changed")){
    return true;
  }
  return false;
}

void C_USBhost::SetMode(char mode){
  serial.write("MODE ");
  serial.write(mode);
  serial.write("\r\n");
  
  delay(500); 

  char str[65]; byte i;
  while(serial.available()){
    str[i++] = serial.read();
  }

  if(debug){
    Serial.print(F("USB host board, changing mode response: ")); Serial.println(str);
  }
  
  if(strstr(str, "Mode Changed")){
    return true;
  }
  return false;
}


byte C_USBhost::GetKey() {
  // many keys can be pressed at the same time but only 1 value can be returned
  // so there's an array that holds up to 6 keys that were potentially pressed
  // until that array is empty no communication with usbhost will occur
  for(int i=0; i<6; i++){
    if(collectedAsciiValues[i]){
      byte val = collectedAsciiValues[i];
      collectedAsciiValues[i] = 0;
      return val;  
    }
  }
  
  if (serial.available() > 0) {
    if (serial.available() == 63) {
      fullBufferFlag_preventHold = true; //P(F("OCCUPIED BUFFER BYTES (serial.available): ")); PL((int)serial.available());
    }
    hidText[hbc++] = serial.read(); // hbc is hid bytes count (number of bytes that was read already from USBhost which sends something like 00-00-04-00-00-00-00-00a)
    hbc = IgnoreBytesIfUseless(hbc);                                       // decreases hbc if required

    if (hbc == 26) {                                              // if the full string (25 bytes long) representing 8 HID values is received (e.g. "\n\r02-00-04-00-00-00-00-00")
      hidText[hbc] = 0;
      ConvertInputToBytes(hidText, rawHID);
      Send_Report(rawHID);
      SaveTheKeys();
      CleanUpVars();

      FullBuffer_BugPrevention();
    }
  }
  return 0;
}



byte C_USBhost::IgnoreBytesIfUseless(byte index) {
  if ((index == 1 && hidText[index - 1] != 10) || (index == 2 && hidText[index - 1] != 13)) {
    index--; // 2 bytes (10 and 13) are received before the string that includes raw HID info //P(F("hbc_VALUE: ")); P(hbc); P(", Char: "); PL(hidText[hbc-1]);
  }
  return index;
}

void C_USBhost::ConvertInputToBytes(char* input, byte* raw_bytes) {
  for (byte j = 0; j < 8; j++) {
    char buff[3] = {input[j * 2 + j + 2], input[j * 2 + j + 3], 0};  // convert string to 8 HID bytes
    raw_bytes[j] = (byte)strtoul(buff, NULL, 16);
    memset(buff, 0, sizeof(buff));
  }
}

void C_USBhost::Send_Report(byte* bytes) {
  KeyReport kr = {bytes[0], bytes[1], 
    {
      bytes[2], bytes[3], bytes[4], bytes[5], bytes[6], bytes[7]
    }
  };
  HID().SendReport(2, &kr, sizeof(KeyReport)); /*PL(F("Report Sent."));*/
}


void C_USBhost::SaveTheKeys(){
  if(WasAnyKeyPressed()){
    byte keys_pressed[6] = {0};
    GetKeysPressed(keys_pressed);

    for(int i = 0; i < sizeof(keys_pressed); i++)
    {
       byte key = keys_pressed[i];
       if(key){
        if(debug){
          Serial.print(F("\nrawHID string: ")); Serial.println(hidText);                                                                       // debug line
          Serial.print(F("rawHID key detected: hex - ")); Serial.print(key, HEX); Serial.print(F(", int - ")); Serial.println((int)key);       // debug line
        }
        
        byte asciiKey = HID_to_ASCII(key, WasShiftDown());
        
        
        if(debug){Serial.print(F("Ascii key detected: hex - ")); Serial.print(asciiKey, HEX); Serial.print(F(", int - ")); Serial.print((int)asciiKey); Serial.print(F(", char - ")); Serial.println((char)asciiKey);}
        
        if(asciiKey){
          collectedAsciiValues[i] = (char)asciiKey;
        }
        asciiKey = 0;
      }
      key = 0; 
    }
  }
  else if (WasModifierPressed()){} 
  else{
    if(debug){
        Serial.print(F("\nRELEASED_OR_LOST\nrawHID string: ")); Serial.println(hidText);                                                                       // debug line
        //Serial.print(F("rawHID key detected: hex - ")); Serial.print(key, HEX); Serial.print(F(", int - ")); Serial.println((int)key);       // debug line
    }
  } // was released
}

byte C_USBhost::HID_to_ASCII(byte key, bool shiftDown){
  for(byte i=0; i<128; i++){                                                      // asciiMap contains HID values (some modified), their order indicates which ascii value should be used 
    byte b = pgm_read_byte(asciiMap + i);                                         // some HID values are modified - if the HID value from the asciiMap array is over 127 then it means that shift must be used with it 
    if(!(shiftDown == false && b >= 128) && !(shiftDown == true && b < 128))      // so if the user actually was holding shift then take into account only the values over 127 when trying to convert it to ascii
      if(key == (shiftDown ? b ^ 128 : b))                                        // if shift was not pressed then take search through values equal or lower than 127
        return i;
  }   
  for(byte i=0; i < 16; i++)                                    // numpad keys
    if(key == pgm_read_byte(keyPadMap + i)) 
      return i+42;
  return 0; 
}


bool C_USBhost::WasAnyKeyPressed() {
  for (int i = 2; i < 8; i++) {
    if(!WasKeyPreviouslyPressed(rawHID[i])){
      return true;
    }
  } return false;
}

bool C_USBhost::WasKeyPreviouslyPressed(byte key){
  for (int i = 2; i < 8; i++) {
    if(prevRawHID[i] == key){
      return true; 
    }
  } return false;
}

bool C_USBhost::WasModifierPressed() {
  return ((rawHID[0] > 0) && (rawHID[0] != prevRawHID[0]));
}

void C_USBhost::GetKeysPressed(byte* keys_pressed) {
  for (int i = 2; i < 8; i++) {
    if (rawHID[i] > 0 && !WasKeyPreviouslyPressed(rawHID[i])) {
      keys_pressed[i-2] = rawHID[i];
    }
  }
}

bool C_USBhost::WasShiftDown() {
  return (IsBitHigh(rawHID[0], 1) || IsBitHigh(rawHID[0], 5)); // if 2nd or 6th bit (left/right shift)
}

bool C_USBhost::IsBitHigh(byte byteToConvert, byte bitToReturn) {
  byte mask = 1 << bitToReturn;  // thanks to Marc Gravell's https://stackoverflow.com/questions/9804866/return-a-specific-bit-as-boolean-from-a-byte-value
  return (byteToConvert & mask) == mask;
}

void C_USBhost::ReleaseAllButtons(char* reason) {
  KeyReport kr = {0, 0, {0, 0, 0, 0, 0, 0}};
  HID().SendReport(2, &kr, sizeof(KeyReport));
   
  if(debug){Serial.print(F("RELEASING ALL BUTTONS. Reason: ")); Serial.println(reason);}
}

void C_USBhost::CleanUpVars() {
  for (byte j = 0; j < sizeof(rawHID); j++) {
    prevRawHID[j] = rawHID[j];
  } 
  memset(rawHID, 0, sizeof(rawHID));
  memset(hidText, 0, sizeof(hidText));
  hbc = 0;
}

void C_USBhost::FullBuffer_BugPrevention() {
  if (fullBufferFlag_preventHold) {
    ReleaseAllButtons("USBhost buffer was full. Bytes could be lost. Holding bug was possible.");
    if (this->serial.available() < 1) {
      fullBufferFlag_preventHold = false;
    }
  }
}














