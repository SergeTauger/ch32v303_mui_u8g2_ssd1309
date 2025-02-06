/*
 * debounce.c
 *
 *  Created on: Apr 25, 2022
 *      Author: gryph
 */

#include "debounce.h"
#include <stdbool.h>

void    update_button(const pin_t pin, uint8_t (* const isSet)(const pin_t), volatile uint8_t *button_history){
    *button_history = *button_history << 1;
    uint8_t is_set = (*isSet)(pin);
    *button_history |= is_set;
}

uint8_t IsButtonSwitchedToPressed(volatile uint8_t *button_history){
    uint8_t pressed = 0;
    if ((*button_history & MASK) == BOT_MASK){
        pressed = 1;
        *button_history = 0xFF;
    }
    return pressed;
}

uint8_t IsButtonSwitchedToReleased(volatile uint8_t *button_history){
    uint8_t released = 0;
    if ((*button_history & MASK) == TOP_MASK){
            released = 1;
            *button_history = 0;
    }
    return released;
}

uint8_t IsButtonNowPressed(volatile uint8_t *button_history){
	return (*button_history == 0xFF);
}

uint8_t IsButtonNowReleased(volatile uint8_t *button_history){
	return (*button_history == 0);
}

void ButtonManagerNoRTOS(ctx_simpl_t * const ctx){
	ctx->ticks +=1;
	for(size_t idx=0; idx < ctx->capacity ; idx++)
	{
		button_simpl_t* storage = ctx->storage + idx;
		if(storage->debounce_data == NULL){
			continue;
		}
		update_button(storage->debounce_data->pin, ctx->isSet, &(storage->debounce_data->button_history));
		switch (storage->type){
		case PRESS_PENDING:
			if(IsButtonSwitchedToPressed(&(storage->debounce_data->button_history))){
//				storage->count += 1;
				*(ctx->retvals + idx) += 1;
				storage->type = RELEASE_PENDING;
				storage->PressTime = ctx->ticks;
				ctx->requestPending |= 1<<idx;
			}
			break;
		case RELEASE_PENDING:
			if(IsButtonSwitchedToReleased(&(storage->debounce_data->button_history))){
				storage->type = PRESS_PENDING;
				storage->ReleaseTime = ctx->ticks;
			}
			break;
		default:
			continue;
		}
//		*(ctx->retvals + idx) = storage->count;
	}
//	//Return count at every iteration, zero on request
//	for(size_t idx=0; idx < ctx->capacity ; idx++)
//	{
//		*(ctx->retvals + idx) = counts[idx];
//	}

	size_t req = (ctx->requestPending >> 16) & 0xFFFF;
	while(req){
		size_t idx =  __builtin_ffs(req) - 1;
		req &= ~(1<< idx);
		ctx->requestPending &= ~(1<< (idx+16));
		button_simpl_t* storage = ctx->storage + idx;
//		storage->count = 0;
		storage->PressTime = 0;
		storage->ReleaseTime = 0;
		*(ctx->retvals + idx) = 0;
	}


	/*
	if (ctx->requestPending == VrsVdisReq){
		volatile uint32_t *bkup_ret = ctx->retvals;
		for(size_t idx=0; idx < ctx->capacity ; idx++)
		{
			*(ctx->retvals) = counts[idx];
			ctx->retvals++;
		}
		ctx->retvals = bkup_ret;
		ctx->requestPending = VrsVdisNoReq;
	} else if (ctx->requestPending < VrsVdisReq){
		volatile button_simpl_t* storage = ctx->storage + ctx->requestPending;
		storage->count = 0;
		storage->PressTime = 0;
		storage->ReleaseTime = 0;
		ctx->requestPending = VrsVdisNoReq;
	}
	*/
}

