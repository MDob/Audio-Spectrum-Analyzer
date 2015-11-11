/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>
#include "COMM/comm.h"
#include "usart.h"
#include "adc_cfg.h"
#include "dma_cfg.h"
#include "config_sw.h"
#include "light_ws2812_cortex.h"

#if defined(__GNUC__)
void board_init(void) WEAK __attribute__((alias("system_board_init")));
#elif defined(__ICCARM__)
void board_init(void);
#  pragma weak board_init=system_board_init
#endif

void system_board_init(void)
{
    system_interrupt_enable_global();
    SysTick_Config(system_gclk_gen_get_hz(GCLK_GENERATOR_0));
    
    /* Peripheral Initialization */
    COMM_init();
    USART_init();
    ADC_init();
    DMA_init();
    
    /* GPIO Initialization */
    CONFIG_configurePins();
    ws2812_init();
    
}