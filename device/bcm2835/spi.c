/**********************************************************************************************************************************************
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

 Author    : Geoffrey 
 
 Date	   : 2012

 version   :

**********************************************************************************************************************************************/

#include "gpio.h"
#include "spi.h"
#include <integer.h>
#include <stdio.h>
#include <stdlib.h>

/* delay*/
/*void wait ( unsigned int delay )
{
	while ( delay-- )
	{
		asm("mov r0, r0");	*//* No-op *//*
	}
}*/

/* Functions to modify SPI0 CS register */
void spi_cs_set(unsigned int pin)
{
	unsigned int var = read_from_register(get_spi0register_addr(BCM2835_SPI0_CONTROL_OFFSET));
	printf("\nBCM2835_SPI0_CONTROL[%08x] %08x" ,get_spi0register_addr(BCM2835_SPI0_CONTROL_OFFSET), var);
	var |= 1 << pin;
	printf(" expected %08x ", var);
	write_to_register(get_spi0register_addr(BCM2835_SPI0_CONTROL_OFFSET), var);
	var = read_from_register(get_spi0register_addr(BCM2835_SPI0_CONTROL_OFFSET));
	printf(" -> %08x" ,var);
}

/* if <0, reset the entire CS register */
void spi_cs_reset(int pin)
{
	unsigned int var = read_from_register(get_spi0register_addr(BCM2835_SPI0_CONTROL_OFFSET));
	printf("\nBCM2835_SPI0_CONTROL[%08x] %08x" ,get_spi0register_addr(BCM2835_SPI0_CONTROL_OFFSET), var);
	if (pin < 0)
	{
		var = 0;
	}
	else if (pin < 32)
	{
		var &= ~(1 << pin);
	}
	printf(" expected %08x ", var);
	write_to_register(get_spi0register_addr(BCM2835_SPI0_CONTROL_OFFSET), var);
	var = read_from_register(get_spi0register_addr(BCM2835_SPI0_CONTROL_OFFSET));
	printf(" -> %08x" ,var);
}

unsigned int spi_cs_read_pin (int pin)
{
	unsigned int var = read_from_register(get_spi0register_addr(BCM2835_SPI0_CONTROL_OFFSET));
	printf("\nBCM2835_SPI0_CONTROL[%08x] pin%d %d." ,get_spi0register_addr(BCM2835_SPI0_CONTROL_OFFSET), pin, ((var >> pin) & 1));
	return ((var >> pin)  & 1);
}

void spi_clk_set(unsigned int kHz)
{
	write_to_register(get_spi0register_addr(BCM2835_SPI0_CLK_OFFSET), kHz);
	printf("\nBCM2835_SPI0_CLK[%08x] %d." ,get_spi0register_addr(BCM2835_SPI0_CLK_OFFSET), kHz);
}

void spi_init(void)
{
	printf("\nDoing spi_init ...");
        bcm2835_gpio_fsel(BCM2835_GPIO(7), BCM2835_GPIO_ALT0);       // CE1
        bcm2835_gpio_fsel(BCM2835_GPIO(8), BCM2835_GPIO_ALT0);       // CE0
        bcm2835_gpio_fsel(BCM2835_GPIO(9), BCM2835_GPIO_ALT0);       // MISO
        bcm2835_gpio_fsel(BCM2835_GPIO(10), BCM2835_GPIO_ALT0);      // MOSI
        bcm2835_gpio_fsel(BCM2835_GPIO(11), BCM2835_GPIO_ALT0);      // CLK
	printf("\nDone spi_init.");
}

void spi_teardown(void)
{
	printf("\nDoing spi_teardown ...");
        bcm2835_gpio_fsel(BCM2835_GPIO(7), BCM2835_GPIO_INPUT);       // CE1
        bcm2835_gpio_fsel(BCM2835_GPIO(8), BCM2835_GPIO_INPUT);       // CE0
        bcm2835_gpio_fsel(BCM2835_GPIO(9), BCM2835_GPIO_INPUT);       // MISO
        bcm2835_gpio_fsel(BCM2835_GPIO(10), BCM2835_GPIO_INPUT);      // MOSI
        bcm2835_gpio_fsel(BCM2835_GPIO(11), BCM2835_GPIO_INPUT);      // CLK
	printf("\nDone spi_teardown.");
}

