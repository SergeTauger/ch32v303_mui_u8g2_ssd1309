/*
 * buttons.h
 *
 *  Created on: Feb 5, 2025
 *      Author: stauger
 */

#ifndef USER_BUTTONS_H_
#define USER_BUTTONS_H_

#include "ch32v30x_conf.h"
#include "misc.h"

typedef enum {
	HOME,
	NEXT,
	PREV,
	SELECT,
	NBUTTONS,
} sens_t;


void 	PrepareButtons(void);
uint8_t GetHit(sens_t id);
uint8_t GetRelease(sens_t id);
uint8_t IsHit(sens_t id);
uint8_t IsHitProcessOnce(sens_t id);
uint8_t IsReleased(sens_t id);
void    ClearCnt(uint8_t board, sens_t id);

void 	BTNs_Init(void);
void QueryButtons(void);

#endif /* USER_BUTTONS_H_ */
