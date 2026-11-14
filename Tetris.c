#include <stdio.h>
#include "Tetris.h"
#include "tetris_screen.h"

void tetris(void) {
    Difficulty diff = NORMAL;//시작 할 때에는 NORMAL
    screen_console();//게임 시작 전 콘솔 창 초기화

    MenuResult r = show_main_menu(&diff);
    //show_main_menu(&diff):프로그램에 여기서 정지,버튼을 선택해야 계속
    //r은 screen.c에서 반환,(START,EXIT)반환가능
    clear_screen();// 화면 지우기
    if (r == START) printf("게임 시작! 난이도: %d\n", diff);
    else printf("홈으로 돌아가기\n");
    return;
}