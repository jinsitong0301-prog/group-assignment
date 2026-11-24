    // Minesweeper.cpp : 애플리케이션에 대한 진입점을 정의합니다.
    //

    #include "framework.h"
    #include "Minesweeper.h"
    #include <stdlib.h>
    #include <time.h>

    #define MAX_LOADSTRING 100

    // 전역 변수:
    int opened[8][8] = { 0 };
    int map[8][8] = { 0 };
    HINSTANCE hInst;                                // 현재 인스턴스입니다.
    WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
    WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

    // 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
    ATOM                MyRegisterClass(HINSTANCE hInstance);
    BOOL                InitInstance(HINSTANCE, int);
    LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
    INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
    void InitBoard();

    int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                         _In_opt_ HINSTANCE hPrevInstance,
                         _In_ LPWSTR    lpCmdLine,
                         _In_ int       nCmdShow)
    {
        UNREFERENCED_PARAMETER(hPrevInstance);
        UNREFERENCED_PARAMETER(lpCmdLine);

        // TODO: 여기에 코드를 입력합니다.

        // 전역 문자열을 초기화합니다.
        LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
        LoadStringW(hInstance, IDC_MINESWEEPER, szWindowClass, MAX_LOADSTRING);
        MyRegisterClass(hInstance);

        // 애플리케이션 초기화를 수행합니다:
        if (!InitInstance (hInstance, nCmdShow))
        {
            return FALSE;
        }

        HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MINESWEEPER));

        MSG msg;

        // 기본 메시지 루프입니다:
        while (GetMessage(&msg, nullptr, 0, 0))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        return (int) msg.wParam;
    }



    //
    //  함수: MyRegisterClass()
    //
    //  용도: 창 클래스를 등록합니다.
    //
    ATOM MyRegisterClass(HINSTANCE hInstance)
    {
        WNDCLASSEXW wcex;

        wcex.cbSize = sizeof(WNDCLASSEX);

        wcex.style          = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc    = WndProc;
        wcex.cbClsExtra     = 0;
        wcex.cbWndExtra     = 0;
        wcex.hInstance      = hInstance;
        wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MINESWEEPER));
        wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
        wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MINESWEEPER);
        wcex.lpszClassName  = szWindowClass;
        wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

        return RegisterClassExW(&wcex);
    }

    //
    //   함수: InitInstance(HINSTANCE, int)
    //
    //   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
    //
    //   주석:
    //
    //        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
    //        주 프로그램 창을 만든 다음 표시합니다.
    //
    BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
    {
       hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
       InitBoard();
       HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
          CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

       if (!hWnd)
       {
          return FALSE;
       }

       ShowWindow(hWnd, nCmdShow);
       UpdateWindow(hWnd);

       return TRUE;
    }

    void InitBoard()
    {
        int i, j;
        int count = 0;

        srand((unsigned int)time(NULL));

        // 지뢰 10개 배치
        while (count < 10)
        {
            i = rand() % 8;
            j = rand() % 8;

            if (map[i][j] != -1)
            {
                map[i][j] = -1;
                count++;
            }
        }

        // 주변 지뢰 개수 계산
        for (i = 0; i < 8; i++)
        {
            for (j = 0; j < 8; j++)
            {
                if (map[i][j] == -1)
                    continue;

                count = 0;

                for (int r = -1; r <= 1; r++)
                {
                    for (int c = -1; c <= 1; c++)
                    {
                        int nr = i + r;
                        int nc = j + c;

                        if (nr >= 0 && nr < 8 &&
                            nc >= 0 && nc < 8)
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
    //
    //  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
    //
    //  용도: 주 창의 메시지를 처리합니다.
    //
    //  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
    //  WM_PAINT    - 주 창을 그립니다.
    //  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
    //
    //
    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_COMMAND:
            {
                int wmId = LOWORD(wParam);
                // 메뉴 선택을 구문 분석합니다:
                switch (wmId)
                {
                case IDM_ABOUT:
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                    break;
                case IDM_EXIT:
                    DestroyWindow(hWnd);
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
                }
            }
            break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc;
            HBRUSH brush;
            RECT rect;
            int i, j;

            hdc = BeginPaint(hWnd, &ps);

            for (i = 0; i < 8; i++)
            {
                for (j = 0; j < 8; j++)
                {
                    rect.left = 50 + j * 40;
                    rect.top = 50 + i * 40;
                    rect.right = 90 + j * 40;
                    rect.bottom = 90 + i * 40;

                    if (opened[i][j] == 1)
                    {
                        brush = CreateSolidBrush(RGB(180, 180, 180));
                    }
                    else
                    {
                        brush = CreateSolidBrush(RGB(240, 240, 240));
                    }

                    FillRect(hdc, &rect, brush);

                    DeleteObject(brush);

                    FrameRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
                    if (opened[i][j] == 1)
                    {
                        WCHAR text[10];

                        if (map[i][j] == -1)
                        {
                            wcscpy_s(text, L"*");
                        }
                        else
                        {
                            wsprintfW(text, L"%d", map[i][j]);
                        }

                        SetBkMode(hdc, TRANSPARENT);
                        SetTextColor(hdc, RGB(0, 0, 200));

                        TextOutW(hdc,
                            62 + j * 40,
                            60 + i * 40,
                            text,
                            lstrlenW(text));
                    }
                }
            }

            EndPaint(hWnd, &ps);
        }
        break;
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
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }

    // 정보 대화 상자의 메시지 처리기입니다.
    INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
    {
        UNREFERENCED_PARAMETER(lParam);
        switch (message)
        {
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
            {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
        }
        return (INT_PTR)FALSE;
    }
