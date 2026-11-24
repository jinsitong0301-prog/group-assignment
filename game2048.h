// game2048.h - 2048 게임 헤더
#ifndef GAME2048_H
#define GAME2048_H

/* ===== main.c에서 부르는 함수 ===== */

// 2048 게임 시작.
// 2048 메인 화면 -> 게임 -> 2048 메인 화면 순서로 돌다가
// "Back to Main Menu"를 고르면 종합 프로그램의 메인 메뉴로 돌아간다.
void run_2048(void);

/* ===== 2048 안에서만 쓰는 화면 함수 (game2048_screen.c) ===== */
// 다른 게임의 함수와 이름이 겹치지 않도록 모두 g2048_로 시작한다.

#define G2048_DIFF_COUNT 3   // 난이도 개수 (512, 1024, 2048)

void g2048_gotoxy(int x, int y);     // 커서를 (x, y)로 옮기기
void g2048_set_color(int color);     // 글자 색 바꾸기
void g2048_hide_cursor(void);        // 깜빡이는 커서 숨기기

int g2048_goal(int diff);            // 난이도 번호(0~2) -> 목표 숫자(512, 1024, 2048)
int g2048_diff_color(int diff);      // 난이도 번호(0~2) -> 난이도 색

// 2048 메인 화면. *diff에 고른 난이도가 저장된다.
// Start Game이면 1, Back to Main Menu면 0을 돌려준다.
int g2048_show_menu(int *diff, const int best[]);

#endif
