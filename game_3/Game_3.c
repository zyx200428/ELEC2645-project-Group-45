#include "Game_3.h"
#include "InputHandler.h"
#include "LCD.h"
#include "stm32l4xx_hal.h"

extern ST7789V2_cfg_t cfg0;

MenuState Game3_Run(void)
{
    while (1) {
        Input_Read();

        if (current_input.btn3_pressed) {
            return MENU_STATE_HOME;
        }

        LCD_Fill_Buffer(0);
        LCD_printString("GAME 3 PLACEHOLDER", 25, 50, 1, 2);
        LCD_printString("No game added here", 40, 100, 1, 1);
        LCD_printString("yet.", 105, 120, 1, 1);
        LCD_printString("Press BT3 to return", 35, 170, 1, 1);
        LCD_Refresh(&cfg0);

        HAL_Delay(30);
    }
}
 
        

       
