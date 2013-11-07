


#include "DI.h"
#include <UTFT.h>
#include "wetterstation.h"
#include <EEPROM.h>


DI::DI(int startx, int starty, int deltax, int deltay, UTFT *ptrUTFT){
  _UTFT = ptrUTFT;
  x0=startx;
  y0=starty;
  dx=deltax;
  dy=deltay;
  BorderColor1=VGA_GRAY1;
  BorderColor2=VGA_GRAY2;
  BackgroundColor=VGA_GRAY; 
  DrawBorder=true;
  DrawBackground=true;
  TextDefaultColor=VGA_BLUE;
  Text1="----";
  Text1_X=10; 
  Text1_Y=10;
  Text1_Show=true;
  Text1_Color=TextDefaultColor;
  Text2="";
  Text2_Size=1; 
  Text2_X=-1; 
  Text2_Y=-1;
  Text2_Color=TextDefaultColor;
  Text2_Show=false;
  Text3="";
  Text3_Size=1; 
  Text3_X=-1; 
  Text3_Y=-1;
  Text3_Color=TextDefaultColor;
  Text3_Show=false;
  Text4="";
  Text4_Size=1; 
  Text4_X=-1; 
  Text4_Y=-1;
  Text4_Color=TextDefaultColor;
  Text4_Show=false;
  Text5="";
  Text5_Size=1; 
  Text5_X=-1; 
  Text5_Y=-1;
  Text5_Color=TextDefaultColor;
  Text5_Show=false;
  ActionText="";
}

void DI::init() {
  if ( Text1_Show ) {
    if (Text1_X == -1 && Text1_Y == -1){
      if (dx>(16*strlen(Text1)+10) && dy>20) Text1_Size=2; else Text1_Size=1; 
    }
    if (Text1_X == -1) Text1_X=(dx/2)-(4*Text1_Size*strlen(Text1)); 
    if (Text1_Y == -1) Text1_Y=(dy/2)-(4*Text1_Size);
  }
}

boolean DI::IsTouched(int x, int y){
  return ((x>x0) && (x<(x0+dx)) && (y>y0) && (y<(y0+dy))); 
}

void DI::Draw(){
  if (DrawBackground) {
    if (DrawBorder) {
      _UTFT->setColor(BorderColor1);
      _UTFT->drawRoundRect(x0,y0,x0+dx,y0+dy);
      _UTFT->setColor(BorderColor1);
      _UTFT->drawRoundRect(x0+1,y0+1,x0+dx-1,y0+dy-1);
      _UTFT->setColor(BackgroundColor);
      _UTFT->fillRoundRect(x0+2,y0+2,x0+dx-2,y0+dy-2);    
    } else {
      _UTFT->setColor(BackgroundColor);
      _UTFT->fillRoundRect(x0,y0,x0+dx,y0+dy);    
    }
  }
  Refresh();
}

void DI::Refresh(){
  if (Text1_Show) {
    if (Text1_Size==1) _UTFT->setFont(SmallFont); else _UTFT->setFont(BigFont);
    _UTFT->setColor(Text1_Color);
    _UTFT->setBackColor(BackgroundColor);
    _UTFT->print(Text1,x0+Text1_X,y0+Text1_Y);  
  }
  if (Text2_Show) {
    if (Text2_Size==1) _UTFT->setFont(SmallFont); else  _UTFT->setFont(BigFont);
    _UTFT->setColor(Text2_Color);
    _UTFT->setBackColor(BackgroundColor);
    _UTFT->print(Text2,x0+Text2_X,y0+Text2_Y);  
  }
  if (Text3_Show) {
    if (Text3_Size==1) _UTFT->setFont(SmallFont); else  _UTFT->setFont(BigFont);
    _UTFT->setColor(Text3_Color);
    _UTFT->setBackColor(BackgroundColor);
    _UTFT->print(Text3,x0+Text3_X,y0+Text3_Y);  
  }
  if (Text4_Show) {
    if (Text4_Size==1) _UTFT->setFont(SmallFont); else  _UTFT->setFont(BigFont);
    _UTFT->setColor(Text4_Color);
    _UTFT->setBackColor(BackgroundColor);
    _UTFT->print(Text4,x0+Text4_X,y0+Text4_Y);  
  }
  if (Text5_Show) {
    if (Text5_Size==1) _UTFT->setFont(SmallFont); else  _UTFT->setFont(BigFont);
    _UTFT->setColor(Text5_Color);
    _UTFT->setBackColor(BackgroundColor);
    _UTFT->print(Text5,x0+Text5_X,y0+Text5_Y);  
  }
}
///////////  Ende Klasse DI ///////////////


