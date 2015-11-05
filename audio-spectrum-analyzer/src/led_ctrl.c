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
/*                           LOCAL DEPENDENCIES                         */
/*======================================================================*/
#include "led_cfg.h"
#include "pwm_cfg.h"
#include "task.h"

/*======================================================================*/
/*                           FUNCTION PROTOTYPES                        */
/*======================================================================*/
void led_formTxBuffer( uint8_t* inputArray, uint8_t* outputBuffer, uint16_t num_led );
void led_fadingRainbow( bool reset );
void led_swirlyColours( bool reset );
void led_setStrip( LED_Data_t* strip );
void led_blink( LED_Data_t *strip, uint32_t milliseconds );

/*======================================================================*/
/*                          FUNCTION DECLARATIONS                       */
/*======================================================================*/
void led_formTxBuffer( uint8_t* inputArray, uint8_t* outputBuffer, uint16_t num_led )
{
    uint16_t    LED_index;
    uint8_t     byte_index;
   
   for( LED_index = 0; LED_index < ( num_led * BYTES_PER_LED ); LED_index++, inputArray++ )
    {
        for( byte_index = 0; byte_index <= 7; byte_index++ )
        {
            if( *inputArray & _RSVAR( INPUT_MASK, byte_index ) )
            {
                *outputBuffer = PWM_LED_ONE;
            }
            else
            {
                *outputBuffer = PWM_LED_ZERO;
            }
            outputBuffer++;
        }
    }
    
    // Padding
    *outputBuffer = PWM_PERIOD_CYCLES;
}

void led_fadingRainbow( bool reset )
{
    static uint8_t state = 0;
    static LED_Data_t stripColour = {
        .colour.red     = 0,
        .colour.green   = 0,
        .colour.blue    = 0
    };    
    
    if( reset )
    {
        // Reset state and stripColour to 0
        memset(&stripColour, 0x00, sizeof(stripColour));
        state = 0;
    }
    
    for(uint8_t i = 0; i < LED_NUM; i++)
    {
        LED_setLED( &stripColour, i );
    }
    
    switch( state )
    {
        case 0:
            if( stripColour.colour.blue > 0 )
                stripColour.colour.blue--;
        
            stripColour.colour.red++;
            if(stripColour.colour.red >= 255)
                state = 1;
            
            else if(stripColour.colour.red >= 127)
            stripColour.colour.green++;

            break;
            
        case 1:
            if( stripColour.colour.red > 0 )
                stripColour.colour.red--;
        
            stripColour.colour.green++;
            if(stripColour.colour.green >= 255)
                state = 2;
                
            else if(stripColour.colour.green >= 127)
                stripColour.colour.blue++;
                
            break;
            
        case 2:
            if( stripColour.colour.red > 0 )
                stripColour.colour.red--;
        
            if( stripColour.colour.green > 0 )
                stripColour.colour.green--;
        
            stripColour.colour.blue++;
            if(stripColour.colour.blue >= 255)
                state = 0;
                
            break;
    }    
}

void led_swirlyColours( bool reset )
{
    static uint8_t sec_led = 0;
    
    static bool up = false;
    static bool col = true;
    
    static LED_Data_t singleLed = {
        .colour.green   = 45,
        .colour.red     = 225,
        .colour.blue    = 25,
    };
    
    static LED_Data_t offLED = {
        .colour.green   = 0,
        .colour.red     = 0,
        .colour.blue    = 0,
    };        
    
    static LED_Data_t onLED = {
        .colour.green   = 127,
        .colour.red     = 0,
        .colour.blue    = 0,
    };    

    for(uint8_t i = 0; i < LED_NUM; i++)
    {
        if( i % 2 )
        {
            LED_setLED( &singleLed, i );
            }else{
            if((i - sec_led) == 0)
            {
                LED_setLED( &onLED, i );
                }else{
                LED_setLED( &offLED, i );
            }
        }
    }
    
    if(col){
        sec_led += 2;
        if(sec_led >= 144)
        col = false;
        }else{
        sec_led -= 2;
        if(sec_led <= 0)
        col = true;
    }
    
    
    if(up){
        singleLed.colour.red++;
        onLED.colour.red++;
        
        if(singleLed.colour.red >= 255)
        up = false;
        
        }else{
        singleLed.colour.red--;
        onLED.colour.red--;
        
        if(singleLed.colour.red <= 0)
        up = true;
    }
}

void led_setStrip( LED_Data_t* strip )
{
    for( uint8_t i = 0; i < LED_NUM; i++ )
    {
        LED_setLED( strip, i );
    }
}

void led_blink( LED_Data_t *strip, uint32_t milliseconds )
{
    static bool on = true;
    static uint32_t count = 0;
    
    LED_Data_t led;
    led.colour.red = 0;
    led.colour.green = 0;
    led.colour.blue = 0;
    
    count++;
    if( count > (milliseconds/WAIT_TIME_MS))
    {
        on = !on;
        count = 0;
    }
    
    if( on )
    {
        led_setStrip(strip);
    } else {
        led_setStrip(&led);
    }
}

void led_pattern( patternType pattern )
{
    switch( pattern )
    {
        case WHOOSH:
            break;
        case RAINBOW:
            led_fadingRainbow( false );
            break;
        case SWIRL:
            led_swirlyColours( false );
            break;
        default:
            break;
    }
}

void TASK_outputFormingLED( void *pvParameters )
{
    UNUSED( pvParameters );
    
    uint8_t     *pLEDArray       = ledArray;    
    uint8_t     *pLEDPWMArray   = LED_PWMBuffer;
    
    /* Default setting */
    LED_Packet_t rxLED;
    rxLED.cmd = RGB;
    
    for(;;)
    {
        //led_fadingRainbow( false );
        //led_swirlyColours( false );
        //led_blink( &rxRGB, 250 );
        //led_setStrip( &rxRGB );
        if( xLEDQueue != 0 )
        {
            xQueueReceive( xLEDQueue, &rxLED, 0 );
        }
        
        switch( rxLED.cmd )
        {
            case RGB:
                led_setStrip( &rxLED.LED );
                break;
            case BLNK:
                if(rxLED.period <= 0)
                    led_setStrip( &rxLED.LED );
                else
                    led_blink( &rxLED.LED, rxLED.period );
                break;
            case PTRN:
                led_pattern( rxLED.pattern );
                break;
            default:
                break;
        }

        /* Form the buffer */
        led_formTxBuffer( pLEDArray, pLEDPWMArray, LED_NUM );
        
        /* Wait for verification that ledArray is valid */
        //xSemaphoreTake( LEDSemaphore, portMAX_DELAY );
        
        //tcc_enable(&TCC_instanceLED);
        
        /* Continuously attempt to send the buffer using DMA */
        taskENTER_CRITICAL();
        //dma_start_transfer_job( &zDMA_LEDResourceTx );
        dma_start_transfer_job(&zDMA_LEDResourcePWM);
        taskEXIT_CRITICAL();
        
        /* Wait until the buffer has been transmitted */
        while( ! ( *pDMA_Status & _LS( LED_TX_DONE ) ) );
       
        /* Yield to oncoming traffic */
        vTaskDelay(WAIT_TIME_MS);
    }
}