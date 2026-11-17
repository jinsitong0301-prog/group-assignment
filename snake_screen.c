// screen.c
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "snake_screen.h"

void snake_screen_console(void) {
    system("chcp 65001>nul");//UTF-8로 설정(한글 오류 피함)
    CONSOLE_CURSOR_INFO ci = { 1, FALSE };   // 커서 숨기기
    //CONSOLE_CURSOR_INFO:커서의 속성을 정의하는 struct,{커서의 크기(1~100),보일수 있는가(TURE/FALSE)}
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
    //GetStdHandle(STD_OUTPUT_HANDLE):지금 이 콘솔 창
    //SetConsoleCursorInfo:콘솔 커서 정보 설정
    system("mode con cols=80 lines=30");   // 콘솔 창 크기 설정
}

void snake_gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };//x,y를 상자 안에 보관하기
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void snake_set_color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    //콘솔의 색갈 설정해 줌
}
void snake_clear_screen(void) { system("cls"); }
MenuResult snake_show_main_menu(Difficulty *diff){
    const char* diff_name[3]={"EASY","NORMAL","HARD"};
    int sel=0;   // 현재 선택된 메뉴 번호,처음은 0(첫번쨰)
    snake_clear_screen();//들어로면 화면을 한번 지우기
    while(1){
        snake_set_color(11);
        snake_gotoxy(33,8);
        printf("SNAKE GAME");

        snake_set_color(sel==0?14:7);
        snake_gotoxy(32,13);
        printf(sel==0?">>":"  ");
        snake_gotoxy(35,13);
        printf("게임 시작");
        snake_set_color(sel==1?14:7);
        snake_gotoxy(20,15);
        printf(sel==1?">>":"  ");
        snake_gotoxy(22,45);
        printf("난이도 선택 ");
        for(int i=0;i<3;i++){
            snake_set_color(i==*diff?14:8);
            if(i==*diff){
                printf("[%s]",diff_name[i]);
            }
            else{
                printf("  %s  ",diff_name[i]);
            }
        }

        snake_set_color(sel==2?14:7);
        snake_gotoxy(32,17);
        printf(sel==2?">>":"  ");
        snake_gotoxy(35,17);
        printf("종료");
        int key=_getch();
        if(key==224){
            key=_getch();
            if(key==72) sel=(sel+3-1)%3;
            else if(key==80) sel=(sel+1)%3;
            if(sel==1){
            if(key==75) *diff=(*diff+3-1)%3;
            else if(key==77) *diff=(*diff+1)%3;}
        }
        else if(key==13){
            if(sel==0) return SNAKE_START;
            else if(sel==1){
                *diff=(*diff+1)%3;
            }
            else if(sel==2) return SNAKE_EXIT;
        }

    }
}