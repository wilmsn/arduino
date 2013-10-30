#ifndef _WETTERSTATION_H
#define _WETTERSTATION_H

// Radiochannel
#define RADIOCHANNEL 90

// Nodenames
#define MASTER_NODE 0
#define SENSOR_NODE_1 1
#define SENSOR_NODE_2 2
#define SENSOR_NODE_3 3
#define SENSOR_NODE_4 4
#define SENSOR_NODE_5 5
#define SENSOR_NODE_6 6

// Sensors
#define SENSOR001 1001
#define SENSOR002 1002
#define SENSOR003 1003
#define SENSOR004 1004
#define SENSOR005 1005
#define SENSOR006 1006
#define SENSOR007 1007
#define SENSOR008 1008
#define SENSOR009 1009
#define SENSOR010 1010
#define SENSOR011 1011
#define SENSOR012 1012

// Some more Colors
#define VGA_GRAY1		0b0100010000001000
#define VGA_GRAY2		0b0010000010000010

extern uint8_t SmallFont[];
extern uint8_t BigFont[];

enum Screen_t {Screen_Main, Screen_SetClock, Screen_SetDate, Screen_TempDetails, Screen_PresureDetails, Screen_HydrDetails};


struct payload_t
{
  unsigned long sensor;
  float value;
};


#endif
