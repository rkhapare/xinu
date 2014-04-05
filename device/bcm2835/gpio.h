/*
 * Copyright (C) 2012 Vikram Narayananan
 * <vikram186@gmail.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _GPIO_H_//#ifndef _BCM2835_GPIO_H_
#define _GPIO_H_//#define _BCM2835_GPIO_H_

#define BCM2835_GPIO_BASE		0x20200000
#define BCM2835_GPIO_COUNT		54
// Function Selector Register Offsets
#define BCM2835_GPFSEL0_OFFSET		0x0
#define BCM2835_GPFSEL1_OFFSET		0x4
#define BCM2835_GPFSEL2_OFFSET          0x8
#define BCM2835_GPFSEL3_OFFSET          0xC
#define BCM2835_GPFSEL4_OFFSET          0x10
#define BCM2835_GPFSEL5_OFFSET          0x14
// Pin Output Set & Clear Register Offsets
#define BCM2835_GPSET0_OFFSET		0x1C
#define BCM2835_GPSET1_OFFSET           0x20
#define BCM2835_GPCLR0_OFFSET           0x28
#define BCM2835_GPCLR1_OFFSET           0x2C
#define BCM2835_GPCMR0_OFFSET		0x28
// Pin Level Register Offsets
#define BCM2835_GPLEV0_OFFSET		0x34
#define BCM2835_GPLEV1_OFFSET           0x38
// Pin Pull-Up/Down Enable & Clock Register Offsets
#define BCM2835_GPPUD_OFFSET           	0x94
#define BCM2835_GPPUDCLK0_OFFSET    	0x98
#define BCM2835_GPPUDCLK1_OFFSET       	0x9C

/// Base Physical Address of the System Timer registers
#define BCM2835_ST_BASE			0x20003000
// Defines for ST
// GPIO register offsets from BCM2835_ST_BASE.
// Offsets into the ST Peripheral block in bytes per 12.1 System Timer Registers
// The System Timer peripheral provides four 32-bit timer channels and a single 64-bit free running counter.
// BCM2835_ST_CLO is the System Timer Counter Lower bits register.
// The system timer free-running counter lower register is a read-only register that returns the current value
// of the lower 32-bits of the free running counter.
// BCM2835_ST_CHI is the System Timer Counter Upper bits register.
// The system timer free-running counter upper register is a read-only register that returns the current value
// of the upper 32-bits of the free running counter.
#define BCM2835_ST_CS 		0x0000 //< System Timer Control/Status
#define BCM2835_ST_CLO 		0x0004 //< System Timer Counter Lower 32 bits
#define BCM2835_ST_CHI 		0x0008 //< System Timer Counter Upper 32 bits

#define BCM2835_GPIO_PUD_OFF     0x0   	///< Off ? disable pull-up/down
#define BCM2835_GPIO_PUD_DOWN    0x1   	///< Enable Pull Down control
#define BCM2835_GPIO_PUD_UP      0x2    ///< Enable Pull Up control

#define BCM2835_GPIO(gpioNumber) 	((gpioNumber < BCM2835_GPIO_COUNT) ? gpioNumber : -1)
#define get_gpioregister_addr(offset)   (BCM2835_GPIO_BASE + offset)
#define get_stregister_addr(st_offset)   (BCM2835_ST_BASE + st_offset)
#define read_from_register(addr)        (*(volatile unsigned int *)(addr))
#define write_to_register(addr, val)    (*(volatile unsigned int *)(addr)) = (val)

//FUNCTION SELECTORS -> 3 bits -> 8 combinations
#define BCM2835_GPIO_FSEL_MASK		0x7

#define BCM2835_GPIO_INPUT		0x0
#define BCM2835_GPIO_OUTPUT		0x1
#define BCM2835_GPIO_ALT0		0x4
#define BCM2835_GPIO_ALT1		0x5
#define BCM2835_GPIO_ALT2		0x6
#define BCM2835_GPIO_ALT3		0x7
#define BCM2835_GPIO_ALT4		0x3
#define BCM2835_GPIO_ALT5		0x2

#define BCM2835_GPIO_COMMON_BANK(gpio)	((gpio < 32) ? 0 : 1)
#define BCM2835_GPIO_COMMON_SHIFT(gpio)	(gpio & 0x1f)

#define BCM2835_GPIO_FSEL_BANK(gpio)	(gpio / 10)
#define BCM2835_GPIO_FSEL_SHIFT(gpio)	((gpio % 10) * 3)

/*struct bcm2835_gpio_regs {
	u32 gpfsel[6];
	u32 reserved1;
	u32 gpset[2];
	u32 reserved2;
	u32 gpclr[2];
	u32 reserved3;
	u32 gplev[2];
	u32 reserved4;
	u32 gpeds[2];
	u32 reserved5;
	u32 gpren[2];
	u32 reserved6;
	u32 gpfen[2];
	u32 reserved7;
	u32 gphen[2];
	u32 reserved8;
	u32 gplen[2];
	u32 reserved9;
	u32 gparen[2];
	u32 reserved10;
	u32 gppud;
	u32 gppudclk[2];
};*/

int bcm2835_init(void);
int bcm2835_close(void);
void bcm2835_gpio_set(unsigned int pin);
void bcm2835_gpio_clr(unsigned int pin);
void bcm2835_gpio_write(unsigned int pin, unsigned int on);
unsigned int bcm2835_gpio_lev(unsigned int pin);
void bcm2835_gpio_pud(unsigned int pud);
void bcm2835_gpio_pudclk(unsigned int pin, unsigned int on);
void bcm2835_gpio_set_pud(unsigned int pin, unsigned int pud);
void bcm2835_gpio_fsel(unsigned int gpio,unsigned int fSel);
/// Read the System Timer Counter register.
/// \return the value read from the System Timer Counter Lower 32 bits register
unsigned long long bcm2835_st_read(void);
/// Delays for the specified number of microseconds with offset.
/// \param[in] offset_micros Offset in microseconds
/// \param[in] micros Delay in microseconds
void bcm2835_st_delay(unsigned long long offset_micros, unsigned long long micros);
void bcm2835_delayMicroseconds(unsigned long long micros);

#endif /* _BCM2835_GPIO_H_ */
