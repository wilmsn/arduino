/*
Testsender: 
Sendet fortlaufend jede Sekunde ein Signal: Die aktuell gemessene Batteriespannung.
Zweck: Prüfung von Übertragungsqualität und Standorten 
 */
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include "wetterstation.h"


// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(2,1);

// Network uses that radio
RF24Network network(radio);
RF24NetworkHeader header(/*to node*/ MASTER_NODE);
payload_t payload;

#define LED 8
#define VMESS_OUT 0
#define VMESS_IN A3
int i=0;
int vmess=0;

void setup(void)
{
  pinMode(LED, OUTPUT);     
  pinMode(VMESS_OUT, OUTPUT);     
  digitalWrite(LED, HIGH);  
  digitalWrite(VMESS_OUT, HIGH);  
  analogReference(INTERNAL);
  pinMode(VMESS_IN, INPUT);
  SPI.begin();
  radio.begin();
  network.begin(RADIOCHANNEL, SENSOR_NODE_1);
  radio.setDataRate(RF24_250KBPS);
}

void senddata(unsigned long sensor, float value) {
  radio.powerUp();
  network.update();
  payload.sensor = sensor;
  payload.value = value;
  network.write(header,&payload,sizeof(payload));
  radio.powerDown();
}

void loop(void)
{
  digitalWrite(VMESS_OUT, HIGH);
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(100);
  vmess=analogRead(VMESS_IN);
  vmess=vmess+analogRead(VMESS_IN);
  vmess=vmess+analogRead(VMESS_IN);
  vmess=vmess+analogRead(VMESS_IN);
  vmess=vmess+analogRead(VMESS_IN);
  senddata(SENSOR999, vmess/5.0);
  digitalWrite(VMESS_OUT, LOW);
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(900);
}