DIBMP::DIBMP(int startx, int starty, int deltax, int deltay, UTFT *ptrUTFT) : DI(startx, starty, deltax, deltay, ptrUTFT){
  Bitmap=arrow_down_70_40;
}

void DIBMP::Draw(){
  _UTFT->drawBitmap(x0,y0,dx,dy,Bitmap);
}


///////////  Ende Klasse DIBMP ///////////////


DIINT::DIINT(int startx, int starty, int min_v, int max_v, UTFT *ptrUTFT) : DI(startx, starty, 32, 125, ptrUTFT){
  DIAR up(startx, starty, 1, ptrUTFT);
  DIAR down(startx, starty+100, 2, ptrUTFT);
  minValue=min_v;
  maxValue=max_v;
  Value=5;
  Changed_LastTime =0;
  if (max_v < 10) dx=20; else if (max_v < 100) dx=40; else if (max_v < 1000) dx=55; else if (max_v < 10000) dx=70; else dx=85;  
  dy=20; 
}

void DIINT::Draw(){
   _UTFT->setColor(BackgroundColor);
   _UTFT->setBackColor(BackgroundColor);
   _UTFT->fillRect(x0, y0, x0+dx, y0+dy);
   _UTFT->setColor(Text1_Color);
   _UTFT->setFont(BigFont);
   if (Value < 10) 
     _UTFT->printNumI(Value,x0+dx/2-8,y0+2);
   else if (Value < 100)   
     _UTFT->printNumI(Value,x0+dx/2-2*8,y0+2);
   else if (Value < 1000)   
     _UTFT->printNumI(Value,x0+dx/2-3*8,y0+2);
   else if (Value < 10000)   
     _UTFT->printNumI(Value,x0+dx/2-4*8,y0+2);
   else _UTFT->printNumI(Value,x0+dx/2-5*8,y0+2);
}

boolean DIINT::Inc(){
  if (Changed_LastTime < millis()-500) {
    if (Value < maxValue) {
      Value++;
      Changed_LastTime = millis();
      return true;
    } else return false;
  }
}

boolean DIINT::Dec(){
  if (Changed_LastTime < millis()-500) {
    if (Value > minValue) {
      Value--;
      Changed_LastTime = millis();
      return true;
    } else return false;
  }
}


///////////  Ende Klasse DIBMP ///////////////


DIAR::DIAR(int startx, int starty, int _dir, UTFT *ptrUTFT) : DI(startx, starty, 32, 25, ptrUTFT){
  dir=_dir;
}

void DIAR::Draw(){
  if (dir == 1) {
    _UTFT->setColor(64, 64, 128);
    _UTFT->fillRoundRect(x0, y0, x0+32, y0+25);
    _UTFT->setColor(255, 255, 255);
    _UTFT->drawRoundRect(x0, y0, x0+32, y0+25);
    _UTFT->setColor(128, 128, 255);
    for (int i=0; i<15; i++)
      _UTFT->drawLine(x0+6+(i/1.5), y0+20-i, x0+27-(i/1.5), y0+20-i);
  } 
  if (dir == 2) {
    _UTFT->setColor(64, 64, 128);
    _UTFT->fillRoundRect(x0, y0, x0+32, y0+25);
    _UTFT->setColor(255, 255, 255);
    _UTFT->drawRoundRect(x0, y0, x0+32, y0+25);
    _UTFT->setColor(128, 128, 255);
    for (int i=0; i<15; i++)
      _UTFT->drawLine(x0+6+(i/1.5), y0+5+i, x0+27-(i/1.5), y0+5+i);
  }
}


///////////  Ende Klasse DIBMP ///////////////

