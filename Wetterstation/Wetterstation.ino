/*********************************************************************
#####################################################################
*/

#include <UTFT.h>
#include <UTouch.h>
#include <SPI.h>
#include <EEPROM.h>
#include <DS1307.h>
#include <RF24Network.h>
#include <RF24.h>
#include "wetterstation.h"
#include "DI.h"

Screen_t Screen=Screen_Main;
boolean Startup;


UTFT    display(ITDB32S,38,39,40,41);  
UTouch  myTouch(6,5,4,3,2);

// Init the DS1307
DS1307 rtc(A0, A1);

// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(A7,53);

// Network uses that radio
RF24Network network(radio);



DI DITI1(0,0,320,20, &display);
DICL DICL1(0,21,160,160, &display);
DIDA DIDA1(10,181,140,50, &display);
DIMW DIMW1(160,31,159,50, &display);
DIMW DIMW2(160,101,159,50, &display);
DIMW DIMW3(160,171,129,50, &display);
DIBAT DIBAT1(305,3,8,12, &display);
DI DI_OK(199,200,120,39, &display);
DI DI_Cancel(0,200,120,39, &display);
//DIBMP DI_Up(20,20,71,40, &display);
//DIAR DIAR1(100,20,1, &display);
//DIAR DIAR2(100,100,2, &display);
// Set Time
DIAR SetH_up(50,40,1,&display);
DIAR SetH_down(50,140,2,&display);
DIINT SetH(47,90,0,23, &display);
DIAR SetM_up(120,40,1,&display);
DIAR SetM_down(120,140,2,&display);
DIINT SetM(117,90,0,59, &display);
DIAR SetS_up(190,40,1,&display);
DIAR SetS_down(190,140,2,&display);
DIINT SetS(187,90,0,59, &display);
// Set Date
DIAR SetDay_up(50,40,1,&display);
DIAR SetDay_down(50,140,2,&display);
DIINT SetDay(47,90,1,31, &display);
DIAR SetMon_up(120,40,1,&display);
DIAR SetMon_down(120,140,2,&display);
DIINT SetMon(117,90,1,12, &display);
DIAR SetYear_up(195,40,1,&display);
DIAR SetYear_down(195,140,2,&display);
DIINT SetYear(180,90,2000,2059, &display);

byte lasthour=-1, lastmin=-1, lastday=-1, lastsec=-1;
Time now;

void DrawWetterstation(){
  Screen=Screen_Main;
  display.clrScr();
  display.fillScr(VGA_BLACK);
  DITI1.Draw();
  DICL1.Draw();
  DIDA1.Draw();
  DIMW1.Draw();
  DIMW2.Draw();
  DIMW3.Draw();  
  DIBAT1.Draw();    
  DICL1.Refresh();
}

void DrawSetClock(){
  Screen=Screen_SetClock;
  display.clrScr();    
  display.fillScr(VGA_BLACK);
  SetH.Value=now.hour;
  SetH_up.Draw();
  SetH_down.Draw();
  SetH.Draw();
  SetM.Value=now.min;
  SetM_up.Draw();
  SetM_down.Draw();
  SetM.Draw();
  SetS.Value=now.sec;
  SetS_up.Draw();
  SetS_down.Draw();
  SetS.Draw();
  DI_OK.Draw();
  DI_Cancel.Draw();
}

void DrawSetDate() {
  Screen=Screen_SetDate;
  display.clrScr();    
  display.fillScr(VGA_BLACK);
  SetDay.Value=now.date;
  SetDay_up.Draw();
  SetDay_down.Draw();
  SetDay.Draw();
  SetMon.Value=now.mon;
  SetMon_up.Draw();
  SetMon_down.Draw();
  SetMon.Draw();
  SetYear.Value=now.year;
  SetYear_up.Draw();
  SetYear_down.Draw();
  SetYear.Draw();  
  DI_OK.Draw();
  DI_Cancel.Draw();
}
  
void DrawTempDetails(){
  Screen=Screen_TempDetails;
  display.clrScr();
  
  DI_OK.Draw();
}
void DrawPresureDetails(){
  Screen=Screen_PresureDetails;
  display.clrScr();
  
  DI_OK.Draw();
}
void DrawHydrDetails(){
  Screen=Screen_HydrDetails;
  display.clrScr();
  
  DI_OK.Draw();
}

