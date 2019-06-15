
#include "utils.h"
#include "globals.h"
#include <Arduino.h>

// Flush buffer of the SMS serial
void SMSSerialFlush(){
  while(SMSSERIAL.available() > 0){
    //int byte = SMSSERIAL.read();
    SMSSERIAL.readString();
#ifdef DEBUG
    //Serial.print((char)byte);
#endif
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
      Serial.println(F("Timeout reading response"));
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
  SMSSERIAL.println(F("AT+CMEE=1")); //enable debug
  readResponse();
  Serial.println(F("--")); 
  
  SMSSERIAL.println(F("AT")); //Once the handshake test is successful, it will back to OK
  readResponse();
  Serial.println(F("--"));
  
  SMSSERIAL.println(F("AT+CSQ")); //Signal quality test, value range is 0-31 , 31 is the best
  readResponse();
  Serial.println(F("--"));
  
  SMSSERIAL.println(F("AT+CCID")); //Read SIM information to confirm whether the SIM is plugged
  readResponse();
  Serial.println(F("--"));

  //read stored sms
  SMSSERIAL.println(F("AT+CMGL=\"ALL\"")); 
  readResponse();
  Serial.println(F("--"));

  //check card is properly inserted
  SMSSERIAL.println(F("AT+CPIN?")); 
  String res = readResponse();

#ifdef DEBUG
  if(res.indexOf(F("CPIN: READY")) <= 0){
    Serial.println(F("The SIM card is not ready to be used. Check you inserted id correctly"));
  }
#endif
}


// Returns 1 if SMS was sucessfully sent or 0 if not
void sendSMSMessage(String txt){
  //return;
#ifdef DEBUGWITHOUTSIM
  Serial.print(F("Sending SMS: "));
  Serial.println(txt);
  return;
#endif
  SMSSERIAL.println(F("AT+CMGF=1"));
  delay(2);

  SMSSERIAL.println("AT+CMGS=\"" LEAK_PHONE_NUMBER "\""); //phone number with land code 
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
  String sol = data.substring(strIndex[0], strIndex[1]);
  sol.replace("\n", "");
  sol.replace("\r", "");
  return found>index ? sol : "";
}
