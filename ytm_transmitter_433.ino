// {"inTemp": 4.48, "outTemp": 3.47, "forHeater": 2.98}
#include "DHT.h"
#include <VirtualWire.h>

#define led_pin  13
#define DHTPIN 2     
#define DHTTYPE DHT22   
#define transmit_pin 12

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
    Serial.begin(9600);
    vw_set_tx_pin(transmit_pin);
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);       // Bits per sec
    
    pinMode(led_pin, OUTPUT);
}

void loop()
{
  char msg[7];
  float t = dht.readTemperature();
  String transmitStr =  "h" + String(t);
  Serial.println(transmitStr);
  if(transmitStr != ""){
    for(byte m = 0; m < 8; m++) msg[m] = transmitStr.charAt(m);
  }
  digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
  vw_send((uint8_t *)msg, 7);
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(led_pin, LOW);
  delay(5000);
}

