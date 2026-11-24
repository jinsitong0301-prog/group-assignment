// game2048.c - 2048 게임
#define _CRT_SECURE_NO_WARNINGS   // Visual Studio가 fopen 등을 에러로 막지 않게 (include보다 위에)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "game2048.h"

#define N 4
#define BEST_FILE "game2048_best.txt"   // 최고 점수 저장 파일

// 화면 배치 (80 x 30 콘솔 기준, 보드 + 오른쪽 정보가 가운데에 오도록)
#define BOARD_X 13              // 보드 왼쪽 위 x
#define BOARD_Y 6               // 보드 왼쪽 위 y
#define PANEL_X (BOARD_X + 39)  // 오른쪽 정보 x (보드 가로 33칸 + 간격 6칸)
#define MSG_Y   (BOARD_Y + 18)  // 보드 아래 메시지 y

static int board[N][N];                  // 4x4 보드. 0은 빈 칸
static int score;                        // 이번 판 점수
static int diff = 1;                     // 난이도 번호: 0(512), 1(1024), 2(2048). 처음은 1024
static int best[G2048_DIFF_COUNT];       // 난이도별 최고 점수

/* ===== 최고 점수 파일 ===== */

// 파일에서 난이도별 최고 점수 3개를 읽는다. 파일이 없으면 모두 0
static void load_best(void)
{
    for (int i = 0; i < G2048_DIFF_COUNT; i++) best[i] = 0;

    FILE *fp = fopen(BEST_FILE, "r");
    if (fp == NULL) return;              // 처음 실행하면 파일이 없다
    for (int i = 0; i < G2048_DIFF_COUNT; i++)
        if (fscanf(fp, "%d", &best[i]) != 1) best[i] = 0;
    fclose(fp);
}

// 난이도별 최고 점수 3개를 파일에 쓴다
static void save_best(void)
{
    FILE *fp = fopen(BEST_FILE, "w");
    if (fp == NULL) return;              // 저장에 실패해도 게임은 계속
    for (int i = 0; i < G2048_DIFF_COUNT; i++)
        fprintf(fp, "%d\n", best[i]);
    fclose(fp);
}

// 이번 점수가 지금 난이도의 최고 점수보다 높으면 저장하고 1을 돌려준다
static int update_best(void)
{
    if (score > best[diff]) {
        best[diff] = score;
        save_best();
        return 1;
    }
    return 0;
}

/* ===== 보드 준비와 새 타일 ===== */

// 보드의 모든 칸을 0으로 만든다
static void init_board(void)
{
    for (int r = 0; r < N; r++)
        for (int c = 0; c < N; c++)
            board[r][c] = 0;
    score = 0;      // 점수 초기화
}

// 빈 칸 중 하나를 무작위로 골라 4 또는 2를 놓는다
static void add_tile(void)
{
    int empty = 0;
    for (int r = 0; r < N; r++)
        for (int c = 0; c < N; c++)
            if (board[r][c] == 0) empty++;

    if (empty == 0) return;

    int pick = rand() % empty;
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (board[r][c] == 0) {
                if (pick == 0) {
                    board[r][c] = (rand() % 10 == 0) ? 4 : 2;
                    return;
                }
                pick--;
            }
        }
    }
}

/* ===== 왼쪽으로 밀고 합치기 ===== */

// 모든 줄을 왼쪽으로 밀고, 이웃한 같은 숫자를 한 번씩 합친다
// 예: 2 0 2 4 -> 4 4 0 0,  2 2 2 2 -> 4 4 0 0
// 하나라도 움직였으면 1, 아니면 0을 돌려준다
static int slide_left(void)
{
    int moved = 0;

    for (int r = 0; r < N; r++) {
        int tmp[N] = { 0 };
        int res[N] = { 0 };
        int k = 0;

        for (int c = 0; c < N; c++)
            if (board[r][c] != 0) tmp[k++] = board[r][c];

        for (int c = 0; c < N - 1; c++) {
            if (tmp[c] != 0 && tmp[c] == tmp[c + 1]) {
                tmp[c] *= 2;
                score += tmp[c];
                tmp[c + 1] = 0;
            }
        }

        k = 0;
        for (int c = 0; c < N; c++)
            if (tmp[c] != 0) res[k++] = tmp[c];

        for (int c = 0; c < N; c++) {
            if (board[r][c] != res[c]) moved = 1;
            board[r][c] = res[c];
        }
    }
    return moved;
}

/* ===== 네 방향으로 움직이기 ===== */

