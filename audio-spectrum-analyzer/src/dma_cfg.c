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
void dma_configureResourceTx(struct dma_resource *tx_resource, uint8_t trigger);
void dma_configureResourceRx(struct dma_resource *rx_resource, uint8_t trigger);

void dma_setupDescriptorTx( DmacDescriptor *tx_descriptor, uint32_t srcAddr,
                            uint32_t destAddr, uint16_t bufferLen );
void dma_setupDescriptorRx( DmacDescriptor *rx_descriptor, uint32_t srcAddr,
                            uint32_t destAddr, uint16_t bufferLen );

/*======================================================================*/
/*                          FUNCTION DECLARATIONS                       */
/*======================================================================*/
void DMA_init( void )
{   
    /* LED DMA Init */
    DMA_configureLED();
    
    /* FTDI DMA Init */
    DMA_configureFTDI();
    
    /* Bluetooth DMA Init */
    DMA_configureBluetooth();
}

void dma_configureResourceTx( struct dma_resource *tx_resource, uint8_t trigger )
{
    struct dma_resource_config tx_config;
    
    dma_get_config_defaults( &tx_config );
    
    /* Setup trigger and action configuration */
    tx_config.peripheral_trigger = trigger;
    tx_config.trigger_action = DMA_TRIGGER_ACTON_BEAT;
    
    /* Allocate DMA channel for resource */
    dma_allocate( tx_resource, &tx_config );
}

void dma_configureResourceRx( struct dma_resource *rx_resource, uint8_t trigger )
{
    struct dma_resource_config rx_config;
    
    dma_get_config_defaults( &rx_config );
    
    /* Setup trigger and action configuration */
    rx_config.peripheral_trigger = trigger;
    rx_config.trigger_action = DMA_TRIGGER_ACTON_BEAT;
    
    /* Allocate DMA channel for resource */
    dma_allocate( rx_resource, &rx_config );
}

void dma_setupDescriptorTx( DmacDescriptor *tx_descriptor, uint32_t srcAddr,
                            uint32_t destAddr, uint16_t bufferLen )
{
    struct dma_descriptor_config tx_descriptor_config;
    
    dma_descriptor_get_config_defaults( &tx_descriptor_config );
    
    /* Configure descriptor */
    tx_descriptor_config.beat_size              = DMA_BEAT_SIZE_HWORD;
    tx_descriptor_config.dst_increment_enable   = false;
    tx_descriptor_config.block_transfer_count   = bufferLen;
    tx_descriptor_config.source_address         = srcAddr;
    tx_descriptor_config.destination_address    = destAddr;
    
    /* Configure DMA descriptor with previous configuration */
    dma_descriptor_create( tx_descriptor, &tx_descriptor_config );
}

void dma_setupDescriptorRx( DmacDescriptor *rx_descriptor, uint32_t srcAddr,
                            uint32_t destAddr, uint16_t bufferLen )
{
    struct dma_descriptor_config rx_descriptor_config;
    
    dma_descriptor_get_config_defaults( &rx_descriptor_config );
    
    /* Configure descriptor */
    rx_descriptor_config.beat_size              = DMA_BEAT_SIZE_HWORD;
    rx_descriptor_config.src_increment_enable   = false;
    rx_descriptor_config.block_transfer_count   = bufferLen;
    rx_descriptor_config.source_address         = srcAddr;
    rx_descriptor_config.destination_address    = destAddr;
    
    /* Configure DMA descriptor with previous configuration */
    dma_descriptor_create( rx_descriptor, &rx_descriptor_config );
}

