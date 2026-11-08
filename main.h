// 현재는 게임 초기 화면(메인 화면)을 표시하는 용도로만 사용됨.
// 추후 게임 진행 기능이 추가되면 그때 이 코드를 수정할 예정.
#include <stdio.h>
#include "screen.h"

int main(void) {
    Difficulty diff = NORMAL;//시작 할 때에는 NORMAL
    screen_console();//게임 시작 전 콘솔 창 초기화

    MenuResult r = show_main_menu(&diff);
    //show_main_menu(&diff):프로그램에 여기서 정지,버튼을 선택해야 계속
    //r은 screen.c에서 반환,(START,EXIT)반환가능
    clear_screen();// 화면 지우기
    if (r == START) printf("게임 시작! 난이도: %d\n", diff);
    else printf("게임 종료\n");
    return 0;
}