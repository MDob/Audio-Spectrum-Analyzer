/*************************************************************************
 *  File Name:      pwm_cfg.h
 *  Author:         Michael Dobinson
 *  Date Created:   2015-10-25
 *
 *  Sub-files:
 *      pwm_cfg.c
 *
 *  Brief:
 *      Provides configuration settings for PWM control
 *
 *  [Compiled and tested with Atmel Studio 7]
 *
*************************************************************************/
#ifndef PWM_CFG_H_
#define PWM_CFG_H_

/*======================================================================*/
/*                          GLOBAL DEPENDENCIES                         */
/*======================================================================*/
#include "compiler.h"
#include "user_board.h"
#include "tcc.h"

#define PWM_PERIOD_NS       1250
#define PWM_PERIOD_CYCLES   100      // 1250e-9 * 48e6

#define PWM_LED_ZERO        75      // 20 = 416ns
#define PWM_LED_ONE         59      // 38 = 791ns

/*======================================================================*/
/*                      GLOBAL VARIABLE DEFINITIONS                     */
/*======================================================================*/
struct tcc_module TCC_instanceLED;

/*======================================================================*/
/*                          FUNCTION PROTOTYPES                         */
/*======================================================================*/
void PWM_configureTCC( uint32_t period );
void PWM_setBuffer( void );
void PWM_init( void );

#endif /* PWM_CFG_H_ */