/*************************************************************************
 *  File Name:      config_sw.h
 *  Author:         Michael Dobinson
 *  Date Created:   2015-10-24
 *
 *  Sub-files:
 *      config_sw.c
 *
 *  Brief:
 *      Provides configuration settings for GPIO
 *
 *  [Compiled and tested with Atmel Studio 7]
 *
*************************************************************************/
#ifndef CONFIG_SW_H_
#define CONFIG_SW_H_

/*======================================================================*/
/*                          GLOBAL DEPENDENCIES                         */
/*======================================================================*/
#include "compiler.h"
#include "user_board.h"
#include "port.h"
#include "pinmux.h"

/*======================================================================*/
/*                          FUNCTION PROTOTYPES                         */
/*======================================================================*/
void CONFIG_configurePins( void );

#endif /* CONFIG_SW_H_ */