#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include "snake.h"
#include "snake_screen.h"

#define SN_W   30              // 게임판 가로 칸 수 (한 칸 = 문자 2개)
#define SN_H   22              // 게임판 세로 칸 수
#define SN_OX  10              // 화면 x 시작 위치
#define SN_OY  4               // 화면 y 시작 위치
#define SN_MAX (SN_W * SN_H)   // 뱀 제일 큰 길이

typedef enum { SN_UP, SN_DOWN, SN_LEFT, SN_RIGHT } SnDir;// 뱀 이동 방향

static int sn_x[SN_MAX], sn_y[SN_MAX];   // 뱀 몸의 좌표 (0번이 머리)
static int sn_len;                        // 뱀 길이
static int sn_food_x, sn_food_y;          // 먹이 위치
static int sn_score;                      // 점수

// 게임판 좌표(cx, cy)에 문자열 출력
static void sn_put(int cx, int cy, const char *s) {
    snake_gotoxy(SN_OX + cx * 2, SN_OY + cy);
    printf("%s", s);
}
// 점수 표시
static void sn_draw_score(void) {
    snake_set_color(14);
    snake_gotoxy(SN_OX, 1);
    printf("SCORE: %d   ", sn_score);
}
// 벽 그리기
static void sn_draw_wall(void) {
    snake_set_color(8);
    for (int x = -1; x <= SN_W; x++) {
        sn_put(x, -1, "##");
        sn_put(x, SN_H, "##");
    }//좌우 칸 그리기
    for (int y = 0; y < SN_H; y++) {
        sn_put(-1, y, "##");
        sn_put(SN_W, y, "##");
    }//상하 칸 그리기
}

// (x, y)가 뱀 몸 위인지 검사
static int sn_on_snake(int x, int y) {
    for (int i = 0; i < sn_len; i++)
        if (sn_x[i] == x && sn_y[i] == y) return 1;
    return 0;
}

// 뱀과 겹치지 않는 곳에 먹이 생성, 빈 칸이 없으면 0 반환
static int sn_spawn_food(void) {
    if (sn_len >= SN_MAX) return 0;// 뱀이 판을 꽉 채움
    do {
        sn_food_x = rand() % SN_W;
        sn_food_y = rand() % SN_H;
    } while (sn_on_snake(sn_food_x, sn_food_y));   // 뱀 몸과 겹치면 다시 생성
    snake_set_color(12);
    sn_put(sn_food_x, sn_food_y, "()");
    return 1;
}

