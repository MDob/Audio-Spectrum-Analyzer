/*************************************************************************
 *  Project Name:   Audio Spectrum Analyzer
 *  Author:         Michael Dobinson
 *  Date Created:   2015-10-23
 *
 *  Brief:
 *      On reset, program execution begins in main() as called 
 *      from startup_samd21.c
 *
 *  To Do:
 *      - Add remaining functionality
 *      - Start using doxygen
 *
 *  [Compiled and tested with Atmel Studio 7]
 *
*************************************************************************/

/*======================================================================*/
/*                            DEPENDENCIES                              */
/*======================================================================*/
#include <asf.h>
#include "config_sw.h"
//#include "spi_cfg.h"
#include "usart_cfg.h"
#include "led_cfg.h"
#include "dma_cfg.h"
#include "pwm_cfg.h"
#include "adc_cfg.h"
#include "comm.h"

/*======================================================================*/
/*                         FUNCTION PROTOTYPES                          */
/*======================================================================*/

void vApplicationStackOverflowHook( TaskHandle_t *pxTask, char *pcTaskName );
void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationTickHook( void );

static void TASK_Heartbeat( void* pvParameters );

static void setupHardware( void );
static void setupTasks( void );


/*======================================================================*/
/*                     FreeRTOS EXCEPTION HANDLERS                      */
/*======================================================================*/

/* Called if stack overflow during execution */
extern void vApplicationStackOverflowHook(TaskHandle_t *pxTask, char *pcTaskName)
{
	//printf("stack overflow %x %s\r\n", pxTask, (portCHAR *)pcTaskName);
	/* If the parameters have been corrupted then inspect pxCurrentTCB to
	 * identify which task has overflowed its stack.
	 */
	for (;;) 
    {
		/* Loop forever */
	}
}

/* Called if memory allocation fails */
extern void vApplicationMallocFailedHook(void)
{
	for(;;)
    {
		/* Loop forever */
	}
}

/* This function is called by FreeRTOS idle task */
extern void vApplicationIdleHook(void)
{
    /* Do nothing */
}

/* This function is called by FreeRTOS each tick */
extern void vApplicationTickHook(void)
{
    /* Do nothing */
}

/*======================================================================*/
/*                         FUNCTION DECLARATIONS                        */
/*======================================================================*/

static void TASK_Heartbeat( void* pvParameters )
{
    /* Nothing to do here.... yet */
    UNUSED(pvParameters);

    for(;;)
    {
        port_pin_toggle_output_level(LED0_PIN);
        port_pin_toggle_output_level(LED1_PIN);
        vTaskDelay(500);
    }
}

static void setupHardware( void )
{
    /* System Configuration */
    system_init();
    system_interrupt_enable_global();
    SysTick_Config(system_gclk_gen_get_hz(GCLK_GENERATOR_0));
    
    /* Comms Initialization */
    //SPI_configureLED();
    COMM_init();
    USART_init();
    PWM_init();
    ADC_init();
    DMA_init();
    
    /* GPIO Initialization */
    CONFIG_configurePins();
    CONFIG_configureWDT();
}

static void setupTasks( void )
{
    /*========================= FreeRTOS TASK DECLARATIONS - PRIORITY 4 ========================*/
    // None 

    /*========================= FreeRTOS TASK DECLARATIONS - PRIORITY 3 ========================*/
    // None

    /*========================= FreeRTOS TASK DECLARATIONS - PRIORITY 2 ========================*/
    // None

    /*========================= FreeRTOS TASK DECLARATIONS - PRIORITY 1 ========================*/
    
    /* LED Tasks */
    xTaskCreate(    TASK_outputFormingLED,  ( const char* ) "LED_OUT",      100,    NULL,   1,  NULL );
    
    /* FTDI Tasks */
    xTaskCreate(    TASK_FTDI,              ( const char* ) "ECHO",         100,    NULL,   1,  NULL );
    xTaskCreate(    TASK_mainParser,        ( const char* ) "PARSE",        100,    NULL,   1,  NULL );
    
    /* Bluetooth Tasks */
    //xTaskCreate(    TASK_ReadBluetooth,     ( const char* ) "BLUETOOTH",    100,    NULL,   1,  NULL );
    
    /* SPI Tasks */
    //xTaskCreate(    TASK_SendSPI,           ( const char* ) "SPI_TX",       100,    NULL,   1,  NULL );
    //xTaskCreate(    TASK_ReadSPI,           ( const char* ) "SPI_RX",       100,    NULL,   1,  NULL );

    /*========================= FreeRTOS TASK DECLARATIONS - PRIORITY 0 ========================*/
    
    /* Misc Tasks */
    xTaskCreate(    TASK_Heartbeat,         ( const char* ) "HEARTBEAT",    100,    NULL,   0,  NULL );
}

int main (void)
{
    /* Setup system and peripherals */
    setupHardware();

    /* Setup FreeRTOS tasks */
    setupTasks();
    
    /* Start the scheduler! */
    vTaskStartScheduler();

    for(;;);
}
