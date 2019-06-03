#ifndef BOARDHARDWARE_H
#define BOARDHARDWARE_H

#if defined(__arm__)
#define ONBOARD_RAM 0x04FF
#endif

#if defined(__avr_atmega328p__)
#endif

#if defined(__AVR__)
#define USE_EPROM
#define CALC_RAM_SIZE
#define ONBOARD_RAM 1024
#endif

#define BOARD_LED_MAX7219 true
#define BOARD_WIRED_LED false

#if BOARD_LED_MAX7219
#define LED_CS 15
#endif

extern byte aCols[8];
extern byte aRows[3];

extern byte ledSelect[8];
extern byte ledSelect7[8];

#endif