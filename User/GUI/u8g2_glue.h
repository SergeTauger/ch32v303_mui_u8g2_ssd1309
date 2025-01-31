/*
 * u8g2_glue.h
 *
 *  Created on: Jan 29, 2025
 *      Author: stauger
 */

#ifndef USER_GUI_U8G2_GLUE_H_
#define USER_GUI_U8G2_GLUE_H_

#include "ch32v30x_conf.h"
#include "u8g2.h"
#include "u8x8.h"

typedef enum {
	data_e,
	cmd_e,
} transmisson_type_e;

uint8_t u8x8_gpio_and_delay_HW_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_byte_HW_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

#endif /* USER_GUI_U8G2_GLUE_H_ */