void DMA_configureLED( void )
{
    uint32_t txSourceAddress        = ( uint32_t )LED_TxBuffer + sizeof( LED_TxBuffer );
    uint32_t txDestinationAddress   = ( uint32_t )( &spi_instanceLED.hw->SPI.DATA.reg );
    
    uint32_t rxSourceAddress        = ( uint32_t )( &spi_instanceLED.hw->SPI.DATA.reg );
    uint32_t rxDestinationAddress   = ( uint32_t )LED_RxBuffer + sizeof( LED_RxBuffer );
    
    /* Configure DMA Tx/Rx resources */
    dma_configureResourceTx( &zDMA_LEDResourceTx, LED_SPI_SERCOM_DMAC_ID_TX );
    dma_configureResourceRx( &zDMA_LEDResourceRx, LED_SPI_SERCOM_DMAC_ID_RX );
    
    /* Setup DMA Tx/Rx descriptors */
    dma_setupDescriptorTx(  &DMA_TxLEDDescriptor, txSourceAddress,
                            txDestinationAddress, sizeof(LED_TxBuffer)/sizeof(uint32_t));
    dma_setupDescriptorRx(  &DMA_RxLEDDescriptor, rxSourceAddress,
                            rxDestinationAddress, LED_BUFFER_LEN );
    
    /* Add DMA Tx/Rx descriptors to DMA Tx/Rx resources */
    dma_add_descriptor( &zDMA_LEDResourceTx, &DMA_TxLEDDescriptor );
    dma_add_descriptor( &zDMA_LEDResourceRx, &DMA_RxLEDDescriptor );
    
    /* Register DMA Tx/Rx transfer callbacks */
    dma_register_callback( &zDMA_LEDResourceTx, dma_LEDTxDone, DMA_CALLBACK_TRANSFER_DONE );
    dma_register_callback( &zDMA_LEDResourceRx, dma_LEDRxDone, DMA_CALLBACK_TRANSFER_DONE );
    
    /* Enable DMA Tx/Rx callbacks */
    dma_enable_callback( &zDMA_LEDResourceTx, DMA_CALLBACK_TRANSFER_DONE );
    dma_enable_callback( &zDMA_LEDResourceRx, DMA_CALLBACK_TRANSFER_DONE );
}

void DMA_configureFTDI( void )
{
    uint32_t txSourceAddress        = ( uint32_t )FTDI_TxBuffer + sizeof( FTDI_TxBuffer );
    uint32_t txDestinationAddress   = ( uint32_t )( &usart_instanceFTDI.hw->USART.DATA.reg );
    
    uint32_t rxSourceAddress        = ( uint32_t )( &usart_instanceFTDI.hw->USART.DATA.reg );
    uint32_t rxDestinationAddress   = ( uint32_t )FTDI_RxBuffer + sizeof( FTDI_RxBuffer );
    
    /* Configure DMA Tx/Rx resources */
    dma_configureResourceTx( &zDMA_FTDIResourceTx, FTDI_USART_SERCOM_DMAC_ID_TX );
    dma_configureResourceRx( &zDMA_FTDIResourceRx, FTDI_USART_SERCOM_DMAC_ID_RX );
    
    /* Setup DMA Tx/Rx descriptors */
    dma_setupDescriptorTx(  &DMA_TxFTDIDescriptor, txSourceAddress,
                            txDestinationAddress, FTDI_BUFFER_LEN );
    dma_setupDescriptorRx(  &DMA_RxFTDIDescriptor, rxSourceAddress,
                            rxDestinationAddress, FTDI_BUFFER_LEN );

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
    dma_configureResourceTx( &zDMA_BluetoothResourceTx, BT_USART_SERCOM_DMAC_ID_TX );
    dma_configureResourceRx( &zDMA_BluetoothResourceRx, BT_USART_SERCOM_DMAC_ID_RX );
    
    /* Setup DMA Tx/Rx descriptors */
    dma_setupDescriptorTx(  &DMA_TxBluetoothDescriptor, txSourceAddress,
                            txDestinationAddress, BLUETOOTH_BUFFER_LEN );
    dma_setupDescriptorRx( &DMA_RxBluetoothDescriptor, rxSourceAddress,
                            rxDestinationAddress, BLUETOOTH_BUFFER_LEN );

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