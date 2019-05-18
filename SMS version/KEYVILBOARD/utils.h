
#include <Arduino.h>

void SMSSerialFlush();
String readResponse();
void collectDebugInfo();
void sendSMSMessage(String txt);
String getValue(String data, String separator, int index);