void ButtonManagerNoRTOSPressRelease(ctx_simpl_t * const ctx){
	ctx->ticks +=1;
	for(size_t idx=0; idx < ctx->capacity ; idx++)
	{
		button_simpl_t* storage = ctx->storage + idx;
		if(storage->debounce_data == NULL){
			continue;
		}
		update_button(storage->debounce_data->pin, ctx->isSet, &(storage->debounce_data->button_history));
		switch (storage->type){
		case PRESS_PENDING:
			if(IsButtonSwitchedToPressed(&(storage->debounce_data->button_history))){
//				if (storage->pulldir == PULLDOWN){
//					storage->count += 1;
					*(ctx->retvals + idx) += 1;
//				}
				storage->type = RELEASE_PENDING;
				storage->PressTime = ctx->ticks;
			}
			break;
		case RELEASE_PENDING:
			if(IsButtonSwitchedToReleased(&(storage->debounce_data->button_history))){
//				if (storage->pulldir == PULLUP){
//					storage->count += 1;
					*(ctx->retvals + idx) += 1;
//				}
				storage->type = PRESS_PENDING;
				storage->ReleaseTime = ctx->ticks;
			}
			break;
		default:
			continue;
		}
	}

	size_t req = (ctx->requestPending >> 16) & 0xFFFF;
	while(req){
		size_t idx =  __builtin_ffs(req) - 1;
		req &= ~(1<< idx);
		ctx->requestPending &= ~(1<< (idx+16));
		button_simpl_t* storage = ctx->storage + idx;
//		storage->count = 0;
		storage->PressTime = 0;
		storage->ReleaseTime = 0;
		*(ctx->retvals + idx) = 0;
	}
}
#ifdef USE_FREERTOS
//Called at timer overflow
void ButtonManagerFromISR(debounce_ctx_t *ctx){
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	ctx->ticks +=1;

	for(size_t index=0; index < ctx->capacity ; index++)
	{
		volatile button_t* storage = ctx->storage + index;
		if(((storage->debounce_data == NULL)&&
			   (storage->type != DELAY )) ||
				(storage->type == DEBOUNCE_PENDING )){
			continue;
		}
		update_button(storage->debounce_data->pin, ctx->isSet, &(storage->debounce_data->button_history));
		switch (storage->type){
		case DEBOUNCE_PENDING:
			//Will never enter here
			//When pending buttons are not updated
//			debug_send("I\r\n");
			break;
		case PRESS_PENDING:
			if(IsButtonSwitchedToPressed(&(storage->debounce_data->button_history))){
//				debug_send("P\r\n");
				storage->PressTime = ctx->ticks;
				if (isRtosRunning() && (storage->message != NULL)){
					vTaskNotifyGiveFromISR(storage->message, &xHigherPriorityTaskWoken);
				} else {
					storage->type = RELEASE_PENDING;
//					if (storage->PressCb != NULL){
//						(storage->PressCb)(storage);
//					}
				}
			}
			break;
		case RELEASE_PENDING:
			if(IsButtonSwitchedToReleased(&(storage->debounce_data->button_history))){
				storage->ReleaseTime = ctx->ticks;
//				debug_send("R %d\r\n", storage->ReleaseTime - storage->PressTime);
				if (isRtosRunning() && (storage->message != NULL)){
					vTaskNotifyGiveFromISR(storage->message, &xHigherPriorityTaskWoken);
				} else {
					storage->type = PRESS_PENDING;
//					if (storage->ReleaseCb != NULL){
//						(storage->ReleaseCb)(storage);
//					}
				}
			}
			break;
		case DELAY:
//			debug_send("W\r\n");
			//Completely useless outside FreeRtos
			if(storage->time != 0){
				storage->time --;
			} else {
				storage->type = DEBOUNCE_PENDING;
				vTaskNotifyGiveFromISR(storage->message,&xHigherPriorityTaskWoken);
			}
			break;
		default:
			continue;
		}
	    if( xHigherPriorityTaskWoken ){
	    	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	    }
	}
}

