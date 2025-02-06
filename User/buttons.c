/*
 * buttons.c
 *
 *  Created on: Feb 5, 2025
 *      Author: stauger
 */

#include "ch32v30x.h"
#include "debounce.h"
#include "buttons.h"


static uint8_t IsButtonSetFromISR(const pin_t pin);


static debounce_data_t btnsData[NBUTTONS] = {
	    {{BTN_PORT, HOME}, 0},
		{{BTN_PORT, NEXT}, 0},
		{{BTN_PORT, PREV}, 0},
		{{BTN_PORT, SELECT}, 0},
};

static button_simpl_t btnsState[NBUTTONS] = {
	    {NONE,&(btnsData[0]), 0, 0},
		{NONE,&(btnsData[1]), 0, 0},
		{NONE,&(btnsData[2]), 0, 0},
		{NONE,&(btnsData[3]), 0, 0},
};
static volatile uint32_t btnsHitCnt[NBUTTONS] = {0};

ctx_simpl_t btnsBufCtx = {
				.capacity = NBUTTONS,
				.ticks = 0,
				.storage = &(btnsState[0]),
				.requestPending = 0,
				.retvals = &btnsHitCnt[0],
				.isSet = IsButtonSetFromISR,
};

static uint8_t IsButtonSetFromISR(const pin_t pin){
	return !GPIO_ReadInputDataBit(pin.port, 1 << pin.pin);
}

uint8_t GetHit(sens_t id){
	uint8_t  ret = 0;
	uint32_t hits = 0;
	do {
		hits = btnsHitCnt[id];
	} while (hits != btnsHitCnt[id]);
	if (hits > 0){
		ret = 1;
	}
	return ret;
}

uint8_t GetRelease(sens_t id){
	uint8_t  ret = 0;
	if (GetHit(id) && IsReleased(id)){
		ret = 1;
	}
	return ret;
}

void PrepareButtons(void){
	for (uint8_t k=0; k<btnsBufCtx.capacity; k++){
		btnsState[k].type = PRESS_PENDING;
	}
}

uint8_t IsHit(sens_t id){
	uint8_t ret = 0;
	if (id < NBUTTONS){
		ret = (btnsBufCtx.requestPending & (1 << id)) == (1 << id);
	}
	return ret;
}

uint8_t IsHitProcessOnce(sens_t id){
	uint8_t ret = 0;
	if (id < NBUTTONS){
		ret = (btnsBufCtx.requestPending & (1 << id)) == (1 << id);
	}
	if (ret){
		btnsBufCtx.requestPending &= ~(1 << id);
	}
	return ret;
}

uint8_t IsReleased(sens_t id){
	uint8_t ret = 0;
	if (id < NBUTTONS){
		ret = IsButtonNowReleased(&(btnsData[id].button_history));
	}
	return ret;
}
void ClearCnt(uint8_t board, sens_t id){
	if (id < NBUTTONS){
		size_t req = (1 << id) << 16;
		do {
			btnsBufCtx.requestPending |= req;
		} while ((btnsBufCtx.requestPending & req) == 0);
	}
}

//To be called periodically
void QueryButtons(void){
	ButtonManagerNoRTOS(&btnsBufCtx);
}

void BTNs_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin   = (1 << BTN4_HOME_PIN) | (1 << BTN3_PREV_PIN) |
									(1 << BTN2_NEXT_PIN) | (1 << BTN1_SELECT_PIN) ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(BTN_PORT, &GPIO_InitStructure);

	PrepareButtons();
}
