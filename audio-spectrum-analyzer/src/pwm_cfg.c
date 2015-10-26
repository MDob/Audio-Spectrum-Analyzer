/*************************************************************************
 *  File Name:      led_ctrl.c
 *  Author:         Michael Dobinson
 *  Date Created:   2015-10-24
 *
 *  Brief:
 *      
 *
 *  [Compiled and tested with Atmel Studio 7]
 *
*************************************************************************/
/*======================================================================*/
/*                          LOCAL DEPENDENCIES                          */
/*======================================================================*/
#include "pwm_cfg.h"
#include "dma_cfg.h"

/*======================================================================*/
/*                         FUNCTION DECLARATIONS                        */
/*======================================================================*/void PWM_init( void )
{
    //PWM_setBuffer();
    PWM_configureTCC( ( uint32_t ) PWM_PERIOD_CYCLES );
}

void PWM_configureTCC( uint32_t period )
{
    struct tcc_config config_tcc;
    tcc_get_config_defaults( &config_tcc, LED_PWM_MODULE );
    
    config_tcc.counter.period = PWM_PERIOD_CYCLES;
    config_tcc.counter.clock_source = GCLK_GENERATOR_4;
    
    config_tcc.compare.wave_generation = TCC_WAVE_GENERATION_SINGLE_SLOPE_PWM;
    config_tcc.compare.wave_polarity[LED_PWM_CHANNEL] = TCC_WAVE_POLARITY_1;
    config_tcc.compare.match[LED_PWM_CHANNEL] = ( uint32_t ) PWM_PERIOD_CYCLES;
    
    config_tcc.pins.enable_wave_out_pin[LED_PWM_OUTPUT] = true;
    config_tcc.pins.wave_out_pin[LED_PWM_OUTPUT]        = LED_PWM_PIN;
    config_tcc.pins.wave_out_pin_mux[LED_PWM_OUTPUT]    = LED_PWM_MUX;
    
    tcc_init( &TCC_instanceLED, LED_PWM_MODULE, &config_tcc );
    tcc_enable( &TCC_instanceLED );
}

void PWM_setBuffer( void )
{
    for(uint8_t i = 0; i < LED_NUM; i++)
    {
        for(uint8_t green = 0; green <= 7; green++)
        {
            LED_PWMBuffer[(i * 24) + green] = PWM_LED_ONE;
        }
        for(uint8_t red = 0; red <= 7; red++)
        {
            LED_PWMBuffer[(i * 24) + 8 + red] = PWM_LED_ONE;
        }
        for(uint8_t blue = 0; blue <= 7; blue++)
        {
            LED_PWMBuffer[(i * 24) + 16 + blue] = PWM_LED_ONE;
        }
        
    }
}