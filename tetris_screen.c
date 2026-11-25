// screen.c
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "tetris_screen.h"

void screen_console(void) {
    system("chcp 65001>nul");//UTF-8로 설정(한글 오류 피함)
    CONSOLE_CURSOR_INFO ci = { 1, FALSE };   // 커서 숨기기
    //CONSOLE_CURSOR_INFO:커서의 속성을 정의하는 struct,{커서의 크기(1~100),보일수 있는가(TURE/FALSE)}
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
    //GetStdHandle(STD_OUTPUT_HANDLE):지금 이 콘솔 창
    //SetConsoleCursorInfo:콘솔 커서 정보 설정
    system("mode con cols=80 lines=30");   // 콘솔 창 크기 설정
}

void gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };//x,y를 상자 안에 보관하기
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void set_color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    //콘솔의 색갈 설정해 줌
}

void clear_screen(void) { system("cls"); }

MenuResult show_main_menu(Difficulty* diff) {
    const char* diff_name[3] = { "EASY", "NORMAL", "HARD" };
    int sel = 0;   // 현재 선택된 메뉴 번호,처음은 0(첫번쨰)

    clear_screen();//들어로면 화면을 한번 지우기
    while (1) {
        // 제목 출력
        set_color(11);//파란색으로 설정
        gotoxy(33, 8);//이곳으로 이동
        printf("T E T R I S");

        // 메뉴 항목 출력 (선택된 항목은 노란색)(총 3개 매뉴가 있음:시작==0,난이도==1,종료==2)
        set_color(sel == 0 ? 14 : 7);
        //i == sel->선택한 색갈은 노랑색(14)
        //선택이 아니면 하얀색(7);
        gotoxy(32, 13);//이곳으로 이동
        printf(sel == 0 ? ">>" : "  ");
        //선택하고 있는 행 앞에는 >>가 있음,다른것은 모두 "  "로
        gotoxy(35, 13);
        printf("게임 시작");

        // 현재 난이도 표시
        set_color(sel == 1 ? 14 : 7);
        gotoxy(20, 15);
        printf(sel == 1 ? ">>" : "  ");
        gotoxy(22, 15);
        printf("난이도 선택 ");
        for (int i = 0; i < 3; i++) {
            set_color(i == *diff ? 14 : 8);
            printf(i == *diff ? "[%s]" : "  %s  ", diff_name[i]);
        }

        //게임 종료
        set_color(sel == 2 ? 14 : 7);
        gotoxy(32, 17);
        printf(sel == 2 ? ">>" : "  ");
        gotoxy(35, 17);
        printf("게임 종료");

        int key = _getch();//키를 입력하는것을 기다림
        if (key == 224) {                       // 방향키는 먼저 224가 들어옴
            key = _getch();
            if (key == 72) sel = (sel + 2) % 3;                      // 위쪽 키
            else if (key == 80) sel = (sel + 1) % 3;                 // 아래쪽 키
            else if (key == 75 && sel == 1) *diff = (*diff + 2) % 3; // 왼쪽 키: 난이도 낮추기
            else if (key == 77 && sel == 1) *diff = (*diff + 1) % 3; // 오른쪽 키: 난이도 높이기
        }
        else if (key == 13) {                 // 엔터 키
            if (sel == 0) {
                return START;
            }//개임 시작
            if (sel == 2) {
                return EXIT;
            }//개임 종료
            //if (sel == 1)은 난이도 선택이라서 RETURN안 해도 가능
        }
    }
}

//=========
// game screen
//========

// blocks color
int block_color(int value) {
    int colors[] = {
        7, // empty
        11, // I-cyan
        14, // O- yellow
        13, // T- purple
        10, // S- green
        12, // Z- red
        9, // J- blue
        6 // L- orange
    } ;
    if (value < 1 || value > 7)
        return 7;

    return colors[value];
}
// difficult name
const char* difficulty_name(Difficulty diff) {
    switch (diff) {
        case EASY: return "EASY";
        case NORMAL: return "NORMAL";
        case HARD: return "HARD";
        default: return "NORMAL";
    }
}

