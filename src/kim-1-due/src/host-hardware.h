#ifndef HOSTHARDWARE_H
#define HOSTHARDWARE_H

#include <stdint.h>

#if defined(__arm__)
#define ONBOARD_RAM 0x04FF
#define HOST_KEYBOARD_KIM_UNO
#endif

#if defined(TARGETWEB)
#define ONBOARD_RAM 0x04FF
#endif


#if defined(__avr_atmega328p__)
#endif

#if defined(__AVR__)
#define USE_EPROM
#define CALC_RAM_SIZE
#define ONBOARD_RAM 1024
#define EMULATE_KEYBOARD
#endif

#ifdef TARGETWEB
#define BOARD_LED_MAX7219 false
#define BOARD_WIRED_LED false
#else
#define BOARD_LED_MAX7219 true
#define BOARD_WIRED_LED false
#endif

#if BOARD_LED_MAX7219
#define LED_CS 15
#endif

#ifdef HOST_KEYBOARD_KIM_UNO
extern uint8_t aCols[8];
extern uint8_t aRows[3];

extern uint8_t ledSelect[8];
extern uint8_t ledSelect7[8];
#endif

#endif