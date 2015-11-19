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
#include <string.h>
#include "led_cfg.h"
#include "light_ws2812_cortex.h"
#include "pwm_cfg.h"
#include "task.h"

/*======================================================================*/
/*                           FUNCTION PROTOTYPES                        */
/*======================================================================*/
void led_formTxBuffer( uint8_t* inputArray, uint8_t* outputBuffer, uint16_t num_led );
void led_fadingRainbow( bool reset );
void led_swirlyColours( bool reset );
void led_setStrip( LED_Data_t* strip );
void led_shiftStrip( bool direction );
void led_runningRainbow( bool reset );
void led_pattern( patternType pattern );

void led_blink( LED_Data_t *strip, uint32_t milliseconds );

static LED_Data_t offLED = {
    .colour.green   = 0,
    .colour.red     = 0,
    .colour.blue    = 0,
};

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
            if( stripColour.colour.blue >= 2 )
                stripColour.colour.blue-=2;
        
            stripColour.colour.red++;
            if(stripColour.colour.red >= 255)
                state = 1;
            
            else if(stripColour.colour.red >= 200)
            stripColour.colour.green++;

            break;
            
        case 1:
            if( stripColour.colour.red >= 2 )
                stripColour.colour.red-=2;
        
            stripColour.colour.green++;
            if(stripColour.colour.green >= 255)
                state = 2;
                
            else if(stripColour.colour.green >= 200)
                stripColour.colour.blue++;
                
            break;
            
        case 2:
            if( stripColour.colour.red >= 2 )
                stripColour.colour.red-=2;
        
            if( stripColour.colour.green >= 2 )
                stripColour.colour.green-=2;
        
            stripColour.colour.blue++;
            if(stripColour.colour.blue >= 255)
                state = 0;
                
            break;
    }    
}

void led_shiftStrip( bool direction )
{
    LED_Data_t tempLED;
    LED_Data_t nextLED;
    uint16_t i = 0;
    
    for(i = 0; i < (LED_NUM - 1); i++ )
    {
        if( direction )
        {
            if(!i)
            {
                LED_getLED( &tempLED, i );
                LED_getLED( &nextLED, LED_NUM-1 );
                LED_setLED( &nextLED, i );
            }
            LED_getLED( &nextLED, i+1);
            LED_setLED( &tempLED, i+1 );
            tempLED = nextLED;
        }
    }
}

void led_runningRainbow( bool reset )
{
    static bool set = false;    
    LED_Data_t test;
    
    if( ( !set ) || reset)
    {
        test.colour.red = 255;
        led_setStrip(&offLED);
        LED_setLED( &test, 0 );
        LED_setLED( &test, 72 );
        test.colour.green = 125;
        LED_setLED( &test, 12 );
        LED_setLED( &test, 84 );
        test.colour.red = 0;
        test.colour.green = 255;
        test.colour.blue = 125;
        LED_setLED( &test, 24 );
        LED_setLED( &test, 96 );
        test.colour.green = 0;
        test.colour.blue = 255;
        LED_setLED( &test, 36 );
        LED_setLED( &test, 108 );
        test.colour.red = 0;
        test.colour.green = 255;
        test.colour.blue = 0;
        LED_setLED( &test, 48 );
        LED_setLED( &test, 120 );
        test.colour.red = 125;
        test.colour.green = 0;
        test.colour.blue = 255;
        LED_setLED( &test, 60 );
        LED_setLED( &test, 131 );
        set = true;
    }
    
    /* Setup dithered rainbow across the strip */
    /* Dithering: 18 LEDs/colour, middle two = 255 */
    /* Each LED decreases by 32 */
    /* Need to figure out easy way to do relative brightness... */
   
    led_shiftStrip(true);
    
}

void led_swirlyColours( bool reset )
{
    static uint8_t sec_led = 0;
    
    static bool up = false;
    static bool col = true;
    
    static LED_Data_t singleLed = {
        .colour.green   = 225,
        .colour.red     = 0,
        .colour.blue    = 25,
    };
    
    static LED_Data_t onLED = {
        .colour.green   = 0,
        .colour.red     = 127,
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
    static uint8_t currState = 0;
    switch( pattern )
    {
        case RUNNING:
            if((currState != RUNNING) || !(LEDFlag & _LS(PTRN)))
            {
                currState = RUNNING; 
                led_runningRainbow( true );
            }else{
                led_runningRainbow( false );
            }
            break;
        case RAINBOW:
            if(currState != RAINBOW || !(LEDFlag & _LS(PTRN)))
            {
                currState = RAINBOW;
                led_fadingRainbow( true );
            }else{
                led_fadingRainbow( false );
            }
            break;
        case SWIRL:
            if(currState != SWIRL || !(LEDFlag & _LS(PTRN)))
            {
                currState = SWIRL;
                led_swirlyColours( true );
            }else{
                led_swirlyColours( false );
            }
            break;
        default:
            break;
    }
}

void TASK_outputFormingLED( void *pvParameters )
{
    UNUSED( pvParameters );
    
    uint8_t     *pLEDArray       = ledArray;
    
    /* Default setting */
    LED_Packet_t rxLED;
    rxLED.cmd = RGB;
    LEDFlag = 0;
    
    for(;;)
    {
        if( xLEDQueue != 0 )
        {
            xQueueReceive( xLEDQueue, &rxLED, 0 );
        }
        
        switch( rxLED.cmd )
        {
            case RGB:
                LEDFlag = _LS(RGB);
                led_setStrip( &rxLED.LED );
                break;
            case BLNK:
                LEDFlag = _LS(BLNK);
                if(rxLED.period <= 0)
                    led_setStrip( &rxLED.LED );
                else
                    led_blink( &rxLED.LED, rxLED.period );
                break;
            case PTRN:
                led_pattern( rxLED.pattern );
                LEDFlag = _LS(PTRN);
                break;
            case AUD:
                LEDFlag = _LS(AUD);
                break;
            default:
                break;
        }

        taskENTER_CRITICAL();
        ws2812_sendarray(pLEDArray, LED_NUM * 3);
        taskEXIT_CRITICAL();
        
        /* Yield to oncoming traffic */
        vTaskDelay(WAIT_TIME_MS);
    }
}