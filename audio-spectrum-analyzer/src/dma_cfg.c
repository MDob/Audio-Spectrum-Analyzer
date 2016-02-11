/*************************************************************************
 *  File Name:      dma_cfg.c
 *  Author:         Michael Dobinson
 *  Date Created:   2015-10-23
 *
 *  Brief:
 *      Contains configuration functions for DMA
 *
 *  [Compiled and tested with Atmel Studio 7]
 *
*************************************************************************/

/*======================================================================*/
/*                           LOCAL DEPENDENCIES                         */
/*======================================================================*/
#include "dma_cfg.h"

/*======================================================================*/
/*                           FUNCTION PROTOTYPES                        */
/*======================================================================*/
void dma_configureResource(struct dma_resource *resource, uint8_t trigger);

void dma_setupDescriptor(   DmacDescriptor *descriptor, uint32_t srcAddr,
                            uint32_t destAddr, uint16_t bufferLen, 
                            uint8_t beat_size, bool src_inc, bool dest_inc );

/*======================================================================*/
/*                          FUNCTION DECLARATIONS                       */
/*======================================================================*/
void DMA_init( void )
{   
    /* LED DMA Init */
    #ifdef USING_SPI
    DMA_configureLEDSPI();
    #endif
    
    //DMA_configureLEDPWM();
    
    /* FTDI DMA Init */
    DMA_configureFTDI();
    
    /* Bluetooth DMA Init */
    DMA_configureBluetooth();
}

void dma_configureResource( struct dma_resource *resource, uint8_t trigger )
{
    struct dma_resource_config config;
    
    dma_get_config_defaults( &config );
    
    /* Setup trigger and action configuration */
    config.peripheral_trigger = trigger;
    config.trigger_action = DMA_TRIGGER_ACTON_BEAT;
    
    /* Allocate DMA channel for resource */
    dma_allocate( resource, &config );
}

void dma_setupDescriptor(   DmacDescriptor *descriptor, uint32_t srcAddr,
                            uint32_t destAddr, uint16_t bufferLen, 
                            uint8_t beat_size, bool src_inc, bool dest_inc )
{
    struct dma_descriptor_config descriptor_config;
    
    dma_descriptor_get_config_defaults( &descriptor_config );
    
    /* Configure descriptor */
    descriptor_config.beat_size             = beat_size;
    descriptor_config.dst_increment_enable  = dest_inc;
    descriptor_config.src_increment_enable  = src_inc;
    descriptor_config.block_transfer_count  = bufferLen;
    descriptor_config.source_address        = srcAddr;
    descriptor_config.destination_address   = destAddr;
    
    /* Configure DMA descriptor with previous configuration */
    dma_descriptor_create( descriptor, &descriptor_config );
}


void DMA_configureLEDSPI( void )
{
    #ifdef USING_SPI
    uint32_t txSourceAddress        = ( uint32_t )LED_TxBuffer + sizeof( LED_TxBuffer );
    uint32_t txDestinationAddress   = ( uint32_t )( &spi_instanceLED.hw->SPI.DATA.reg );
    
    uint32_t rxSourceAddress        = ( uint32_t )( &spi_instanceLED.hw->SPI.DATA.reg );
    uint32_t rxDestinationAddress   = ( uint32_t )LED_RxBuffer + sizeof( LED_RxBuffer );
    
    /* Configure DMA Tx/Rx resources */
    dma_configureResource( &zDMA_LEDResourceTx, LED_SPI_SERCOM_DMAC_ID_TX );
    dma_configureResource( &zDMA_LEDResourceRx, LED_SPI_SERCOM_DMAC_ID_RX );
    
    /* Setup DMA Tx/Rx descriptors */
    dma_setupDescriptor(    &DMA_TxLEDDescriptor, txSourceAddress,
                            txDestinationAddress, LED_TX_BUFFER_LEN, 
                            DMA_BEAT_SIZE_HWORD, true, false );
                            
    dma_setupDescriptor(    &DMA_RxLEDDescriptor, rxSourceAddress,
                            rxDestinationAddress, LED_RX_BUFFER_LEN, 
                            DMA_BEAT_SIZE_HWORD, true, false );
    
    /* Add DMA Tx/Rx descriptors to DMA Tx/Rx resources */
    dma_add_descriptor( &zDMA_LEDResourceTx, &DMA_TxLEDDescriptor );
    dma_add_descriptor( &zDMA_LEDResourceRx, &DMA_RxLEDDescriptor );
    
    /* Register DMA Tx/Rx transfer callbacks */
    dma_register_callback( &zDMA_LEDResourceTx, dma_LEDTxDone, DMA_CALLBACK_TRANSFER_DONE );
    dma_register_callback( &zDMA_LEDResourceRx, dma_LEDRxDone, DMA_CALLBACK_TRANSFER_DONE );
    
    /* Enable DMA Tx/Rx callbacks */
    dma_enable_callback( &zDMA_LEDResourceTx, DMA_CALLBACK_TRANSFER_DONE );
    dma_enable_callback( &zDMA_LEDResourceRx, DMA_CALLBACK_TRANSFER_DONE );
    #endif
}

