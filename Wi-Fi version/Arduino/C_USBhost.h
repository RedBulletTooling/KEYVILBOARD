#ifndef C_USBhost_H
#define C_USBhost_H

#include <Keyboard.h>
#include <Arduino.h>

class C_USBhost
{
public:
    C_USBhost(HardwareSerial& serial);
    C_USBhost(HardwareSerial& serial, bool debug_state);
    
    byte GetKey();
    void Begin(unsigned long baud_rate);
    void SetMode(char mode);
    void SetBaudRate(char* baud_rate);
    static void ReleaseAllButtons(char* reason);   

private:
  HardwareSerial& serial;
   
  char hidText[27];
  byte hbc = 0;                                                     // hidText string buffer count 
  byte rawHID[8];                                                   // modifier_bit_map, manufacturer(ignore) , key1, key2, key3, key4, key5, key6
  byte prevRawHID[8];
  byte fullBufferFlag_preventHold;
  byte collectedAsciiValues[6];

  byte IgnoreBytesIfUseless(byte index);                             
  void ConvertInputToBytes(char* input, byte* raw_bytes);                              
  void Send_Report(byte* bytes);         
  void SaveTheKeys();       
  byte HID_to_ASCII(byte key, bool shiftDown, bool windowsDown, bool altDown, bool ctrlDown);                     
  bool WasAnyKeyPressed();      
  bool WasKeyPreviouslyPressed(byte key);                              
  bool WasModifierPressed();                               
  void GetKeysPressed(byte* keys_pressed);                                    
  bool WasShiftDown();
  bool WasWindowsDown();
  bool WasAltDown();
  bool WasCtrlDown();                                     
  bool IsBitHigh(byte byteToConvert, byte bitToReturn);              
  void CleanUpVars();                                      
  void FullBuffer_BugPrevention();                         
};




#endif



