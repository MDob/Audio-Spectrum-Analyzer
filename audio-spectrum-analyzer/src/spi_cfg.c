/*
 * spi_cfg.c
 *
 * Created: 2015-10-24 1:26:21 PM
 *  Author: Michael
 */ 

#include "spi_cfg.h"

void SPI_configureLED(void)
{
	struct spi_config config_led_spi;

	spi_get_config_defaults(&config_led_spi);
	
	config_led_spi.mode_specific.master.baudrate = SPI_BAUDRATE;
	config_led_spi.mux_setting = LED_SPI_SERCOM_MUX_SETTING;
	config_led_spi.pinmux_pad0 = LED_SPI_SERCOM_PINMUX_PAD0;
	config_led_spi.pinmux_pad1 = LED_SPI_SERCOM_PINMUX_PAD1;
	config_led_spi.pinmux_pad2 = LED_SPI_SERCOM_PINMUX_PAD2;
	config_led_spi.pinmux_pad3 = LED_SPI_SERCOM_PINMUX_PAD3;
	
	spi_init(&spi_instanceLED, LED_SPI_MODULE, &config_led_spi);
	spi_enable(&spi_instanceLED);
}