// void DMA_configureLEDPWM( void )
// {
//     uint32_t sourceAddress        = ( uint32_t )LED_PWMBuffer + sizeof( LED_PWMBuffer );
//     uint32_t destinationAddress   = ( uint32_t )( &LED_PWM_MODULE->CCB[LED_PWM_CHANNEL] );
//    
//     dma_configureResource( &zDMA_LEDResourcePWM, LED_PWM_DMAC_ID );
//     
//     dma_setupDescriptor(    &DMA_LedPWMDescriptor, sourceAddress, 
//                             destinationAddress, LED_PWM_BUFFER_LEN,
//                             DMA_BEAT_SIZE_BYTE, true, false );
//     
//     dma_add_descriptor( &zDMA_LEDResourcePWM, &DMA_LedPWMDescriptor );
//     //dma_add_descriptor( &zDMA_LEDResourcePWM, &DMA_LedPWMDescriptor );
//     
//     dma_register_callback( &zDMA_LEDResourcePWM, dma_LEDDone, DMA_CALLBACK_TRANSFER_DONE );
//     dma_enable_callback( &zDMA_LEDResourcePWM, DMA_CALLBACK_TRANSFER_DONE );
//     
//     //dma_start_transfer_job( &zDMA_LEDResourcePWM );
// }

void DMA_configureFTDI( void )
{
    uint32_t txSourceAddress        = ( uint32_t )FTDI_TxBuffer + sizeof( FTDI_TxBuffer );
    uint32_t txDestinationAddress   = ( uint32_t )( &usart_instanceFTDI.hw->USART.DATA.reg );
    
    uint32_t rxSourceAddress        = ( uint32_t )( &usart_instanceFTDI.hw->USART.DATA.reg );
    uint32_t rxDestinationAddress   = ( uint32_t )FTDI_RxBuffer + sizeof( FTDI_RxBuffer );
    
    /* Configure DMA Tx/Rx resources */
    dma_configureResource( &zDMA_FTDIResourceTx, FTDI_USART_SERCOM_DMAC_ID_TX );
    dma_configureResource( &zDMA_FTDIResourceRx, FTDI_USART_SERCOM_DMAC_ID_RX );
    
    /* Setup DMA Tx/Rx descriptors */
    dma_setupDescriptor(    &DMA_TxFTDIDescriptor, txSourceAddress,
                            txDestinationAddress, FTDI_TX_BUFFER_LEN,
                            DMA_BEAT_SIZE_BYTE, true, false );
                            
    dma_setupDescriptor(    &DMA_RxFTDIDescriptor, rxSourceAddress,
                            rxDestinationAddress, FTDI_RX_BUFFER_LEN,
                            DMA_BEAT_SIZE_BYTE, false, true );

    /* Add DMA Tx/Rx descriptors to DMA Tx/Rx resources */
    dma_add_descriptor( &zDMA_FTDIResourceTx, &DMA_TxFTDIDescriptor );
    dma_add_descriptor( &zDMA_FTDIResourceRx, &DMA_RxFTDIDescriptor );
    
    /* Register DMA Tx/Rx transfer callbacks */
    dma_register_callback( &zDMA_FTDIResourceTx, dma_FTDITxDone, DMA_CALLBACK_TRANSFER_DONE );
    dma_register_callback( &zDMA_FTDIResourceRx, dma_FTDIRxDone, DMA_CALLBACK_TRANSFER_DONE );
    
    /* Enable DMA Tx/Rx callbacks */
    dma_enable_callback( &zDMA_FTDIResourceTx, DMA_CALLBACK_TRANSFER_DONE );
    dma_enable_callback( &zDMA_FTDIResourceRx, DMA_CALLBACK_TRANSFER_DONE );
}

void DMA_configureBluetooth( void )
{
    uint32_t txSourceAddress        = ( uint32_t )Bluetooth_TxBuffer + sizeof( Bluetooth_TxBuffer );
    uint32_t txDestinationAddress   = ( uint32_t )( &usart_instanceBluetooth.hw->USART.DATA.reg );
    
    uint32_t rxSourceAddress        = ( uint32_t )( &usart_instanceBluetooth.hw->USART.DATA.reg );
    uint32_t rxDestinationAddress   = ( uint32_t )Bluetooth_RxBuffer + sizeof( Bluetooth_RxBuffer );
    
    /* Configure DMA Tx/Rx resources */
    dma_configureResource( &zDMA_BluetoothResourceTx, BT_USART_SERCOM_DMAC_ID_TX );
    dma_configureResource( &zDMA_BluetoothResourceRx, BT_USART_SERCOM_DMAC_ID_RX );
    
    /* Setup DMA Tx/Rx descriptors */
    dma_setupDescriptor(    &DMA_TxBluetoothDescriptor, txSourceAddress,
                            txDestinationAddress, BLUETOOTH_TX_BUFFER_LEN,
                            DMA_BEAT_SIZE_HWORD, true, false );
                            
    dma_setupDescriptor(    &DMA_RxBluetoothDescriptor, rxSourceAddress,
                            rxDestinationAddress, BLUETOOTH_RX_BUFFER_LEN,
                            DMA_BEAT_SIZE_HWORD, false, true );

    /* Add DMA Tx/Rx descriptors to DMA Tx/Rx resources */
    dma_add_descriptor( &zDMA_BluetoothResourceTx, &DMA_TxBluetoothDescriptor );
    dma_add_descriptor( &zDMA_BluetoothResourceRx, &DMA_RxBluetoothDescriptor );
    
    /* Register DMA Tx/Rx transfer callbacks */
    dma_register_callback( &zDMA_BluetoothResourceTx, dma_BluetoothTxDone, DMA_CALLBACK_TRANSFER_DONE );
    dma_register_callback( &zDMA_BluetoothResourceRx, dma_BluetoothRxDone, DMA_CALLBACK_TRANSFER_DONE );
    
    /* Enable DMA Tx/Rx callbacks */
    dma_enable_callback( &zDMA_BluetoothResourceTx, DMA_CALLBACK_TRANSFER_DONE );
    dma_enable_callback( &zDMA_BluetoothResourceRx, DMA_CALLBACK_TRANSFER_DONE );
}