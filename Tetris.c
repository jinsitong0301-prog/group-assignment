#include <stdio.h>
#include <conio.h>
#include "Tetris.h"
#include "tetris_screen.h"
#include <windows.h>
#include <stdlib.h>
#include <time.h>

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

void generate_piece(int piece[4][4]); // 함수 선언

// Rotate block clockwise
void rotate_piece(int current[4][4], int rotated[4][4]) {

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            rotated[x][3 - y] = current[y][x]; // 시계 방향으로 90° 회전
        }
    }
}

// Clear completed lines
int clear_lines(int board[20][10]) {

    int lines = 0;

    for(int y = 19; y >= 0; y--) {

        int full = 1;

        // Check whether the row is full
        for (int x = 0; x < 10; x++) {
            if (board[y][x] == 0) {
                full = 0;
                break;
            }
        }

        // Clear the full row
        if (full) {
            lines++;

            // Move rows above down
            for (int row = y; row > 0; row--) {
                for (int x = 0; x < 10; x++) {
                    board[row][x] = board[row - 1][x];
                }
            }

            // Clear the top row
            for (int x = 0; x < 10; x++) {
                board[0][x] = 0;
            }

            // Check the same row again
            y++;
        }
    }
    return lines;
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

    int current[4][4] = {0};
    int next[4][4] = {0};

    generate_piece(current);
    generate_piece(next);

    int cur_x = 3;
    int cur_y = 0;

    int score = 0;
    int level = 1;
    int high_score = 0;

    // Random Seed
    srand((unsigned int)time(NULL));

    // Game Loop
    DWORD last_drop = GetTickCount();
    int game_over = 0; // 게임 루프를 중지
    int lost = 0; // 패배로 인해 게임이 종료되는지 또는 플레이어가 스스로 종료하는지 확인
    // ESC -> gameover =0; lost =0;
    // 패배 -> gameover =1; lost =1;

    while (!game_over) {
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
        // Keyboard input
        if(_kbhit()) { // 방금 키보드의 아무 키나 눌렀는지 확인 - 키가 없으면 확인하고 계속 실행
            int key = _getch(); //  Enter 없이 누른 키를 읽음
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
                // Down
                else if (key == 80) { // 아래쪽 키 사용하여 떨어지는 블록을 더 빠르게 밀어냅
                    if (can_move_tetris(board, current, cur_x, cur_y, 0, 1))
                        cur_y++;
                }
                else if (key == 72) {
                    int rotated[4][4] = {0};

                    rotate_piece(current, rotated);

                    if (can_move_tetris(board, rotated, cur_x, cur_y, 0, 0)) {

                        for (int y = 0; y < 4; y++) {
                            for (int x = 0; x < 4; x++) {
                                current[y][x] = rotated[y][x];
                            }

                        }
                    }
                }
            }
            //Exit Game
            else if (key == 27) { // Esc - Exit the game
                game_over = 1;
            }

        }

        // Automatic falling
        DWORD now = GetTickCount(); //  현재 시스템 시간(밀리초)을 가져옴

        if (now - last_drop >= 500) { // 두 테스트 사이의 시간 간격을 계산 // 500ms=0.5s
                                    // 블록은 0.5초마다 자동으로 한 칸씩 떨어집
            last_drop = now; // 타임라인 업데이트

            // Move Down
            if (can_move_tetris(board, current, cur_x, cur_y, 0, 1)) {
                cur_y++;
            }
            
            // Cannot move down -> lock block (블록 고정).
            else {
                for (int y = 0; y < 4; y++) {
                    for (int x = 0; x < 4; x++) {
                        if (current[y][x] != 0) {

                            int bx = cur_x + x;
                            int by = cur_y + y;

                            if (by >= 0 && by < 20 &&
                                bx >= 0 && bx < 20) {
                                    board[by][bx] = current[y][x]; // 현재 블록을 보드에 고정
                            }
                        }
                    }

                }
                // Clear lines and update score
                int lines = clear_lines(board);
                
                if (lines == 1)
                    score += 100;
                else if (lines == 2)
                    score += 300;
                else if (lines == 3)
                    score += 500;
                else if (lines == 4) 
                    score += 800;
            
                // Move next block to current
                for (int y = 0; y < 4; y++) {
                    for (int x = 0; x < 4; x++) {
                        current[y][x] = next[y][x];
                    }
                }

                // Create new next block
                generate_piece(next); // 새로운 블록 모양을 만들고 다음 배열에 저장

                // Reset position
                cur_x = 3;
                cur_y = 0;

                // Check Game Over
                if (!can_move_tetris(board, current, cur_x, cur_y, 0, 0)) { // 새 블록이 시작 위치에 나타날 수 있는지 확인
                    game_over = 1;
                    lost = 1; // 새 블록이 원래 위치에 나타날 수 없으면 게임이 종료됨
                }
            }
        }
        Sleep(30); // 일정 시간 동안 프로그램을 중지(0.03s) (각 루프가 끝나면 프로그램은 다음 루프를 계속하기 전에 약 30ms 동안 일시 중지됩)
                // Game Loop의 반복 속도 조정
    }

    clear_screen();
    
    if (lost) {
        int new_record = 0;

        if (score > high_score) {
            high_score = score;
            new_record = 1;
        }

        show_game_over(score, high_score, new_record);
    }
    return;
}

// Create a random Tetris blocks (Tetrominoes)
void generate_piece(int piece[4][4]) {
    int shapes[7][4][4] = {
        // I
        {
            {1, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
        // O
        {
            {2, 2, 0, 0},
            {2, 2, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        },
        // T
        {
            {0, 3, 0, 0},
            {3, 3, 3, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        },
        // S
        {
            {0, 4, 4, 0},
            {4, 4, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        },
        // Z
        {
            {5, 5, 0, 0},
            {0, 5, 5, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        },
        // J
        {
            {0, 0, 6, 0},
            {6, 6, 6, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        },
        // L
        {
            {7, 0, 0, 0},
            {7, 7, 7, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        }
    };

    int type = rand() % 7;

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            piece[y][x] = shapes[type][y][x];
        }
    }
}