void ButtonManager(debounce_ctx_t *ctx){
	ctx->ticks +=1;

	for(size_t index=0; index < ctx->capacity ; index++)
	{
		volatile button_t* storage = ctx->storage + index;
		if(((storage->debounce_data == NULL)&&
			   (storage->type != DELAY )) ||
				(storage->type == DEBOUNCE_PENDING )){
			continue;
		}
		update_button(storage->debounce_data->pin, ctx->isSet, &(storage->debounce_data->button_history));
//		debug_send("%d " BYTE_TO_BINARY_PATTERN"\r\n", index, BYTE_TO_BINARY(storage->debounce_data->button_history));
		switch (storage->type){
		case DEBOUNCE_PENDING:
			//Will never enter here
			//When pending buttons are not updated
//			debug_send("I\r\n");
			break;
		case PRESS_PENDING:
//			debug_send("Pe\r\n");
			if(IsButtonSwitchedToPressed(&(storage->debounce_data->button_history))){
//				debug_send("P!\r\n");
				storage->PressTime = ctx->ticks;
				if (isRtosRunning() && (storage->message != NULL)){
//					debug_send("PS!\r\n");
					xTaskNotifyGive(storage->message);
				} else {
					storage->type = RELEASE_PENDING;
//					if (storage->PressCb != NULL){
//						(storage->PressCb)(storage);
//					}
				}
			}
			break;
		case RELEASE_PENDING:
			if(IsButtonSwitchedToReleased(&(storage->debounce_data->button_history))){
				storage->ReleaseTime = ctx->ticks;
//				debug_send("R %d\r\n", storage->ReleaseTime - storage->PressTime);
				if (isRtosRunning() && (storage->message != NULL)){
//					debug_send("RS!\r\n");
					xTaskNotifyGive(storage->message);
				} else {
					storage->type = PRESS_PENDING;
//					if (storage->ReleaseCb != NULL){
//						(storage->ReleaseCb)(storage);
//					}
				}
			}
			break;
		case DELAY:
//			debug_send("W\r\n");
			//Completely useless outside FreeRtos
			if(storage->time != 0){
				storage->time --;
			} else {
				storage->type = DEBOUNCE_PENDING;
//				debug_send("Switched to pending\r\n");
				xTaskNotifyGive(storage->message);
			}
			break;
		default:
			continue;
		}
	}
}

uint32_t TimerSet(debounce_ctx_t* ctx, const uint32_t addticks)
{
    int32_t ticks_;

    taskENTER_CRITICAL();
    ticks_ = ctx->ticks;
    taskEXIT_CRITICAL();

    return ticks_ + addticks;
}


bool TimerIsExpired(debounce_ctx_t *ctx, const uint32_t timer)
{
    int32_t ticks_;

    taskENTER_CRITICAL();
    ticks_ = ctx->ticks;
    taskEXIT_CRITICAL();


    return ((ticks_ - timer) < (1UL << 31));
}


uint32_t TimerRemainingMs(debounce_ctx_t *ctx, const uint32_t timer)
{
    int32_t ticks_;

    taskENTER_CRITICAL();
    ticks_ = ctx->ticks;
    taskEXIT_CRITICAL();


    if ((ticks_ - timer) > (1UL << 31))
    {
        return (timer - ticks_);
    }
    else
    {
        return 0;
    }
}


uint32_t TimerPassMs(debounce_ctx_t *ctx, const uint32_t timer)
{
    int32_t ticks_;

    taskENTER_CRITICAL();
    ticks_ = ctx->ticks;
    taskEXIT_CRITICAL();

    return (ticks_ - timer);
}

uint32_t GetTimeElapsed(button_t **baddr){
		return ((*baddr)->ReleaseTime - (*baddr)->PressTime);
}

uint32_t GetTimeElapsedInv(button_t **baddr){
		return ((*baddr)->PressTime - (*baddr)->ReleaseTime);
}


void DelayTicksInit(debounce_ctx_t *ctx){
//Initialization is done in main using cubeMX

// Flush all Delays
    for(size_t index=0; index < ctx->capacity; index++)
    {
    	volatile button_t *storage = ctx->storage + index;
    	storage->message = NULL;
    	storage->time = 0;
    	storage->type = NONE;
    	storage->debounce_data = NULL;
    	storage->ReleaseCb = NULL;
    	storage->PressCb = NULL;
    }
    //Overflow will happen every 1 ms
//    HAL_LPTIM_Counter_Start_IT(&hlptim1, 1);
//    static debounce_data_t test = {{GPIOC, GPIO_PIN_3}, 0};
//    delays1ms[0].debounce_data = &test;
}


