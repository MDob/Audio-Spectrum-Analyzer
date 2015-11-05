/*************************************************************************
 *  File Name:      comm.h
 *  Author:         Michael Dobinson
 *  Date Created:   2015-10-27
 *
 *  Sub-files:
 *      comm.c
 *
 *  Brief:
 *
 *  [Compiled and tested with Atmel Studio 7]
 *
*************************************************************************/


#ifndef COMM_H_
#define COMM_H_

#include "compiler.h"
#include "stdint.h"

#define FTDI_MAX_RX_LEN     32
#define BT_MAX_RX_LEN       64
#define PARSER_MAX_CMD_LEN  32
#define PARSER_MAX_ARG_LEN  4
#define PARSER_MAX_ARG      10

#define IDENTIFIER(a, b, c, d)   ( ( ( ( uint32_t ) a) << ( 3*8 ) )  + \
                                   ( ( ( uint32_t ) b) << ( 2*8 ) )  + \
                                   ( ( ( uint32_t ) c) << ( 1*8 ) )  + \
                                   ( ( ( uint32_t ) d) << ( 0*8 ) ) )

#define CMD_RGB     IDENTIFIER('r','g','b', 0 )
#define CMD_PTRN    IDENTIFIER('p','t','r','n')
#define CMD_INP     IDENTIFIER('i','n','p', 0 )
#define CMD_OUT     IDENTIFIER('o','u','t', 0 )
#define CMD_SET     IDENTIFIER('s','e','t', 0 )
#define CMD_CURR    IDENTIFIER('c','u','r','r')
#define CMD_BLNK    IDENTIFIER('b','l','n','k')
#define CMD_HELP    IDENTIFIER('h','e','l','p')

void COMM_init( void );
void TASK_ftdiParser( void *pvParameters );
void TASK_bluetoothParser( void *pvParameters );
void TASK_mainParser( void *pvParameters );

#endif /* COMM_H_ */