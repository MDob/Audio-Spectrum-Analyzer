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

/*======================================================================*/
/*                           LOCAL DEPENDENCIES                         */
/*======================================================================*/
#include "comm.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "led_cfg.h"

/*======================================================================*/
/*                      GLOBAL CONSTANT DEFINITIONS                     */
/*======================================================================*/
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

const unsigned char const charMap[128] = {
    ['-']   = MINUS,
    [' ']   = WHITESPACE,
    ['\t']  = WHITESPACE,
    ['\r']  = LINEEND,
    ['\n']  = LINEEND,
    ['\0']  = LINEEND,
    ['\4']  = LINEEND
};

const unsigned char const shellTransitions[STATES_LENGTH][TOKEN_LENGTH][2] = {
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

/*======================================================================*/
/*                      GLOBAL VARIABLE DEFINITIONS                     */
/*======================================================================*/
enum shellStates State = ARGUMENT;
unsigned char argv[PARSER_MAX_ARG][PARSER_MAX_ARG_LEN] = {{0}};
    
uint8_t argc    = 1;
uint8_t argIdx  = 0;
bool newCmd     = false;

/*======================================================================*/
/*                           FUNCTION PROTOTYPES                        */
/*======================================================================*/
int16_t comm_strCompare         ( const void *pvString1, const void *pvString2 );
void    comm_shellProcessChar   ( const unsigned char currentChar );
void    comm_shellReset         ( void );
void    comm_shellErr           ( void );

/*======================================================================*/
/*                          FUNCTION DECLARATIONS                       */
/*======================================================================*/
void comm_shellErr( void )
{
    xQueueSend( xFTDITxQueue, "\r\nErroneous entry detected", (TickType_t) 5 );
    xQueueSend( xFTDITxQueue, ", please refactor your input\r\n", (TickType_t) 5 );
    comm_shellReset();
}

int16_t comm_strCompare( const void *pvString1, const void *pvString2 )
{
    char* const *pString1 = pvString1;
    char* const *pString2 = pvString2;
    
    return( strcmp( *pString1, *pString2 ) );
}

void comm_shellReset( void )
{
    State = ARGUMENT;
    
    argc = 1;
    argIdx = 0;
    
    memset(argv, '\0', ( PARSER_MAX_ARG * PARSER_MAX_ARG_LEN ) );
    
    newCmd = false;
}

void comm_shellProcessChar( const unsigned char currentChar )
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
                    comm_shellErr();
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
                    comm_shellErr();
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
        comm_shellErr();
    }
}

void COMM_init( void )
{
    xParserQueue = xQueueCreate( PARSER_MAX_CMD_LEN, PARSER_MAX_CMD_LEN * sizeof( char ) );
    Assert( xParserQueue );
    
    xLEDQueue = xQueueCreate( 2, sizeof( LED_Packet_t ) );
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
                    comm_shellProcessChar( *pBuffer );
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
/*                            xQueueSend( xBluetoothTxQueue, "\r\nColour Set!\r\n", (TickType_t) 5 );*/
                            xQueueSend( xLEDQueue, &ledPacket, (TickType_t) 0 );
                            break;
                        }
                        case CMD_PTRN:
                        {
                            ledPacket.cmd = PTRN;
                            ledPacket.pattern = atoi( (const char*) argv[1] );
                            
                            xQueueSend( xFTDITxQueue, "\r\nPattern Set!\r\n", (TickType_t) 5 );
/*                            xQueueSend( xBluetoothTxQueue, "\r\nPattern Set!\r\n", (TickType_t) 5 );*/
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
/*                            xQueueSend( xBluetoothTxQueue, "\r\nBlink Period Set!\r\n", (TickType_t) 5 );*/
                            xQueueSend( xLEDQueue, &ledPacket, (TickType_t) 0 );
                            break;
                        }
                        case CMD_AUD:
                        {
                            ledPacket.cmd = AUD;
                            xQueueSend( xFTDITxQueue, "\r\nAudio Reactivity On!\r\n", (TickType_t) 5 );
/*                            xQueueSend( xBluetoothTxQueue, "\r\nAudio Reactivity On!\r\n", (TickType_t) 5 );*/
                            xQueueSend( xLEDQueue, &ledPacket, (TickType_t) 0 );
                            break;
                        }
                        case CMD_HELP_0:
                        case CMD_HELP_1:
                        {
                            xQueueSend( xFTDITxQueue, "\r\nCommand list: rgb, blnk, ptrn\r\n", portMAX_DELAY );
                            xQueueSend( xFTDITxQueue, "Use '/?' for command-", portMAX_DELAY );
                            xQueueSend( xFTDITxQueue, "specific help\r\n", portMAX_DELAY );
                            
//                             xQueueSend( xBluetoothTxQueue, "\r\nCommand list: rgb, blnk, ptrn\r\n", portMAX_DELAY );
//                             xQueueSend( xBluetoothTxQueue, "Use '/?' for command-", portMAX_DELAY );
//                             xQueueSend( xBluetoothTxQueue, "specific help\r\n", portMAX_DELAY );
                            break;
                        }
                        case CMD_HELP_2:
                        {
                            /* Get second argument, if command return command specific info */
                            /* e.g. '/? rgb'                                                */
                            xQueueSend( xFTDITxQueue, "rgb usage: \'rgb R G B\' (0-255)\r\n", portMAX_DELAY );
                            xQueueSend( xFTDITxQueue, "blnk usage: \'blnk #\' (20-9999 ms)\r\n", portMAX_DELAY );
                            xQueueSend( xFTDITxQueue, "ptrn usage: \'ptrn #\' (0-2)\r\n", portMAX_DELAY );
                            
//                             xQueueSend( xBluetoothTxQueue, "rgb usage: \'rgb R G B\' (0-255)\r\n", portMAX_DELAY );
//                             xQueueSend( xBluetoothTxQueue, "blnk usage: \'blnk #\' (20-9999 ms)\r\n", portMAX_DELAY );
//                             xQueueSend( xBluetoothTxQueue, "ptrn usage: \'ptrn #\' (0-2)\r\n", portMAX_DELAY );
                            break;
                        }
                        default:
                            comm_shellErr();
                            break;
                        
                    }
                    comm_shellReset();
                }
            }
        }
        taskYIELD();
    }
}