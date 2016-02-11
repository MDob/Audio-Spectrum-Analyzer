/*************************************************************************
 *  File Name:      comm.h
 *  Author:         Michael Dobinson
 *  Date Created:   2015-10-27
 *
 *  Sub-files:
 *      comm.c
 *
 *  Brief:
 *
 *  [Compiled and tested with Atmel Studio 7]
 *
*************************************************************************/

#ifndef COMM_H_
#define COMM_H_

/*======================================================================*/
/*                          GLOBAL DEPENDENCIES                         */
/*======================================================================*/
#include "user_board.h"
#include "compiler.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#include "spi.h"
#include "usart.h"

/*======================================================================*/
/*                      GLOBAL CONSTANT DEFINITIONS                     */
/*======================================================================*/

/* General */
#define BT_MAX_RX_LEN       32
#define PARSER_MAX_CMD_LEN  32
#define PARSER_MAX_ARG_LEN  4
#define PARSER_MAX_ARG      5

#define IDENTIFIER(a, b, c, d)   ( ( ( ( uint32_t ) a) << ( 3*8 ) )  + \
( ( ( uint32_t ) b) << ( 2*8 ) )  + \
( ( ( uint32_t ) c) << ( 1*8 ) )  + \
( ( ( uint32_t ) d) << ( 0*8 ) ) )

#define CMD_RGB     IDENTIFIER('r','g','b', 0 )
#define CMD_PTRN    IDENTIFIER('p','t','r','n')
#define CMD_INP     IDENTIFIER('i','n','p', 0 )
#define CMD_OUT     IDENTIFIER('o','u','t', 0 )
#define CMD_SET     IDENTIFIER('s','e','t', 0 )
#define CMD_CURR    IDENTIFIER('c','u','r','r')
#define CMD_BLNK    IDENTIFIER('b','l','n','k')
#define CMD_AUD     IDENTIFIER('a','u','d', 0 )
#define CMD_HELP_0  IDENTIFIER('h','e','l','p')
#define CMD_HELP_1  IDENTIFIER('?', 0,  0,  0 )
#define CMD_HELP_2  IDENTIFIER('/','?', 0,  0 )

/* USART */
#define BAUDRATE_BLUETOOTH      9600
#define BAUDRATE_FTDI           9600

#define MAX_CALLBACK_BUFFER_LEN 5
#define FTDI_MAX_RX_LEN         32
#define BLUETOOTH_MAX_RX_LEN    32

/* SPI */
#define SPI_BAUDRATE            3500000UL
#define BUFFER_LENGTH           9

/*======================================================================*/
/*                      GLOBAL VARIABLE DECLARATIONS                    */
/*======================================================================*/

/* General */

/* USART */
struct usart_module usart_instanceBluetooth;
struct usart_module usart_instanceFTDI;

uint8_t rx_bufferFTDI[MAX_CALLBACK_BUFFER_LEN];
uint8_t rx_bufferBluetooth[MAX_CALLBACK_BUFFER_LEN];

SemaphoreHandle_t txSemaphoreFTDI, rxSemaphoreFTDI;
SemaphoreHandle_t txSemaphoreBluetooth, rxSemaphoreBluetooth;

QueueHandle_t xFTDITxQueue, xFTDIRxQueue;
QueueHandle_t xBluetoothTxQueue, xBluetoothRxQueue;
QueueHandle_t xParserQueue;

/* SPI */
static const uint32_t const buffer_tx[BUFFER_LENGTH] = {
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

/*======================================================================*/
/*                      EXTERNAL FUNCTION PROTOTYPES                    */
/*======================================================================*/

/* General */

/* USART */

/* SPI */

/*======================================================================*/
/*                          FUNCTION PROTOTYPES                         */
/*======================================================================*/

/* General */
void COMM_init              ( void );
void TASK_bluetoothParser   ( void *pvParameters );
void TASK_mainParser        ( void *pvParameters );

/* USART */
void USART_init             ( void );
void TASK_FTDI              ( void *pvParameters );
void TASK_Bluetooth         ( void *pvParameters );

/* SPI */
void TASK_SPI               ( void *pvParameters );

#endif /* COMM_H_ */