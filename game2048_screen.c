// game2048_screen.c - 2048 메인 화면과 콘솔 화면 함수
#define _CRT_SECURE_NO_WARNINGS   // Visual Studio가 sprintf를 에러로 막지 않게 (include보다 위에)
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include "game2048.h"

static const int goals[G2048_DIFF_COUNT] = { 512, 1024, 2048 };   // 난이도별 목표 숫자
static const int colors[G2048_DIFF_COUNT] = { 10, 13, 12 };       // 초록, 분홍, 빨강

/* ===== 콘솔 화면 함수 ===== */

void g2048_gotoxy(int x, int y)
{
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void g2048_set_color(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void g2048_hide_cursor(void)
{
    CONSOLE_CURSOR_INFO ci = { 1, FALSE };   // {커서 크기, 보이는지}
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
}

int g2048_goal(int diff)
{
    return goals[diff];
}

int g2048_diff_color(int diff)
{
    return colors[diff];
}

/* ===== 2048 메인 화면 ===== */

// 제목 글자 (노란색)
static void draw_title(void)
{
    g2048_set_color(14);
    g2048_gotoxy(25, 3);  printf(" ____    ___   _  _     ___  ");
    g2048_gotoxy(25, 4);  printf("|___ \\  / _ \\ | || |   ( _ ) ");
    g2048_gotoxy(25, 5);  printf("  __) || | | || || |_  / _ \\ ");
    g2048_gotoxy(25, 6);  printf(" / __/ | |_| ||__   _|| (_) |");
    g2048_gotoxy(25, 7);  printf("|_____| \\___/    |_|   \\___/ ");
    g2048_set_color(7);
}

// 메뉴 한 줄: 선택된 줄은 노란색 + >>
static void draw_menu_item(int y, const char *text, int selected)
{
    g2048_set_color(selected ? 14 : 7);
    g2048_gotoxy(22, y);
    printf(selected ? ">>" : "  ");
    g2048_gotoxy(25, y);
    printf("%s", text);
    g2048_set_color(7);
}

int g2048_show_menu(int *diff, const int best[])
{
    int sel = 0;           // 선택된 메뉴: 0 Start Game, 1 Difficulty, 2 Back to Main Menu
    int choosing = 0;      // 1이면 난이도를 고르는 중
    int preview = *diff;   // 고르는 중에 보여 주는 난이도
    char buf[16];

    system("cls");
    draw_title();

    while (1) {
        int shown = choosing ? preview : *diff;   // 화면에 보여 줄 난이도

        // 난이도별 최고 점수 (난이도 색)
        g2048_set_color(g2048_diff_color(shown));
        g2048_gotoxy(28, 10);
        printf("%4d BEST SCORE : %-8d", g2048_goal(shown), best[shown]);
        g2048_set_color(7);

        // 메뉴 3줄
        draw_menu_item(13, "Start Game", sel == 0);
        draw_menu_item(15, "Difficulty", sel == 1);
        draw_menu_item(17, "Back to Main Menu", sel == 2);

        // Difficulty 줄의 512 1024 2048
        for (int i = 0; i < G2048_DIFF_COUNT; i++) {
            if (i == shown) {
                if (choosing) g2048_set_color(g2048_diff_color(i));   // 고르는 중: 난이도 색
                else g2048_set_color(sel == 1 ? 14 : 15);           // 평소: 노랑 또는 흰색
                sprintf(buf, "[%d]", g2048_goal(i));
            } else {
                g2048_set_color(8);                                    // 나머지: 어두운 회색
                sprintf(buf, " %d ", g2048_goal(i));
            }
            g2048_gotoxy(38 + i * 7, 15);
            printf("%-7s", buf);
        }

        // 맨 아래 안내 한 줄
        g2048_set_color(8);
        g2048_gotoxy(22, 21);
        if (choosing) printf("Left/Right : choose    Enter : OK        ");
        else          printf("Up/Down : select    Enter : confirm      ");
        g2048_set_color(7);

        int key = _getch();

        if (choosing) {
            // 난이도 고르는 중: 좌우만 받고, 위아래와 ESC는 무시
            if (key == 0 || key == 224) {
                key = _getch();
                if (key == 75) preview = (preview + G2048_DIFF_COUNT - 1) % G2048_DIFF_COUNT;   // 왼쪽
                else if (key == 77) preview = (preview + 1) % G2048_DIFF_COUNT;                  // 오른쪽
            }
            else if (key == 13) {      // Enter: 확정, >>는 Difficulty에 그대로
                *diff = preview;
                choosing = 0;
            }
        }
        else {
            if (key == 0 || key == 224) {
                key = _getch();
                if (key == 72) sel = (sel + 2) % 3;        // 위
                else if (key == 80) sel = (sel + 1) % 3;   // 아래
            }
            else if (key == 13) {      // Enter
                if (sel == 0) return 1;                     // 게임 시작
                if (sel == 1) { choosing = 1; preview = *diff; }   // 난이도 고르기 시작
                if (sel == 2) return 0;                     // 메인 메뉴로
            }
            else if (key == 27) {      // ESC: 메인 메뉴로
                return 0;
            }
        }
    }
}
