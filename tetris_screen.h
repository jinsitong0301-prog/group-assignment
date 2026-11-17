// screen.h
#ifndef SCREEN_H
#define SCREEN_H

typedef enum {EASY,NORMAL,HARD} Difficulty;  // 난이도 데이터 이름 설정
typedef enum {START,EXIT } MenuResult;       // 메인 화면 선택 결과

void screen_console(void);          // 게임 시작 전 콘솔 창 초기화
void gotoxy(int x, int y);       // 커서 위치 이동
void set_color(int color);       // 글자 색상 설정
void clear_screen(void);         // 화면 지우기

MenuResult show_main_menu(Difficulty *diff);  // 메인 화면 출력, 선택 결과와 난이도 반환

void draw_game_screen(
    const int board[20][10],
    const int current[4][4],
    int cur_x, int cur_y,
    const int next[4][4],
    int score, 
    int level,
    int high_score,
    Difficulty diff
);

// game over screen
int show_game_over(int score, int high_score, int new_record);
#endif