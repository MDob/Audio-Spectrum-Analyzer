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

void TASK_outputFormingLED( void *pvParameters )
{
    UNUSED( pvParameters );
    
    uint8_t     *pLEDArray       = ledArray;    
    uint8_t     *pLEDPWMArray   = LED_PWMBuffer;
    
    /*LED_Data_t singleLed;
    singleLed.colour.green   = 45;
    singleLed.colour.red     = 225;
    singleLed.colour.blue    = 25;
    
    LED_Data_t offLED;
    offLED.colour.green   = 0;
    offLED.colour.red     = 0;
    offLED.colour.blue    = 0;
    
    LED_Data_t onLED;
    onLED.colour.green   = 127;
    onLED.colour.red     = 0;
    onLED.colour.blue    = 0; 
    
    bool up = false;
    bool col = true;
    uint8_t sec_led = 0; */
    
    LED_Data_t stripColour = {
        .colour.red     = 0,
        .colour.green   = 0,
        .colour.blue    = 0
    };
    
    uint8_t state = 0;
    
    for(;;)
    {
        for(uint8_t i = 0; i < LED_NUM; i++)
        {
            LED_setLED( &stripColour, i );
        }
        
        // Fade in/out rainbow thing
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
        
        // Swirly colour changer
        // Currently just a proof of concept
        /*
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
        }*/
    
        /* Form the buffer */
        led_formTxBuffer( pLEDArray, pLEDPWMArray, LED_NUM );
        //LED_PWMBuffer[LED_PWM_BUFFER_LEN-1] = PWM_PERIOD_CYCLES;
        
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
        vTaskDelay(20);
    }
}