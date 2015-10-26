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
    uint8_t     shift_index = 0;
    uint16_t    LED_index;
    uint8_t     byte_index;
    
    *outputBuffer = PWM_PERIOD_CYCLES;
    outputBuffer++;
    
   for( LED_index = 0; LED_index < ( num_led * BYTES_PER_LED ); LED_index++, inputArray++ )
    {
        for( byte_index = 0; byte_index <= 7; byte_index++ )
        {
            #ifdef USING_SPI
            if( *inputArray & _RSVAR( INPUT_MASK, byte_index ) )
            {
                *outputBuffer |= _RSVAR( LED_1_CODE, shift_index );
            }
            else
            {
                *outputBuffer |= _RSVAR( LED_0_CODE, shift_index );
            }
                
            shift_index += LED_BITS_PER_BIT;
                
            if( shift_index > 31 )
            {
                *( ++outputBuffer ) = 0;
                shift_index = shift_index - 32;
            }
            #endif
            
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
        //*(outputBuffer++) = PWM_PERIOD_CYCLES;
        //*(outputBuffer++) = PWM_PERIOD_CYCLES;
    }
    //outputBuffer--;
}

void TASK_outputFormingLED( void *pvParameters )
{
    UNUSED( pvParameters );
    uint8_t     *pLEDArray       = ledArray;
    #ifdef USING_SPI
    uint32_t    *pLED_TxBuffer   = LED_TxBuffer;
    #endif
    
    uint8_t     *pLEDPWMArray   = LED_PWMBuffer;
    
    /* Allocate memory for buffer */
    //LED_allocateMemory();
    
    //LEDSemaphore = xSemaphoreCreateBinary();
    //Assert( LEDSemaphore );
    
    LED_Data_t singleLed;
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
    
    LED_Data_t onLED1;
    onLED1.colour.green   = 255;
    onLED1.colour.red     = 0;
    onLED1.colour.blue    = 0;
    
    bool up = false;
    bool col = true;
    uint8_t sec_led = 0;
    
    for(;;)
    {
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
        
    
        /* Form the buffer */
        led_formTxBuffer( pLEDArray, pLEDPWMArray, LED_NUM );
        LED_PWMBuffer[LED_PWM_BUFFER_LEN-1] = PWM_PERIOD_CYCLES;
        
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
        vTaskDelay(10);
    }
}