/*************************************************************************
 *  File Name:      spi_cfg.h
 *  Author:         Michael Dobinson
 *  Date Created:   2015-10-23
 *
 *  Sub-files:
 *      spi_cfg.c
 *      spi_ctrl.c
 *
 *  Brief:
 *      Provides configuration settings for SPI
 *
 *  [Compiled and tested with Atmel Studio 7]
 *
*************************************************************************/
#ifndef SPI_CFG_H_
#define SPI_CFG_H_

/*======================================================================*/
/*                          GLOBAL DEPENDENCIES                         */
/*======================================================================*/
#include "compiler.h"
#include "user_board.h"
#include "FreeRTOS.h"
#include "semphr.h"

#include "spi.h"

/*======================================================================*/
/*                      GLOBAL CONSTANT DEFINITIONS                     */
/*======================================================================*/
#define SPI_BAUDRATE 4000000UL
#define BUFFER_LENGTH 9

/*======================================================================*/
/*                      GLOBAL VARIABLE DECLARATIONS                    */
/*======================================================================*/
static const uint32_t buffer_tx[BUFFER_LENGTH] = {
    0b10001000100010001000100010001000,
    0b10001000100010001000100010001000,
    0b10001000100010001000100010001000,
    
    0b10001000100010001000100010001000,
    0b10001000100010001000100010001000,
    0b10001000100010001000100010001000,
    
    0b10001000100010001000100010001000,
    0b11101110111011101110111011101110,
    0b10001000100010001000100010001000
};

struct spi_module spi_instanceLED;

SemaphoreHandle_t txSemaphoreLED;
SemaphoreHandle_t rxSemaphoreLED;

/*======================================================================*/
/*                      EXTERNAL FUNCTION PROTOTYPES                    */
/*======================================================================*/
void TASK_SendSPI( void *pvParameters );
void TASK_ReadSPI( void *pvParameters );

/*======================================================================*/
/*                          FUNCTION PROTOTYPES                         */
/*======================================================================*/
void SPI_configureLED(void);

#endif /* SPI_CFG_H_ */