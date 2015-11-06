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
//void LED_allocateMemory( void )
//{
    /* Calculate 32-bit blocks to allocate */
    //uint32_t bytesToAllocate =  ( uint32_t ) CEILING( ( SPI_BITS_PER_LED * LED_NUM ), 32 );
    
    /* Allocate memory */
    //LED_TxBuffer = ( uint32_t* ) pvPortMalloc( bytesToAllocate * sizeof(uint8_t ));
//}

/*void LED_deallocateMemory( void )
{
    vPortFree( LED_TxBuffer );
}*/

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

/* WIP */
void LED_setStripBrightnessRelative( uint8_t relativeBrightness )
{
    //uint8_t LED_index = 0;
    //uint8_t maxLevel = 0;
    
    /* Keep running maximum */
    
    /* Change relative brightness of all LEDs */
    /* Make this hella efficient */
    
}