DICL::DICL(int startx, int starty, int deltax, int deltay, UTFT *ptrUTFT) : DI(startx, starty, deltax, deltay, ptrUTFT){
  DrawBorder=false;
  Text1="12";
  Text2="3";
  Text3="6";
  Text4="9";
  clockCenterX=deltax/2;
  clockCenterY=deltay/2;
  StundenZeigerFarbe=VGA_RED;
  MinutenZeigerFarbe=VGA_GREEN;
  SekundenZeigerFarbe=VGA_LIME;
  ZifferBlattFarbe=VGA_BLUE;
  BackgroundColor=VGA_BLACK;
  Text1_Show=true;
  Text2_Show=true;
  Text3_Show=true;
  Text4_Show=true;
  RefreshAll=true;
  hour=-1;
  init();
  old_sec=0;
  old_min=0;
  old_hour=0;
}

void DICL::init(){
  Text1_X=clockCenterX-strlen(Text1)*4;
  Text1_Y=clockCenterY-dy/2+12;
  Text2_X=clockCenterX+dx/2-(strlen(Text2)*8)-10;
  Text2_Y=clockCenterY-5;
  Text3_X=clockCenterX-strlen(Text3)*4;
  Text3_Y=clockCenterY+dy/2-22;
  Text4_X=clockCenterX-dx/2+15;
  Text4_Y=clockCenterY-5;
}

void DICL::drawMark(int h)
{
  float x1, y1, x2, y2;
  
  h=h*30;
  h=h+270;
  
  x1=(dx/2-dx/20)*cos(h*0.0175);
  y1=(dy/2-dy/20)*sin(h*0.0175);
  x2=(dx/2-dx/10)*cos(h*0.0175);
  y2=(dy/2-dx/10)*sin(h*0.0175);
  
  _UTFT->drawLine(x1+x0+clockCenterX, y1+y0+clockCenterY, x2+x0+clockCenterX, y2+y0+clockCenterY);
}

void DICL::drawZeiger(int m, int l, uint16_t col) {
  float x1, y1, x2, y2, x3, y3, x4, y4;

  x1=(l)*cos(m*0.0175);
  y1=(l)*sin(m*0.0175);
  x2=5*cos(m*0.0175);
  y2=5*sin(m*0.0175);
  x3=(3*l/4)*cos((m+4)*0.0175);
  y3=(3*l/4)*sin((m+4)*0.0175);
  x4=(3*l/4)*cos((m-4)*0.0175);
  y4=(3*l/4)*sin((m-4)*0.0175);
  
  _UTFT->setColor(col);
  _UTFT->drawLine(x1+x0+clockCenterX, y1+y0+clockCenterY, x3+x0+clockCenterX, y3+y0+clockCenterY);
  _UTFT->drawLine(x3+x0+clockCenterX, y3+y0+clockCenterY, x2+x0+clockCenterX, y2+y0+clockCenterY);
  _UTFT->drawLine(x2+x0+clockCenterX, y2+y0+clockCenterY, x4+x0+clockCenterX, y4+y0+clockCenterY);
  _UTFT->drawLine(x4+x0+clockCenterX, y4+y0+clockCenterY, x1+x0+clockCenterX, y1+y0+clockCenterY);
}

void DICL::drawSec(int s, uint16_t col) {
  int x,y;
  _UTFT->setColor(col);
  s=s*6;
  s=s+270;
  x=(dx/2-6)*cos(s*0.0175);
  y=(dy/2-6)*sin(s*0.0175);
  _UTFT->fillCircle(x0+clockCenterX+x,y0+clockCenterY+y,2); 
}

void DICL::drawMin(int m, uint16_t col)
{
  m=m*6;
  m=m+270;
  drawZeiger(m, dx/2-25, col);
}

void DICL::drawHour(int h, int m, uint16_t col){
  h=(h*30)+(m/2);
  h=h+270;
  drawZeiger(h, 3*(dx/2-25)/4, col);
}

void DICL::Refresh(){
  if (hour > -1) {
    if (sec == 0 || RefreshAll) {
      DI::Refresh();
      drawMin(old_min, BackgroundColor);
      drawHour(old_hour, old_min, BackgroundColor);
      drawMin(min, MinutenZeigerFarbe);
      drawHour(hour, min, StundenZeigerFarbe);  
      RefreshAll=false;
      old_min=min;
      old_hour=hour;
    }
    drawSec(old_sec,ZifferBlattFarbe);
    drawSec(sec, SekundenZeigerFarbe);
  }
  old_sec=sec;
}

