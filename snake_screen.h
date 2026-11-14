#ifndef SNAKE_SCREEN_H
#define SNAKE_SCREEN_H

typedef enum {SNAKE_EASY,SNAKE_NORMAL,SNAKE_HARD} Difficulty;  // 난이도 데이터 이름 설정
typedef enum {SNAKE_START,SNAKE_EXIT } MenuResult;       // 메인 화면 선택 결과

void snake_screen_console(void);          // 게임 시작 전 콘솔 창 초기화
void snake_gotoxy(int x, int y);       // 커서 위치 이동
void snake_set_color(int color);       // 글자 색상 설정
void snake_clear_screen(void);         // 화면 지우기

MenuResult snake_show_main_menu(Difficulty *diff);  // 메인 화면 출력, 선택 결과와 난이도 반환

#endif