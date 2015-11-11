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

/*======================================================================*/
/*                            MACRO DEFINITIONS                         */
/*======================================================================*/
#define CEILING( x, y ) ( ( ( x ) + ( y ) - 1 ) / ( y ) )

/*======================================================================*/
/*                           FUNCTION PROTOTYPES                        */
/*======================================================================*/


/*======================================================================*/
/*                          FUNCTION DECLARATIONS                       */
/*======================================================================*/

/*  If passed a pointer to an LED instance, set the relative brightness in it.
    Otherwise, set the LED at the point on the strip indicated by num */
void LED_setRelBrightness( LED_Data_t* pLED, uint16_t num, uint8_t relBrightness )
{
    LED_Data_t LED;
    
    if( pLED )
    {
        /* Get current brightness for that LED */
        uint16_t sumSqr =   ( pLED->data[0] * pLED->data[0] ) +
                            ( pLED->data[1] * pLED->data[1] ) +
                            ( pLED->data[2] * pLED->data[2] );
                            
        /* Find fixed-point divisor based on relBrightness */
        
        /* Set LED brightness */
        
    }
    else
    {
        /* Get brightness on strip LED */
        LED_getLED( &LED, num );
        
        uint16_t sumSqr =   ( LED.data[0] * LED.data[0] ) +
                            ( LED.data[1] * LED.data[1] ) +
                            ( LED.data[2] * LED.data[2] );
        
        /* Find fixed-point divisor based on relBrightness */
        
        /* Set brightness on the strip */
        LED_setLED( &LED, num );
        
    }
}

void LED_getLED( LED_Data_t* LED, uint16_t num )
{
    memcpy( LED->data, &ledArray[BYTES_PER_LED * num], sizeof( uint8_t ) * BYTES_PER_LED );
}

void LED_setLED( LED_Data_t* LED, uint16_t num )
{
    uint8_t *pLEDArray = ledArray;
    
    pLEDArray += sizeof( uint8_t ) * BYTES_PER_LED * num;
    memcpy( pLEDArray, LED->data, sizeof( uint8_t ) * BYTES_PER_LED );
}

void LED_setStripUniform( LED_Data_t* colourData )
{
    uint8_t LED_index = 0;
    uint8_t *pLEDArray = ledArray;
    
    while( LED_index < LED_NUM )
    {
        memcpy( pLEDArray, colourData->data, sizeof( uint8_t ) * BYTES_PER_LED );
        pLEDArray += sizeof( uint8_t ) * BYTES_PER_LED;
    }
}