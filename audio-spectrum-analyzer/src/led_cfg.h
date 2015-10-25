/*************************************************************************
 *  File Name:      led_cfg.h
 *  Author:         Michael Dobinson
 *  Date Created:   2015-10-24
 *
 *  Sub-files:
 *      led_ctrl.c
 *
 *  Brief:
 *      Provides configuration settings for LED control
 *
 *  [Compiled and tested with Atmel Studio 7]
 *
*************************************************************************/
#ifndef LED_CTRL_H_
#define LED_CTRL_H_

/*======================================================================*/
/*                          GLOBAL DEPENDENCIES                         */
/*======================================================================*/
#include "compiler.h"
#include "FreeRTOS.h"
#include "dma_cfg.h"

/*======================================================================*/
/*                      GLOBAL CONSTANT DEFINITIONS                     */
/*======================================================================*/
#define LED_TYPE    WS2812B
#define LED_NUM     144

#define LED_1_CODE          0b11100000000000000000000000000000
#define LED_0_CODE          0b10000000000000000000000000000000

#define CODES_PER_32BITS    6
#define LED_NUM             144
#define LED_BITS_PER_BIT    4
#define LED_BITS_PER_LED    24
#define SPI_BITS_PER_LED    LED_BITS_PER_BIT * LED_BITS_PER_LED

#define BYTES_PER_LED       3

/*======================================================================*/
/*                            TYPE DEFINITIONS                          */
/*======================================================================*/
typedef union LED_Data 
{
    uint8_t data[3];
    
    struct {
        uint8_t green;
        uint8_t red;
        uint8_t blue;
    }colour;
    
}LED_Data;

/*======================================================================*/
/*                      GLOBAL VARIABLE DEFINITIONS                     */
/*======================================================================*/
uint8_t ledArray[LED_NUM * 3];

/*======================================================================*/
/*                     EXTERNAL FUNCTION PROTOTYPES                     */
/*======================================================================*/
void TASK_outputFormingLED( void *pvParameters );

/*======================================================================*/
/*                          FUNCTION PROTOTYPES                         */
/*======================================================================*/
void LED_allocateMemory( void );
void LED_deallocateMemory( void );

void LED_setLED( LED_Data* LED, uint16_t num );
void LED_setStripUniform( LED_Data* colourData );
void LED_setStripBrightnessRelative( uint8_t relativeBrightness );

#endif /* LED_CTRL_H_ */