void setup()   {
  Screen=Screen_Main;
  Serial.begin(9600);
  radio.begin();
  network.begin(RADIOCHANNEL, MASTER_NODE);
  display.InitLCD();
  display.setFont(BigFont);
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_HI);
  Startup = true;
  radio.setDataRate(RF24_250KBPS);
  DITI1.Text1_X=30;
  DITI1.Text1_Y=2;
  DITI1.Text1="Wetterstation V8";
  DITI1.Text1_Size=2;
  DITI1.Text1_Show=true;

  DIMW1.Dezimals=1;
  DIMW1.Text1=("Temperatur");
  DIMW1.Text1_Color=VGA_BLUE;
  DIMW1.Text1_Size=1;
  DIMW1.Text1_X=5;
  DIMW1.Text1_Y=5;
  DIMW1.Text1_Show=true;
  DIMW1.Text2=("C");
  DIMW1.Text2_Color=VGA_BLUE;
  DIMW1.Text2_Size=1;
  DIMW1.Text2_X=100;
  DIMW1.Text2_Y=35;
  DIMW1.Text2_Show=true;
  DIMW1.Text3=("----");
  DIMW1.Text3_Color=VGA_RED;
  DIMW1.Text3_Size=2;
  DIMW1.Text3_X=10;
  DIMW1.Text3_Y=30;
  DIMW1.Text3_Show=true;
  DIMW1.EEPROM_Base = 0x0000;
  DIMW1.ActionText="Temperatur";
  DIMW1.init();

  DIMW2.Text1=("Luftdruck");
  DIMW2.Text1_Color=VGA_BLUE;
  DIMW2.Text1_Size=1;
  DIMW2.Text1_X=5;
  DIMW2.Text1_Y=5;
  DIMW2.Text1_Show=true;
  DIMW2.Text2=("hPa");
  DIMW2.Text2_Color=VGA_BLUE;
  DIMW2.Text2_Size=1;
  DIMW2.Text2_X=100;
  DIMW2.Text2_Y=35;
  DIMW2.Text2_Show=true;
  DIMW2.Text3=("----");
  DIMW2.Text3_Color=VGA_RED;
  DIMW2.Text3_Size=2;
  DIMW2.Text3_X=10;
  DIMW2.Text3_Y=30;
  DIMW2.Text3_Show=true;
  DIMW2.EEPROM_Base = 0x0100;
  DIMW2.ActionText="Luftdruck";
  DIMW2.init();
  
  DIMW3.Dezimals=3;
  DIMW3.Text1=("Spannung");
  DIMW3.Text1_Color=VGA_BLUE;
  DIMW3.Text1_Size=1;
  DIMW3.Text1_X=5;
  DIMW3.Text1_Y=5;
  DIMW3.Text1_Show=true;
  DIMW3.Text2=("V");
  DIMW3.Text2_Color=VGA_BLUE;
  DIMW3.Text2_Size=1;
  DIMW3.Text2_X=100;
  DIMW3.Text2_Y=35;
  DIMW3.Text2_Show=true;
  DIMW3.Text3=("----");
  DIMW3.Text3_Color=VGA_RED;
  DIMW3.Text3_Size=2;
  DIMW3.Text3_X=10;
  DIMW3.Text3_Y=30;
  DIMW3.Text3_Show=true;
  DIMW3.EEPROM_Base = 0x0200;
  DIMW3.ActionText="Feuchte";
  DIMW3.init();

  now = rtc.getTime();

  DICL1.Text1_Size=1;
  DICL1.Text2_Size=1;
  DICL1.Text3_Size=1;
  DICL1.Text4_Size=1;
  DICL1.Text1_Color=VGA_GREEN;
  DICL1.Text2_Color=VGA_RED;
  DICL1.Text3_Color=VGA_YELLOW;
  DICL1.Text4_Color=VGA_AQUA;
  DICL1.Text1="XII";
  DICL1.Text2="III";
  DICL1.Text3="VI";
  DICL1.Text4="IX";
  DICL1.init();
  
  DI_OK.Text1="OK";
  DI_OK.init();
  DI_Cancel.Text1="Cancel";
  DI_Cancel.init();

  //DIDA1.newValue(now.year, rtc.getDateStr(FORMAT_SHORT), now.date, rtc.getDOWStr(FORMAT_SHORT));
  DIDA1.newValue(rtc.getDateStr(FORMAT_SHORT), rtc.getDOWStr(FORMAT_LONG));