void SetPressCb(button_t **baddr, void (*PressCb)(button_t *baddr)){
	taskENTER_CRITICAL();
	(*baddr)->PressCb = PressCb;
	taskEXIT_CRITICAL();
}

void SetReleaseCb(button_t **baddr,  void (*ReleaseCb)(button_t *baddr)){
	taskENTER_CRITICAL();
	(*baddr)->ReleaseCb = ReleaseCb;
	taskEXIT_CRITICAL();
}

bool CreateButton(debounce_ctx_t *ctx, debounce_data_t* pin_data, button_t **baddr){
	/* Task are daemons and are not supposed to be deleted*/
	bool out = false;
	TaskHandle_t xMessage;
	xMessage = xTaskGetCurrentTaskHandle();
    taskENTER_CRITICAL();
    size_t index = 0;
    volatile button_t *storage;
    for(index=0; index < ctx->capacity; index++){
    	storage = ctx->storage+index;
        if ((storage->debounce_data == NULL)&&
			(storage->type == NONE)){
			storage->message = xMessage;
			storage->debounce_data = pin_data;
			storage->type = DELAY;
			storage->time = 1;
            out = true;
            break;
	    }
    }
    taskEXIT_CRITICAL();
    if (out){
    	*baddr = (button_t*)storage;
    	return  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    } else {
    	return out;
    }
}

bool DelayTicks(debounce_ctx_t *ctx, uint16_t ticks){
	bool        out=false;
	TaskHandle_t xMessage;

	xMessage = xTaskGetCurrentTaskHandle();

    taskENTER_CRITICAL();
    /*search a previously used slot */
    volatile button_t *storage;
    for(size_t index=0; index < ctx->capacity; index++){
    	storage = ctx->storage + index;
        if ((storage->type == DEBOUNCE_PENDING) &&
            (storage->debounce_data == NULL)){
        	storage->message = xMessage;
        	storage->time = ticks;
        	storage->type = DELAY;
            out = true;
            break;
	    }
	}
    /*if no recycled slots, create a new one */
    if (out == false){
        for(size_t index=0; index < ctx->capacity; index++){
        	storage = ctx->storage + index;
            if ((storage->type == NONE) &&
                (storage->debounce_data == NULL)){
            	storage->message = xMessage;
            	storage->time = ticks;
            	storage->type = DELAY;
                out = true;
                break;
    	    }
    	}
    }
    taskEXIT_CRITICAL();


    if(out){
        return ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    } else {
        return out;
    }
}
//Should be called after CreateButton
bool PressDebounce(debounce_ctx_t *ctx, debounce_data_t* pin_data){
	bool        out=false;

    taskENTER_CRITICAL();
    for(size_t index=0; index < ctx->capacity; index++){
    	volatile button_t* storage = ctx->storage + index;
        if (((storage->type == DEBOUNCE_PENDING) || (storage->type == RELEASE_PENDING)) &&
        	(storage->debounce_data == pin_data)) {

        	storage->type = PRESS_PENDING;
            out = true;
            break;
	    }
	}
    taskEXIT_CRITICAL();

    if(out){
    	//Calling from FreeRtos task: suspend task till button is pressed
    	if (isRtosRunning()){
			return ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		//Calling outside FreeRtos
		} else {
			return out;
		}
    } else {
        return out;
    }
}

//Should be called after CreateButton
bool ReleaseDebounce(debounce_ctx_t *ctx, debounce_data_t* pin_data){
	bool        out=false;

    taskENTER_CRITICAL();
    for(size_t index=0; index < ctx->capacity; index++){
    	volatile button_t *storage = ctx->storage + index;
        if (((storage->type == DEBOUNCE_PENDING) || (storage->type == PRESS_PENDING)) &&
        	(storage->debounce_data == pin_data)) {

        	storage->type = RELEASE_PENDING;
            out = true;
            break;
	    }
	}
    taskEXIT_CRITICAL();


    if(out){
    	//Calling from FreeRtos task: suspend task till button is pressed
    	if (isRtosRunning()){
			return ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		//Calling outside FreeRtos
		} else {
			return out;
		}
    } else {
        return out;
    }
}
#endif
