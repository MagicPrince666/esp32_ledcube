#include "freertos/FreeRTOS.h"
#include "mycube.h"
#include "contrl.h"

const unsigned char test[][8][8] = {
    {{0xFF, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF}, // 8
     {0x00, 0x7E, 0x42, 0x42, 0x42, 0x42, 0x7E, 0x00},
     {0x00, 0x00, 0x3C, 0x24, 0x24, 0x3C, 0x00, 0x00},
     {0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00},
     {0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00},
     {0x00, 0x00, 0x3C, 0x24, 0x24, 0x3C, 0x00, 0x00},
     {0x00, 0x7E, 0x42, 0x42, 0x42, 0x42, 0x7E, 0x00},
     {0xFF, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF}}};

void mycube(int tv)
{
  unsigned char cube[8][8] = {
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
    };

  unsigned int x,y,z;
  int times = tv;
  int layer = 0;
  int magic = 0;

  for(z = 0; z < 13; z++){
    if(z < 6) magic = z;
    else magic = 12 - z;
    switch(magic){
      case 0: cube[layer][7] = 0x00;
              cube[layer][6] = 0x00;
              cube[layer][5] = 0x00;
              cube[layer][4] = 0x00;
              cube[layer][3] = 0x08;
              cube[layer][2] = 0x00;
              cube[layer][1] = 0x00;
              cube[layer][0] = 0x00;
      break;
      case 1: cube[layer][7] = 0x00;
              cube[layer][6] = 0x00;
              cube[layer][5] = 0x00;
              cube[layer][4] = 0x08;
              cube[layer][3] = 0x14;
              cube[layer][2] = 0x08;
              cube[layer][1] = 0x00;
              cube[layer][0] = 0x00;
      break;
      case 2: cube[layer][6] = 0x00;
              cube[layer][5] = 0x08;
              cube[layer][4] = 0x14;
              cube[layer][3] = 0x22;
              cube[layer][2] = 0x14;
              cube[layer][1] = 0x08;
              cube[layer][0] = 0x00;
      break;
      case 3: cube[layer][7] = 0x00;
              cube[layer][6] = 0x08;
              cube[layer][5] = 0x22;
              cube[layer][4] = 0x00;
              cube[layer][3] = 0x41;
              cube[layer][2] = 0x00;
              cube[layer][1] = 0x22;
              cube[layer][0] = 0x08;
      break;
      case 4: cube[layer][7] = 0x08;
              cube[layer][6] = 0x41;
              cube[layer][5] = 0x00;
              cube[layer][4] = 0x00;
              cube[layer][3] = 0x80;
              cube[layer][2] = 0x00;
              cube[layer][1] = 0x00;
              cube[layer][0] = 0x41;
      break;
      case 5: cube[layer][7] = 0x80;
              cube[layer][6] = 0x00;
              cube[layer][5] = 0x00;
              cube[layer][4] = 0x00;
              cube[layer][3] = 0x00;
              cube[layer][2] = 0x00;
              cube[layer][1] = 0x00;
              cube[layer][0] = 0x00;
      break;
      case 6: cube[layer][7] = 0x00;
              cube[layer][6] = 0x00;
              cube[layer][5] = 0x00;
              cube[layer][4] = 0x00;
              cube[layer][3] = 0x00;
              cube[layer][2] = 0x00;
              cube[layer][1] = 0x00;
              cube[layer][0] = 0x00;
      break;
    }
    
    while(times--){
      for(y = 0; y < 8; y++){
        for(x = 0; x < 8; x++){
          hc595_write(cube[y][x]);
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


void rotating_mycube_(int tv)
{
  unsigned char cube[8][8] = {
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
    };

  unsigned int x,y,z;
  int times = tv;

  for(z = 0; z < 8; z++) {
    switch(z) {
      case 0: for( uint8_t i = z; i < 8 - z; i++) {
                cube[7 - z][i] = 0x01<<i;
              }
      break;
      case 1: for( uint8_t i = z; i < 8 - z; i++) {
                cube[7 - z][i] = 0x01<<i;
              }
              cube[7 - z][7] = 0x40;
              cube[7 - z][0] = 0x02;
      break;
      case 2: for( uint8_t i = z; i < 8 - z; i++) {
                cube[7 - z][i] = 0x01<<i;
              }
              cube[7 - z][7] = 0x20;
              cube[7 - z][6] = 0x20;
              cube[7 - z][1] = 0x04;
              cube[7 - z][0] = 0x04;
      break;
      case 3: for( uint8_t i = 0; i < 4; i++) {
                cube[7 - z][i] = 0x08;
                cube[7 - z][7 - i] = 0x10;
              }
      break;
      case 4: for( uint8_t i = 0; i < 4; i++) {
                cube[7 - z][7 - i] = 0x08;
                cube[7 - z][i] = 0x10;
              }
      break;
      case 5: for( uint8_t i = 2; i < 6; i++) {
                cube[7 - z][7 - i] = 0x01<<i;
              }
              cube[7 - z][7] = 0x04;
              cube[7 - z][6] = 0x04;
              cube[7 - z][1] = 0x20;
              cube[7 - z][0] = 0x20;
      break;
      case 6: for( uint8_t i = 1; i < 7; i++) {
                cube[7 - z][7 - i] = 0x01<<i;
              }
              cube[7 - z][7] = 0x02;
              cube[7 - z][0] = 0x40;
      break;
      case 7: for( uint8_t i = 0; i < 8; i++) {
                cube[7 - z][7 - i] = 0x01<<i;
              }
      break;
    }
    
    while(times--) {
      for(y = 0; y < 8; y++) {
        for(x = 0; x < 8; x++) {
          hc595_write(cube[y][x]);
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

void _sin_cube(const unsigned char po[][8][8], unsigned int cnt, int tv) {
  unsigned int x,y,z;
  int times = tv;
  for(z = 0; z < cnt; z++){
    while(times--) {
      for(y = 0; y < 8; y++){
        for(x = 0; x < 8; x++){
          hc595_write(po[z][7-y][7-x]);
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

void _display(uint16_t time, uint8_t dat)
{
    unsigned char x, y, z;
    int times = 0;
    for (z = 0; z < 1; z++) {
        while (times < time) {
            for (y = 0; y < 8; y++) {
                for (x = 0; x < 8; x++) {
                    hc595_write(dat);
                }
                hc595out();
                // cen_on(y);
                usleep(500);
                cen_on(9);
            }
            times++;
        }
    }
}

void light_on(uint16_t time)
{
    unsigned char x, y, z;
    int times = 0;
    for (z = 0; z < 1; z++) {
        while (times < time) {
            for (y = 0; y < 8; y++) {
                for (x = 0; x < 8; x++) {
                    if (x == 0 || x == 7) {
                        hc595_write(0xFF);
                    } else {
                        hc595_write(0x81);
                    }
                }
                hc595out();
                usleep(500);
                cen_on(9);
            }
            times++;
        }
    }
}

void light_off(uint16_t time)
{
    unsigned char x, y, z;
    int times = 0;
    for (z = 0; z < 1; z++) {
        while (times < time) {
            for (y = 0; y < 8; y++) {
                for (x = 0; x < 8; x++) {
                    hc595_write(0x00);
                }
                hc595out();
                usleep(500);
                cen_on(9);
            }
            times++;
        }
    }
}

void general(const unsigned char po[][8][8], unsigned int cnt, int tv)
{
    unsigned int x, y, z;
    int times = tv;
    for (z = 0; z < cnt; z++) {
        while (times--) {
            for (y = 0; y < 8; y++) {
                for (x = 0; x < 8; x++) {
                    hc595_write(po[z][y][x]);
                }
                hc595out();
                cen_on(y);
                usleep(500);
                cen_on(8);
            }
        }
        times = tv;
        // printf("Z=%d\n",z);
    }
}

void _hourglass(const unsigned char po[][8][8], unsigned int cnt, int tv)
{
    unsigned int x, y, z;
    int times = tv;
    for (z = 0; z < cnt; z++) {
        while (times--) {
            for (y = 0; y < 8; y++) {
                for (x = 0; x < 8; x++) {
                    hc595_write(po[z][7 - y][x]);
                }
                hc595out();
                cen_on(y);
                usleep(500);
                cen_on(8);
            }
        }
        times = tv;
        // printf("Z=%d\n",z);
    }
}

void displayking(int tv)
{
    uint8_t x, y, z, i = 0, j = 0, num;
    int times          = tv;
    uint8_t Cube[8][8] = {0};
    uint8_t cen[8]     = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

    for (i = 0; i < 8; i++) {
        if (i < 4)
            z = -8 * i + 28;
        else
            z = 8 * i - 28;

        j = i;
        for (num = 0; num < z; num++) {
            if (i < 4) {
                switch (num / (z / 4)) {
                case 0: {
                    Cube[i][j] |= Cube[i][j] >> 1 | cen[7 - i];
                } break;
                case 1: {
                    Cube[i][j++] |= cen[i];
                } break;
                case 2: {
                    Cube[i][7 - i] |= Cube[i][7 - i] << 1 | cen[i];
                } break;
                case 3: {
                    Cube[i][j] |= Cube[i][j] | cen[7 - i];
                    j--;
                } break;
                }
            } else {
                switch (num / (z / 4)) {
                case 0: {
                    Cube[i][7 - j] |= Cube[i][7 - i] >> 1 | cen[i];
                } break;
                case 1: {
                    Cube[i][7 - j] |= cen[7 - i];
                    j--;
                } break;
                case 2: {
                    Cube[i][i] |= Cube[i][i] << 1 | cen[7 - i];
                } break;
                case 3: {
                    Cube[i][7 - j] |= Cube[i][7 - j] | cen[i];
                    j++;
                } break;
                }
            }

            while (times--) {
                for (y = 0; y < 8; y++) {
                    for (x = 0; x < 8; x++) {
                        hc595_write(Cube[y][x]);
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
