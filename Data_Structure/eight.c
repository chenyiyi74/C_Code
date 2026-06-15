#include <windows.h>
#include <stdio.h>

#define BOARD_SIZE 8
#define MAX_SOLUTIONS 100

int solutions[MAX_SOLUTIONS][BOARD_SIZE];
int solutionCount = 0;
int currentSolution = 0;

int queen[BOARD_SIZE];
int col[BOARD_SIZE];
int diag1[20];
int diag2[20];

HWND hBtnPrev;
HWND hBtnNext;

void SaveSolution()
{
    int i;
    for (i = 0; i < BOARD_SIZE; i++)
    {
        solutions[solutionCount][i] = queen[i];
    }
    solutionCount++;
}

void DFS(int row)
{
    int c;

    if (row == BOARD_SIZE)
    {
        SaveSolution();
        return;
    }

    for (c = 0; c < BOARD_SIZE; c++)
    {
        if (col[c])
            continue;

        if (diag1[row - c + 7])
            continue;

        if (diag2[row + c])
            continue;

        queen[row] = c;

        col[c] = 1;
        diag1[row - c + 7] = 1;
        diag2[row + c] = 1;

        DFS(row + 1);

        col[c] = 0;
        diag1[row - c + 7] = 0;
        diag2[row + c] = 0;
    }
}

void DrawBoard(HDC hdc)
{
    int startX = 20;
    int startY = 50;
    int cell = 60;

    int row, c;

    HBRUSH whiteBrush = CreateSolidBrush(RGB(255,255,255));
    HBRUSH grayBrush = CreateSolidBrush(RGB(200,200,200));
    HBRUSH queenBrush = CreateSolidBrush(RGB(255,0,0));

    for(row = 0; row < 8; row++)
    {
        for(c = 0; c < 8; c++)
        {
            RECT rect;

            rect.left   = startX + c * cell;
            rect.top    = startY + row * cell;
            rect.right  = rect.left + cell;
            rect.bottom = rect.top + cell;

            FillRect(
                hdc,
                &rect,
                ((row + c) % 2) ? grayBrush : whiteBrush
            );

            Rectangle(
                hdc,
                rect.left,
                rect.top,
                rect.right,
                rect.bottom
            );
        }
    }

    SelectObject(hdc, queenBrush);

    for(row = 0; row < 8; row++)
    {
        int qcol = solutions[currentSolution][row];

        int x1 = startX + qcol * cell + 12;
        int y1 = startY + row * cell + 12;
        int x2 = x1 + 36;
        int y2 = y1 + 36;

        Ellipse(hdc, x1, y1, x2, y2);

        TextOutA(
            hdc,
            x1 + 12,
            y1 + 8,
            "Q",
            1
        );
    }

    DeleteObject(whiteBrush);
    DeleteObject(grayBrush);
    DeleteObject(queenBrush);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    char buffer[128];

    switch(msg)
    {
    case WM_CREATE:

        hBtnPrev = CreateWindowA(
            "BUTTON",
            "上一方案",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            550,
            180,
            120,
            40,
            hwnd,
            (HMENU)1,
            NULL,
            NULL
        );

        hBtnNext = CreateWindowA(
            "BUTTON",
            "下一方案",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            550,
            250,
            120,
            40,
            hwnd,
            (HMENU)2,
            NULL,
            NULL
        );

        break;

    case WM_COMMAND:

        switch(LOWORD(wParam))
        {
        case 1:

            currentSolution--;

            if(currentSolution < 0)
                currentSolution = solutionCount - 1;

            InvalidateRect(hwnd, NULL, TRUE);
            break;

        case 2:

            currentSolution++;

            if(currentSolution >= solutionCount)
                currentSolution = 0;

            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }

        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        sprintf(buffer, "方案总数：%d", solutionCount);
        TextOutA(hdc, 20, 15, buffer, lstrlenA(buffer));

        sprintf(
            buffer,
            "当前方案：%d / %d",
            currentSolution + 1,
            solutionCount
        );

        TextOutA(hdc, 20, 550, buffer, lstrlenA(buffer));

        Rectangle(hdc, 15, 45, 505, 535);

        DrawBoard(hdc);

        EndPaint(hwnd, &ps);
    }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd,msg,wParam,lParam);
    }

    return 0;
}

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    
    WNDCLASSA wc;

    MSG msg;

    DFS(0);

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "EightQueensClass";

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowA(
        "EightQueensClass",
        "八皇后问题",
        WS_OVERLAPPEDWINDOW,
        100,
        50,
        750,
        650,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while(GetMessage(&msg,NULL,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}