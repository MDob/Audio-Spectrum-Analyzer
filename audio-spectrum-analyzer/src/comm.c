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
    xQueueSend( xFTDITxQueue, "\r\nErroneous entry detected, please refactor your input", (TickType_t) 5 );
    xQueueSend( xFTDITxQueue, ", please refactor your input\r\n", (TickType_t) 5 );
    shellReset();
}

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
    
    xLEDQueue = xQueueCreate( 3, sizeof( LED_Packet_t ) );
    Assert( xLEDQueue );
}

void TASK_mainParser( void *pvParameters )
{
    char buffer[PARSER_MAX_CMD_LEN];
    char *pBuffer = buffer;

    LED_Packet_t ledPacket;
    
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
                            ledPacket.cmd   = RGB;
                            ledPacket.LED.colour.red   = atoi( (const char*) argv[1]);
                            ledPacket.LED.colour.green = atoi( (const char*) argv[2]);
                            ledPacket.LED.colour.blue  = atoi( (const char*) argv[3]);
                            
                            xQueueSend( xFTDITxQueue, "\r\nColour Set!\r\n", (TickType_t) 5 );
                            xQueueSend( xLEDQueue, &ledPacket, (TickType_t) 0 );
                            break;
                        }
                        case CMD_PTRN:
                        {
                            ledPacket.cmd = PTRN;
                            ledPacket.pattern = atoi( (const char*) argv[1] );
                            
                            xQueueSend( xFTDITxQueue, "\r\nPattern Set!\r\n", (TickType_t) 5 );
                            xQueueSend( xLEDQueue, &ledPacket, (TickType_t) 0 );
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
                            ledPacket.cmd       = BLNK;
                            ledPacket.period    = atoi( (const char*) argv[1]);
                            xQueueSend( xFTDITxQueue, "\r\nBlink Period Set!\r\n", (TickType_t) 5 );
                            xQueueSend( xLEDQueue, &ledPacket, (TickType_t) 0 );
                            break;
                        }
                        case CMD_HELP_0:
                        case CMD_HELP_1:
                        {
                            xQueueSend( xFTDITxQueue, "\r\nCommand list: rgb, blnk, ptrn\r\n", portMAX_DELAY );
                            xQueueSend( xFTDITxQueue, "Use '/?' for command-", portMAX_DELAY );
                            xQueueSend( xFTDITxQueue, "specific help", portMAX_DELAY );
                            break;
                        }
                        case CMD_HELP_2:
                        {
                            /* Get second argument, if command return command specific info */
                            /* e.g. '/? rgb'                                                */
                            xQueueSend( xFTDITxQueue, "rgb usage: \'rgb R G B\' (0-255)\r\n", portMAX_DELAY );
                            xQueueSend( xFTDITxQueue, "blnk usage: \'blnk #\' (20-9999 ms)\r\n", portMAX_DELAY );
                            xQueueSend( xFTDITxQueue, "ptrn usage: \'ptrn #\' (0-2)\r\n", portMAX_DELAY );
                        }
                        default:
                            shellErr();
                            break;
                        
                    }
                    shellReset();
                }
            }
        }
        taskYIELD();
    }
}