void spi_start(void)
{
	printf("\nDoing spi_start ...");
	spi_init();
	
	spi_cs_reset(-1);		// Clear CS
        spi_cs_set(SPI_C_CLEAR_RX);	// Set RX
        spi_cs_set(SPI_C_CLEAR_TX);	// Set TX
	
	spi_cs_reset(SPI_C_CPOL);	// Reset CPOL
	spi_cs_reset(SPI_C_CPHA);	// Reset CPHA
	spi_cs_reset(SPI_C_CS0);	// Reset CS0
	spi_cs_reset(SPI_C_CS1);	// Reset CS1
	spi_cs_reset(SPI_C_CSPOL0);	// Reset CSPOL0

	spi_clk_set(1024);		// Set CLK
	printf("\nDone spi_start.");
}

BYTE * spi_txrx(const BYTE* txBuf, BYTE* rxBuf, unsigned int len)
{
	unsigned int i;
	char dummy='\0';	

	spi_start();

	if(txBuf != NULL && rxBuf != NULL) printf("\nExchanging ");
	else if (txBuf != NULL) printf("\nWriting");
	else printf("\nReading");
	printf(" %d bytes...", len);

	spi_cs_set(SPI_C_CLEAR_RX);	// Set RX
        spi_cs_set(SPI_C_CLEAR_TX);	// Set TX
	spi_cs_set(SPI_C_TA);		// Set TA

	for (i=0; i < len; i++)
	{
		while (!spi_cs_read_pin(SPI_C_TXD)) { bcm2835_delayMicroseconds(1000);}//wait(1000000); }	// Wait for TXD
        	write_to_register(get_spi0register_addr(BCM2835_SPI0_FIFO_OFFSET), txBuf?txBuf[i]:dummy);	// Write to TX FIFO
        	while (!spi_cs_read_pin(SPI_C_DONE)) { bcm2835_delayMicroseconds(1000);}//wait(1000000); } 	// Wait for DONE
        	dummy = read_from_register(get_spi0register_addr(BCM2835_SPI0_FIFO_OFFSET));			// Read from RX FIFO
		if (rxBuf) { rxBuf[i] = dummy; }
	}
	spi_cs_reset(SPI_C_TA);		// Reset TA

	spi_teardown();

	printf("\nDone spi_txrx.");
	return rxBuf;
}

void spi_pin_init(void)
{
        unsigned int var;
        /* Read current value of GPFSEL1- pin 10 & 11 */
        var = read_from_register(get_gpioregister_addr(BCM2835_GPFSEL1_OFFSET));        //var = *gpioGPFSEL1;
        var &=~((BCM2835_GPIO_FSEL_MASK << 0) | (BCM2835_GPIO_FSEL_MASK << 3));         // set as input pins GPIO = 000
        var |= ((BCM2835_GPIO_ALT0 << 0) | (BCM2835_GPIO_ALT0 << 3));                   // set to alt function ALT0, GPIO = 000
        /* Write back updated value */
        write_to_register(get_gpioregister_addr(BCM2835_GPFSEL1_OFFSET), var);          //*gpioGPFSEL1 = var;

        /* Read current value of GPFSEL0- pin 7,8 & 9 */
        var = read_from_register(get_gpioregister_addr(BCM2835_GPFSEL0_OFFSET));        //var = *gpioGPFSEL0;
        var &=~((BCM2835_GPIO_FSEL_MASK << 21) | (BCM2835_GPIO_FSEL_MASK << 24) | (BCM2835_GPIO_FSEL_MASK << 27));// set input pins GPIO = 000
        var |= ((BCM2835_GPIO_ALT0 << 21) | (BCM2835_GPIO_ALT0 << 24) | (BCM2835_GPIO_ALT0 << 27)); // set to alt function ALT0, GPIO = 000
        /* Write back updated value */
        write_to_register(get_gpioregister_addr(BCM2835_GPFSEL0_OFFSET), var);          //*gpioGPFSEL0 = var;
}

