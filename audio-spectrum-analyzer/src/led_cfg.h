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
#define MAX_LED_NUM 144

#define LED_1_CODE          0b11100000000000000000000000000000
#define LED_0_CODE          0b11000000000000000000000000000000
#define INPUT_MASK			0b10000000

#define LED_BITS_PER_BIT    4
#define LED_BITS_PER_LED    24
#define SPI_BITS_PER_LED    LED_BITS_PER_BIT * LED_BITS_PER_LED

#define BYTES_PER_LED       3
#define WAIT_TIME_MS        20

#define T1H 900
#define T1L 600
#define T0H 400
#define T0L 900
#define RES 6000

#define BASS_L_LEN      45
#define BASS_R_LEN      45
#define MID_L_LEN       19
#define MID_R_LEN       19
#define TREB_LEN        16

/*======================================================================*/
/*                            TYPE DEFINITIONS                          */
/*======================================================================*/
typedef enum {
    RGB,
    PTRN,
    INP,
    OUT,
    SET,
    CURR,
    BLNK,
    AUD,
    NUM_CMD,
}cmdType;

typedef enum {
    RAINBOW,
    SWIRL,
    RUNNING,
    FILM,
    NUM_PATTERN,
}patternType;

typedef union LED_Data 
{
    uint8_t data[3];
    
    struct {
        uint8_t green;
        uint8_t red;
        uint8_t blue;
    }colour;
    
}LED_Data_t;

typedef struct LED_Packet
{
    cmdType     cmd;
    patternType pattern;
    LED_Data_t  LED;
    uint32_t    period;
}LED_Packet_t;

typedef union spectrumCast {
    uint8_t array[MAX_LED_NUM * 3];
    
    struct{
        uint8_t bassL[BASS_L_LEN * 3];
        uint8_t midsL[MID_L_LEN * 3];
        uint8_t treb[TREB_LEN * 3];
        uint8_t midsR[MID_R_LEN * 3];
        uint8_t bassR[BASS_R_LEN * 3];
    }freq;
}spectrumCast_t;

/*======================================================================*/
/*                      GLOBAL VARIABLE DEFINITIONS                     */
/*======================================================================*/
uint8_t ledArray[MAX_LED_NUM * 3];
SemaphoreHandle_t LEDSemaphore;

enum colourNames{
    COLOUR_RED,
    COLOUR_ORANGE,
    COLOUR_YELLOW,
    COLOUR_GREEN,
    COLOUR_AQUA,
    COLOUR_BLUE,
    COLOUR_PURPLE,
    COLOUR_PINK,
    
    MAX_COLOURS
};

uint16_t LEDFlag;
QueueHandle_t xLEDQueue;

/*======================================================================*/
/*                     EXTERNAL FUNCTION PROTOTYPES                     */
/*======================================================================*/
void TASK_outputFormingLED( void *pvParameters );

/*======================================================================*/
/*                          FUNCTION PROTOTYPES                         */
/*======================================================================*/
void LED_setLED( LED_Data_t* LED, uint16_t num );
void LED_getLED( LED_Data_t* LED, uint16_t num );
void LED_setStripUniform( LED_Data_t* colourData );
void LED_setRelBrightness( LED_Data_t* pLED, uint16_t num, uint8_t relBrightness );

#endif /* LED_CTRL_H_ */