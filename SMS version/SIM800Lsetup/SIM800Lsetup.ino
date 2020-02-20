#include <SoftwareSerial.h>

SoftwareSerial SMSSERIAL(8, 9);

void setup() {
  Serial.begin(115200);
  SMSSERIAL.begin(57700);
}

void loop() {
  if (SMSSERIAL.available()) {
    Serial.write(SMSSERIAL.read());
  }
  
  if (Serial.available()) {
    SMSSERIAL.write(Serial.read());
  }
}

//Commands (every new line is new enter - the slashes are to be erased before entering);
//AT+IPR=57600
//AT+CMGF=1
//AT+CNMI=2,2,0,0,0
//AT&W
