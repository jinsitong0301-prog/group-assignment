#include "game2048.h"
#include "Tetris.h"
#include "tetris_screen.h"
#include <stdio.h>
#include <conio.h>
#include "snake.h"
#define MENU_count 5

static void draw_menu(int sel){
    const char*items[MENU_count]={"TETRIS","SNAKE GAME","2048 GAME","MINESWEEPER","EXIT"};
//게임의 메뉴 항목을 배열로 저장

    set_color(11);
    gotoxy(33, 7);
    printf("GAME COLLECTION");
//게임의 제목 출력

    for(int i=0;i<MENU_count;i++){
        set_color(sel==i?14:7);
        gotoxy(32,11+i*2);
        printf("%s", items[i]);
    }
    //게임의 메뉴 항목 출력, 선택된 항목은 노란색(14), 선택되지 않은 항목은 흰색(7)
}

int main(void)
{
    int sel=0;

     screen_console();
      screen_console();
    while(1){
        draw_menu(sel);
        int key=_getch();//키 입력 대기
        if(key==224){
            key=_getch();// 방향키 입력 처리
            if(key==72) sel=(sel+MENU_count-1)%MENU_count;//
            else if(key==80) sel=(sel+1)%MENU_count;
        }//방향키 입력 처리, 위쪽 키는 sel을 감소시키고, 아래쪽 키는 sel을 증가시킴. 메뉴 항목 수를 고려하여 순환하도록 함
        else if(key==13){
            clear_screen();
            if(sel==0){
                tetris();
            }//선택된 항목이 0이면 tetris() 함수를 호출하여 테트리스 게임을 시작
            else if(sel==1){
                snake();
            }//선택된 항목이 1이면 snake() 함수를 호출하여 스네이크 게임을 시작
            else if(sel==2){
                run_2048();
            }//선택된 항목이 2이면 run_2048() 함수를 호출하여 2048 게임을 시작
            else if(sel==3){
                MINESWEEPER();
            }
            else if(sel==4){
                break;
            }//선택된 항목이 4이면 프로그램 종료
            clear_screen();
        }
    }
    clear_screen();
    return 0;
}
