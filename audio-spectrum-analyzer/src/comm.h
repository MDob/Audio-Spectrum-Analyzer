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


#define FTDI_MAX_RX_LEN     32
#define BT_MAX_RX_LEN       64
#define PARSER_MAX_CMD_LEN  32

enum commandType {
    COMMAND_1 = 0,
    COMMAND_2,
    COMMAND_3,
    MAX_COMMANDS,
};

void TASK_ftdiParser( void *pvParameters );
void TASK_bluetoothParser( void *pvParameters );
void TASK_mainParser( void *pvParameters );

#endif /* COMM_H_ */