void spi_begin(void)
{	
	unsigned int var;
	spi_pin_init();
	//spi_init();
        
	var = read_from_register(get_spi0register_addr(BCM2835_SPI0_CONTROL_OFFSET));	//var = *SPI0_CONTROL;
	var = 0;									
	var |= (1 << SPI_C_CLEAR_RX) | (1 << SPI_C_CLEAR_TX);				// Clear TX and RX FIFO's
	//write_to_register(get_gpioregister_addr(BCM2835_SPI0_CONTROL_OFFSET), var);	//*SPI0_CONTROL = 0;
	
	/* Clear TX and RX FIFO's */
	//*SPI0_CONTROL |= (1 << SPI_C_CLEAR_RX) | (1 << SPI_C_CLEAR_TX);
	
	//var = *SPI0_CONTROL;
 	/* set data mode = 0 */
	var &= ~((1 << SPI_C_CPOL) | (1 << SPI_C_CPHA));	
	
	/* set to CS0 and Chip Select Polarity=LOW */
	var &= ~((1 << SPI_C_CS0) | (1 << SPI_C_CS1));
	var &= ~(1 << SPI_C_CSPOL0);

	/* Write back updated value */
	write_to_register(get_spi0register_addr(BCM2835_SPI0_CONTROL_OFFSET), var);     	//*SPI0_CONTROL = var;
        /* set clock (in kHz) */
        //var = 1024;
	write_to_register(get_spi0register_addr(BCM2835_SPI0_CLK_OFFSET), 1024);     	//*SPI0_CLK = 1024;       
}

unsigned int spi_xchg(unsigned char value)
{
	unsigned int var = 0;
	unsigned int ret = 0;
	var = read_from_register(get_spi0register_addr(BCM2835_SPI0_CONTROL_OFFSET));           	//var = *SPI0_CONTROL;
	var|= (1 << SPI_C_CLEAR_RX) | (1<<SPI_C_CLEAR_TX);					// Clear TX and RX FIFO's
	var|= (1 << SPI_C_TA);									// Set TA = 1
	/* Write back updated value */
	write_to_register(get_spi0register_addr(BCM2835_SPI0_CONTROL_OFFSET), var);     		//*SPI0_CONTROL =var;
	
	/* Maybe wait for TXD */
	var = read_from_register(get_spi0register_addr(BCM2835_SPI0_CONTROL_OFFSET));           	//var = *SPI0_CONTROL;
	while (!(var & (1 << SPI_C_TXD)))
	{
		//wait(1000000);
		bcm2835_delayMicroseconds(1000);   
	}
              
	/* Write to TX FIFO */
	write_to_register(get_spi0register_addr(BCM2835_SPI0_FIFO_OFFSET), value);             	//*SPI0_FIFO = value;

	/* Wait for DONE to be set */
	//while (!(var & (1 << SPI_C_DONE))){
		    //wait(1000000);
		    bcm2835_delayMicroseconds(1000);
	//}
	/* Read RX FIFO */
	ret = read_from_register(get_spi0register_addr(BCM2835_SPI0_FIFO_OFFSET));           	//ret = *SPI0_FIFO;

    	/* Set TA = 0 */
    	var = read_from_register(get_spi0register_addr(BCM2835_SPI0_CONTROL_OFFSET));           	//var = *SPI0_CONTROL;
	var &= ~(1 << SPI_C_TA);
	/* Write back updated value */
	write_to_register(get_spi0register_addr(BCM2835_SPI0_CONTROL_OFFSET), var);             	//*SPI0_CONTROL =var;

    return ret;
}

void spi_end(void)
{
 	/* Set all the SPI0 pins back to input */
	unsigned int var;
	/* Read current value of GPFSEL1- pin 10 & 11 */
	var = read_from_register(get_gpioregister_addr(BCM2835_GPFSEL1_OFFSET));        //var = *gpioGPFSEL1;
	var &=~((BCM2835_GPIO_FSEL_MASK<<0) | (BCM2835_GPIO_FSEL_MASK<<3)); 		// set as input pins GPIO = 000
       	/* Write back updated value */
	write_to_register(get_gpioregister_addr(BCM2835_GPFSEL1_OFFSET), var);           //*gpioGPFSEL1 =var;
	
	/* Read current value of GPFSEL1- pin 7,8 & 9 */
	var = read_from_register(get_gpioregister_addr(BCM2835_GPFSEL0_OFFSET));        //var = *gpioGPFSEL0;
	var &=~((BCM2835_GPIO_FSEL_MASK<<21) | (BCM2835_GPIO_FSEL_MASK<<24) | (BCM2835_GPIO_FSEL_MASK<<27)); // set as input pins GPIO = 000
        /* Write back updated value */
	write_to_register(get_gpioregister_addr(BCM2835_GPFSEL0_OFFSET), var);           //*gpioGPFSEL1 =var;
}
