# ch32v303_mui_u8g2_ssd1309
Demo on using MUI (u8g2 gui extension) with ch32v303cbt6 MCU, ssd1309 screen and few buttons

## Topics covered
- IDE configuration
- MCU and SSD1309 (HW 4-wire SPI) connection
- Porting u8g2
- Using your own debounce routine (own u8x8_GetMenuEvent implementation)
- Basic menu using MUI with Cyrillic letters

## Possible issues when creating a project with not Latin symbols
Im MRS file encoding can be not overriden by IDE and project encoding settings. Please check text in bottom right corner of IDE window, it should be UTF-8. Otherwise right click on it, choose "File properties", change to UTF-8. If your non-latin text changed to unreadable -  Ctrl-Z,Ctrl-S.
