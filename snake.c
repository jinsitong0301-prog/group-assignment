// snake.c
#include <stdio.h>
#include <conio.h>
#include "snake.h"
#include "tetris_screen.h"   // 临时借用公共函数，等贪吃蛇做好再换

void snake(void)
{
    clear_screen();
    set_color(10);
    gotoxy(30, 12);
    printf("SNAKE GAME (COMMING SOON)");
    _getch();
}