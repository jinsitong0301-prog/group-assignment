// game2048.c - 2048 게임
#define _CRT_SECURE_NO_WARNINGS   // Visual Studio가 일부 함수를 에러로 막지 않게 (include보다 위에)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "game2048.h"

#define N 4

static int board[N][N];   // 4x4 보드. 0은 빈 칸
static int score;         // 점수 (오늘은 쓰지 않아도 됨)

/* ===== 1단계: 보드 준비와 그리기 ===== */

// 보드의 모든 칸을 0으로 만든다
static void init_board(void)
{
    for (int r = 0;r < N;r++)
        for (int c = 0;c < N;c++)
            board[r][c] = 0;
    score = 0;      //모든 칸을 0으로 초기화
}

// 화면을 지우고 보드를 그린다
// 빈 칸(0)은 공백, 숫자는 칸 안에 출력
static void draw(void)
{
    system("cls");   //콘솔화면지우기
    for (int r = 0;r < N;r++) {
        printf("+------+------+------+------+\n");
        for (int c = 0;c < N;c++) {
            if (board[r][c] == 0)
                printf("|      ");
            else if (board[r][c] < 10)
                printf("|  %d   ", board[r][c]);   // 1자리: 앞 2칸, 뒤 3칸
            else if (board[r][c] < 100)
                printf("|  %d  ", board[r][c]);    // 2자리: 앞 2칸, 뒤 2칸
            else if (board[r][c] < 1000)
                printf("| %d  ", board[r][c]);     // 3자리: 앞 1칸, 뒤 2칸
            else
                printf("| %d ", board[r][c]);      // 4자리: 앞 1칸, 뒤 1칸
        }
        printf("|\n");
    }
    printf("+------+------+------+------+\n");
}

/* ===== 2단계: 새 타일 놓기 ===== */

// 빈 칸 중 하나를 무작위로 골라 4또는2를 놓는다 
static void add_tile(void) {
    int empty = 0;
    for (int r = 0;r < N;r++)
        for(int c=0;c<N;c++)
        if (board[r][c] == 0)empty++;

    if (empty == 0) return;

    int pick = rand() % empty;
    for (int r = 0;r < N;r++) {
        for (int c = 0;c < N;c++) {
            if (board[r][c] == 0) {
                if (pick == 0) {
                    board[r][c]=(rand() % 10 == 0) ? 4 : 2;
                    return;
                }
                pick--;
            }
        }
    }
}

/* ===== 4단계: 왼쪽으로 밀고 합치기 ===== */

// 모든 줄을 왼쪽으로 밀고, 이웃한 같은 숫자를 한 번씩 합친다
// 예: 2 0 2 4 -> 4 4 0 0,  2 2 2 2 -> 4 4 0 0
// 하나라도 움직였으면 1, 아니면 0을 돌려준다
static int slide_left(void)
{
    int moved = 0;

    for (int r = 0;r < N;r++) {
        int tmp[N] = { 0 };
        int res[N] = { 0 };
        int k = 0;

        for (int c = 0;c < N;c++)
            if (board[r][c] != 0)tmp[k++] = board[r][c];

        for (int c = 0;c < N - 1;c++) {
            if (tmp[c] != 0 && tmp[c] == tmp[c + 1]) {
                tmp[c] *= 2;
                score += tmp[c];
                tmp[c + 1] = 0;
            }
        }

        k = 0;
        for (int c = 0;c < N;c++)
            if (tmp[c] != 0)res[k++] = tmp[c];

        for (int c = 0;c < N;c++) {
            if (board[r][c] != res[c])moved = 1;
            board[r][c] = res[c];
        }
    }
    return moved;
}

/* ===== 5단계: 네 방향으로 움직이기 ===== */

// 보드를 시계 방향으로 90도 돌린다
static void rotate(void)
{
    int tmp[N][N];
    for (int r = 0;r < N;r++) 
        for (int c = 0;c < N;c++)
            tmp[c][N - 1 - r] = board[r][c];

    for (int r = 0;r < N;r++)
        for (int c = 0;c < N;c++)
            board[r][c] = tmp[r][c];
}

// 보드를 turns번 돌린 뒤 왼쪽으로 밀고, 다시 원래 방향으로 돌려놓는다
// 왼쪽: 0번, 아래: 1번, 오른쪽: 2번, 위: 3번
// 움직였으면 1, 아니면 0을 돌려준다
static int move_tiles(int turns)
{
    for (int i = 0;i < turns;i++)rotate();
    int moved = slide_left();
    for (int i = 0;i < (4 - turns) % 4;i++)rotate();

    return moved;
}

/* ===== 3단계: 게임 진행 ===== */

// main.c에서 부르는 2048 시작 함수
void run_2048(void)
{
    srand((unsigned int)time(NULL));
    init_board();
    add_tile();
    add_tile();

    while (1) {
        draw();
        int key = _getch();
        if (key == 27) return;          // ESC
        if (key == 0 || key == 224) {
            key = _getch();
            int moved = 0;
            if (key == 75) moved = move_tiles(0);       // 왼쪽
            else if (key == 80) moved = move_tiles(1);  // 아래
            else if (key == 77) moved = move_tiles(2);  // 오른쪽
            else if (key == 72) moved = move_tiles(3);  // 위
            if (moved) add_tile();                      // 움직였을 때만 새 타일
        }
    }
}

