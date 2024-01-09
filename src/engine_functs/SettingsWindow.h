#define SETTINGSWINDOW_H
#include "..\include.h"

HDC hdc;
bool isMouseOverNewFileBtn = false;
bool isMouseOverOpenFileBtn = false;
bool isMouseOverExitBtn = false;
bool isMouseOverMinimizeBtn = false;
bool isMouseOverSettingsBtn = false;
BOOL isDragging = FALSE;
HCURSOR arrow_cursor = LoadCursorW(0, IDC_ARROW);
HCURSOR hand_cursor = LoadCursorW(0, IDC_HAND);

MenuButton exitbtn = MenuButton(
    40, 30, 600 - 20, -1, 600, 600,
    1, 1, NULL, NULL,
    22, NULL, L"Comic Sans MS",
    RGB(250, 220, 230), RGB(250, 220, 230), RGB(25, 14, 45),
    RGB(15, 15, 8), RGB(10, 10, 40), RGB(255, 209, 220),
    //border, text, background
    NULL, L" X"
);
MenuButton minimizebtn = MenuButton(
    40, 30, 520, -1, 600, 600,
    1, 1, NULL, NULL,
    22, NULL, L"Comic Sans MS",
    RGB(250, 220, 230), RGB(250, 220, 230), RGB(238, 130, 238),
    RGB(15, 15, 8), RGB(10, 10, 40), RGB(255, 209, 220),
    //border, text, background
    NULL, L" -"
);

TCHAR buffer1[256], buffer2[256];

LRESULT CALLBACK menuSettingsProc(HWND menusettingshwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    static HWND hwndTextBox1, hwndTextBox2;

    hdc = GetDC(menusettingshwnd);

    exitbtn.setHDC(hdc);
    minimizebtn.setHDC(hdc);

    switch (msg)
    {
    case WM_CREATE:
    {
        SendMessage(menusettingshwnd, WM_PAINT, 0, 0);

        hwndTextBox1 = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            L"EDIT",
            L"",
            WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
            20, 20, 80, 30,
            menusettingshwnd,
            NULL,
            GetModuleHandle(NULL),
            NULL);

        hwndTextBox2 = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            L"EDIT",
            L"",
            WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
            20, 80, 80, 30,
            menusettingshwnd,
            NULL,
            GetModuleHandle(NULL),
            NULL);
    }
    break;

        case WM_ERASEBKGND:
        {
            RECT rect = { 0, 0, 600, 600 };
            HBRUSH brush = CreateSolidBrush(RGB(8, 7, 13));
            FillRect(hdc, &rect, brush);
            DeleteObject(brush);
            UpdateWindow(menusettingshwnd);
            return TRUE;
        }
        break;

        case WM_COMMAND:
        {
            if (HIWORD(wParam) == EN_UPDATE)
            {
                if ((HWND)lParam == hwndTextBox1)
                {
                    GetWindowText(hwndTextBox1, buffer1, sizeof(buffer1) / sizeof(TCHAR));
                }
                else if ((HWND)lParam == hwndTextBox2)
                {
                    GetWindowText(hwndTextBox2, buffer2, sizeof(buffer2) / sizeof(TCHAR));
                }
            }
        }
        break;

        case WM_PAINT:
        {

            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(menusettingshwnd, &ps);

            exitbtn.drawBox(false);
            minimizebtn.drawBox(false);

            SetTextColor(hdc, RGB(255, 255, 255));

            TextOut(hdc, 50, 200, buffer1, lstrlen(buffer1));
            TextOut(hdc, 50, 300, buffer2, lstrlen(buffer2));

            EndPaint(menusettingshwnd, &ps);

        }
        break;

        case WM_MOUSEMOVE:
        {

            WPARAM a = wParam;
            short x = LOWORD(lParam);
            short y = HIWORD(lParam);

            bool isCurrentlyOverExitBtn = exitbtn.isMouseOverButton(x, y);
            bool isCurrentlyOverMinimizeBtn = minimizebtn.isMouseOverButton(x, y);

            if (isCurrentlyOverExitBtn != isMouseOverExitBtn) {
                exitbtn.drawBox(isCurrentlyOverExitBtn);
                isMouseOverExitBtn = isCurrentlyOverExitBtn;
            }

            if (isCurrentlyOverMinimizeBtn != isMouseOverMinimizeBtn) {
                minimizebtn.drawBox(isCurrentlyOverMinimizeBtn);
                isMouseOverMinimizeBtn = isCurrentlyOverMinimizeBtn;
            }

            if (
                isCurrentlyOverExitBtn ||
                isCurrentlyOverMinimizeBtn
                ) {
                SetCursor(hand_cursor);
            }
            else {
                SetCursor(arrow_cursor);
            }

        }
        break;

        case WM_LBUTTONDOWN:
        {

            short x = LOWORD(lParam);
            short y = HIWORD(lParam);

            if (exitbtn.isMouseOverButton(x, y)) {

                SendMessage(menusettingshwnd, WM_CLOSE, 0, 0);
                break;

            }

            if (minimizebtn.isMouseOverButton(x, y)) {

                SendMessage(menusettingshwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
                break;

            }

        }
        break;

        case WM_CLOSE:
        {

            DestroyWindow(menusettingshwnd);

        }
        break;

    }

    return DefWindowProc(menusettingshwnd, msg, wParam, lParam);
}

void menuOpenSettingsWindow() {

    HINSTANCE hInstance = { 0 };

    WNDCLASSEX windowClass = { 0 };
    windowClass.cbSize = sizeof(windowClass);
    windowClass.style = 0x0020; //CS_OWNDC
    windowClass.lpfnWndProc = menuSettingsProc;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = hInstance;
    windowClass.hIcon = nullptr;
    windowClass.hCursor = nullptr;
    windowClass.hbrBackground = nullptr;
    windowClass.lpszMenuName = nullptr;
    windowClass.hIconSm = nullptr;
    windowClass.lpszClassName = L"MOGE Settings Window";

    RegisterClassEx(&windowClass);

    HWND menusettingshwnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_ACCEPTFILES,
        windowClass.lpszClassName, L"MOGE Settings",
        WS_POPUP,
        500, 100, 600, 600,
        nullptr, nullptr, windowClass.hInstance, nullptr);
    RegisterClassEx(&windowClass);

    ShowWindow(menusettingshwnd, 10);

}