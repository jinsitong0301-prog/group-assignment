#include <stdio.h>
#include <conio.h>
#include "Tetris.h"
#include "tetris_screen.h"

void tetris(void) {
    Difficulty diff = NORMAL;//시작 할 때에는 NORMAL
    screen_console();//게임 시작 전 콘솔 창 초기화

    MenuResult r = show_main_menu(&diff);
    //show_main_menu(&diff):프로그램에 여기서 정지,버튼을 선택해야 계속
    //r은 screen.c에서 반환,(START,EXIT)반환가능
    if (r == EXIT) {
        clear_screen();
        return;
    }
    // 화면 지우기
    clear_screen();
    //
    int board[20][10] = {0};

    int current[4][4] = {
        {0, 1, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}   
    };

    int next[4][4] = {
        {2, 2, 0, 0},
        {2, 2, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };

    int cur_x = 3;
    int cur_y = 0;

    int score = 0;
    int level = 1;
    int high_score = 0;
    //
    draw_game_screen(
        board,
        current,
        cur_x,
        cur_y,
        next,
        score,
        level,
        high_score,
        diff
    );

    // 
    _getch();

    clear_screen();

    //if (r == START) printf("게임 시작! 난이도: %d\n", diff);
    //else printf("홈으로 돌아가기\n");
    return;
}