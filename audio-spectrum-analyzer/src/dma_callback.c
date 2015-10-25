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
#include "FreeRTOS.h"
#include "semphr.h"

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
    /* Set status flag */
    DMA_Status |= _LS( LED_TX_DONE );
}

void dma_LEDRxDone( struct dma_resource* const resource )
{
    static portBASE_TYPE wake_task = pdFALSE;

    /* Ensure that the semaphore exists */
    Assert( rxSemaphoreLED );
       
    /* Continue reading the incoming LED data stream */
    dma_start_transfer_job( &zDMA_LEDResourceRx );
    
    /* Unblock parsing task */
    xSemaphoreGiveFromISR( rxSemaphoreLED, &wake_task );
    
    /* Allow the parsing task to take over */
    if( wake_task == pdTRUE )
    {
        portYIELD();
    }
}

void dma_FTDITxDone( struct dma_resource* const resource )
{
    /* Set status flag */
    DMA_Status |= _LS( FTDI_TX_DONE );
}

void dma_FTDIRxDone( struct dma_resource* const resource )
{
    static portBASE_TYPE wake_task = pdFALSE;
    
    /* Unblock parsing task */
    xSemaphoreGiveFromISR( rxSemaphoreFTDI, &wake_task );
    
    /* Allow the parsing task to take over */
    if( wake_task == pdTRUE )
    {
        /* Continue reading the incoming FTDI data stream */
        dma_start_transfer_job(&zDMA_FTDIResourceRx);
        
        portYIELD();
    }    
}

void dma_BluetoothTxDone( struct dma_resource* const resource )
{
    /* Set status flag */
    DMA_Status |= _LS( BT_TX_DONE );
}

void dma_BluetoothRxDone( struct dma_resource* const resource )
{
    static portBASE_TYPE wake_task = pdFALSE;
    
    /* Ensure that the semaphore exists */
    Assert( rxSemaphoreBluetooth );
    
    /* Continue reading the incoming Bluetooth data stream */
    dma_start_transfer_job( &zDMA_BluetoothResourceRx );
    
    /* Unblock parsing task */
    xSemaphoreGiveFromISR( rxSemaphoreBluetooth, &wake_task );
    
    /* Allow the parsing task to take over */
    if( wake_task == pdTRUE )
    {
        portYIELD();
    }
}