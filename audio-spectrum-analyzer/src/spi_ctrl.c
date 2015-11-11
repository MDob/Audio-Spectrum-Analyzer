/*************************************************************************
 *  File Name:      spi_ctrl.c
 *  Author:         Michael Dobinson
 *  Date Created:   2015-10-24
 *
 *  Brief:
 *      Contains FreeRTOS tasks for SPI and other control
 *      related functions.
 *
 *  [Compiled and tested with Atmel Studio 7]
 *
*************************************************************************/

/*======================================================================*/
/*                          LOCAL DEPENDENCIES                          */
/*======================================================================*/
#include "spi_cfg.h"
#include "dma_cfg.h"
#include "task.h"

/*======================================================================*/
/*                         FUNCTION DECLARATIONS                        */
/*======================================================================*/
// void TASK_SendSPI( void *pvParameters )
// {
//     UNUSED( pvParameters );
//     
//     /* Create a semaphore */
//     //txSemaphoreLED = xSemaphoreCreateBinary();
//     
//     /* Ensure that semaphore is valid */
//     Assert( txSemaphoreLED );
//     
//     for(;;)
//     {
//         /* Take semaphore from led_ctrl */
//         //xSemaphoreTake( txSemaphoreLED, portMAX_DELAY );
//         
//         /* Start DMA transfer */
//         // dma_start_transfer_job( &zDMA_LEDResourceTx );
//     }
// }

// void TASK_ReadSPI( void *pvParameters )
// {
//     UNUSED( pvParameters );
//     
//     /* Create a semaphore */
//     rxSemaphoreLED = xSemaphoreCreateBinary();
//     
//     /* Ensure that semaphore is valid */
//     Assert( rxSemaphoreLED );
//     
//     /* Start DMA reception */
//     dma_start_transfer_job( &zDMA_LEDResourceRx );
//     
//     for(;;)
//     {
//         /* Block task until DMA read complete */
//         xSemaphoreTake( rxSemaphoreLED, portMAX_DELAY );
//         
//         /* Do something */
//         
//         /* Respond..? */
//         dma_start_transfer_job( &zDMA_LEDResourceTx );
//         
//         /* Yield to oncoming traffic */
//         taskYIELD();
//     }
// }