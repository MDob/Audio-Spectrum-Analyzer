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

#include "FreeRTOS.h"
#include "task.h"

/*======================================================================*/
/*                         FUNCTION DECLARATIONS                        */
/*======================================================================*/
void TASK_EchoFTDI( void *pvParameters )
{
    /* Start DMA reception */
    dma_start_transfer_job( &zDMA_FTDIResourceRx );

    UNUSED(pvParameters);

    for(;;)
    {
        if( *pDMA_Status & ( 1 << FTDI_RX_DONE ) )
        {
            *pDMA_Status &= ~( 1 << FTDI_RX_DONE );
            memcpy(FTDI_TxBuffer, (const uint8_t* ) FTDI_RxBuffer, sizeof(FTDI_RxBuffer));
            dma_start_transfer_job( &zDMA_FTDIResourceTx );
        }
        taskYIELD();
    }
}