// 한 판 진행. 1 반환: 다시 시작, 0 반환: 메인 메뉴로
static int sn_play(Difficulty diff) {
    static const int delay[3] = { 150, 100, 60 };   // EASY, NORMAL, HARD 속도(ms)
    static const int dx[4] = { 0, 0, -1, 1 };       // UP, DOWN, LEFT, RIGHT
    static const int dy[4] = { -1, 1, 0, 0 };
    SnDir dir = SN_RIGHT, next = SN_RIGHT;// 현재 이동 방향, 다음 이동 방향
    int win = 0;// 승리 여부, 1이면 승리, 0이면 패배

    // 초기화: 길이 3, 화면 가운데에서 오른쪽으로 출발
    sn_len = 3;
    sn_score = 0;
    for (int i = 0; i < 3; i++) {
        sn_x[i] = SN_W / 2 - i;// 머리 좌표는 sn_x[0], sn_y[0]
        sn_y[i] = SN_H / 2;
    }

    snake_clear_screen();
    sn_draw_wall();// 벽 그리기
    snake_set_color(10);
    sn_put(sn_x[0], sn_y[0], "OO");// 머리
    snake_set_color(2);
    sn_put(sn_x[1], sn_y[1], "oo");// 몸
    sn_put(sn_x[2], sn_y[2], "oo");
    sn_spawn_food();// 먹이 생성
    sn_draw_score();
    snake_set_color(8);
    snake_gotoxy(SN_OX, SN_OY + SN_H + 2);// 게임 조작 안내
    printf("ARROW/WASD: move   ESC: quit");

    while (1) {
        // 1) 키 입력 (기다리지 않고, 눌린 키만 처리)
        while (_kbhit()) {
            int key = _getch();
            if (key == 224 || key == 0) {            // 방향키는 두 번에 걸쳐 들어옴
                key = _getch();
                if (key == 72) next = SN_UP;
                else if (key == 80) next = SN_DOWN;
                else if (key == 75) next = SN_LEFT;
                else if (key == 77) next = SN_RIGHT;
            }
            else if (key == 'w' || key == 'W') next = SN_UP;
            else if (key == 's' || key == 'S') next = SN_DOWN;
            else if (key == 'a' || key == 'A') next = SN_LEFT;
            else if (key == 'd' || key == 'D') next = SN_RIGHT;
            else if (key == 27) return 0;            // ESC
        }
        if (((int)dir ^ 1) == (int)next) next = dir; // 정반대 방향(U↔D, L↔R)은 무시
        dir = next;

        // 2) 머리가 갈 다음 칸 계산
        int nx = sn_x[0] + dx[dir];// 다음 머리 좌표
        int ny = sn_y[0] + dy[dir];
        int eat = (nx == sn_food_x && ny == sn_food_y);// 먹이를 먹으면 1, 아니면 0

        // 3) 충돌 검사: 벽, 자기 몸 (먹지 않으면 꼬리는 비켜 주므로 제외)
        if (nx < 0 || nx >= SN_W || ny < 0 || ny >= SN_H) break;// 벽 충돌
        int limit = eat ? sn_len : sn_len - 1;// 먹으면 꼬리까지 검사, 아니면 꼬리는 비켜 주므로 제외
        int hit = 0;// 자기 몸 충돌 0은 충돌 안함
        for (int i = 0; i < limit; i++)
            if (sn_x[i] == nx && sn_y[i] == ny) { hit = 1; break; }
        if (hit) break;

        // 4) 뱀 이동: 몸을 한 칸씩 뒤로 밀고 머리 자리에 새 좌표
        int tail_x = sn_x[sn_len - 1];// 꼬리 좌표 저장
        int tail_y = sn_y[sn_len - 1];
        if (eat) sn_len++;// 먹으면 길이 증가
        for (int i = sn_len - 1; i > 0; i--) {
            sn_x[i] = sn_x[i - 1];
            sn_y[i] = sn_y[i - 1];
        }// 몸을 한 칸씩 뒤로 밀기
        sn_x[0] = nx;// 머리 좌표 갱신
        sn_y[0] = ny;

        // 5) 화면 갱신: 꼬리 지우기, 이전 머리는 몸으로, 새 머리 그리기
        if (!eat) sn_put(tail_x, tail_y, "  ");// 꼬리 지우기
        snake_set_color(2);
        sn_put(sn_x[1], sn_y[1], "oo");// 이전 머리 그리기
        snake_set_color(10);
        sn_put(nx, ny, "OO");// 새 머리 그리기

        // 6) 먹이를 먹었으면 점수, 새 먹이
        if (eat) {
            sn_score += 10;
            sn_draw_score();
            if (!sn_spawn_food()) { win = 1; break; }   // 판이 가득 참 = 승리
        }

        Sleep(delay[diff]);// 난이도에 따라 잠시 대기
    }

    // 게임 종료 화면
    int cy = SN_OY + SN_H / 2;// 화면 중앙 y 좌표
    snake_set_color(win ? 14 : 12);// 승리: 노란색, 패배: 빨간색
    snake_gotoxy(35, cy);
    printf("%s", win ? " YOU WIN! " : " GAME OVER ");
    snake_set_color(7);// 원래 색으로
    snake_gotoxy(35, cy + 1);
    printf(" SCORE: %d ", sn_score);
    snake_gotoxy(28, cy + 3);

    printf("ENTER: retry   ESC: menu");

    while (_kbhit()) _getch();     // 게임 중에 눌러 둔 키 버리기
    Sleep(300);
    while (_kbhit()) _getch();   // 게임 종료 후 바로 키를 누르면 메뉴로 안 넘어가고 다시 시작되는 문제 방지

    while (1) {
        int key = _getch();
        if (key == 13) return 1;// ENTER: 다시 시작
        if (key == 27) return 0;// ESC: 메인 메뉴
    }
}

void snake(void)
{
    Difficulty diff = SNAKE_NORMAL;// 시작 할 때에는 NORMAL
    snake_screen_console();
    srand((unsigned)time(NULL));// 난수 초기화

    if (snake_show_main_menu(&diff) != SNAKE_START) return;
    while (sn_play(diff)) { }      // 1이 반환되는 동안 다시 시작
}