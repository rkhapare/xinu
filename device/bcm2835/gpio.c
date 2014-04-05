#include "gpio.h"//#include "bcm2835/gpio.h"
#include <stdio.h>

/* delay*/
/*void wait_asm ( unsigned int delay )
{
        while ( delay-- )
        {
                asm("mov r0, r0");     */ /* No-op */
/*        }
}*/

int bcm2835_init(void) {
	return 1;
}

int bcm2835_close(void) {
	return 1;
}

#define BCM2835_PERI_SET_BITS(a, v, m)		a = ((a) & ~(m)) | ((v) & (m));

void bcm2835_gpio_set(unsigned int pin)
{
	//BCM2835_GPIO->GPSET0 = 1 << pin;
	unsigned int var = read_from_register(get_gpioregister_addr(BCM2835_GPSET0_OFFSET));
        var = 1 << pin;
        write_to_register(get_gpioregister_addr(BCM2835_GPSET0_OFFSET), var);
}

void bcm2835_gpio_clr(unsigned int pin)
{
	//BCM2835_GPIO->GPCLR0  = 1 << pin;
	unsigned int var = read_from_register(get_gpioregister_addr(BCM2835_GPCLR0_OFFSET));
	var = 1 << pin;
    	write_to_register(get_gpioregister_addr(BCM2835_GPCLR0_OFFSET), var);
}

void bcm2835_gpio_write(unsigned int pin, unsigned int on) {
	if (on)
		bcm2835_gpio_set(pin);
	else
		bcm2835_gpio_clr(pin);
}

unsigned int bcm2835_gpio_lev(unsigned int pin)
{
    //uint32_t value = BCM2835_GPIO->GPLEV0;
    //return (value & (1 << pin)) ? HIGH : LOW;
    unsigned int var = read_from_register(get_gpioregister_addr(BCM2835_GPLEV0_OFFSET));
    return (var & (1 << pin)) ? 1 : 0;
}

/// Read the System Timer Counter register.
/// \return the value read from the System Timer Counter Lower 32 bits register
//uint64_t bcm2835_st_read(void);

/// Delays for the specified number of microseconds with offset.
/// \param[in] offset_micros Offset in microseconds
/// \param[in] micros Delay in microseconds
//extern void bcm2835_st_delay(uint64_t offset_micros, uint64_t micros);

// Read the System Timer Counter (64-bits)
unsigned long long bcm2835_st_read(void)
{
    //volatile unsigned int* addr;
    unsigned long long st = 0;
    //paddr = bcm2835_st + BCM2835_ST_CHI/4;
    //addr = (volatile unsigned int *) (BCM2835_ST_BASE + BCM2835_ST_CHI/4);
    //st = bcm2835_peri_read(paddr);
    //st = read_from_register(addr);
    //st <<= 32;
    //paddr = bcm2835_st + BCM2835_ST_CLO/4;
    //addr = (volatile unsigned int *) (BCM2835_ST_BASE + BCM2835_ST_CLO/4);
    //st += bcm2835_peri_read(paddr);
    unsigned int bits = read_from_register(get_stregister_addr(BCM2835_ST_CHI));
    st += bits;
    st <<= 32;
    bits = read_from_register(get_stregister_addr(BCM2835_ST_CLO));
    st += bits;
    return st;
}

// Delays for the specified number of microseconds with offset
void bcm2835_st_delay(unsigned long long offset_micros, unsigned long long micros)
{
    unsigned long long compare = offset_micros + micros;

    while(bcm2835_st_read() < compare)
	;
}

void bcm2835_delayMicroseconds(unsigned long long micros)
{
    bcm2835_st_delay(bcm2835_st_read(), micros);
}

void bcm2835_gpio_pud(unsigned int pud)
{
    //BCM2835_GPIO->GPPUD = pud;
    unsigned int var = read_from_register(get_gpioregister_addr(BCM2835_GPPUD_OFFSET));
    var &= 0;
    var |= pud;
    write_to_register(get_gpioregister_addr(BCM2835_GPPUD_OFFSET), var);
}

void bcm2835_gpio_pudclk(unsigned int pin, unsigned int on)
{
    //BCM2835_GPIO->GPPUDCLK0 =  (on ? 1 : 0) << pin;
    unsigned int var = read_from_register(get_gpioregister_addr(BCM2835_GPPUDCLK0_OFFSET));
    var = (on ? 1 : 0) << pin; 
    write_to_register(get_gpioregister_addr(BCM2835_GPPUDCLK0_OFFSET), var);
}

void bcm2835_gpio_set_pud(unsigned int pin, unsigned int pud)
{
    bcm2835_gpio_pud(pud);
    bcm2835_delayMicroseconds(10);
    //wait_asm(150);
    bcm2835_gpio_pudclk(pin, 1);
    bcm2835_delayMicroseconds(10);
    //wait_asm(150);
    bcm2835_gpio_pud(BCM2835_GPIO_PUD_OFF);
    bcm2835_gpio_pudclk(pin, 0);
}

void bcm2835_gpio_fsel(unsigned int gpio,unsigned int fSel)
{
        unsigned int var;
        unsigned int gpioBankNumber = BCM2835_GPIO_FSEL_BANK(gpio);
	volatile unsigned int *gpioAddr = (volatile unsigned int *)BCM2835_GPIO_BASE;

        switch (gpioBankNumber)
        {
		case 0: gpioAddr = (volatile unsigned int *)get_gpioregister_addr(BCM2835_GPFSEL0_OFFSET); break;
		case 1: gpioAddr = (volatile unsigned int *)get_gpioregister_addr(BCM2835_GPFSEL1_OFFSET); break;
		case 2: gpioAddr = (volatile unsigned int *)get_gpioregister_addr(BCM2835_GPFSEL2_OFFSET); break;
		case 3: gpioAddr = (volatile unsigned int *)get_gpioregister_addr(BCM2835_GPFSEL3_OFFSET); break;
		case 4: gpioAddr = (volatile unsigned int *)get_gpioregister_addr(BCM2835_GPFSEL4_OFFSET); break;
		case 5: gpioAddr = (volatile unsigned int *)get_gpioregister_addr(BCM2835_GPFSEL5_OFFSET); break;
                default: printf("GPIO numbers > 54 not supported."); return;
        }
	var = read_from_register(gpioAddr);
        var &=~(BCM2835_GPIO_FSEL_MASK << BCM2835_GPIO_FSEL_SHIFT(gpio));
        var |= (fSel << BCM2835_GPIO_FSEL_SHIFT(gpio));
        write_to_register(gpioAddr, var);
}