// 보드를 시계 방향으로 90도 돌린다
static void rotate(void)
{
    int tmp[N][N];
    for (int r = 0; r < N; r++)
        for (int c = 0; c < N; c++)
            tmp[c][N - 1 - r] = board[r][c];

    for (int r = 0; r < N; r++)
        for (int c = 0; c < N; c++)
            board[r][c] = tmp[r][c];
}

// 보드를 turns번 돌린 뒤 왼쪽으로 밀고, 다시 원래 방향으로 돌려놓는다
// 왼쪽: 0번, 아래: 1번, 오른쪽: 2번, 위: 3번
static int move_tiles(int turns)
{
    for (int i = 0; i < turns; i++) rotate();
    int moved = slide_left();
    for (int i = 0; i < (4 - turns) % 4; i++) rotate();
    return moved;
}

/* ===== 승리와 게임 오버 확인 ===== */

// 빈 칸이 있거나, 이웃한 같은 숫자가 있으면 아직 움직일 수 있다
static int can_move(void)
{
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (board[r][c] == 0) return 1;
            if (c < N - 1 && board[r][c] == board[r][c + 1]) return 1;
            if (r < N - 1 && board[r][c] == board[r + 1][c]) return 1;
        }
    }
    return 0;
}

// 목표 숫자를 만들었는지 확인
static int reached_goal(void)
{
    int goal = g2048_goal(diff);
    for (int r = 0; r < N; r++)
        for (int c = 0; c < N; c++)
            if (board[r][c] >= goal) return 1;
    return 0;
}

/* ===== 게임 화면 그리기 ===== */

// 숫자마다 다른 글자 색
static int tile_color(int value)
{
    switch (value) {
    case 2:    return 15;   // 흰색
    case 4:    return 14;   // 노랑
    case 8:    return 6;    // 갈색
    case 16:   return 12;   // 빨강
    case 32:   return 13;   // 분홍
    case 64:   return 5;    // 보라
    case 128:  return 10;   // 연두
    case 256:  return 2;    // 초록
    case 512:  return 11;   // 하늘
    case 1024: return 9;    // 파랑
    default:   return 224;  // 2048: 노란 배경에 검은 글자
    }
}

// 화면을 지우고 바뀌지 않는 부분(제목, 칸 테두리, 오른쪽 글자)을 그린다
static void draw_frame(void)
{
    system("cls");

    // 제목: 디지털 숫자 모양 2048 (노란색, 보드 왼쪽에 맞춤)
    g2048_set_color(14);
    g2048_gotoxy(BOARD_X, BOARD_Y - 4);  printf(" _   _       _ ");
    g2048_gotoxy(BOARD_X, BOARD_Y - 3);  printf(" _| | | |_| |_|");
    g2048_gotoxy(BOARD_X, BOARD_Y - 2);  printf("|_  |_|   | |_|");
    g2048_set_color(7);

    // 칸 테두리: 칸 하나는 테두리 포함 가로 8칸 x 세로 4줄 (글자 비율 때문에 정사각형으로 보임)
    for (int r = 0; r <= N; r++) {
        int y = BOARD_Y + r * 4;
        g2048_gotoxy(BOARD_X, y);
        printf("+-------+-------+-------+-------+");
        if (r == N) break;
        for (int line = 1; line <= 3; line++) {
            for (int c = 0; c <= N; c++) {
                g2048_gotoxy(BOARD_X + c * 8, y + line);
                printf("|");
            }
        }
    }

    // 오른쪽 정보의 이름 부분 (하늘색)
    g2048_set_color(11);
    g2048_gotoxy(PANEL_X, BOARD_Y + 1);   printf("SCORE");
    g2048_gotoxy(PANEL_X, BOARD_Y + 4);   printf("BEST SCORE");
    g2048_gotoxy(PANEL_X, BOARD_Y + 7);   printf("GOAL TILE");
    g2048_set_color(8);
    g2048_gotoxy(PANEL_X, BOARD_Y + 12);  printf("Arrow : move");
    g2048_gotoxy(PANEL_X, BOARD_Y + 13);  printf("Esc   : quit");
    g2048_set_color(7);
}

