/*************************************************************************
 *  File Name:      comm.c
 *  Author:         Michael Dobinson
 *  Date Created:   2015-10-27
 *
 *  Brief:
 *      
 *
 *  [Compiled and tested with Atmel Studio 7]
 *
*************************************************************************/
#include "comm.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart_cfg.h"
#include "led_cfg.h"

QueueHandle_t xParserQueue;

int compare_string( const void *pvString1, const void *pvString2 );
//const uint8_t header[] = "\t#---------------------------------------------------------------#\r\n";

//void ftdi_generateInitialMenu( void )
//{
    // Emit header
    /*
    snprintf( (char*) FTDI_TxBuffer, sizeof(FTDI_TxBuffer), "%s", header );
    dma_start_transfer_job( &zDMA_FTDIResourceTx );
    
    while( !( DMA_Status & _LS(FTDI_TX_DONE) ) );
    DMA_Status &= ~_LS( FTDI_TX_DONE );
    
    snprintf( (char*) FTDI_TxBuffer, sizeof(FTDI_TxBuffer), "\t|\t\tAudio Analyzer - Main Menu\t\t|\r\n" );
    dma_start_transfer_job( &zDMA_FTDIResourceTx );
    
    while( !( DMA_Status & _LS(FTDI_TX_DONE) ) );
    DMA_Status &= ~_LS( FTDI_TX_DONE );
    
    snprintf( (char*) FTDI_TxBuffer, sizeof(FTDI_TxBuffer, "%s", header );
    dma_start_transfer_job( &zDMA_FTDIResourceTx );*/
//}

int compare_string( const void *pvString1, const void *pvString2 )
{
    char* const *pString1 = pvString1;
    char* const *pString2 = pvString2;
    
    return( strcmp( *pString1, *pString2 ) );
}

void COMM_init( void )
{
    xParserQueue = xQueueCreate( PARSER_MAX_CMD_LEN, PARSER_MAX_CMD_LEN * sizeof( char ) );
    Assert( xParserQueue );
    
    xLEDQueue = xQueueCreate( 1, sizeof( LED_Data_t ) );
    Assert( xLEDQueue );
}

void TASK_ftdiParser( void *pvParameters )
{
    static char rxBuffer[FTDI_MAX_RX_LEN];
    static char *rx = rxBuffer;
    
    uint16_t rxByte;
    
    for(;;)
    {
        if( xFTDIQueue != 0 )        
        {
            /* Continuously read incoming buffer waiting for newline while echoing incoming data */
            if( xQueueReceive( xFTDIQueue, &rxByte, ( TickType_t ) 10 ) )
            {
                /* Look for backspace character */
                if( rxByte == 127 )
                {
                    if( rx == rxBuffer )
                    {
                        /* Do nothing */
                    }
                    else
                    {
                        rx--;
                        *rx = 0;
                    }
                }
                else if( rxByte == 13 )
                {
                    memcpy( rx, "\0", sizeof( char ) );
                
                    char parserCmd[FTDI_MAX_RX_LEN];
                    strncpy(parserCmd, rxBuffer,  sizeof( parserCmd ) );
                
                    /* Pass command to the main parser */
                    if( xParserQueue != 0 )
                    {
                        xQueueSend( xParserQueue, parserCmd, ( TickType_t ) 10 );
                    }
                    rx = rxBuffer;
                    vTaskDelay(50);
                }
                else
                {
                    /* Copy byte into buffer */
                    *rx = ( char ) rxByte;
                
                    /* Reset buffer pointer on overflow */
                    if( rx == &rxBuffer[FTDI_MAX_RX_LEN-1] )
                    {
                        rx = rxBuffer;
                    }
                    else
                    {
                        rx++;
                    }
                }
            }
        }
        vTaskDelay(5);
    }
}

void TASK_bluetoothParser( void *pvParameters )
{
    
    for(;;)
    {
        /* Continuously read incoming buffer waiting for end of sequence */
        
        /* On enter, parse the command into a command type and its arguments */
        
        /* Pass this to the main parser */
    }
}

void TASK_mainParser( void *pvParameters )
{
    const char *command[] =
    {
        "inp",
        "out",
        "ptrn",
        "rgb",
        "set",
    };
    
    char buffer[PARSER_MAX_CMD_LEN];
    char *cmd;
    
    LED_Data_t rgb;
    
    for(;;)
    {
        if( xParserQueue != 0 )
        {
            if( xQueueReceive( xParserQueue, buffer, ( TickType_t ) 10 ) )
            {
                cmd = strtok(buffer, " ");
                if( !strcmp( cmd, "rgb" ) )
                {
                    cmd = strtok(NULL, " ");
                    if(cmd)
                        rgb.colour.red = atoi(cmd);
                    cmd = strtok(NULL, " ");
                    if(cmd)
                        rgb.colour.green = atoi(cmd);
                    cmd = strtok(NULL, " ");
                    if(cmd)
                        rgb.colour.blue = atoi(cmd);
                    
                    xQueueSend( xLEDQueue, &rgb, 0 );
                }
            }
        }
        taskYIELD();
    }
}