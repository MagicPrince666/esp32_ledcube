#include "freertos/FreeRTOS.h"
#include "heart.h"
#include "contrl.h"
#include <inttypes.h>

/**********************************************
 * Miss Lynette, you are so pritty
 * You are my star
 * What is the star
 * The sun far away from me
***********************************************/
const uint8_t lynette[][8] = {
{0x00,0x7E,0x40,0x40,0x78,0x40,0x40,0x00},/*"F",0*/
{0x00,0x00,0x00,0x3C,0x42,0x42,0x3C,0x00},/*"o",1*/
{0x00,0x00,0x00,0x2C,0x30,0x20,0x20,0x00},/*"r",2*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",3*/
{0x00,0x40,0x40,0x40,0x40,0x40,0x7E,0x00},/*"L",4*/
{0x00,0x00,0x00,0x42,0x64,0x2C,0x18,0x70},/*"y",5*/
{0x00,0x00,0x00,0x5C,0x62,0x42,0x42,0x00},/*"n",6*/
{0x00,0x00,0x00,0x3C,0x42,0x7C,0x3C,0x00},/*"e",7*/
{0x00,0x00,0x10,0xFC,0x10,0x10,0x1E,0x00},/*"t",8*/
{0x00,0x00,0x10,0xFC,0x10,0x10,0x1E,0x00},/*"t",9*/
{0x00,0x00,0x00,0x3C,0x42,0x7C,0x3C,0x00},/*"e",10*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",11*/
};

void blew_heart(int tv) {
  unsigned char heart[2][8][8] = {
    {
      {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
      {0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x00},
      {0x00,0x00,0x18,0x3c,0x18,0x00,0x00,0x00},
      {0x00,0x00,0x18,0x3c,0x18,0x00,0x00,0x00},
      {0x00,0x3c,0x7e,0x3c,0x18,0x00,0x00,0x00},
      {0x00,0x00,0x18,0x7e,0x18,0x00,0x00,0x00},
      {0x00,0x00,0x00,0x24,0x00,0x00,0x00,0x00},
      {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
    },
    {
      {0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00},
      {0x00,0x00,0x00,0x3c,0x3c,0x00,0x00,0x00},
      {0x00,0x00,0x18,0x7e,0x7e,0x18,0x00,0x00},
      {0x00,0x00,0x3c,0x7e,0x7e,0x3c,0x00,0x00},
      {0x00,0x18,0x7e,0xff,0xff,0x7e,0x18,0x00},
      {0x00,0x00,0x7e,0xff,0xff,0x7e,0x00,0x00},
      {0x00,0x00,0x00,0x66,0x66,0x00,0x00,0x00},
      {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
    }
  };

  unsigned int x,y,z;
  int times = tv;
  for(z = 0; z <7; z++) {
    while(times--) {
      for(y = 0; y < 8; y++) {
        for(x = 0; x < 8; x++) {
            hc595_write(heart[0][7-y][7-x]);
        }
        hc595out();
        cen_on(y);
        usleep(500);
        cen_on(8);
      }  
    }
    times = tv;
    while(times--) {
      for(y = 0; y < 8; y++) {
        for(x = 0; x < 8; x++) {
            hc595_write(heart[1][7-y][7-x]);
        }
        hc595out();
        cen_on(y);
        usleep(500);
        cen_on(8);
      }  
    }
    times = tv;
  }

}

void _my_heart(int tv)
{
  unsigned char heart[8][8] = {
    {0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x3c,0x3c,0x00,0x00,0x00},
    {0x00,0x00,0x18,0x7e,0x7e,0x18,0x00,0x00},
    {0x00,0x00,0x3c,0x7e,0x7e,0x3c,0x00,0x00},
    {0x00,0x18,0x7e,0xff,0xff,0x7e,0x18,0x00},
    {0x00,0x00,0x7e,0xff,0xff,0x7e,0x00,0x00},
    {0x00,0x00,0x00,0x66,0x66,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
  };

  unsigned int x,y,z;
  int times = tv;
  for(z = 0; z <= 8; z++) {
    while(times--) {
      for(y = 0; y < 8; y++) {
        for(x = 0; x < 8; x++) {
          hc595_write(heart[7-y][7-x]);
        }
        hc595out();
        cen_on(y);
        usleep(800 - 100*z);
        cen_on(8);
        usleep(100 + 100*z);
      }
    }
    times = tv;
  }
}

void _heartbeat(int tv)
{
  unsigned char heart[8][8] = {
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x66,0x66,0x00,0x00,0x00},
    {0x00,0x00,0x7e,0xff,0xff,0x7e,0x00,0x00},
    {0x00,0x18,0x7e,0xff,0xff,0x7e,0x18,0x00},
    {0x00,0x00,0x3c,0x7e,0x7e,0x3c,0x00,0x00},
    {0x00,0x00,0x18,0x7e,0x7e,0x18,0x00,0x00},
    {0x00,0x00,0x00,0x3c,0x3c,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00}
  };

  unsigned int x,y,z;
  int times = tv;
  for(z = 0; z < 4; z++) {
    while(times--) {
      for(y = 0; y < 8; y++) {
        for(x = 0; x < 8; x++) {
          if(z%2 == 1) {
            if(x<7)
              hc595_write(heart[y][x+1]);
            else hc595_write(0);
          } else {
            hc595_write(heart[y][x]);
          }
        }
        hc595out();
        cen_on(y);
        usleep(500);
        cen_on(8);
      }
    }
    times = tv;
  }
}

void _fail_heart(int tv)
{
  unsigned char heart[8][8] = {
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x66,0x66,0x00,0x00,0x00},
    {0x00,0x00,0x7e,0xff,0xff,0x7e,0x00,0x00},
    {0x00,0x18,0x7e,0xff,0xff,0x7e,0x18,0x00},
    {0x00,0x00,0x3c,0x7e,0x7e,0x3c,0x00,0x00},
    {0x00,0x00,0x18,0x7e,0x7e,0x18,0x00,0x00},
    {0x00,0x00,0x00,0x3c,0x3c,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00}
  };

  unsigned int x,y,z;
  int times = tv;
  for(z = 0; z < 8; z++) {
    while(times--) {
      for(y = 0; y < 8; y++) {
        for(x = 0; x < 8; x++) {
            if(y > z)
              hc595_write(heart[y-z][x]);
            else hc595_write(0);
        }
        hc595out();
        cen_on(y);
        usleep(500);
        cen_on(8);
      }
    }
    times = tv;
  }
}

void for_lynette_(int tv)
{
  uint8_t cube[8][8] = {0};
  uint8_t dat = 0;

  unsigned int x,y,z;
  int times = tv;
  for (int  i = 0; i < 11;i++){
    for(z = 0; z < 8; z++) {

      while(times--) {
        for(y = 0; y < 8; y++) {//体

          dat =   (lynette[i][y]  << z )  | (lynette[i + 1][y] >> (7-z));//字幕向左滚动算法
          cube[y][0]  = dat;
          cube[y][1]  = dat;

          for(x = 0; x < 8; x++) {//面
            hc595_write(cube[y][x]);//线
          }
          hc595out();
          cen_on(y);
          usleep(500);
          cen_on(8);
        }
      }
      times = tv;
    }
  }
}
