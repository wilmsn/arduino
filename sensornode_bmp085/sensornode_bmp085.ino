/*
 */
#include <Wire.h>
#include <BMP085.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include "LowPower.h"
#include "wetterstation.h"

/* BMP085 Luftdrucksensor
   Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect 5V to 5V DC
   Connect GROUND to common ground
*/
BMP085 bmp;


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
  Wire.begin();
  bmp.begin();
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
  senddata(SENSOR001, bmp.readTemperature());
  senddata(SENSOR002, bmp.readPressure());
  digitalWrite(VMESS_OUT, HIGH);
  LowPower.idle(SLEEP_500MS, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF);
  vmess=analogRead(VMESS_IN);
  vmess=vmess+analogRead(VMESS_IN);
  vmess=vmess+analogRead(VMESS_IN);
  vmess=vmess+analogRead(VMESS_IN);
  vmess=vmess+analogRead(VMESS_IN);
  digitalWrite(VMESS_OUT, LOW);
  senddata(SENSOR003, vmess/5.0);
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  LowPower.idle(SLEEP_120MS, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF);
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  
}
