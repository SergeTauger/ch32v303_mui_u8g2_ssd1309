/*
 * u8g2_glue.c
 *
 *  Created on: Jan 29, 2025
 *      Author: stauger
 */

#include "u8g2_glue.h"
#include "ch32v30x_spi.h"
#include "ch32v30x_gpio.h"

static void LCD_Init(void);
static void BTNs_INIT(void);

static transmisson_type_e transmitDC = cmd_e;

static void BTNs_INIT(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin   = (1 << BTN4_HOME_PIN) | (1 << BTN3_PREV_PIN) |
									(1 << BTN2_NEXT_PIN) | (1 << BTN1_SELECT_PIN) ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(BTN_PORT, &GPIO_InitStructure);
}

static void LCD_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin   = 1 << LCD_DC_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(LCD_DC_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = LCD_RES_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(LCD_RES_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = LCD_CS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(LCD_CS_PORT, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_SPI1, ENABLE);

	//SCK
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//MISO - not initialized

	//MOSI
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Master, full-duplex, soft SS, CPOL=0, CPHA=0, baudrate = 96/16 (0b011)
	LCD_SPI->CTLR1 =  SPI_CTLR1_MSTR | SPI_CTLR1_SSI | SPI_CTLR1_SSM | SPI_CTLR1_BR_0 | SPI_CTLR1_BR_1;
	LCD_SPI->I2SCFGR &= 0xF7FF; //clear bit 11
	LCD_SPI->CRCR = 7; //Reset state

	LCD_SPI->CTLR1 = SPI_CTLR1_SPE;

	//Set CS high
	LCD_CS_PORT->BSHR = GPIO_BSHR_BS0 << LCD_CS_PIN;
}

uint8_t u8x8_gpio_and_delay_HW_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr){
	switch(msg){
	case U8X8_MSG_DELAY_10MICRO:
		Delay_Us(arg_int*10);
		break;

    case U8X8_MSG_DELAY_MILLI:
		Delay_Ms(arg_int);
		break;

    case U8X8_MSG_GPIO_CS:				// CS (chip select) pin: Output level in arg_int
    	if ( arg_int == 0 ){
    		LCD_CS_PORT->BSHR = GPIO_BSHR_BR0 << LCD_CS_PIN;
    	} else {
    		LCD_CS_PORT->BSHR = GPIO_BSHR_BS0 << LCD_CS_PIN;
    	}
    	break;

     case U8X8_MSG_GPIO_DC:				// DC (data/cmd, A0, register select) pin: Output level in arg_int
     	if ( arg_int == 0 ){
     		LCD_DC_PORT->BSHR = GPIO_BSHR_BR0 << LCD_DC_PIN;
     	} else {
     		LCD_DC_PORT->BSHR = GPIO_BSHR_BS0 << LCD_DC_PIN;
     	}
     	break;

     case U8X8_MSG_GPIO_RESET:			// Reset pin: Output level in arg_int/*
      	if ( arg_int == 0 ){
      		LCD_RES_PORT->BSHR = GPIO_BSHR_BR0 << LCD_RES_PIN;
      	} else {
      		LCD_RES_PORT->BSHR = GPIO_BSHR_BS0 << LCD_RES_PIN;
      	}
      	break;

     case U8X8_MSG_GPIO_MENU_SELECT:
		u8x8_SetGPIOResult(u8x8, GPIO_ReadInputDataBit(BTN_PORT, BTN1_SELECT_PIN));
		break;
     case U8X8_MSG_GPIO_MENU_NEXT:
    	u8x8_SetGPIOResult(u8x8, GPIO_ReadInputDataBit(BTN_PORT, BTN2_NEXT_PIN));
		break;
     case U8X8_MSG_GPIO_MENU_PREV:
    	u8x8_SetGPIOResult(u8x8, GPIO_ReadInputDataBit(BTN_PORT, BTN3_PREV_PIN));
		break;
     case U8X8_MSG_GPIO_MENU_HOME:
    	u8x8_SetGPIOResult(u8x8, GPIO_ReadInputDataBit(BTN_PORT, BTN4_HOME_PIN));
		break;

    default:
		u8x8_SetGPIOResult(u8x8, 1);
		break;
	}
	return 1;
}


uint8_t u8x8_byte_HW_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
	uint8_t *data;
	switch(msg) {
	case U8X8_MSG_BYTE_SEND:
    	data = (uint8_t *)arg_ptr;
    	while( arg_int > 0 ) {
    		while( ( LCD_SPI->STATR & SPI_STATR_TXE ) == 0 ){
    			;
    		}
    		LCD_SPI->DATAR = (uint16_t) *data;
    		data++;
    		arg_int--;
    	}
    	break;
    case U8X8_MSG_BYTE_INIT:
    	LCD_Init();
    	BTNs_INIT();
    	break;
    case U8X8_MSG_BYTE_SET_DC:
    	//If not switching between data and command, why wait for transfer completion?
    	if ((arg_int != 0) && (transmitDC == cmd_e)) { //Switching to data
        	while ( LCD_SPI->STATR & SPI_STATR_BSY) {          // wait for transfer completion
        		;
        	}
    		transmitDC = data_e;
        	u8x8_gpio_SetDC(u8x8, arg_int);
    	} else if ((arg_int == 0) && (transmitDC == data_e)) {
        	while ( LCD_SPI->STATR & SPI_STATR_BSY) {          // wait for transfer completion
        		;
        	}
    		transmitDC = cmd_e;
    		u8x8_gpio_SetDC(u8x8, arg_int);    	}
    	break;
    case U8X8_MSG_BYTE_START_TRANSFER:
		//Not using delays. Seems that bus frequency is high enough
    	while ( LCD_SPI->STATR & SPI_STATR_BSY) {          // wait for transfer completion
    		;
    	}
		u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_enable_level);
		break;
    case U8X8_MSG_BYTE_END_TRANSFER:
		//Not using delays. Seems that bus frequency is high enough
    	while ( LCD_SPI->STATR & SPI_STATR_BSY) {          // wait for transfer completion
    		;
    	}
		u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
		break;
    default:
      return 0;
  }
  return 1;
}

/* TODO Replace GetMenuEvent with own to get rid of u8g2_debounce
 * uint8_t u8x8_GetMenuEvent(u8x8_t *u8x8)
{
int c;
c = getc(stdin);
switch(c)
{
case 'n':
return U8X8_MSG_GPIO_MENU_NEXT;
case 'p':
return U8X8_MSG_GPIO_MENU_PREV;
case 's':
return U8X8_MSG_GPIO_MENU_SELECT;
case 'h':
return U8X8_MSG_GPIO_MENU_HOME;
case 'x':
exit(0);
default:
break;
}
return 0;
}
*/
