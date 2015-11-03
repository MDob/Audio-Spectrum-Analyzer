/*************************************************************************
 *  File Name:      usart_ctrl.c
 *  Author:         Michael Dobinson
 *  Date Created:   2015-10-24
 *
 *  Brief:
 *      Contains FreeRTOS tasks for USART and other control 
 *      related functions.
 *
 *  [Compiled and tested with Atmel Studio 7]
 *
*************************************************************************/

/*======================================================================*/
/*                          LOCAL DEPENDENCIES                          */
/*======================================================================*/
#include "usart_cfg.h"
#include "dma_cfg.h"
#include "comm.h"
#include "task.h"

/*======================================================================*/
/*                         FUNCTION DECLARATIONS                        */
/*======================================================================*/
void TASK_ReadFTDI( void *pvParameters )
{
    UNUSED( pvParameters );
    
    /* Create a semaphore */
    rxSemaphoreFTDI = xSemaphoreCreateBinary();
    
    /* Ensure that semaphore is valid */
    Assert( rxSemaphoreFTDI );
    
    /* Create a queue */
    xFTDIQueue = xQueueCreate( FTDI_MAX_RX_LEN, sizeof( uint16_t ) );
    
    /* Ensure that the queue is valid */
    Assert( xFTDIQueue );
    
    /* Start reading */
    dma_start_transfer_job( &zDMA_FTDIResourceRx );

    for(;;)
    {
        /* Block task until DMA read complete */
        if( xSemaphoreTake( rxSemaphoreFTDI, portMAX_DELAY ) == pdTRUE )
        {
            /* Copy reception buffer to transmission buffer */
            memset( FTDI_TxBuffer, 0x00, sizeof(FTDI_TxBuffer));
            memcpy( FTDI_TxBuffer, (const uint8_t* ) FTDI_RxBuffer, sizeof( FTDI_RxBuffer ) );
            
            /* Send the buffer using DMA */
            dma_start_transfer_job( &zDMA_FTDIResourceTx );
            xQueueSend( xFTDIQueue, FTDI_RxBuffer, ( TickType_t ) 5 );
            while( !(*pDMA_Status & _LS(FTDI_TX_DONE)));
            
            *pDMA_Status &= ~_LS(FTDI_TX_DONE);
            
            /* Keep reading */
            dma_start_transfer_job( &zDMA_FTDIResourceRx );
        }
        else
        {
            /* Yield to oncoming traffic */
            taskYIELD();
        }
        
        taskYIELD();
    }
}

void TASK_ReadBluetooth( void *pvParameters )
{
    UNUSED( pvParameters );
    
    /* Create a semaphore */
    rxSemaphoreBluetooth = xSemaphoreCreateBinary();
    
    /* Ensure that the semaphore is valid */
    Assert( rxSemaphoreBluetooth );
    
    /* Start DMA reception */
    dma_start_transfer_job( &zDMA_BluetoothResourceRx );

    for(;;)
    {
        /* Block task until DMA read complete */
        xSemaphoreTake( rxSemaphoreBluetooth, portMAX_DELAY );
        
        /* Do something */
        
        /* Respond..? */
        dma_start_transfer_job( &zDMA_BluetoothResourceTx );
        
        /* Yield to oncoming traffic */
        taskYIELD();
    }
}