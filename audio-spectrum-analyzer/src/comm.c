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
void shellProcessChar( const unsigned char currentChar );
void shellReset( void );
void shellErr( void );

enum shellTokens {
    LITERAL = 0,
    WHITESPACE,
    MINUS,
    LINEEND,
    TOKEN_LENGTH
};

enum shellStates {
    ARGUMENT,
    ARGUMENT_MINUS,
    STATES_LENGTH
};

enum shellActions {
    NOTHING,
    NEXT_ARG,
    NEXT_CHAR,
    CALL,
    ACTIONS_LENGTH
};

const unsigned char charMap[128] = {
    ['-']   = MINUS,
    [' ']   = WHITESPACE,
    ['\t']  = WHITESPACE,
    ['\r']  = LINEEND,
    ['\n']  = LINEEND,
    ['\0']  = LINEEND,
    ['\4']  = LINEEND
};

const unsigned char shellTransitions[STATES_LENGTH][TOKEN_LENGTH][2] = {
    [ARGUMENT] = {
        [LITERAL]       = {ARGUMENT, NEXT_CHAR},
        [WHITESPACE]    = {ARGUMENT, NEXT_ARG},
        [MINUS]         = {ARGUMENT_MINUS, NOTHING},
        [LINEEND]       = {ARGUMENT, CALL},
    },
    [ARGUMENT_MINUS] = {
        [LITERAL]       = {ARGUMENT, NEXT_CHAR},
        [WHITESPACE]    = {ARGUMENT, NEXT_CHAR},
        [MINUS]         = {ARGUMENT, NEXT_CHAR},
        [LINEEND]       = {ARGUMENT, NEXT_CHAR},
    },
};

unsigned char argv[PARSER_MAX_ARG][PARSER_MAX_ARG_LEN] = {{0}};
uint8_t argc = 1;
bool newCmd = false;
enum shellStates State = ARGUMENT;
uint8_t argIdx = 0;

void shellProcessChar( const unsigned char currentChar )
{
    const unsigned char *transition = shellTransitions[State][charMap[currentChar]];
    State = ( enum shellStates )( transition[0] );
    enum shellActions action = ( enum shellActions )( transition[1] );
    
    if( (currentChar >= 0 ) && (currentChar <= 127) )
    {
        switch( action )
        {
            case NEXT_CHAR:
            {
                if( argIdx < PARSER_MAX_ARG_LEN )
                {
                    argv[argc - 1][argIdx++] = currentChar;
                } else {
                    shellErr();
                }
                break;
            }            
            case NEXT_ARG:
            {
                if( argc < PARSER_MAX_ARG )
                {
                    argc++;
                    argIdx = 0;
                } else {
                    shellErr();
                }
                break;
            }            
            case CALL:
            {
                newCmd = true;
                break;
            }
            default:
            {
                break;
            }            
        }
    } else {
        shellErr();
    }
}

void shellReset( void )
{
    State = ARGUMENT;
    
    argc = 1;
    argIdx = 0;
    
    memset(argv, '\0', ( PARSER_MAX_ARG * PARSER_MAX_ARG_LEN ) );
    
    newCmd = false;
}

void shellErr( void )
{
    shellReset();
    
    /* Respond over USART */
}

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
        if( xFTDIRxQueue != 0 )        
        {
            /* Continuously read incoming buffer waiting for newline while echoing incoming data */
            if( xQueueReceive( xFTDIRxQueue, &rxByte, ( TickType_t ) 10 ) )
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
    char buffer[PARSER_MAX_CMD_LEN];
    char *pBuffer = buffer;

    LED_Data_t rgbSet;
    
    for(;;)
    {
        if( xParserQueue != 0 )
        {
            if( xQueueReceive( xParserQueue, buffer, ( TickType_t ) 10 ) )
            {
                while( ( !newCmd ) && ( pBuffer != &buffer[PARSER_MAX_CMD_LEN] ) )
                {
                    shellProcessChar( *pBuffer );
                    pBuffer++;
                }
                pBuffer = buffer;
                
                if( newCmd )
                {
                    /* Process Command */
                    uint32_t id = IDENTIFIER(argv[0][0], argv[0][1], argv[0][2], argv[0][3]);
                    switch(id)
                    {
                        case CMD_RGB:
                        {
                            rgbSet.colour.red     = atoi( (const char*) argv[1]);
                            rgbSet.colour.green   = atoi( (const char*) argv[2]);
                            rgbSet.colour.blue    = atoi( (const char*) argv[3]);
                            
                            xQueueSend( xFTDITxQueue, "\r\nLED Colour Set!\r\n", (TickType_t) 5 );
                            xQueueSend( xLEDQueue, &rgbSet, (TickType_t) 0 );
                            
                            break;
                        }
                        case CMD_PTRN:
                        {
                            break;
                        }
                        case CMD_INP:
                        {
                            break;
                        }
                        case CMD_OUT:
                        {
                            break;
                        }
                        case CMD_SET:
                        {
                            break;
                        }
                        case CMD_CURR:
                        {
                            break;
                        }
                        case CMD_BLNK:
                        {
                            break;
                        }
                        
                    }
                    shellReset();
                }
            }
        }
        taskYIELD();
    }
}