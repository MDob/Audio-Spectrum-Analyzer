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
    
    /* Start DMA reception */
    dma_start_transfer_job( &zDMA_FTDIResourceRx );

    for(;;)
    {
        /* Block task until DMA read complete */
        xSemaphoreTake( rxSemaphoreFTDI, portMAX_DELAY );
        
        /* Copy reception buffer to transmission buffer */
        memcpy( FTDI_TxBuffer, (const uint8_t* ) FTDI_RxBuffer, sizeof( FTDI_RxBuffer ) );
        
        /* Send the buffer using DMA */
        dma_start_transfer_job( &zDMA_FTDIResourceTx );

        while( !(*pDMA_Status & _LS(FTDI_TX_DONE)))
        {
            taskYIELD();
        }
        
        *pDMA_Status &= ~_LS(FTDI_TX_DONE);
        
        /* Keep reading */
        //dma_start_transfer_job( &zDMA_FTDIResourceRx );
        
        /* Yield to oncoming traffic */
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