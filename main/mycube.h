#ifndef __MYCUBE_H
#define __MYCUBE_H

#include <inttypes.h>

void mycube(int tv);
void rotating_mycube_(int tv);
void _sin_cube(const unsigned char po[][8][8], unsigned int cnt, int tv);
void displayking(int tv);
void _hourglass(const unsigned char po[][8][8], unsigned int cnt, int tv);
void general(const unsigned char po[][8][8], unsigned int cnt, int tv);
void light_on(uint16_t time);
void light_off(uint16_t time);
void _display(uint16_t time, uint8_t dat);

#endif