//  DIMW3.NewValue(-70.7);
  

  Serial.println("Radio eingeschaltet");
  if (radio.isPVariant()) Serial.println("Radio is a nRF24L01+ (or compatible)"); 
  if (radio.getDataRate() == RF24_250KBPS) Serial.println("Speed 250 KBPS"); 
  if (radio.getDataRate() == RF24_1MBPS) Serial.println("Speed 1 MBPS"); 
  if (radio.getDataRate() == RF24_2MBPS) Serial.println("Speed 2 MBPS"); 
  if (radio.getPALevel() == RF24_PA_MIN) Serial.println("PA Level: MIN(-18 dBM)");
  if (radio.getPALevel() == RF24_PA_LOW) Serial.println("PA Level: LOW(-12 dBM)");
  if (radio.getPALevel() == RF24_PA_HIGH) Serial.println("PA Level: MED(-6 dBM)");
  if (radio.getPALevel() == RF24_PA_MAX) Serial.println("PA Level: HIGH(0 dBM)");
  if (radio.testCarrier()) Serial.println("Carrier detected on Channel");
  if (radio.testRPD()) Serial.println("Signal => -64dBm detected on Channel");
/*  // Channel Scanner
  for (uint8_t channel=0; channel < 128; channel++){
    radio.setChannel(channel);
    Serial.print("Result for channel ");
    Serial.print(channel);
    Serial.print(": ");
    if (radio.testCarrier()) Serial.print("Carrier detected ");
    if (radio.testRPD()) Serial.println("Signal => -64dBm detected");
    Serial.println();
  } */  
  DrawWetterstation();
}


