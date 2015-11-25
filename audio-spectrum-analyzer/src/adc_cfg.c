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
#include "led_cfg.h"
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

typedef enum {
    ADC_MIC,
    ADC_AUX,
    ADC_CONF,
    NUM_ADC_CHAN
}adcChannel_t;

typedef enum {
    AUDIO_BASS,
    AUDIO_MIDS,
    AUDIO_TREBLE,
    NUM_AUDIO_TYPES
}audioType_t;

uint8_t adcFlags = 0;
adcChannel_t currChannel = ADC_AUX;

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

void adc_readChannel( adcChannel_t channel, uint16_t *buffer, uint16_t samples);
void adc_setChannel( adcChannel_t channel );
void adc_disableChannel( adcChannel_t channel );
void adc_enableChannel( adcChannel_t channel );

void setAudio( int16_t *freq, uint8_t *LED, int16_t lenFreq, uint16_t lenLED, uint16_t binDC, audioType_t type );

/*======================================================================*/
/*                          FUNCTION DECLARATIONS                       */
/*======================================================================*/
void TASK_adcFFT( void *pvParameters )
{
    UNUSED(pvParameters);
    spectrumCast_t *ledSpectrum = (spectrumCast_t *)ledArray;
    uint16_t *buffer = (uint16_t *) audioADCBuffer;
    uint16_t i;
    
    ADC_init();
    
    adc_setChannel( currChannel );
    adc_readChannel( currChannel, buffer, ADC_SAMPLES );
    
    for(;;)
    {
        if( adcFlags & _LS( AUDIO_DONE ) )
        {
            memset( audioImag, 0x00, sizeof( audioImag ) );
            fix_fft( ( short * ) audioADCBuffer, ( short * ) audioImag, LOG2_SAMPLES, !INVERSE_FFT );
            
            for( i = 0; i < ADC_SAMPLES; i++ )
            {
                audioADCBuffer[i] = 3 * sqrt( ( ( long ) audioADCBuffer[i] * 
                                                ( long ) audioADCBuffer[i] ) +
                                              ( ( long ) audioImag[i] * 
                                                ( long ) audioImag[i] ) );
            }
            
            /* Do something with result */
            if( LEDFlag & _LS(AUD) )
            {
                setAudio( &audioADCBuffer[1], ledSpectrum->freq.bassL, 4, BASS_L_LEN, audioADCBuffer[0], AUDIO_BASS );
                setAudio( &audioADCBuffer[1], ledSpectrum->freq.bassR, 4, BASS_R_LEN, audioADCBuffer[0], AUDIO_BASS );
                
                setAudio( &audioADCBuffer[5], ledSpectrum->freq.midsL, 15, MID_L_LEN, audioADCBuffer[0], AUDIO_MIDS );
                setAudio( &audioADCBuffer[5], ledSpectrum->freq.midsR, 15, MID_L_LEN, audioADCBuffer[0], AUDIO_MIDS );
                
                setAudio( &audioADCBuffer[20], ledSpectrum->freq.treb, 43, TREB_LEN, audioADCBuffer[0], AUDIO_TREBLE );
            }
            
            /* Clear it out */
            memset( audioADCBuffer, 0x00, sizeof( audioADCBuffer ) );
            
            adc_readChannel( currChannel, buffer, ADC_SAMPLES );
            
            adcFlags = 0;
        }
        
        vTaskDelay(10);
    }
}

void setAudio( int16_t *freq, uint8_t *LED, int16_t lenFreq, uint16_t lenLED, uint16_t binDC, audioType_t type )
{
    uint16_t max = 0;
    uint16_t maxPos = 0;
    uint16_t avg = 0;
    
    for( uint16_t i = 0; i < lenFreq; i++, freq++ )
    {
        if( *freq > max )
        {
            max = *freq;
            maxPos = i;
        }
        
        avg += *freq;
    }
    
    // Calculate average intensity
    avg /= lenFreq;
    
    for( uint16_t i = 0; i < lenLED; i++ )
    {
        switch( type )
        {
            case AUDIO_BASS:
                /* Green */
                *LED = 0;
                LED++;
                    
                /* Red */
                *LED = ((float)(*LED) * 0.8) + (float)((20 + ((255 - 20) * ((float)max * 5/(float)binDC))) * 0.2);
                LED++;
                    
                /* Blue */
                *LED = ((float)(*LED) * 0.8) + (float)((25 + ((255 - 25) * ((float)max * 5/(float)binDC))) * 0.2);
                LED++;
                break;
                
            case AUDIO_MIDS:
                /* Green */
                *LED = ((float)(*LED) * 0.8) + (float)((5 + ((255 - 5) * ((float)max * 5/(float)binDC))) * 0.2);
                LED++;
            
                /* Red */
                *LED = ((float)(*LED) * 0.8) + (float)((30 + ((255 - 30) * ((float)max * 5/(float)binDC))) * 0.2);
                LED++;
            
                /* Blue */
                *LED = 0;
                LED++;
                break;
                
            case AUDIO_TREBLE:
                /* Green */
                *LED = ((float)(*LED) * 0.8) + (float)((30 + ((255 - 30) * ((float)max * 5/(float)binDC))) * 0.2);
                LED++;
                
                /* Red */
                *LED = 0;
                LED++;
                
                /* Blue */
                *LED = ((float)(*LED) * 0.8) + (float)((5 + ((255 - 5) * ((float)max * 5/(float)binDC))) * 0.2);
                LED++;
                break;
                
            default:
                /* Do nothing */
                break;
        }
    }
}

