/*************************************************************************
 *  File Name:      dma_cfg.h
 *  Author:         Michael Dobinson
 *  Date Created:   2015-10-23
 *
 *  Sub-files:
 *      dma_cfg.c
 *      dma_callback.c
 *
 *  Brief:
 *      Provides configuration settings for DMA
 *
 *  [Compiled and tested with Atmel Studio 7]
 *
*************************************************************************/
#ifndef DMA_CFG_H_
#define DMA_CFG_H_

/*======================================================================*/
/*                          GLOBAL DEPENDENCIES                         */
/*======================================================================*/
#include "compiler.h"
#include "user_board.h"
#include "dma.h"
#include "spi_cfg.h"
#include "usart_cfg.h"
#include "led_cfg.h"

/*======================================================================*/
/*                      GLOBAL CONSTANT DEFINITIONS                     */
/*======================================================================*/
#define LED_NUM                 144
#define INT_PER_LED             3

#define LED_TX_BUFFER_LEN       LED_NUM * INT_PER_LED
#define LED_RX_BUFFER_LEN       4

#define LED_BUFF_PADDING        1
#define LED_PWM_BUFFER_LEN      (LED_NUM * 24) + LED_BUFF_PADDING

#define FTDI_RX_BUFFER_LEN      1
#define FTDI_TX_BUFFER_LEN      64
#define BLUETOOTH_BUFFER_LEN    4

/*======================================================================*/
/*                      GLOBAL VARIABLE DEFINITIONS                     */
/*======================================================================*/

/* DMA Descriptors */
COMPILER_ALIGNED(16)
DmacDescriptor DMA_TxLEDDescriptor;
DmacDescriptor DMA_RxLEDDescriptor;

COMPILER_ALIGNED(16)
DmacDescriptor DMA_TxFTDIDescriptor;
DmacDescriptor DMA_RxFTDIDescriptor;

COMPILER_ALIGNED(16)
DmacDescriptor DMA_TxBluetoothDescriptor;
DmacDescriptor DMA_RxBluetoothDescriptor;

COMPILER_ALIGNED(16)
DmacDescriptor DMA_LedPWMDescriptor;

/* DMA Completion Flags */
/* Note: RX_DONE flags implemented with semaphores */
enum DMA_Status_Flags {
    LED_TX_DONE,
    LED_RX_DONE,
    
    FTDI_TX_DONE,
    FTDI_RX_DONE,
    
    BT_TX_DONE,
    BT_RX_DONE,
    
    MAX_DMA_STATUSES
};

/* Global DMA Resources */
struct dma_resource zDMA_LEDResourceTx;
struct dma_resource zDMA_LEDResourceRx;

struct dma_resource zDMA_LEDResourcePWM;

struct dma_resource zDMA_FTDIResourceTx;
struct dma_resource zDMA_FTDIResourceRx;

struct dma_resource zDMA_BluetoothResourceTx;
struct dma_resource zDMA_BluetoothResourceRx;

/* DMA Buffers */
#ifdef USING_SPI
uint32_t    LED_TxBuffer[LED_TX_BUFFER_LEN];
uint8_t     LED_RxBuffer[LED_RX_BUFFER_LEN];
#endif

uint16_t    FTDI_TxBuffer[FTDI_TX_BUFFER_LEN];
uint16_t    FTDI_RxBuffer[FTDI_RX_BUFFER_LEN];

uint16_t    Bluetooth_TxBuffer[BLUETOOTH_BUFFER_LEN];
uint16_t    Bluetooth_RxBuffer[BLUETOOTH_BUFFER_LEN];

uint8_t     LED_PWMBuffer[LED_PWM_BUFFER_LEN];

/*======================================================================*/
/*                    EXPORTED VARIABLE DEFINITIONS                     */
/*======================================================================*/
extern volatile uint8_t* pDMA_Status;

/*======================================================================*/
/*                    EXTERNAL FUNCTION PROTOTYPES                      */
/*======================================================================*/
void dma_LEDTxDone( struct dma_resource* const resource );
void dma_LEDRxDone( struct dma_resource* const resource );

void dma_FTDITxDone( struct dma_resource* const resource );
void dma_FTDIRxDone( struct dma_resource* const resource );

void dma_BluetoothTxDone( struct dma_resource* const resource );
void dma_BluetoothRxDone( struct dma_resource* const resource );

void dma_LEDDone( struct dma_resource* const resource );

/*======================================================================*/
/*                      FUNCTION PROTOTYPES                             */
/*======================================================================*/
void DMA_init( void );

void DMA_configureLEDSPI( void );
void DMA_configureLEDPWM( void );
void DMA_configureFTDI( void );
void DMA_configureBluetooth( void );

#endif /* DMA_CFG_H_ */