// game board
void draw_cell(int x, int y, int value) {
    gotoxy(x, y);

    if (value == 0) {
        set_color(8);
        printf("  ");
    }
    else {
        set_color(block_color(value));
        printf("[]");
    }

    set_color(7);
}

// draw game screen
void draw_game_screen(
    const int board[20][10],
    const int current[4][4],
    int cur_x, int cur_y,
    const int next[4][4],
    int score,
    int level,
    int high_score,
    Difficulty diff
) {
    int left = 2;
    int top = 2;

    // tieu de
    gotoxy(30,0);
    set_color(11);
    printf("T E T R I S");
    set_color(7);

    // ve vien tren
    gotoxy(left, top);
    printf("+--------------------+");

    // 10x20 board
    for (int y = 0; y < 20; y++) {
        gotoxy(left, top + y +1);
        printf("|");

        for (int x = 0; x < 10; x++) {
            int value = board[y][x];

            // hien thi khoi dang roi
            int bx = x - cur_x;
            int by = y - cur_y;

            if (by >= 0 && bx < 4 &&
                by >= 0 && bx < 4) {
                    if (current[by][bx] != 0)
                        value = current[by][bx];
                }
            draw_cell(left + 1 + x * 2,
                      top + y + 1,
                      value);
        }
        gotoxy(left + 21, top + y + 1);
        printf("|");
}
//vien duoi
gotoxy(left, top + 21);
printf("+--------------------+");

// information panel

int sx = 29;

// next block
gotoxy(sx, 2);
set_color(14);
printf("NEXT BLOCK");
set_color(7);

for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
        int value = next[y][x];

        draw_cell(sx + x * 2,
                  4+y,
                  value);
    }
}


// score 
gotoxy(sx, 10);
set_color(11);
printf("SCORE");
set_color(7);

gotoxy(sx, 11);
printf("%-10d", score);

// level
gotoxy(sx, 13);
set_color(11);
printf("LEVEL");
set_color(7);

gotoxy(sx, 14);
printf("%-10d", level);

// high score
gotoxy(sx, 16);
set_color(14);
printf("HIGH SCORE");
set_color(7);

gotoxy(sx, 17);
printf("%-10d", high_score);

// difficulty
gotoxy(sx, 19);
set_color(11);
printf("DIFFICULTY");
set_color(7);

gotoxy(sx, 14);
printf("%-10d", difficulty_name(diff));

// control guide
gotoxy(sx, 22);
set_color(14);
printf("CONTROLS");
set_color(7);

gotoxy(sx, 23);
printf("Left : Move Left");

gotoxy(sx, 24);
printf("Right : Move Right");

gotoxy(sx, 25);
printf("Z     : Rolate Left");

gotoxy(sx, 26);
printf("X     : Rolate Right");

set_color(7);
}

// game over screen
int show_game_over(int score, int high_score, int new_record) {
    int sel = 0;
    
    clear_screen();

    while (1) {
        set_color(12);
        gotoxy(30, 5);
        printf("===== GAME OVER =====");
        set_color(7);

        gotoxy(30, 8);
        printf("FINAL SCORE : %d", score);

        gotoxy(30, 10);
        printf("HIGH SCORE : %d", high_score);

        if (new_record) {
            set_color(14);
            gotoxy(30, 12);
            printf("NEW HIGH SCORE!");
            set_color(7);
        }
        
        // restart
        gotoxy(30, 16);
        printf("%s Restart Game",
                sel == 0 ? ">>" : " ");
        
        // main menu
        gotoxy(30, 18);
        printf("%s Main Menu",
                sel == 1 ? ">>" : " ");

        int key =_getch();

        if (key == 224) {
            key = _getch();

            if (key == 72)
                sel = (sel + 1) % 2;

            else if (key == 80)
                sel = (sel + 1) % 2;
        }
        else if (key == 13) {
            return (sel == 0) ? 1 : 0;
        }
    }
}