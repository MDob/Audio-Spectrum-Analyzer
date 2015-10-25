/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include "spi_cfg.h"
#include "usart_cfg.h"
#include "led_ctrl.h"
#include "config_sw.h"
#include "dma_cfg.h"

int main (void)
{
	//uint8_t string[] = "Hello, world!\r\n";
	
	/* System Configuration */
	system_init();
	system_interrupt_enable_global();
	
	SysTick_Config(system_gclk_gen_get_hz(GCLK_GENERATOR_0));
	delay_init();
	
	/* Configuration */
	//SPI_configureLED();
	//CONFIG_configurePins();
	USART_init();
	DMA_init();
	
	//dma_start_transfer_job(&zDMA_LEDResourceTx);
	//usart_write_buffer_job(&usart_instanceFTDI, string, sizeof(string));
	
	dma_start_transfer_job( &zDMA_FTDIResourceRx );
	
	while(1)
	{
		//port_pin_toggle_output_level(PIN_PA27);
		//port_pin_toggle_output_level(PIN_PA28);
		//delay_ms(100);
		
		if( *pDMA_Status & ( 1 << FTDI_RX_DONE ) )
		{
			*pDMA_Status &= ~( 1 << FTDI_RX_DONE );
			memcpy(FTDI_TxBuffer, (const uint8_t* ) FTDI_RxBuffer, sizeof(FTDI_RxBuffer));
			dma_start_transfer_job( &zDMA_FTDIResourceTx );
		}
		
	}
	
	return 0;
}