void DICL::Draw(){
  DI::Draw();
  _UTFT->setColor(ZifferBlattFarbe);
  for (int i=0; i<5; i++) _UTFT->drawCircle(x0+clockCenterX, y0+clockCenterY, (dx/2)-5-i);
  for (int i=0; i<3; i++) _UTFT->drawCircle(x0+clockCenterX, y0+clockCenterY, i);
  for (int i=0; i<12; i++) if ((i % 3)!=0) drawMark(i);
  RefreshAll=true;
  Refresh();
}

void DICL::newValue(int h, int m, int s){
  sec=s;
  hour=h;
  min=m; 
}

///////////////////////////////// Ende DICL ///////////////////////////////////////

DIDA::DIDA(int startx, int starty, int deltax, int deltay, UTFT *ptrUTFT) : DI(startx, starty, deltax, deltay, ptrUTFT){
  BackgroundColor=VGA_WHITE;
  Text1_Color=VGA_BLACK;
  Text1_Show=false;
}
void DIDA::Draw(){
  _UTFT->setColor(BackgroundColor);
  _UTFT->setBackColor(BackgroundColor);
  _UTFT->fillRoundRect(x0,y0,x0+dx,y0+dy);    
  _UTFT->setColor(Text1_Color);
  for (int i=1; i<8; i++)  {
    int ts=(x0+dx)/8;
    _UTFT->drawLine(x0+(i*ts), y0+1, x0+(i*ts)-1, y0+4);
    _UTFT->drawLine(x0+(i*ts)+1, y0+1, x0+(i*ts), y0+4);
    _UTFT->drawLine(x0+(i*ts)+2, y0+1, x0+(i*ts)+1, y0+4);
  }
  if (dx > 130) _UTFT->setFont(BigFont); else _UTFT->setFont(SmallFont);
/*  
  _UTFT->print(dow, x0+1*dx/20, y0+dy/5);
  if (day<10) _UTFT->printNumI(day, x0+9*dx/20, y0+dy/5);
  else           _UTFT->printNumI(day, x0+8*dx/20, y0+dy/5);
  _UTFT->print(month,x0+13*dx/20, y0+dy/5);
  _UTFT->printNumI(year, x0+dx/3, y0+2*dy/3);
*/
  _UTFT->print(dow, x0+1*dx/20, y0+dy/5);
  _UTFT->print(date, x0+1*dx/20, y0+2*dy/3);
}

//void DIDA::newValue(int y, char* m, int d, char* dw){
//  year=y; month=m; day=d; dow=dw;
void DIDA::newValue(char* da, char* dw){  
  date=da; dow=dw;
}



DIMW::DIMW(int startx, int starty, int deltax, int deltay, UTFT *ptrUTFT) : DI(startx, starty, deltax, deltay, ptrUTFT){
  Dezimals=0;
  Value=-99999;
  Value_Valid = false;
  Value_LastTime=0;
  TextDefaultColor=VGA_BLUE;
  TextErrorColor=VGA_RED;
  Value_X=5;
  Value_Y=30;
  Text1_Size=1;
  Text1_X=5;
  Text1_Y=5;
  Text1_Show=true;
  Text2_Size=1;
  Text2_X=70;
  Text2_Y=35;
  Text2_Show=true;
}

void DIMW::NewValue(float newvalue){
  Value=newvalue;
  Value_LastTime=millis();
  Value_Valid=true;
  Value_IsOld=false;
  Text3_Show=false;
  Value_IsNew=true;
}

float DIMW::GetValue(){
  if (Value_Valid) return Value; else return 0.0;
}

void DIMW::StoreData(){
  byte i;
  for (i=24; i>0; i--) {
    EEPROM.write(EEPROM_Base+i,EEPROM.read(EEPROM_Base+i-1));
  } 
  EEPROM.write(EEPROM_Base,byte((Value-StoreOffset)*StoreFactor));
}

void DIMW::StoreAggData(){
  int i;
  int Val_Min=255, Val_Max=0, Val_Read;
  for (i=255; i>26; i--) {
    EEPROM.write(EEPROM_Base+i,EEPROM.read(EEPROM_Base+i-1));
  } 
  for (byte i=0; i<24; i++){
    Val_Read = EEPROM.read(EEPROM_Base+i);
    if (Val_Read > Val_Max) Val_Max = Val_Read;
    if (Val_Read < Val_Min) Val_Min = Val_Read;
  }
  EEPROM.write(EEPROM_Base+25,Val_Min);
  EEPROM.write(EEPROM_Base+26,Val_Max);
}

