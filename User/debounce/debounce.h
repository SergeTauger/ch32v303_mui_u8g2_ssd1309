/*
 * debounce.h
 *
 *  Created on: Apr 25, 2022
 *      Author: gryph
 */

#ifndef SRC_DEBOUNCE_DEBOUNCE_H_
#define SRC_DEBOUNCE_DEBOUNCE_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "misc.h"

#define BOT_MASK ((1<<2)|(1<<1)|(1<<0))
#define TOP_MASK ((1<<7)|(1<<6))
#define MASK (TOP_MASK | BOT_MASK)

enum {
	PULLUP,
	PULLDOWN,
};

typedef enum {
	NONE = 0,
	PRESS_PENDING,
	RELEASE_PENDING,
	DEBOUNCE_PENDING,
	DELAY,
} task_type_e;

typedef struct{
	const pin_t pin;
	uint8_t     button_history;
//	onRelease_fp releaseFcn;
//	onPress_fp   pressFcn;
} debounce_data_t;


typedef struct {
	task_type_e                type;
	debounce_data_t* const     debounce_data;
	volatile uint32_t          PressTime;
	volatile uint32_t          ReleaseTime;
//	volatile uint32_t          count;
} button_simpl_t;

typedef struct button_struct button_t;
struct button_struct {
#ifdef USE_FREERTOS
	TaskHandle_t              message;
#endif
	uint32_t                  time;
	task_type_e               type;
	debounce_data_t*          debounce_data;
	uint32_t                  PressTime;
	uint32_t                  ReleaseTime;
	uint32_t                  count;
	void (*PressCb)(button_t * idx);
	void (*ReleaseCb)(button_t * idx);
};

typedef struct {
	const    size_t    capacity;
	volatile uint32_t  ticks;
	volatile button_t* const storage;
	uint8_t (* const isSet)(const pin_t);
} debounce_ctx_t;
typedef volatile debounce_ctx_t * const debounce_ctx_h;

typedef struct {
	const size_t capacity;
	volatile uint32_t * const retvals;
	volatile size_t  requestPending;
	volatile uint32_t ticks;
	button_simpl_t * const storage;
	uint8_t (* const isSet)(const pin_t);
} ctx_simpl_t;

void    update_button(const pin_t pin, uint8_t (* const isSet)(const pin_t), volatile uint8_t *button_history);
uint8_t IsButtonSwitchedToPressed(volatile uint8_t *button_history);
uint8_t IsButtonSwitchedToReleased(volatile uint8_t *button_history);
uint8_t IsButtonNowPressed(volatile uint8_t *button_history);
uint8_t IsButtonNowReleased(volatile uint8_t *button_history);

void ButtonManagerNoRTOS(ctx_simpl_t * const ctx);
void ButtonManagerNoRTOSPressRelease(ctx_simpl_t * const ctx);
#ifdef USE_FREERTOS
void ButtonManager(debounce_ctx_t *ctx);
void ButtonManagerFromISR(debounce_ctx_t *ctx);
bool DelayTicks(debounce_ctx_t *ctx, uint16_t time);
void DelayTicksInit(debounce_ctx_t *ctx);
uint32_t TimerSet(debounce_ctx_t *ctx, const uint32_t addTimeMs);
uint32_t TimerRemainingMs(debounce_ctx_t *ctx, const uint32_t timer);
uint32_t TimerPassMs(debounce_ctx_t *ctx, const uint32_t timer);
uint32_t GetTimeElapsed(button_t **baddr);
uint32_t GetTimeElapsedInv(button_t **baddr);
bool PressDebounce(debounce_ctx_t *ctx, debounce_data_t* pin_data);
bool ReleaseDebounce(debounce_ctx_t *ctx, debounce_data_t* pin_data);
void SetPressCb(button_t **baddr, void (*PressCb)(button_t *baddr));
void SetReleaseCb(button_t **baddr,  void (*ReleaseCb)(button_t *baddr));
bool CreateButton(debounce_ctx_t *ctx, debounce_data_t* pin_data, button_t **baddr);
#endif

//uint8_t CreateInput(debounce_data_t pin_debounce, uint8_t eval_type, void* cb_data);

#endif /* SRC_DEBOUNCE_DEBOUNCE_H_ */
