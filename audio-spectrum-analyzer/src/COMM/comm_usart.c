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
#include "comm.h"
#include "task.h"
#include "usart.h"
#include "usart_interrupt.h"
#include "dma_cfg.h"

/*======================================================================*/
/*                      LOCAL FUNCTION PROTOTYPES                       */
/*======================================================================*/
void usart_configureCallbacks( void );
void usart_configureBluetooth( void );
void usart_configureFTDI( void );

void usart_txCallbackFTDI( struct usart_module *const usart_module );
void usart_rxCallbackFTDI( struct usart_module *const usart_module );

void usart_txCallbackBluetooth( struct usart_module *const usart_module );
void usart_rxCallbackBluetooth( struct usart_module *const usart_module );

/*======================================================================*/
/*                        FUNCTION DECLARATIONS                         */
/*======================================================================*/
void USART_init( void )
{
    usart_configureBluetooth();
    usart_configureFTDI();
    usart_configureCallbacks();
}

void usart_txCallbackFTDI( struct usart_module *const usart_module )
{
    
}


void usart_rxCallbackFTDI( struct usart_module *const usart_module )
{
    usart_write_buffer_job( &usart_instanceFTDI,
    (uint8_t *)rx_bufferFTDI,
    MAX_CALLBACK_BUFFER_LEN);
}

void usart_txCallbackBluetooth( struct usart_module *const usart_module )
{
    
}

void usart_rxCallbackBluetooth( struct usart_module *const usart_module )
{
    usart_write_buffer_job( &usart_instanceBluetooth,
    (uint8_t *)rx_bufferBluetooth,
    MAX_CALLBACK_BUFFER_LEN);
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
            while( !( DMA_Status & _LS(FTDI_TX_DONE) ) )
            {
                taskYIELD();
            }
            DMA_Status &= !(_LS(FTDI_TX_DONE));
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

void TASK_Bluetooth( void *pvParameters )
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