void ADC_init( void )
{
    /* Configure ADC channels */
    adc_configureConf();
    adc_configureMic();
    adc_configureAux();
    
    /* Configure ADC callbacks */
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

void adc_readChannel( adcChannel_t channel, uint16_t *buffer, uint16_t samples)
{
    switch( channel )
    {
        case ADC_MIC:
            adc_read_buffer_job( &mic_instanceADC, buffer, samples );
            break;
        
        case ADC_AUX:
            adc_read_buffer_job( &aux_instanceADC, buffer, samples );
            break;
            
        case ADC_CONF:
            adc_read_buffer_job( &conf_instanceADC, buffer, samples );
            break;
            
        default:
            /* Do nothing */
            break;
    }
}

void adc_setChannel( adcChannel_t channel )
{
    adc_disableChannel( currChannel );
    
    currChannel = channel;
    
    adc_enableChannel( currChannel );
}

void adc_enableChannel( adcChannel_t channel )
{
    /* Enable the chosen channel */
    switch( channel )
    {
        case ADC_MIC:
            adc_enable( &mic_instanceADC );
            adc_enable_callback( &mic_instanceADC, ADC_CALLBACK_READ_BUFFER );
            break;
        
        case ADC_AUX:
            adc_enable( &aux_instanceADC );
            adc_enable_callback( &aux_instanceADC, ADC_CALLBACK_READ_BUFFER );
            break;
            
        case ADC_CONF:
            adc_enable( &conf_instanceADC );
            adc_enable_callback( &conf_instanceADC, ADC_CALLBACK_READ_BUFFER );
            break;
            
        default:
            /* Do nothing */
            break;
    }
}

void adc_disableChannel( adcChannel_t channel )
{
    /* Disable the chosen channel */
    switch( channel )
    {
        case ADC_MIC:
            adc_disable( &mic_instanceADC );
            adc_disable_callback( &mic_instanceADC, ADC_CALLBACK_READ_BUFFER );
            break;
        
        case ADC_AUX:
            adc_disable( &aux_instanceADC );
            adc_disable_callback( &aux_instanceADC, ADC_CALLBACK_READ_BUFFER );
            break;
            
        case ADC_CONF:
            adc_disable( &conf_instanceADC );
            adc_disable_callback( &conf_instanceADC, ADC_CALLBACK_READ_BUFFER );
            break;
            
        default:
            /* Do nothing */
            break;
    }
}

void adc_configureCallbacks()
{
    adc_register_callback( &mic_instanceADC, adc_audioCallback, ADC_CALLBACK_READ_BUFFER );
    adc_register_callback( &aux_instanceADC, adc_audioCallback, ADC_CALLBACK_READ_BUFFER );
    adc_register_callback( &conf_instanceADC, adc_confCallback, ADC_CALLBACK_READ_BUFFER );
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
    config_adc.clock_prescaler      = ADC_CLOCK_PRESCALER_DIV32;
    config_adc.reference            = ADC_REFERENCE_INTVCC1;
    
    config_adc.positive_input       = MIC_ADC_POS;
    config_adc.negative_input       = MIC_ADC_NEG;
    
    config_adc.differential_mode    = true;
    
    adc_init( &mic_instanceADC, ADC, &config_adc );
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
    config_adc.clock_prescaler      = ADC_CLOCK_PRESCALER_DIV32;
    config_adc.reference            = ADC_REFERENCE_INTVCC1;
    
    config_adc.positive_input       = AUX_ADC_POS;
    config_adc.negative_input       = AUX_ADC_NEG;
    
    config_adc.differential_mode    = true;
    
    adc_init( &aux_instanceADC, ADC, &config_adc );
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
}

void adc_configureAcc( void )
{
    // Configure both accelerometer axes
    // If you can't then make this a configuration function for a single axis
}