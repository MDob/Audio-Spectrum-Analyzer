/*
 * usart_callback.c
 *
 * Created: 2015-10-24 3:56:07 PM
 *  Author: Michael
 */ 

#include "usart_cfg.h"

void usart_rxCallbackFTDI( struct usart_module *const usart_module )
{
	usart_write_buffer_job(	&usart_instanceFTDI,
							(uint8_t *)rx_bufferFTDI,
							MAX_CALLBACK_BUFFER_LEN);
}

void usart_txCallbackFTDI( struct usart_module *const usart_module )
{
	
}

void usart_rxCallbackBluetooth( struct usart_module *const usart_module )
{
	usart_write_buffer_job(	&usart_instanceBluetooth,
							(uint8_t *)rx_bufferBluetooth,
							MAX_CALLBACK_BUFFER_LEN);
}

void usart_txCallbackBluetooth( struct usart_module *const usart_module )
{
	
}