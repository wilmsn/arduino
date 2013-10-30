/*####################################################################





####################################################################*/
#include <DS1307.h>
#include <UTFT.h>
#include "wetterstation.h"

extern prog_uint16_t arrow_down_70_40[];



/*******************************************************
Die Klasse DI stellt ein generisches DisplayItem bereit.
********************************************************/
class DI{
  private:
  protected:
    uint16_t BorderColor1;
    uint16_t BorderColor2;
    boolean DrawBorder;
    boolean DrawBackground;
    int x0;
    int y0;
    int dx;
    int dy; 
    UTFT *_UTFT;
    
  public:
    uint16_t Text1_Color;
    uint16_t Text2_Color;
    uint16_t Text3_Color;
    uint16_t Text4_Color;
    uint16_t Text5_Color;
    uint16_t BackgroundColor; 
    char *Text1;
    int Text1_Size, Text1_X, Text1_Y ;
    boolean Text1_Show;
    char *Text2;
    int Text2_Size, Text2_X, Text2_Y ;
    boolean Text2_Show;
    char *Text3;
    int Text3_Size, Text3_X, Text3_Y ;
    boolean Text3_Show;
    char *Text4;
    int Text4_Size, Text4_X, Text4_Y ;
    boolean Text4_Show;
    char *Text5;
    int Text5_Size, Text5_X, Text5_Y ;
    boolean Text5_Show;
    char *ActionText;
    DI(int startx, int starty, int deltax, int deltay, UTFT *ptrUTFT);
    void Draw(void); 
    boolean IsTouched(int x, int y);
    void TouchAction(void);
    void TouchCancel(void);
    void init(void);
    void Refresh(void);
};

/*********************************************************************************
Die Klasse BMP stellt ein BitmapItem als Kind von DI bereit.
**********************************************************************************/
class DIBMP : public DI{
  private:
  public:
    prog_uint16_t *Bitmap;
    void Draw(void);
    DIBMP(int startx, int starty, int deltax, int deltay, UTFT *ptrUTFT);
};

/*********************************************************************************
Die Klasse BMP stellt ein BitmapItem als Kind von DI bereit.
**********************************************************************************/
class DIAR : public DI{
  private:
  int dir;
  public:
    void Draw(void);
    DIAR(int startx, int starty, int _dir, UTFT *ptrUTFT);
};

/*********************************************************************************
Die Klasse DIINT stellt ein BitmapItem als Kind von DI bereit.
**********************************************************************************/
class DIINT : public DI{
  private:
    unsigned long Changed_LastTime;
  public:
    int Value;
    int maxValue;
    int minValue;
    
    void Draw(void);
    boolean Inc(void);
    boolean Dec(void);
    DIINT(int startx, int starty, int, int, UTFT *ptrUTFT);
};

/*********************************************************************************
Die Klasse DICL stellt eine Uhr als Kind von DI bereit.
**********************************************************************************/
class DICL : public DI{
  private:
  void drawMark(int h);
  void drawSec(int s);
  void drawMin(int m);
  void drawHour(int h, int m);
  void drawZeiger(int m, int l, uint16_t col);
  int clockCenterX;
  int clockCenterY;
  int sec;
  int min;
  int hour;
  Time  t;
  int oldsec;
  boolean RefreshAll;
  public:
  uint16_t StundenZeigerFarbe;
  uint16_t SekundenZeigerFarbe;
  uint16_t MinutenZeigerFarbe;
  uint16_t ZifferBlattFarbe;
  char *Label;
  DICL(int startx, int starty, int deltax, int deltay, UTFT *ptrUTFT);
  void Draw(void);
  void Refresh(void);
  void newValue(int h, int m, int s);
  void init(void);
};

/*********************************************************************************
Die Klasse DIDA stellt eine Kalenderblatt(Datum) als Kind von DI bereit.
**********************************************************************************/
class DIDA : public DI{
  private:
  int year;
  char* date;
  int day;
  char* dow;
  public:
  DIDA(int startx, int starty, int deltax, int deltay, UTFT *ptrUTFT);
  void Draw(void);
  void newValue(char* da, char* dw);
};

/*********************************************************************************
Die Klasse DIMW stellt ein generisches MesswertDisplayItem als Kind von DI bereit.
**********************************************************************************/
class DIMW : public DI{
  private:
    float Value;
    boolean Value_IsOld;
    unsigned long Value_LastTime;
    boolean Value_Valid;
  public:
    boolean Value_IsNew;
    byte Dezimals;
    unsigned int EEPROM_Base;   
    float StoreOffset;
    float StoreFactor;
    uint16_t TextDefaultColor;
    uint16_t TextErrorColor;
    int Value_X, Value_Y;
    DIMW(int startx, int starty, int deltax, int deltay, UTFT *ptrUTFT);
    void Draw(void);
    void DrawGraph(void);
    void StoreData(void);
    void StoreAggData(void);
    void NewValue(float);
    float GetValue(void);
    void Refresh(void);
};

/*********************************************************************************
Die Klasse DIBAT stellt ein generisches Batterie-DisplayItem als Kind von DI bereit.
**********************************************************************************/
class DIBAT : public DI{
  private:
    float Value;
    float Value_yel;
    float Value_red;
    boolean Value_IsOld;
    unsigned long Value_LastTime;
    boolean Value_Valid;
  public:
    boolean Value_IsNew;
    boolean Value_is_yel;
    boolean Value_is_red;
    DIBAT(int startx, int starty, int deltax, int deltay, UTFT *ptrUTFT);
    void Draw(void);
    void NewValue(float);
    void Refresh(void);
};

