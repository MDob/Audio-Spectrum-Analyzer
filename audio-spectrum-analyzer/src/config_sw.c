/*************************************************************************
 *  File Name:      config_sw.c
 *  Author:         Michael Dobinson
 *  Date Created:   2015-10-24
 *
 *  Brief:
 *      Contains configuration functions for GPIO
 *
 *  [Compiled and tested with Atmel Studio 7]
 *
*************************************************************************/

/*======================================================================*/
/*                          LOCAL DEPENDENCIES                          */
/*======================================================================*/
#include "config_sw.h"

/*======================================================================*/
/*                          FUNCTION PROTOTYPES                         */
/*======================================================================*/
void config_configureInputs( void );
void config_configureOutputs( void );

/*======================================================================*/
/*                          FUNCTION DECLARATIONS                         */
/*======================================================================*/
void CONFIG_configurePins( void )
{
    /* Configure inputs and outputs */
    config_configureInputs();
    config_configureOutputs();
}

void CONFIG_configureWDT( void )
{
    /* Disable WDT for now, implement later */
    struct wdt_conf conf_wdt;
    wdt_get_config_defaults( &conf_wdt );

    conf_wdt.enable = false;
    conf_wdt.clock_source = GCLK_GENERATOR_0;

    wdt_set_config( &conf_wdt );
}

void config_configureInputs( void )
{
    struct port_config inputConfig;
    port_get_config_defaults(&inputConfig);
    
    inputConfig.direction = PORT_PIN_DIR_INPUT;
    
    port_pin_set_config(SW0_PIN, &inputConfig);

}

void config_configureOutputs( void )
{
    struct port_config outputConfig;
    port_get_config_defaults(&outputConfig);

    outputConfig.direction = PORT_PIN_DIR_OUTPUT;
    
    port_pin_set_config(LED0_PIN, &outputConfig);
    port_pin_set_config(LED1_PIN, &outputConfig);
    
    port_pin_set_output_level(LED1_PIN, true);
    port_pin_set_output_level(LED0_PIN, false);
}