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