/*************************************************************************
 *  File Name:      adc_cfg.c
 *  Author:         Michael Dobinson
 *  Date Created:   2015-10-27
 *
 *  Brief:
 *      Contains configuration functions for the ADC
 *
 *  [Compiled and tested with Atmel Studio 7]
 *
*************************************************************************/

/*======================================================================*/
/*                           LOCAL DEPENDENCIES                         */
/*======================================================================*/
#include "adc_cfg.h"
#include "adc.h"
#include "dma_cfg.h"
#include "adc_callback.h"
#include "fft.h"
#include "task.h"
#include "arm_math.h"

enum adcFlag {
    AUDIO_DONE,
    CONF_DONE,
    ACCX_DONE,
    ACCY_DONE,
};

uint8_t adcFlags = 0;

/*======================================================================*/
/*                           FUNCTION PROTOTYPES                        */
/*======================================================================*/
void adc_configure          ( void );
void adc_configureCallbacks ( void );

void adc_audioCallback      ( struct adc_module *const module );
void adc_confCallback       ( struct adc_module *const module );

void adc_configureMic       ( void );
void adc_configureAux       ( void );
void adc_configureConf      ( void );
void adc_configureAcc       ( void );

/*======================================================================*/
/*                          FUNCTION DECLARATIONS                       */
/*======================================================================*/
void TASK_adcFFT( void *pvParameters )
{
    UNUSED(pvParameters);
    uint16_t *buffer = audioADCBuffer;
    uint16_t i;
    
    ADC_init();
    adc_read_buffer_job( &aux_instanceADC, buffer, ADC_SAMPLES );
    
    for(;;)
    {
        if( adcFlags & _LS( AUDIO_DONE ) )
        {
            fix_fftr( (short *) audioADCBuffer, LOG2_SAMPLES, 0 );
            
            for( i = 0; i < _LS( ( LOG2_SAMPLES-1 ) ); i++ )
            {
                audioADCBuffer[i] = sqrt( ( audioADCBuffer[i] * 
                                            audioADCBuffer[i] ) +
                                          ( audioADCBuffer[i + _LS( ( LOG2_SAMPLES-1 ) )] * 
                                            audioADCBuffer[i + _LS( ( LOG2_SAMPLES-1 ) )] ) );
            }
            /* Do something with result */
            
            /* Clear it out */
            memset( audioADCBuffer, 0x00, sizeof( audioADCBuffer ) );
            
            adc_read_buffer_job( &aux_instanceADC, buffer, ADC_SAMPLES );
            adcFlags = 0;
        }
        
        vTaskDelay(100);
    }
}

void ADC_init( void )
{
    /* 
     *  For some reason the ADC only allows one instance to be configured at once
     *  Fix this so that multiple instances can be configured or add a task that queues
     *  ADC requests and initializes/executes them in order
     */
    
    //adc_configureConf();
    //adc_configureMic();
    adc_configureAux();
    
    adc_configureCallbacks();
}

void adc_audioCallback( struct adc_module *const module )
{
    adcFlags |= _LS( AUDIO_DONE );
}

void adc_confCallback( struct adc_module *const module )
{
    adcFlags |= _LS( CONF_DONE );
}

void adc_configureCallbacks( void )
{
    //adc_register_callback(&conf_instanceADC, adc_confCallback, ADC_CALLBACK_READ_BUFFER);
    //adc_enable_callback(&conf_instanceADC, ADC_CALLBACK_READ_BUFFER);
    
    //adc_register_callback( &mic_instanceADC, adc_audioCallback, ADC_CALLBACK_READ_BUFFER );
    //adc_enable_callback( &mic_instanceADC, ADC_CALLBACK_READ_BUFFER );
    
    adc_register_callback(&aux_instanceADC, adc_audioCallback, ADC_CALLBACK_READ_BUFFER);
    adc_enable_callback(&aux_instanceADC, ADC_CALLBACK_READ_BUFFER);
}

