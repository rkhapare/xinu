#include <stdlib.h>
#include <stdio.h>
#include <diskio.h>
#include <device/bcm2835/gpio.h>
#include <device/bcm2835/spi.h>

shellcmd xsh_testdiskio(int nargs, char *args[])
{
	printf("\nInitializing BCM2835 GPIO.");
	bcm2835_init();

	printf("\nStarting SPI.");
	spi_start();
}