void loop() {
/*  Generation of new values */
  now = rtc.getTime();
  if (lastsec != now.sec) {
    DICL1.newValue(now.hour, now.min, now.sec);
  }
  if (lastday != now.date) {
    DIDA1.newValue(rtc.getDateStr(FORMAT_SHORT), rtc.getDOWStr(FORMAT_LONG));
  }
  
  network.update();

  // Is there anything ready for us?
  while ( network.available() )
  {
    // If so, grab it and print it out
    RF24NetworkHeader header;
    payload_t payload;
    network.read(header,&payload,sizeof(payload));
    Serial.print("Information von Sensor #");
    Serial.print(payload.sensor);
    Serial.print(" Wert: ");
    Serial.println(payload.value);
    switch (payload.sensor) {
    case SENSOR001:
      DIMW1.NewValue(payload.value);
      break;
    case SENSOR002:  
      DIMW2.NewValue(pow(((95*0.0065)/(DIMW1.GetValue()+273.15)+1),5.257)* payload.value/100.0);
      break;
    case SENSOR003:
      DIMW3.NewValue(payload.value/320);
      DIBAT1.NewValue(payload.value/320);
      break;
    case SENSOR004:
//     DISW2.value=payload.value;
//      DISW2.DrawText();
     break;
    }
  }
/* Store data */
  if (lasthour != now.hour){
    DIMW1.StoreData();
    DIMW2.StoreData();
    DIMW3.StoreData();    
  }
  if (lastday != now.date){
    DIMW1.StoreAggData();
    DIMW2.StoreAggData();
    DIMW3.StoreAggData();    
  }

/* Call Refresh */
  switch (Screen) {
    case Screen_Main:
//       if (DIMW1.Value_IsNew) { DIMW1.Refresh(); Serial.println("Temperatur angezeigt"); }
//       if (DIMW2.Value_IsNew) DIMW2.Draw();
//       if (DIMW3.Value_IsNew) DIMW3.Refresh();
       if (lastsec != now.sec) {
         DICL1.Refresh();
       }
       if (lastmin != now.min) {
         DIMW1.Refresh();
         DIMW2.Refresh();
         DIMW3.Refresh();
       }
       if (lastday != now.date) {
           DIDA1.Refresh();
       }
       break;
    case Screen_SetClock:
      
      
      
          break;  
    case Screen_TempDetails: 
      
          break;  
    case Screen_PresureDetails:
      
          break;  
    case Screen_HydrDetails:  
      
          break;  
  }

    if (myTouch.dataAvailable())
    {
      myTouch.read();
      switch (Screen) {
        case Screen_Main:
          if (DICL1.IsTouched(myTouch.getX(),myTouch.getY()))   DrawSetClock();
          if (DIDA1.IsTouched(myTouch.getX(),myTouch.getY()))   DrawSetDate();
          if (DIMW1.IsTouched(myTouch.getX(),myTouch.getY()))   DrawTempDetails();
          if (DIMW2.IsTouched(myTouch.getX(),myTouch.getY()))   DrawPresureDetails();
          if (DIMW3.IsTouched(myTouch.getX(),myTouch.getY()))   DrawHydrDetails();
          break;
        case Screen_SetClock:
          if (SetH_up.IsTouched(myTouch.getX(),myTouch.getY())) {    SetH.Inc(); SetH.Draw(); }
          if (SetH_down.IsTouched(myTouch.getX(),myTouch.getY())) {  SetH.Dec(); SetH.Draw(); }
          if (SetM_up.IsTouched(myTouch.getX(),myTouch.getY())) {    SetM.Inc(); SetM.Draw(); }
          if (SetM_down.IsTouched(myTouch.getX(),myTouch.getY())) {  SetM.Dec(); SetM.Draw(); }
          if (SetS_up.IsTouched(myTouch.getX(),myTouch.getY())) {    SetS.Inc(); SetS.Draw(); }
          if (SetS_down.IsTouched(myTouch.getX(),myTouch.getY())) {  SetS.Dec(); SetS.Draw(); }
          if (DI_OK.IsTouched(myTouch.getX(),myTouch.getY())) {
            rtc.setTime(SetH.Value,SetM.Value,SetS.Value);
            now = rtc.getTime();
            DICL1.newValue(now.hour, now.min, now.sec);
            DrawWetterstation();  
          }
          if (DI_Cancel.IsTouched(myTouch.getX(),myTouch.getY()))  DrawWetterstation();  
          break;  
        case Screen_SetDate:
          if (SetDay_up.IsTouched(myTouch.getX(),myTouch.getY())) {    SetDay.Inc(); SetDay.Draw(); }
          if (SetDay_down.IsTouched(myTouch.getX(),myTouch.getY())) {  SetDay.Dec(); SetDay.Draw(); }
          if (SetMon_up.IsTouched(myTouch.getX(),myTouch.getY())) {    SetMon.Inc(); SetMon.Draw(); }
          if (SetMon_down.IsTouched(myTouch.getX(),myTouch.getY())) {  SetMon.Dec(); SetMon.Draw(); }
          if (SetYear_up.IsTouched(myTouch.getX(),myTouch.getY())) {    SetYear.Inc(); SetYear.Draw(); }
          if (SetYear_down.IsTouched(myTouch.getX(),myTouch.getY())) {  SetYear.Dec(); SetYear.Draw(); }
          if (DI_OK.IsTouched(myTouch.getX(),myTouch.getY())) {
            rtc.setDate(SetDay.Value,SetMon.Value,SetYear.Value);
            now = rtc.getTime();
            DIDA1.newValue(rtc.getMonthStr(FORMAT_LONG), rtc.getDOWStr(FORMAT_LONG));
            DrawWetterstation();  
          }
          if (DI_Cancel.IsTouched(myTouch.getX(),myTouch.getY()))  DrawWetterstation();  
          break;  
        case Screen_TempDetails: 
          if (DI_OK.IsTouched(myTouch.getX(),myTouch.getY())) {
             DrawWetterstation();  
          }
          break;  
        case Screen_PresureDetails:
          if (DI_OK.IsTouched(myTouch.getX(),myTouch.getY())) {
             DrawWetterstation();  
          }
      
          break;  
        case Screen_HydrDetails:  
          if (DI_OK.IsTouched(myTouch.getX(),myTouch.getY())) {
             DrawWetterstation();  
          }
      
          break;  
        default:
          DrawWetterstation();  
      }
    }

  lastsec = now.sec;
  lastmin = now.min;
  lasthour = now.hour;
  lastday = now.date;
  Startup = false;  
} 

    
