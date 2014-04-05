/**********************************************************************************************************************************************
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

 Author    : Geoffrey 
 
 Date	   : 2012

 version   :

**********************************************************************************************************************************************/

#ifndef _SPI_H
#define _SPI_H
#include <integer.h>
/* GPIO PINS */
/*
volatile unsigned int *gpioGPFSEL0 =  (unsigned int *) 0x20200000;
volatile unsigned int *gpioGPFSEL1 =  (unsigned int *) 0x20200004;
volatile unsigned int *gpioGPSET0  =  (unsigned int *) 0x2020001C;
volatile unsigned int *gpioGPCLR0  =  (unsigned int *) 0x20200028;
volatile unsigned int *gpioGPLEV0  =  (unsigned int *) 0x20200034;
*/

/* SPI0 ADDRESS */
#define BCM2835_SPI0_BASE 		0x20204000
#define BCM2835_SPI0_CONTROL_OFFSET 	0x0
#define BCM2835_SPI0_FIFO_OFFSET 	0x4
#define BCM2835_SPI0_CLK_OFFSET		0x8
#define BCM2835_SPI0_DLEN_OFFSET	0xC
#define BCM2835_SPI0_LTOH_OFFSET	0x10
#define BCM2835_SPI0_DC_OFFSET		0x14

#define get_spi0register_addr(offset)	(BCM2835_SPI0_BASE + offset)

/*
volatile unsigned int *SPI0_CONTROL =  (unsigned int *) BCM2835_SPI0_BASE + 0;	//0x20204000;
volatile unsigned int *SPI0_FIFO    =  (unsigned int *) BCM2835_SPI0_BASE + 4;	//0x20204004;
volatile unsigned int *SPI0_CLK     =  (unsigned int *) BCM2835_SPI0_BASE + 8;	//0x20204008;
volatile unsigned int *SPI0_DLEN    =  (unsigned int *) BCM2835_SPI0_BASE + 12;	//0x2020400C;
volatile unsigned int *SPI0_LTOH    =  (unsigned int *) BCM2835_SPI0_BASE + 16;	//0x20204010;
volatile unsigned int *SPI0_DC      =  (unsigned int *) BCM2835_SPI0_BASE + 20;	//0x20204014;
*/
//void led_toggle();

/* Bitfields in spi_C */
#define SPI_C_LEN_LONG          25
#define SPI_C_DMA_LEN           24
#define SPI_C_CSPOL2            23
#define SPI_C_CSPOL1            22
#define SPI_C_CSPOL0            21
#define SPI_C_RX                20
#define SPI_C_RXR               19
#define SPI_C_TXD               18
#define SPI_C_RXD               17
#define SPI_C_DONE              16
#define SPI_C_LEN               13
#define SPI_C_REN               12
#define SPI_C_ADCS              11
#define SPI_C_INTR              10
#define SPI_C_INTD               9
#define SPI_C_DMAEN              8
#define SPI_C_TA                 7
#define SPI_C_CSPOL              6
#define SPI_C_CLEAR_RX           5
#define SPI_C_CLEAR_TX           4
#define SPI_C_CPOL               3
#define SPI_C_CPHA               2
#define SPI_C_CS1                1
#define SPI_C_CS0                0

//void wait(unsigned int delay);
void spi_begin(void);
unsigned int spi_xchg(unsigned char value);
void spi_end(void);

void spi_start(void);
void spi_teardown(void);
BYTE *spi_txrx(const BYTE* txBuf, BYTE* rxBuf, unsigned int len);

#endif
