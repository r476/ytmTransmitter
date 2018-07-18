// {"inTemp": 4.48, "outTemp": 3.47, "forHeater": 2.98}
#include <VirtualWire.h>
#include <SoftwareSerial.h>
const int led_pin = 13;
const int transmit_pin = 12;
SoftwareSerial mySerial(8, 9); // RX, TX

void setup()
{
    mySerial.begin(9600);
    Serial.begin(9600);
    // Initialise the IO and ISR
    vw_set_tx_pin(transmit_pin);
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);       // Bits per sec
    pinMode(led_pin, OUTPUT);
}

void loop()
{
  String recieveStr =  "";
  
  while(mySerial.available()) {
    delay(20);
    recieveStr += char(mySerial.read());
  }
  if(recieveStr != ""){
  String transmitStrIn = "i" + recieveStr.substring(11, 16);
  String transmitStrOut = "o" + recieveStr.substring(29, 34);
  String transmitStrHeate = "h" + recieveStr.substring(49, 54);
  Serial.println(transmitStrIn + " " + transmitStrOut + " " + transmitStrHeate);
  for(byte n = 0; n < 3; n++){
    char msg[7];
    switch(n){
      case 0: for(byte m = 0; m < 8; m++) msg[m] = transmitStrIn.charAt(m); break;
      case 1: for(byte m = 0; m < 8; m++) msg[m] = transmitStrOut.charAt(m); break;
      case 2: for(byte m = 0; m < 8; m++) msg[m] = transmitStrHeate.charAt(m); break;
    }
    digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
    vw_send((uint8_t *)msg, 7);
    vw_wait_tx(); // Wait until the whole message is gone
    digitalWrite(led_pin, LOW);
  }
  }
}
