/*************************************************************************
 *  File Name:      usart_cfg.c
 *  Author:         Michael Dobinson
 *  Date Created:   2015-10-24
 *
 *  Brief:
 *      Contains configuration functions for USART
 *
 *  [Compiled and tested with Atmel Studio 7]
 *
*************************************************************************/

/*======================================================================*/
/*                        LOCAL DEPENDENCIES                            */
/*======================================================================*/
#include "usart_cfg.h"

/*======================================================================*/
/*                      LOCAL FUNCTION PROTOTYPES                       */
/*======================================================================*/
void usart_configureCallbacks( void );
void usart_configureBluetooth( void );
void usart_configureFTDI( void );

/*======================================================================*/
/*                        FUNCTION DECLARATIONS                         */
/*======================================================================*/
void USART_init( void )
{
    usart_configureBluetooth();
    usart_configureFTDI();
    usart_configureCallbacks();
}

void usart_configureCallbacks( void )
{
    /* Register FTDI Callbacks */
    #ifndef FTDI_USE_DMA
    usart_register_callback(&usart_instanceFTDI,
                            usart_txCallbackFTDI, USART_CALLBACK_BUFFER_TRANSMITTED);
    usart_register_callback(&usart_instanceFTDI,
                            usart_rxCallbackFTDI, USART_CALLBACK_BUFFER_RECEIVED);
                            
    usart_enable_callback(&usart_instanceFTDI, USART_CALLBACK_BUFFER_TRANSMITTED);
    usart_enable_callback(&usart_instanceFTDI, USART_CALLBACK_BUFFER_RECEIVED);
    #endif
    
    /* Register Bluetooth Callbacks */
    #ifndef BT_USE_DMA
    usart_register_callback(&usart_instanceBluetooth,
                            usart_txCallbackBluetooth, USART_CALLBACK_BUFFER_TRANSMITTED);
    usart_register_callback(&usart_instanceBluetooth,
                            usart_rxCallbackBluetooth, USART_CALLBACK_BUFFER_RECEIVED);
    usart_enable_callback(&usart_instanceBluetooth, USART_CALLBACK_BUFFER_TRANSMITTED);
    usart_enable_callback(&usart_instanceBluetooth, USART_CALLBACK_BUFFER_RECEIVED);
    #endif
}

void usart_configureBluetooth( void )
{
    struct usart_config bluetooth_config;
    
    usart_get_config_defaults(&bluetooth_config);
    
    bluetooth_config.baudrate       = BAUDRATE_BLUETOOTH;
    bluetooth_config.mux_setting    = BT_USART_SERCOM_MUX_SETTING;
    bluetooth_config.pinmux_pad0    = BT_USART_SERCOM_PINMUX_PAD0;
    bluetooth_config.pinmux_pad1    = BT_USART_SERCOM_PINMUX_PAD1;
    bluetooth_config.pinmux_pad2    = BT_USART_SERCOM_PINMUX_PAD2;
    bluetooth_config.pinmux_pad3    = BT_USART_SERCOM_PINMUX_PAD3;
    
    while ( usart_init(&usart_instanceBluetooth,
            BT_USART_MODULE, &bluetooth_config) != STATUS_OK );
    
    usart_enable(&usart_instanceBluetooth);
}

void usart_configureFTDI( void )
{
    struct usart_config FTDI_config;
    
    usart_get_config_defaults(&FTDI_config);
    
    FTDI_config.baudrate    = BAUDRATE_FTDI;
    FTDI_config.mux_setting = FTDI_USART_SERCOM_MUX_SETTING;
    FTDI_config.pinmux_pad0 = FTDI_USART_SERCOM_PINMUX_PAD0;
    FTDI_config.pinmux_pad1 = FTDI_USART_SERCOM_PINMUX_PAD1;
    FTDI_config.pinmux_pad2 = FTDI_USART_SERCOM_PINMUX_PAD2;
    FTDI_config.pinmux_pad3 = FTDI_USART_SERCOM_PINMUX_PAD3;
    
    while ( usart_init(&usart_instanceFTDI,
            FTDI_USART_MODULE, &FTDI_config) != STATUS_OK );
    
    usart_enable(&usart_instanceFTDI);
    
}