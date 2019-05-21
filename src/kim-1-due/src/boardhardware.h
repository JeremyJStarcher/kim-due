#ifndef BOARDHARDWARE_H
#define BOARDHARDWARE_H

#if defined(__arm__)
#endif

#if defined(__avr_atmega328p__)
#endif

#if defined(__AVR__)
#define USE_EPROM
#define CALC_RAM_SIZE
#endif

#define BOARD_LED_I2C true
#define BOARD_WIRED_LED false

#if BOARD_LED_I2C
#define LED_I2C_DATA 16
#define LED_I2C_CLK 15
#define LED_I2C_CS 17
#endif

extern byte aCols[8];
extern byte aRows[3];

extern byte ledSelect[8];
extern byte ledSelect7[8];

#endif