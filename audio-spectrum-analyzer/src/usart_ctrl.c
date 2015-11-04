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
void TASK_FTDI( void *pvParameters )
{
    UNUSED( pvParameters );
    
    char Tx[FTDI_TX_BUFFER_LEN];
    
    /* Create a semaphore */
    rxSemaphoreFTDI = xSemaphoreCreateBinary();
    
    /* Ensure that semaphore is valid */
    Assert( rxSemaphoreFTDI );
    
    /* Create a queue */
    xFTDITxQueue = xQueueCreate( 3, FTDI_TX_BUFFER_LEN * sizeof( char ) );
    xFTDIRxQueue = xQueueCreate( FTDI_MAX_RX_LEN, sizeof( uint16_t ) );
    
    /* Ensure that the queue is valid */
    Assert( xFTDITxQueue );
    Assert( xFTDIRxQueue );

    /* Start reading */
    dma_start_transfer_job( &zDMA_FTDIResourceRx );

    for(;;)
    {
        if( xQueueReceive( xFTDITxQueue, Tx, ( TickType_t ) 0 ) == pdTRUE )
        {
            strncpy((char *)FTDI_TxBuffer, Tx, sizeof(FTDI_TxBuffer));
            dma_start_transfer_job(&zDMA_FTDIResourceTx);
            while( !( *pDMA_Status & _LS(FTDI_TX_DONE) ) )
            {
                taskYIELD();
            }
             *pDMA_Status &= !(_LS(FTDI_TX_DONE));
        }
        
        /* Block task until DMA read complete */
        if( xSemaphoreTake( rxSemaphoreFTDI, 5 ) == pdTRUE )
        {
            if( xFTDITxQueue != 0)
            {
                xQueueSend( xFTDITxQueue, FTDI_RxBuffer, ( TickType_t ) 0 );
            }            
            xQueueSend( xFTDIRxQueue, FTDI_RxBuffer, ( TickType_t ) 0 );
            dma_start_transfer_job( &zDMA_FTDIResourceRx );
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