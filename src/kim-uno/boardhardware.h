#ifndef BOARDHARDWARE_H
#define BOARDHARDWARE_H

#if defined(__arm__)
#endif

#if defined(__avr_atmega328p__)
#define CALC_RAM_SIZE
#endif

#if defined(__AVR__)
#define USE_EPROM
#endif

#endif