/*************************************************************************
 *  File Name:      usart_cfg.h
 *  Author:         Michael Dobinson
 *  Date Created:   2015-10-23
 *
 *  Sub-files:
 *      usart_cfg.c
 *      usart_callback.c
 *      usart_ctrl.c
 *
 *  Brief:
 *      Provides configuration settings for USART
 *
 *  [Compiled and tested with Atmel Studio 7]
 *
*************************************************************************/
#ifndef USART_CFG_H_
#define USART_CFG_H_

/*======================================================================*/
/*                          GLOBAL DEPENDENCIES                         */
/*======================================================================*/
#include "compiler.h"
#include "user_board.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#include "usart.h"
#include "usart_interrupt.h"
#include "dma.h"

/*======================================================================*/
/*                      GLOBAL CONSTANT DEFINITIONS                     */
/*======================================================================*/
#define BAUDRATE_BLUETOOTH          9600
#define BAUDRATE_FTDI               9600

#define MAX_CALLBACK_BUFFER_LEN     5
#define FTDI_MAX_RX_LEN             32

/*======================================================================*/
/*                      GLOBAL VARIABLE DECLARATIONS                    */
/*======================================================================*/
struct usart_module usart_instanceBluetooth;
struct usart_module usart_instanceFTDI;

uint8_t rx_bufferFTDI[MAX_CALLBACK_BUFFER_LEN];
uint8_t rx_bufferBluetooth[MAX_CALLBACK_BUFFER_LEN];

SemaphoreHandle_t txSemaphoreFTDI, rxSemaphoreFTDI;
SemaphoreHandle_t txSemaphoreBluetooth, rxSemaphoreBluetooth;

QueueHandle_t xFTDITxQueue, xFTDIRxQueue;
QueueHandle_t xBTTxQueue, xBTRxQueue;
QueueHandle_t xParserQueue;

/*======================================================================*/
/*                      EXTERNAL FUNCTION PROTOTYPES                    */
/*======================================================================*/
void usart_rxCallbackFTDI( struct usart_module *const usart_module );
void usart_txCallbackFTDI( struct usart_module *const usart_module );

void usart_rxCallbackBluetooth( struct usart_module *const usart_module );
void usart_txCallbackBluetooth( struct usart_module *const usart_module );

void TASK_FTDI      ( void *pvParameters );
void TASK_Bluetooth ( void *pvParameters );

/*======================================================================*/
/*                        FUNCTION PROTOTYPES                           */
/*======================================================================*/
void USART_init( void );

#endif /* USART_CFG_H_ */