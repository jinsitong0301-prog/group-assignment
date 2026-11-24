#include <stdio.h>
#include <conio.h>
#include "Tetris.h"
#include "tetris_screen.h"

// 충돌 검사 기능
int can_move_tetris(
    int board[20][10],
    int current[4][4],
    int cur_x,
    int cur_y,
    int dx,
    int dy
) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (current[y][x] == 0)
                continue;
            
            int new_x = cur_x + x + dx;
            int new_y = cur_y + y + dy;

            // check left, right and bottom borders
            if (new_x < 0 || new_x >= 10 || new_y >= 20) //  블록이 플레이 영역 밖으로 나가는 것을 방지
                return 0;
            
            // check collision with fixed blocks
            if (new_y >= 0 && board[new_y][new_x] != 0) // 고정된 블록과 겹치지 않도록함
                return 0;
        }
    }
    return 1;
}

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
    // Game Loop
    while (1) {
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

        int key = _getch(); //  Enter 없이 누른 키를 읽음

        //Exit Game
        if (key == 27) { // Esc - Exit the game
            break;
        }

        // Arrow keys
        if (key == 224 || key == 0) { // 화살표 키 입력
            key = _getch();

            // Left
            if (key == 75) { // <--
                if (can_move_tetris(board, current, cur_x, cur_y, -1, 0))
                    cur_x--;
            }
            // Right
            else if (key == 77) { // -->
                if (can_move_tetris(board, current, cur_x, cur_y, 1, 0)) 
                    cur_x++;
            }
        }
    }

    clear_screen();
    return;
}