void DIMW::DrawGraph(){
//  _UTFT->drawFastVLine(80,y0+2,dy-4);  

}

/*void DIMW::DrawText_Value(){
    if ( valid_value ) _UTFT->print("Value",x0+5,y0+13);
    else _UTFT->print("----",x0+5,y0+dy/2);  */

void DIMW::Refresh(){
  if  (millis() > 50000)  Value_IsOld = (Value_LastTime < (millis() - 50000));
  _UTFT->setFont(BigFont);
  _UTFT->setBackColor(BackgroundColor);
  if (Value_Valid) {
    if (Value_IsOld) _UTFT->setColor(TextErrorColor); else _UTFT->setColor(TextDefaultColor);
    _UTFT->print("    ", x0+Value_X, y0+Value_Y);
    if (! Dezimals) {
      if (Value > 0) {
        if (Value < 10) {
          _UTFT->printNumI(Value, x0+Value_X+48, y0+Value_Y);
        } else {
          if (Value < 100) {
            _UTFT->printNumI(Value, x0+Value_X+32, y0+Value_Y);
          } else {
            if (Value < 1000) {
              _UTFT->printNumI(Value, x0+Value_X+16, y0+Value_Y);
            } else {
              _UTFT->printNumI(Value, x0+Value_X, y0+Value_Y);
            }
          }
        }
      }
    }
    else _UTFT->printNumF(Value, Dezimals, x0+Value_X, y0+Value_Y);
  } else {
    _UTFT->setColor(TextErrorColor);
    _UTFT->print("----", x0+Value_X, y0+Value_Y);
  }
  Value_IsNew=false;
}

void DIMW::Draw(){
  DI::Draw();
  Refresh();
} 
///////////////////////////////// Ende DIMW ///////////////////////////////////////

DIBAT::DIBAT(int startx, int starty, int deltax, int deltay, UTFT *ptrUTFT) : DI(startx, starty, deltax, deltay, ptrUTFT){
  Value=-99999;
  Value_Valid = false;
  Value_LastTime=0;
  DrawBorder=false;
  DrawBackground=false;
  Text1_Show=false;
  Value_yel=2.35;
  Value_red=2.1;
  Value_is_yel=false;
  Value_is_red=false;
}

void DIBAT::Draw(){
  DI::Draw();
  Refresh();
} 

void DIBAT::Refresh(){
  if  (millis() > 50000)  Value_IsOld = (Value_LastTime < (millis() - 50000));
  _UTFT->setBackColor(BackgroundColor);
  if (Value_Valid) {
    if (Value > Value_yel) {      
      _UTFT->setColor(VGA_GREEN);
        Value_is_yel=false;
        Value_is_red=false;
    } else {
      if (Value > Value_red) {      
        _UTFT->setColor(VGA_YELLOW);
        Value_is_yel=true;
        Value_is_red=false;
      } else {
        _UTFT->setColor(VGA_RED);
        Value_is_yel=false;
        Value_is_red=true;
      }
    }      
    _UTFT->fillRect(x0+dx/4, y0, x0+dx-dx/4, y0+dy/4);
    _UTFT->fillRect(x0, y0+dy/4, x0+dx, y0+dy);
  } else {
    _UTFT->setColor(VGA_RED);
    _UTFT->fillRect(x0+dx/4, y0, x0+dx-dx/4, y0+dy/4);
    _UTFT->fillRect(x0, y0+dy/4, x0+dx, y0+dy);
    _UTFT->setColor(VGA_BLACK);
    _UTFT->drawLine(x0, y0+dy-1, x0+dx-1, y0);
    _UTFT->drawLine(x0+1, y0+dy, x0+dx, y0+1);
  }
  Value_IsNew=false;
}

void DIBAT::NewValue(float newvalue){
  Value=newvalue;
  Value_LastTime=millis();
  Value_Valid=true;
  Value_IsOld=false;
  Text3_Show=false;
  Value_IsNew=true;
}
