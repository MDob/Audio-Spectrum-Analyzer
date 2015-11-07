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
    
    static char rxBuffer[FTDI_MAX_RX_LEN];
    static char *Rx = rxBuffer;
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
            /* Look for backspace character */
            if( *FTDI_RxBuffer == 127 )
            {
                if( Rx != rxBuffer )
                {
                    Rx--;
                    *Rx = 0;
                }
            }
            else if( *FTDI_RxBuffer == 13 )
            {
                memcpy( Rx, "\0", sizeof( char ) );
                /* Pass command to the main parser */
                if( xParserQueue != 0 )
                {
                    xQueueSend( xParserQueue, rxBuffer, ( TickType_t ) 10 );
                }
                Rx = rxBuffer;
            }
            else if( !strcmp( ( const char * ) FTDI_RxBuffer, "\027[D" ) ) // Left arrow ANSI
            {
                /* Move pointer around */
            }
            else if( !strcmp( ( const char * ) FTDI_RxBuffer, "\027[C" ) ) // Right arrow ANSI
            {
                /* Move pointer around */
            }
            else if( !strcmp( ( const char * ) FTDI_RxBuffer, "\027[A" ) ) // Up arrow ANSI
            {
                /* Previous command */
            }
            else if( !strcmp( ( const char * ) FTDI_RxBuffer, "\027[B" ) ) // Down arrow ANSI
            {
                /* Next command, if available */
            }
            else
            {
                /* Copy byte into buffer */
                *Rx = ( char ) *FTDI_RxBuffer;
                
                /* Reset buffer pointer on overflow */
                if( Rx == &rxBuffer[FTDI_MAX_RX_LEN-1] )
                {
                    Rx = rxBuffer;
                }
                else
                {
                    Rx++;
                }
            }
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
        
        /* Parse AT commands */
        
        /* Respond with ACK/NAK */
        
        /* Send AT command back if necessary */
        dma_start_transfer_job( &zDMA_BluetoothResourceTx );
        
        /* Yield to oncoming traffic */
        taskYIELD();
    }
}