/*************************************************************************
 *  File Name:      usart_callback.c
 *  Author:         Michael Dobinson
 *  Date Created:   2015-10-24
 *
 *  Brief:
 *      Contains callback functions for USART
 *
 *  [Compiled and tested with Atmel Studio 7]
 *
*************************************************************************/

/*======================================================================*/
/*                          LOCAL DEPENDENCIES                          */
/*======================================================================*/
#include "usart_cfg.h"
#include "dma_cfg.h"

/*======================================================================*/
/*                        FUNCTION DECLARATIONS                         */
/*======================================================================*/
void usart_rxCallbackFTDI( struct usart_module *const usart_module )
{
    usart_write_buffer_job( &usart_instanceFTDI,
                            (uint8_t *)rx_bufferFTDI,
                            MAX_CALLBACK_BUFFER_LEN);
}

void usart_txCallbackFTDI( struct usart_module *const usart_module )
{
    
}

void usart_rxCallbackBluetooth( struct usart_module *const usart_module )
{
    usart_write_buffer_job( &usart_instanceBluetooth,
                            (uint8_t *)rx_bufferBluetooth,
                            MAX_CALLBACK_BUFFER_LEN);
}

void usart_txCallbackBluetooth( struct usart_module *const usart_module )
{
    
}