void adc_configureMic( void )
{
    struct adc_config config_adc;
    struct system_pinmux_config config;
    
    system_pinmux_get_config_defaults( &config );
    adc_get_config_defaults( &config_adc );
    
    config.input_pull               = MIC_ADC_PULL;
    config.mux_position             = MIC_ADC_MUXPOS;
    
    system_pinmux_pin_set_config( MIC_ADC_PIN, &config );
    
    config_adc.resolution           = ADC_RESOLUTION_CUSTOM;
    config_adc.divide_result        = ADC_DIVIDE_RESULT_4;
    config_adc.accumulate_samples   = ADC_ACCUMULATE_SAMPLES_4;
    config_adc.clock_source         = GCLK_GENERATOR_5;
    config_adc.gain_factor          = ADC_GAIN_FACTOR_1X;
    config_adc.clock_prescaler      = ADC_CLOCK_PRESCALER_DIV4;
    config_adc.reference            = ADC_REFERENCE_INTVCC1;
    
    config_adc.positive_input       = MIC_ADC_POS;
    config_adc.negative_input       = MIC_ADC_NEG;
    
    config_adc.differential_mode    = true;
    
    adc_init( &mic_instanceADC, ADC, &config_adc );
    adc_enable( &mic_instanceADC );
}

void adc_configureAux( void )
{
    struct adc_config config_adc;
    struct system_pinmux_config config;
    
    system_pinmux_get_config_defaults( &config );
    adc_get_config_defaults( &config_adc );
    
    config.input_pull               = AUX_ADC_PULL;
    config.mux_position             = AUX_ADC_MUXPOS;
    
    system_pinmux_pin_set_config( AUX_ADC_PIN, &config );
    
    config_adc.resolution           = ADC_RESOLUTION_CUSTOM;
    config_adc.divide_result        = ADC_DIVIDE_RESULT_4;
    config_adc.accumulate_samples   = ADC_ACCUMULATE_SAMPLES_4;
    config_adc.clock_source         = GCLK_GENERATOR_5;
    config_adc.gain_factor          = ADC_GAIN_FACTOR_1X;
    config_adc.clock_prescaler      = ADC_CLOCK_PRESCALER_DIV4;
    config_adc.reference            = ADC_REFERENCE_INTVCC1;
    
    config_adc.positive_input       = AUX_ADC_POS;
    config_adc.negative_input       = AUX_ADC_NEG;
    
    config_adc.differential_mode    = true;
    
    adc_init( &aux_instanceADC, ADC, &config_adc );
    adc_enable( &aux_instanceADC );
}

void adc_configureConf( void )
{
    struct adc_config config_adc;
    struct system_pinmux_config config;
    
    system_pinmux_get_config_defaults( &config );
    adc_get_config_defaults( &config_adc );
    
    config.input_pull               = CONF_ADC_PULL;
    config.mux_position             = CONF_ADC_MUXPOS;
    
    system_pinmux_pin_set_config( CONF_ADC_PIN, &config );
    
    config_adc.resolution           = ADC_RESOLUTION_CUSTOM;
    config_adc.divide_result        = ADC_DIVIDE_RESULT_32;
    config_adc.accumulate_samples   = ADC_ACCUMULATE_SAMPLES_32;
    config_adc.gain_factor          = ADC_GAIN_FACTOR_4X;
    config_adc.clock_prescaler      = ADC_CLOCK_PRESCALER_DIV8;
    config_adc.reference            = ADC_REFERENCE_INTVCC1;
    
    config_adc.positive_input       = CONF_ADC_POS;
    config_adc.negative_input       = CONF_ADC_NEG;
    
    adc_init( &conf_instanceADC, ADC, &config_adc );
    adc_enable( &conf_instanceADC );
}

void adc_configureAcc( void )
{
    // Configure both accelerometer axes
    // If you can't then make this a configuration function for a single axis
}