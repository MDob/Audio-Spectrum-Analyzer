/*
 * config_sw.c
 *
 * Created: 2015-10-24 1:22:09 PM
 *  Author: Michael
 */ 

#include "config_sw.h"

void CONFIG_configurePins( void )
{
	struct port_config pin_conf;
	port_get_config_defaults(&pin_conf);

	pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(PIN_PA27, &pin_conf);
	port_pin_set_config(PIN_PA28, &pin_conf);
	port_pin_set_output_level(PIN_PA28, true);
	port_pin_set_output_level(PIN_PA27, false);
}