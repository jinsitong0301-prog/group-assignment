#include <stdio.h>
#include <conio.h>
#include "snake.h"
#include "snake_screen.h"

void snake(void)
{
    Difficulty diff = (Difficulty)1;  
    snake_screen_console();

    if (snake_show_main_menu(&diff) == SNAKE_START)
        printf("Snake game start! difficulty: %d\n", diff);
    else
        printf("Back to home\n");

    _getch();                       
}