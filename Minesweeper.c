
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include "Minesweeper.h"

#define SIZE 8
#define MINE_COUNT 10

int opened[SIZE][SIZE] = { 0 };
int map[SIZE][SIZE] = { 0 };

void InitBoard(void);
LRESULT CALLBACK MinesweeperWndProc(HWND, UINT, WPARAM, LPARAM);

void InitBoard(void)
{
    int i, j;
    int r, c;
    int nr, nc;
    int count;

    // 게임판 초기화
    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            map[i][j] = 0;
            opened[i][j] = 0;
        }
    }

    srand((unsigned int)time(NULL));

    // 지뢰 10개 무작위 배치
    count = 0;

    while (count < MINE_COUNT)
    {
        i = rand() % SIZE;
        j = rand() % SIZE;

        if (map[i][j] != -1)
        {
            map[i][j] = -1;
            count++;
        }
    }

    // 주변 지뢰 개수 계산
    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            if (map[i][j] == -1)
                continue;

            count = 0;

            for (r = -1; r <= 1; r++)
            {
                for (c = -1; c <= 1; c++)
                {
                    nr = i + r;
                    nc = j + c;

                    if (nr >= 0 && nr < SIZE &&
                        nc >= 0 && nc < SIZE)
                    {
                        if (map[nr][nc] == -1)
                            count++;
                    }
                }
            }

            map[i][j] = count;
        }
    }
}

// 게임 화면 처리
LRESULT CALLBACK MinesweeperWndProc(
    HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int i, j;
    
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc;
        HBRUSH brush;
        RECT rect;
        WCHAR text[10];

        hdc = BeginPaint(hWnd, &ps);

        for (i = 0; i < SIZE; i++)
        {
            for (j = 0; j < SIZE; j++)
            {
                rect.left = 50 + j * 40;
                rect.top = 50 + i * 40;
                rect.right = 90 + j * 40;
                rect.bottom = 90 + i * 40;

                if (opened[i][j] == 1)
                    brush = CreateSolidBrush(RGB(180, 180, 180));
                else
                    brush = CreateSolidBrush(RGB(240, 240, 240));

                FillRect(hdc, &rect, brush);
                DeleteObject(brush);

                FrameRect(
                    hdc,
                    &rect,
                    (HBRUSH)GetStockObject(BLACK_BRUSH)
                );

                if (opened[i][j] == 1)
                {
                    if (map[i][j] == -1)
                    {
                        lstrcpyW(text, L"*");
                    }
                    else
                    {
                        wsprintfW(text, L"%d", map[i][j]);
                    }

                    SetBkMode(hdc, TRANSPARENT);
                    SetTextColor(hdc, RGB(0, 0, 200));

                    TextOutW(
                        hdc,
                        62 + j * 40,
                        60 + i * 40,
                        text,
                        lstrlenW(text)
                    );
                }
            }
        }

        EndPaint(hWnd, &ps);
    }
    break;

    // 마우스 왼쪽 버튼 클릭
    case WM_LBUTTONDOWN:
    {
        int x, y;
        int row, col;

        x = LOWORD(lParam);
        y = HIWORD(lParam);

        if (x >= 50 && x < 370 &&
            y >= 50 && y < 370)
        {
            col = (x - 50) / 40;
            row = (y - 50) / 40;

            opened[row][col] = 1;

            InvalidateRect(hWnd, NULL, TRUE);
        }
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProcW(hWnd, message, wParam, lParam);
    }

    return 0;
}

// 지뢰찾기 게임 실행 함수
void Minesweeper(void)
{
    HINSTANCE hInstance;
    WNDCLASSW wc;
    HWND hWnd;
    MSG msg;
    const WCHAR className[] = L"MinesweeperWindow";

    hInstance = GetModuleHandleW(NULL);

    // 게임판 초기화
    InitBoard();

    // 윈도우 클래스 설정
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = MinesweeperWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIconA(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = className;

    // 윈도우 클래스 등록
    if (!RegisterClassW(&wc))
    {
        if (GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
            return;
    }

    // 게임 창 생성
    hWnd = CreateWindowW(
        className,
        L"Minesweeper",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        430,
        430,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hWnd == NULL)
        return;

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    // 메시지 처리
    while (GetMessageW(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}
