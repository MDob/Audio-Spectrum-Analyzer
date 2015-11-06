/*
 * adc_cfg.c
 *
 * Created: 2015-10-27 8:05:44 PM
 *  Author: Michael
 */ 

#include "adc_cfg.h"
#include "dma_cfg.h"
#include "adc.h"
#include "adc_callback.h"

void ADC_configure( void );
void ADC_configureCallbacks( void );

void adc_micCallback( struct adc_module *const module );
void adc_auxCallback( struct adc_module *const module );
void adc_confCallback( struct adc_module *const module );

void ADC_configureMic( void );
void ADC_configureAux( void );
void ADC_configureConf( void );


void adc_micCallback( struct adc_module *const module )
{
    confADCBuffer = 0;
}

void adc_auxCallback( struct adc_module *const module )
{
    confADCBuffer = 0;
}

void adc_confCallback( struct adc_module *const module )
{
    confADCBuffer = 0;
}

void ADC_init( void )
{
    //ADC_configureConf();
    ADC_configureMic();
    //ADC_configureAux();
    
    
    ADC_configureCallbacks();
}

void ADC_configureCallbacks( void )
{
    //adc_register_callback(&conf_instanceADC, adc_confCallback, ADC_CALLBACK_READ_BUFFER);
    //adc_enable_callback(&conf_instanceADC, ADC_CALLBACK_READ_BUFFER);
    
    adc_register_callback(&mic_instanceADC, adc_micCallback, ADC_CALLBACK_READ_BUFFER);
    adc_enable_callback(&mic_instanceADC, ADC_CALLBACK_READ_BUFFER);
    
    //adc_register_callback(&aux_instanceADC, adc_auxCallback, ADC_CALLBACK_READ_BUFFER);
    //adc_enable_callback(&aux_instanceADC, ADC_CALLBACK_READ_BUFFER);
}

void ADC_configureMic( void )
{
    struct adc_config config_adc;
    struct system_pinmux_config config;
    
    system_pinmux_get_config_defaults(&config);
    adc_get_config_defaults(&config_adc);
    
    config.input_pull               = MIC_ADC_PULL;
    config.mux_position             = MIC_ADC_MUXPOS;
    
    system_pinmux_pin_set_config(MIC_ADC_PIN, &config);
    
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

void ADC_configureAux( void )
{
    struct adc_config config_adc;
    struct system_pinmux_config config;
    
    system_pinmux_get_config_defaults(&config);
    adc_get_config_defaults(&config_adc);
    
    config.input_pull               = AUX_ADC_PULL;
    config.mux_position             = AUX_ADC_MUXPOS;
    
    system_pinmux_pin_set_config(AUX_ADC_PIN, &config);
    
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

void ADC_configureConf( void )
{
    struct adc_config config_adc;
    struct system_pinmux_config config;
    
    system_pinmux_get_config_defaults(&config);
    adc_get_config_defaults(&config_adc);
    
    config.input_pull               = CONF_ADC_PULL;
    config.mux_position             = CONF_ADC_MUXPOS;
    
    system_pinmux_pin_set_config(CONF_ADC_PIN, &config);
    
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

void ADC_configureAcc( void )
{
    
}