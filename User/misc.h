/*
 * misc.h
 *
 *  Created on: Feb 4, 2025
 *      Author: stauger
 */

#ifndef USER_MISC_H_
#define USER_MISC_H_

#include "ch32v30x.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct pin_def{
	GPIO_TypeDef* const port;
	const uint16_t pin;
} pin_t;

#endif /* USER_MISC_H_ */
