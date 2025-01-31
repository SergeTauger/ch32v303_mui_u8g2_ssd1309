/********************************** (C) COPYRIGHT *******************************
* File Name          : ch32v30x_conf.h
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Library configuration file.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#ifndef __CH32V30x_CONF_H
#define __CH32V30x_CONF_H

#include "ch32v30x_adc.h"
#include "ch32v30x_bkp.h"
#include "ch32v30x_can.h"
#include "ch32v30x_crc.h"
#include "ch32v30x_dac.h"
#include "ch32v30x_dbgmcu.h"
#include "ch32v30x_dma.h"
#include "ch32v30x_exti.h"
#include "ch32v30x_flash.h"
#include "ch32v30x_fsmc.h"
#include "ch32v30x_gpio.h"
#include "ch32v30x_i2c.h"
#include "ch32v30x_iwdg.h"
#include "ch32v30x_pwr.h"
#include "ch32v30x_rcc.h"
#include "ch32v30x_rtc.h"
#include "ch32v30x_sdio.h"
#include "ch32v30x_spi.h"
#include "ch32v30x_tim.h"
#include "ch32v30x_usart.h"
#include "ch32v30x_wwdg.h"
#include "ch32v30x_it.h"
#include "ch32v30x_misc.h"

#define LCD_CS_PORT			GPIOA
#define LCD_CS_PIN 			15
#define LCD_RES_PORT		GPIOB
#define LCD_RES_PIN 		12
#define LCD_DC_PORT			GPIOB
#define LCD_DC_PIN 			9
#define LCD_SPI				SPI1

#define BTN_PORT			GPIOA
#define BTN4_HOME_PIN		0
#define BTN3_PREV_PIN		1
#define BTN2_NEXT_PIN		2
#define BTN1_SELECT_PIN		3

#endif /* __CH32V30x_CONF_H */


	
	
	
