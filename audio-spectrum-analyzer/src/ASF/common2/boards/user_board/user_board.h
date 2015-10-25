/*************************************************************************
 *  File Name:      user_board.h
 *  Author:         Michael Dobinson
 *  Date Created:   2015-10-23
 *
 *  Brief:
 *      Provides board-specific configuration settings
 *
 *  [Compiled and tested with Atmel Studio 7]
 *
*************************************************************************/

#ifndef USER_BOARD_H
#define USER_BOARD_H

/*======================================================================*/
/*                             DEPENDENCIES                             */
/*======================================================================*/
#include <conf_board.h>
#include <compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/*======================================================================*/
/*                         FUNCTION PROTOTYPES                          */
/*======================================================================*/
void system_board_init(void);

/*======================================================================*/
/*                      GLOBAL MACRO DEFINITIONS                        */
/*======================================================================*/
#define _LS(x) (1 << x)

/*======================================================================*/
/*                     GLOBAL CONSTANT DEFINITIONS                      */
/*======================================================================*/
#define BOARD_NAME                "AUDIO_ANALYZER"

#define _SAMD21E17A_

#define BOARD_MCK               
#define BOARD_OSC_STARTUP_US    15625

/* General Settings */
#define FTDI_USE_DMA
#define BT_USE_DMA

/* Debug LED Definitions */
#define LED0_PIN                        PIN_PA27
#define LED0_ACTIVE                     true
#define LED0_INACTIVE                   !LED0_ACTIVE

#define LED1_PIN                        PIN_PA28
#define LED1_ACTIVE                     true
#define LED1_INACTIVE                   !LED1_ACTIVE

#define LED_COUNT                       2

/* Config Switch Definitions */
#define SW0_PIN                         PIN_PA10
#define SW0_ACTIVE                      false
#define SW0_INACTIVE                    false
#define SW0_EIC_PIN                     PIN_PA10A_EIC_EXTINT10
#define SW0_EIC_MUX                     MUX_PA10A_EIC_EXTINT10
#define SW0_EIC_PINMUX                  PINMUX_PA10A_EIC_EXTINT10
#define SW0_EIC_LINE                    10

/* LED Output SPI Definitons */
#define LED_SPI_MODULE                  SERCOM0
#define LED_SPI_SERCOM_MUX_SETTING      SPI_SIGNAL_MUX_SETTING_E
#define LED_SPI_SERCOM_PINMUX_PAD0      PINMUX_PA04D_SERCOM0_PAD0
#define LED_SPI_SERCOM_PINMUX_PAD1      PINMUX_UNUSED
#define LED_SPI_SERCOM_PINMUX_PAD2      PINMUX_PA06D_SERCOM0_PAD2
#define LED_SPI_SERCOM_PINMUX_PAD3      PINMUX_PA07D_SERCOM0_PAD3

#define LED_SPI_SERCOM_DMAC_ID_TX       SERCOM0_DMAC_ID_TX
#define LED_SPI_SERCOM_DMAC_ID_RX       SERCOM0_DMAC_ID_RX

#define LED_SS_PIN                      PIN_PA05

/* FTDI USART Defintions */
#define FTDI_USART_MODULE               SERCOM1
#define FTDI_USART_SERCOM_MUX_SETTING   USART_RX_1_TX_0_XCK_1
#define FTDI_USART_SERCOM_PINMUX_PAD0   PINMUX_PA16C_SERCOM1_PAD0
#define FTDI_USART_SERCOM_PINMUX_PAD1   PINMUX_PA17C_SERCOM1_PAD1
#define FTDI_USART_SERCOM_PINMUX_PAD2   PINMUX_UNUSED
#define FTDI_USART_SERCOM_PINMUX_PAD3   PINMUX_UNUSED

#define FTDI_USART_SERCOM_DMAC_ID_TX    SERCOM1_DMAC_ID_TX
#define FTDI_USART_SERCOM_DMAC_ID_RX    SERCOM1_DMAC_ID_RX

/* Bluetooth USART Definitions */
#define BT_USART_MODULE                 SERCOM3
#define BT_USART_SERCOM_MUX_SETTING     USART_RX_1_TX_0_XCK_1
#define BT_USART_SERCOM_PINMUX_PAD0     PINMUX_PA22C_SERCOM3_PAD0
#define BT_USART_SERCOM_PINMUX_PAD1     PINMUX_PA23C_SERCOM3_PAD1
#define BT_USART_SERCOM_PINMUX_PAD2     PINMUX_UNUSED
#define BT_USART_SERCOM_PINMUX_PAD3     PINMUX_UNUSED

#define BT_USART_SERCOM_DMAC_ID_TX      SERCOM3_DMAC_ID_TX
#define BT_USART_SERCOM_DMAC_ID_RX      SERCOM3_DMAC_ID_RX

/* ADC Definitons */
#define ADC_MODULE  ADC
#define AIN0_ADC_CHANNEL                0
#define AIN0_ADC_PIN                    PIN_PA02B_ADC_AIN0
#define AIN0_ADC_PINMUX                 PINMUX_PA02B_ADC_AIN0

#define AIN1_ADC_CHANNEL                1
#define AIN1_ADC_PIN                    PIN_PA03B_ADC_AIN1
#define AIN1_ADC_PINMUX                 PINMUX_PA03B_ADC_AIN1

#define ACCX_ADC_CHANNEL                16
#define ACCX_ADC_PIN                    PIN_PA08B_ADC_AIN16
#define ACCX_ADC_PINMUX                 PINMUX_PA08B_ADC_AIN16

#define ACCY_ADC_CHANNEL                17
#define ACCY_ADC_PIN                    PIN_PA09B_ADC_AIN17
#define ACCY_ADC_PINMUX                 PINMUX_PA09B_ADC_AIN17

#define CONF_ADC_CHANNEL                19
#define CONF_ADC_PIN                    PIN_PA11B_ADC_AIN19
#define CONF_ADC_PINMUX                 PINMUX_PA11B_ADC_AIN19

#ifdef __cplusplus
}
#endif

#endif /* USER_BOARD_H */