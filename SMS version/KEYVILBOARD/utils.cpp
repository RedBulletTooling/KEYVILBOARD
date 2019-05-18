
#include "utils.h"
#include "globals.h"
#include <Arduino.h>

// Flush buffer of the SMS serial
void SMSSerialFlush(){
  while(SMSSERIAL.available() > 0){
    int byte = SMSSERIAL.read();
    Serial.print((char)byte);
  }
}   

String readResponse(){
  delay(500);
  String res;
  int TIMEOUT = 10 *10; // wait 10 seconds
  int incomingByte  = 0;
  while (!SMSSERIAL.available()) 
  {
    delay(100);
    TIMEOUT--;
    if (TIMEOUT == 0){
#ifdef DEBUG
      Serial.println("Timeout reading response");
#endif
      return "";
    }
  }
  
  while (SMSSERIAL.available()) 
  {
    incomingByte  = SMSSERIAL.read();
    if (incomingByte == -1){ // this should never happen 
#ifdef DEBUG
      Serial.println("There is no data to read from SMSSERIAL");
#endif
      break; 
    }
#ifdef DEBUG
    Serial.write(char(incomingByte));
#endif
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

  //read stored sms
  SMSSERIAL.write("AT+CMGL=\"ALL\"\r\n"); 
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
void sendSMSMessage(String txt){
#ifdef DEBUGWITHOUTSIM
  Serial.println("Sending SMS: " + txt);
  return;
#endif
  //mutex_SMS = true;
  SMSSERIAL.write("AT+CMGF=1\r\n");
  delay(2);

  SMSSERIAL.write("AT+CMGS=\"" LEAK_PHONE_NUMBER "\"\r\n"); //phonenumber with land code 
  delay(2);

  SMSSERIAL.write(txt.c_str());
  delay(2);

  SMSSerialFlush();
  
  SMSSERIAL.write((char)26);
  delay(2);
}

String getValue(String data, String separator, int index){
  int found = 0;
  int strIndex[] = {0, -((int)separator.length())};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.substring(i, i+ separator.length()) == separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+separator.length();
        strIndex[1] = (i == maxIndex) ? i+separator.length() : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
