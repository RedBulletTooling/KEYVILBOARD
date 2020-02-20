/* Use this file if you want to configure the USB Host. Upload and open serial port. 
 * If the USB Host has been reset, you need to change the baudrate to 9600 instead of 115200.
 * If you want to get the available commands, type HELP in the serial monitor.
 * 
 * WARNING: ONLY make changes to the USB Host if you know what you're doing.
 * 
 * Standard settings that we perform;
 * KEYBOARD 0
 * MODE 6
 * BAUDRATE 115200
*/
void setup()
{
  Serial1.begin(115200);
  Serial.begin(115200);
}

void loop()
{
  while(Serial1.available()){
    Serial.write((uint8_t)Serial1.read());
  }

  if(Serial.available()){
    while(Serial.available()){
      Serial1.write((uint8_t)Serial.read());
    }
  }
}
