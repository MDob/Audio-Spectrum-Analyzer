/*************************************************************************
 *  File Name:      dma_callback.c
 *  Author:         Michael Dobinson
 *  Date Created:   2015-10-24
 *
 *  Brief:
 *      Contains callback functions for DMA
 *
 *  [Compiled and tested with Atmel Studio 7]
 *
*************************************************************************/

/************************************************************************/
/*                          LOCAL DEPENDENCIES                          */
/************************************************************************/
#include "dma_cfg.h"

/************************************************************************/
/*                      GLOBAL VARIABLE DECLARATIONS                    */
/************************************************************************/
volatile uint8_t DMA_Status = 0;
volatile uint8_t *pDMA_Status = &DMA_Status;

/************************************************************************/
/*                         FUNCTION DECLARATIONS                        */
/************************************************************************/
void dma_LEDTxDone( struct dma_resource* const resource )
{
    /* Use semaphore when FreeRTOS is implemented */
    DMA_Status |= (1 << LED_TX_DONE);
}

void dma_LEDRxDone( struct dma_resource* const resource )
{
    /* Use semaphore when FreeRTOS is implemented */
    DMA_Status |= (1 << LED_RX_DONE);
}

void dma_FTDITxDone( struct dma_resource* const resource )
{
    /* Use semaphore when FreeRTOS is implemented */
    DMA_Status |= (1 << FTDI_TX_DONE);
    //dma_start_transfer_job(&zDMA_FTDIResourceRx);
}

void dma_FTDIRxDone( struct dma_resource* const resource )
{
    /* Use semaphore when FreeRTOS is implemented */
    DMA_Status |= (1 << FTDI_RX_DONE);
    dma_start_transfer_job(&zDMA_FTDIResourceRx);
}

void dma_BluetoothTxDone( struct dma_resource* const resource )
{
    /* Use semaphore when FreeRTOS is implemented */
    DMA_Status |= (1 << BT_TX_DONE);
}

void dma_BluetoothRxDone( struct dma_resource* const resource )
{
    /* Use semaphore when FreeRTOS is implemented */
    DMA_Status |= (1 << BT_RX_DONE);
}