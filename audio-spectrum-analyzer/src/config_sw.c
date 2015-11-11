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
#include "ioport.h"
#include "adc_cfg.h"
#include "adc.h"
#include "adc_callback.h"
#include "extint.h"

/*======================================================================*/
/*                          FUNCTION PROTOTYPES                         */
/*======================================================================*/
void config_configureInputs( void );
void config_configureOutputs( void );

void config_SWTriggered( void );
void config_configureEXTINTCallbacks( void );
void config_configureEXTINTChannel( void );

static bool button = false;

/*======================================================================*/
/*                          FUNCTION DECLARATIONS                       */
/*======================================================================*/
void CONFIG_configurePins( void )
{
    /* Configure inputs and outputs */
    config_configureInputs();
    config_configureOutputs();
}

void config_SWTriggered( void )
{
    // Find a better way to double check this
    // Interrupt triggered on first tick for some reason...
    if( extint_chan_is_detected( SW0_EIC_LINE ) || button )
    {
        //adc_read_buffer_job(&conf_instanceADC, &confADCBuffer, 1);
        //adc_read_buffer_job(&aux_instanceADC, &auxADCBuffer, 128);
        adc_read_buffer_job(&mic_instanceADC, audioADCBuffer, ADC_SAMPLES);
    }
    button = true;
}

void config_configureEXTINTChannel( void )
{
    struct extint_chan_conf config_extint_chan;
    
    config_extint_chan.gpio_pin             = SW0_PIN;
    config_extint_chan.gpio_pin_mux         = SW0_EIC_MUX;
    config_extint_chan.gpio_pin_pull        = EXTINT_PULL_UP;
    config_extint_chan.detection_criteria   = EXTINT_DETECT_RISING;
    extint_chan_set_config( SW0_EIC_LINE, &config_extint_chan );    
}

void config_configureEXTINTCallbacks( void )
{
    extint_register_callback(       config_SWTriggered, SW0_EIC_LINE, 
                                    EXTINT_CALLBACK_TYPE_DETECT );
                                
    extint_chan_enable_callback(    SW0_EIC_LINE, 
                                    EXTINT_CALLBACK_TYPE_DETECT );
}

void config_configureInputs( void )
{
    struct port_config inputConfig;
    port_get_config_defaults(&inputConfig);
    
    inputConfig.direction = PORT_PIN_DIR_INPUT;
    
    port_pin_set_config(SW0_PIN, &inputConfig);
    config_configureEXTINTChannel();
    config_configureEXTINTCallbacks();
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
    
    ioport_set_pin_dir(PIN_PA06, IOPORT_DIR_OUTPUT);
}