// 칸 하나 그리기: 안쪽 가로 7칸 x 세로 3줄, 숫자는 가운데 줄
static void draw_tile(int r, int c)
{
    int x = BOARD_X + 1 + c * 8;
    int y = BOARD_Y + 1 + r * 4;
    int v = board[r][c];

    g2048_gotoxy(x, y);      printf("       ");   // 윗줄 (공백 7칸)
    g2048_gotoxy(x, y + 2);  printf("       ");   // 아랫줄

    g2048_gotoxy(x, y + 1);                       // 가운데 줄
    if (v == 0) {
        printf("       ");
        return;
    }
    g2048_set_color(tile_color(v));
    if (v < 10)        printf("   %d   ", v);    // 1자리: 앞 3칸, 뒤 3칸
    else if (v < 100)  printf("  %d   ", v);     // 2자리: 앞 2칸, 뒤 3칸
    else if (v < 1000) printf("  %d  ", v);      // 3자리: 앞 2칸, 뒤 2칸
    else               printf(" %d  ", v);       // 4자리: 앞 1칸, 뒤 2칸
    g2048_set_color(7);
}

// 숫자와 오른쪽 정보 값 그리기 (움직일 때마다 다시 그림)
static void draw_board(void)
{
    for (int r = 0; r < N; r++)
        for (int c = 0; c < N; c++)
            draw_tile(r, c);

    int shown_best = (score > best[diff]) ? score : best[diff];   // 기록을 넘으면 바로 보여 주기

    g2048_set_color(15);                                          // 점수: 밝은 흰색
    g2048_gotoxy(PANEL_X, BOARD_Y + 2);  printf("%-10d", score);
    g2048_set_color(14);                                          // 최고 점수: 노랑 (난이도와 상관없이)
    g2048_gotoxy(PANEL_X, BOARD_Y + 5);  printf("%-10d", shown_best);
    g2048_set_color(g2048_diff_color(diff));                      // 목표 타일: 난이도 색
    g2048_gotoxy(PANEL_X, BOARD_Y + 8);  printf("%-10d", g2048_goal(diff));
    g2048_set_color(7);
}

// 보드 아래에 메시지 한 줄 쓰기 (line: 0, 1, 2번째 줄)
static void show_message(int line, int color, const char *text)
{
    g2048_gotoxy(BOARD_X, MSG_Y + line);
    printf("%-55s", "");                 // 이전 글자 지우기
    g2048_gotoxy(BOARD_X, MSG_Y + line);
    g2048_set_color(color);
    printf("%s", text);
    g2048_set_color(7);
}

static void clear_messages(void)
{
    for (int i = 0; i < 3; i++) show_message(i, 7, "");
}

/* ===== 게임 진행 ===== */

// 한 판 진행. 다시 하기면 1, 2048 메인 화면으로 가면 0
static int play_game(void)
{
    char text[64];

    init_board();
    add_tile();
    add_tile();
    draw_frame();

    while (1) {
        draw_board();

        // 승리: 결과를 보여 주고 2048 메인 화면으로
        if (reached_goal()) {
            int new_best = update_best();
            sprintf(text, "YOU WIN!  SCORE: %d", score);
            show_message(0, 14, text);
            if (new_best) show_message(1, 14, "*** NEW BEST SCORE! ***");   // 노란 글자 + 별표
            show_message(2, 7, "Press any key to go back.");
            _getch();
            return 0;
        }

        // 게임 오버: 다시 하기 / 2048 메인 화면
        if (!can_move()) {
            int new_best = update_best();
            sprintf(text, "GAME OVER!  SCORE: %d", score);
            show_message(0, 12, text);
            if (new_best) show_message(1, 14, "*** NEW BEST SCORE! ***");   // 노란 글자 + 별표
            show_message(2, 7, "R: try again    ESC: back to 2048 menu");
            while (1) {
                int key = _getch();
                if (key == 'r' || key == 'R') return 1;
                if (key == 27) return 0;
            }
        }

        int key = _getch();

        // ESC: 정말 나갈지 묻기
        if (key == 27) {
            show_message(0, 12, "Quit this game? (Y/N)");
            while (1) {
                int answer = _getch();
                if (answer == 'y' || answer == 'Y') {
                    update_best();
                    return 0;
                }
                if (answer == 'n' || answer == 'N' || answer == 27) break;   // N 또는 ESC: 계속하기
            }
            clear_messages();
            continue;
        }

        // 방향키는 224(또는 0)가 먼저 들어오고, 다음 값이 실제 방향
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

// main.c에서 부르는 2048 시작 함수
void run_2048(void)
{
    srand((unsigned int)time(NULL));
    g2048_hide_cursor();
    load_best();

    while (g2048_show_menu(&diff, best)) {   // Start Game을 고르면 게임 시작
        while (play_game()) {
            // R을 누르면 바로 새 판
        }
    }

    g2048_set_color(7);
    system("cls");
}
