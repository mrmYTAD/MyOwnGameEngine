#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <gdiplus.h>

//this file contains functions used in the main file

//	window procedure / behavior
LRESULT CALLBACK WndProcMenuWindow(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

		case WM_CREATE:
		{

			SetTimer(hwnd, 1, 20, NULL);

		}
		break;

		case WM_TIMER:
		{

			//

		}
		break;

		case WM_PAINT:
		{

			//

		}
		break;

		case WM_DROPFILES:
		{
			const wchar_t* a = L"default value";

			HDROP hDrop = (HDROP)wParam;
			UINT nNumFilesDropped = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);

			for (UINT i = 0; i < nNumFilesDropped; i++)
			{
				TCHAR szFilePath[MAX_PATH];
				DragQueryFile(hDrop, i, szFilePath, MAX_PATH);

				a = szFilePath;
				char buffer[1024];
				wcstombs(buffer, a, sizeof(buffer));
			}

			std::wcout << a << std::endl;

			DragFinish(hDrop);

		}
		break;

		case WM_MOUSEMOVE:
		{

			WPARAM a = wParam;
			short x = LOWORD(lParam);
			short y = HIWORD(lParam);
			if (a == MK_CONTROL) {

				std::wstring titolo = L" X: " + std::to_wstring(x) + L" Y: " + std::to_wstring(y);
				SetWindowText(hwnd, titolo.c_str());

			}
			else {

				SetWindowText(hwnd, L"bau");
				HCURSOR cursor = LoadCursorW(0, IDC_ARROW);
				SetCursor(cursor);

			}

		}
		break;

		case WM_LBUTTONDOWN:
		{
			short x = LOWORD(lParam);
			short y = HIWORD(lParam);
			std::wstring titolo = L" X: " + std::to_wstring(x) + L" Y: " + std::to_wstring(y);
			SetWindowText(hwnd, titolo.c_str());

			HCURSOR cursor = LoadCursorW(0, IDC_ARROW);
			SetCursor(cursor);

		}
		break;

		case WM_CLOSE:
		{
			PostQuitMessage(69);
		}
		break;

	}

    return DefWindowProc(hwnd, msg, wParam, lParam);

}

LRESULT CALLBACK WndProcEngineWindow(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    switch (msg) {

    case WM_CREATE:
    {

        SetTimer(hwnd, 1, 500, NULL);

    }
    break;

    case WM_TIMER:
    {

        std::cout << "uwu" << std::endl;

    }
    break;

    case WM_CLOSE:
    {

        PostQuitMessage(69);

    }
    break;

    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}