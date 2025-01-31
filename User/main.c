/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

#include "ch32v30x.h"

#include "u8g2_glue.h"
#include "mui.h"
#include "mui_u8g2.h"
/* Global typedef */

/* Global define */

/* Global Variable */

static uint8_t mui_hrule(mui_t *ui, uint8_t msg);
static uint16_t menu_get_cnt(void *data);
static const char *menu_get_str(void *data, uint16_t index);

static uint8_t mui_hrule(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      u8g2_DrawHLine(u8g2, 0, mui_get_y(ui), u8g2_GetDisplayWidth(u8g2));
      break;
  }
  return 0;
}


static uint16_t menu_get_cnt(void *data)
{
  return 4;    /* number of menu entries */
}

static const char *alt_menu[] =
	{
		MUI_0 "Goto Main Menu",
		MUI_10 "Option 1",
		MUI_11 "Option 2",
		MUI_13 "Option 3",
	};

static const char *menu_get_str(void *data, uint16_t index)
{
  return ((char**)data)[index];
}

u8g2_t u8g2;                    // u8g2 object
mui_t mui;

muif_t muif_list[] = {
  /* normal text style */
  //MUIF_U8G2_FONT_STYLE(0, u8g2_font_helvR08_tr),
  MUIF_U8G2_FONT_STYLE(0, u8g2_font_6x12_t_cyrillic),

  /* bold text style */
  //MUIF_U8G2_FONT_STYLE(1, u8g2_font_helvB08_tr),
  MUIF_U8G2_FONT_STYLE(1, u8g2_font_haxrcorp4089_t_cyrillic),

  /* monospaced font */
  MUIF_U8G2_FONT_STYLE(2, u8g2_font_profont12_tr),

  /* food and drink */
  MUIF_U8G2_FONT_STYLE(3, u8g2_font_streamline_food_drink_t),

  /* horizontal line (hrule) */
  MUIF_RO("HR", mui_hrule),

  /* main menu */
  MUIF_RO("GP",mui_u8g2_goto_data),
  MUIF_BUTTON("GC", mui_u8g2_goto_form_w1_pi),

  /*alt menu */
  MUIF_U8G2_U16_LIST("LG", NULL, &alt_menu, menu_get_str, menu_get_cnt, mui_u8g2_u16_list_goto_w1_pi),

  MUIF_U8G2_LABEL(),
};


fds_t fds_data[] =


/* top level main menu */
MUI_FORM(0)
MUI_STYLE(1)
MUI_LABEL(5,9, "§¤§Ý§Ñ§Ó§ß§à§Ö §Þ§Ö§ß§ð")
MUI_XY("HR", 0,12)
MUI_STYLE(0)

MUI_DATA("GP",
    MUI_10 "§¯§Ñ§ã§ä§â§à§Û§Ü§Ñ §Ó§í§ç§à§Õ§Ñ|"
    MUI_11 "§£§Ö§ã §Ú§Þ§á§å§Ý§î§ã§Ñ|"
    MUI_13 "§³§Ò§â§à§ã §ß§Ñ§ã§ä§â§à§Ö§Ü|"
    MUI_14 "§®§Ö§ß§ð §ß§Ñ §Ñ§ß§Ô§Ý§Ú§Û§ã§Ü§à§Þ"
    )

MUI_XYA("GC", 5, 23, 0)
MUI_XYA("GC", 5, 35, 1)
MUI_XYA("GC", 5, 47, 2)
MUI_XYA("GC", 5, 59, 3)

MUI_FORM(14)
MUI_STYLE(2)
MUI_LABEL(5,10, "Menu in English")
MUI_XY("HR", 0,13)
MUI_XYA("LG", 5, 25, 0)
MUI_XYA("LG", 5, 37, 1)
MUI_XYA("LG", 5, 49, 2)
;

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemCoreClockUpdate();
	Delay_Init();

	u8g2_Setup_ssd1309_128x64_noname0_f(&u8g2, U8G2_R2, u8x8_byte_HW_spi, u8x8_gpio_and_delay_HW_spi);

	u8g2_InitDisplay(&u8g2);
	u8g2_ClearDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0);
	u8g2_SetFontMode(&u8g2, 1);

	mui_Init(&mui, (void *)&u8g2, fds_data, muif_list, sizeof(muif_list)/sizeof(muif_t));

	mui_GotoForm(&mui, /* form_id= */ 0, /* initial_cursor_position= */ 0);

	uint8_t is_redraw = 1;

	while(1) {
		/* check whether the menu is active */
		if ( mui_IsFormActive(&mui) ){

			/* if so, then draw the menu */

			if ( is_redraw ) {
				u8g2_FirstPage(&u8g2);
				do {
					mui_Draw(&mui);
				} while( u8g2_NextPage(&u8g2) );
				is_redraw = 0;
			}

			/* handle events */

			switch(u8x8_GetMenuEvent(u8g2_GetU8x8(&u8g2))) {
				case U8X8_MSG_GPIO_MENU_SELECT:
					mui_SendSelect(&mui);
					is_redraw = 1;
					break;
				case U8X8_MSG_GPIO_MENU_NEXT:
					mui_NextField(&mui);
					is_redraw = 1;
					break;
				case U8X8_MSG_GPIO_MENU_PREV:
					mui_PrevField(&mui);
					is_redraw = 1;
					break;
				case U8X8_MSG_GPIO_MENU_HOME:
					mui_RestoreForm(&mui);
					is_redraw = 1;
					break;
			}
		} else {
		/* do something else, maybe clear the screen */
		}
	}
}

