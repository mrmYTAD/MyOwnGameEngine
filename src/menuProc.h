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

#include <ShObjIdl.h>
#include <shlobj.h>

bool isMouseOverButton(MenuButton& button, short x, short y) {

	return 
		x > (button.getPosX() - button.getWidth() / 2) &&
		y > button.getPosY() &&
		x < (button.getPosX() + button.getWidth() / 2) &&
		y < (button.getPosY() + button.getHeight());

}

bool isMouseOverNewFileBtn = false;
bool isMouseOverOpenFileBtn = false;
bool isMouseOverExitBtn = false;
bool isMouseOverMinimizeBtn = false;
BOOL isDragging = FALSE;
HCURSOR arrow_cursor = LoadCursorW(0, IDC_ARROW);
HCURSOR hand_cursor = LoadCursorW(0, IDC_HAND);

MenuButton newfilebtn = MenuButton(
	200, 50, 140, 60, 600, 600,
	1, 1, NULL, NULL,
	22, NULL, L"Comic Sans MS",
	RGB(255, 245, 245), RGB(255, 240, 240), RGB(15, 15, 8),
	RGB(238, 130, 238), RGB(238, 130, 238), RGB(24, 21, 38),
	//border, text, background
	NULL, L"NEW FILE"
);
MenuButton openfilebtn = MenuButton(
	200, 50, 140, 120, 600, 600,
	1, 1, NULL, NULL,
	22, NULL, L"Comic Sans MS",
	RGB(0, 250, 0), RGB(0, 220, 0), RGB(15, 10, 10),
	RGB(250, 220, 230), RGB(250, 220, 230), RGB(25, 14, 45),
	//border, text, background
	NULL, L"OPEN FILE"
);
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
	40, 30, 600 - 60 - 20, -1, 600, 600,
	1, 1, NULL, NULL,
	22, NULL, L"Comic Sans MS",
	RGB(250, 220, 230), RGB(250, 220, 230), RGB(238, 130, 238),
	RGB(15, 15, 8), RGB(10, 10, 40), RGB(255, 209, 220),
	//border, text, background
	NULL, L" -"
);

//this file contains functions used in the main file

//	window procedure / behavior
LRESULT CALLBACK WndProcMenuWindow(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	HDC hdc = GetDC(hwnd);

	newfilebtn.setHDC(hdc);
	openfilebtn.setHDC(hdc);
	exitbtn.setHDC(hdc);
	minimizebtn.setHDC(hdc);

	switch (msg) {

		case WM_CREATE:
		{

			SendMessage(hwnd, WM_PAINT, 0, 0);

		}
		break;

		case WM_SYSCOMMAND:
		{

			switch (wParam) {

				case SC_RESTORE:
				{

					InvalidateRect(hwnd, NULL, TRUE);
					SendMessage(hwnd, PRF_ERASEBKGND, 0, 0);
					UpdateWindow(hwnd);

				}
				break;
			}

		}
		break;

		case WM_ERASEBKGND:
		{

			RECT rect = { 0, 0, 600, 600 };
			HBRUSH brush = CreateSolidBrush( RGB(8, 7, 13) );
			FillRect(hdc, &rect, brush);
			DeleteObject(brush);
			UpdateWindow(hwnd);
			return TRUE;
		}
		break;

		case WM_MOVE:
		{

			if (!isDragging)
			{
				InvalidateRect(hwnd, NULL, TRUE);
				UpdateWindow(hwnd);
			}

		}
		break;

		case WM_NCHITTEST:
		{

			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			POINT pt;
			pt.x = x;
			pt.y = y;
			ScreenToClient(hwnd, &pt);

			if (pt.y < 30 && pt.x < 30)
			{
				isDragging = TRUE;
				return HTCAPTION;
			}
			else
			{
				return DefWindowProc(hwnd, msg, wParam, lParam);
			}
		}
		break;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hwnd, &ps);
			newfilebtn.drawBox(false);
			openfilebtn.drawBox(false);
			exitbtn.drawBox(false);
			minimizebtn.drawBox(false);
			for (int i = 0; i < 600; i++) SetPixel(hdc, i, 290, RGB(255, 255, 255));
			for (int i = 0; i < 600; i++) SetPixel(hdc, 290, i, RGB(255, 255, 255));
			for (int i = 0; i < 30; i++) {

				for (int j = 0; j < 30; j++) SetPixel(hdc, i, j, RGB(200, 0, 0));

			}

			EndPaint(hwnd, &ps);

		}
		break;

		case WM_MOUSEMOVE:
		{

			WPARAM a = wParam;
			short x = LOWORD(lParam);
			short y = HIWORD(lParam);

			bool isCurrentlyOverNewFileBtn = isMouseOverButton(newfilebtn, x, y);
			bool isCurrentlyOverOpenFileBtn = isMouseOverButton(openfilebtn, x, y);
			bool isCurrentlyOverExitBtn = isMouseOverButton(exitbtn, x, y);
			bool isCurrentlyOverMinimizeBtn = isMouseOverButton(minimizebtn, x, y);

			if (isCurrentlyOverNewFileBtn != isMouseOverNewFileBtn) {
				newfilebtn.drawBox(isCurrentlyOverNewFileBtn);
				isMouseOverNewFileBtn = isCurrentlyOverNewFileBtn;
			}

			if (isCurrentlyOverOpenFileBtn != isMouseOverOpenFileBtn) {
				openfilebtn.drawBox(isCurrentlyOverOpenFileBtn);
				isMouseOverOpenFileBtn = isCurrentlyOverOpenFileBtn;
			}

			if (isCurrentlyOverExitBtn != isMouseOverExitBtn) {
				exitbtn.drawBox(isCurrentlyOverExitBtn);
				isMouseOverExitBtn = isCurrentlyOverExitBtn;
			}

			if (isCurrentlyOverMinimizeBtn != isMouseOverMinimizeBtn) {
				minimizebtn.drawBox(isCurrentlyOverMinimizeBtn);
				isMouseOverMinimizeBtn = isCurrentlyOverMinimizeBtn;
			}

			if (
				isCurrentlyOverNewFileBtn ||
				isCurrentlyOverOpenFileBtn ||
				isCurrentlyOverExitBtn
				) {
				SetCursor(hand_cursor);
			}
			else {
				SetCursor(arrow_cursor);
			}

			if (a == MK_CONTROL) SetWindowText(hwnd, L"tft la mia vita");

		}
		break;

		case WM_LBUTTONDOWN:
		{
			short x = LOWORD(lParam);
			short y = HIWORD(lParam);
			std::wstring titolo = L" X: " + std::to_wstring(x) + L" Y: " + std::to_wstring(y);
			SetWindowText(hwnd, titolo.c_str());
			SetCursor(arrow_cursor);

			if (isMouseOverButton(exitbtn, x, y)) {

				SendMessage(hwnd, WM_CLOSE, 0, 0);
				break;

			}

			if (isMouseOverButton(minimizebtn, x, y)) {

				SendMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
				break;

			}

			if (isMouseOverButton(newfilebtn, x, y))
			{
				TCHAR szDesktopPath[MAX_PATH];
				if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_DESKTOP, NULL, 0, szDesktopPath)))
				{
					// szDesktopPath now contains the path to the Desktop folder
				}

				IFileSaveDialog* pfd;

				HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&pfd));

				if (SUCCEEDED(hr))
				{
					DWORD dwFlags;

					hr = pfd->GetOptions(&dwFlags);
					if (SUCCEEDED(hr))
					{
						hr = pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
						if (SUCCEEDED(hr))
						{
							COMDLG_FILTERSPEC rgSpec[] =
							{
								{L"All Files", L"*.*"},
								{L"MOGEprj Files", L"*.MOGE"}
							};
							hr = pfd->SetFileTypes(ARRAYSIZE(rgSpec), rgSpec);
							if (SUCCEEDED(hr))
							{
								hr = pfd->SetFileTypeIndex(2);
								if (SUCCEEDED(hr))
								{
									hr = pfd->SetDefaultExtension(L"MOGE");
									if (SUCCEEDED(hr))
									{
										IShellItem* psi;
										SHCreateItemFromParsingName(szDesktopPath, NULL, IID_PPV_ARGS(&psi));
										pfd->SetFolder(psi);
										psi->Release();

										hr = pfd->Show(NULL);
										if (SUCCEEDED(hr))
										{
											IShellItem* psiResult;
											hr = pfd->GetResult(&psiResult);
											if (SUCCEEDED(hr))
											{
												PWSTR pszFilePath = NULL;
												hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

												if (SUCCEEDED(hr))
												{
													std::wofstream file(pszFilePath);
													file.close();

													char cStrFile[260];
													wcstombs(cStrFile, pszFilePath, wcslen(pszFilePath) + 1);

													DestroyWindow(hwnd);

													std::string program = "C:\\Users\\Matteo\\Desktop\\prjs\\cpp\\MyOwnGameEngine\\MyOwnGameEngine\\x64\\Debug\\MyOwnGameEngine.exe";
													std::string parameters = cStrFile;

													std::wstring wProgram(program.begin(), program.end());
													std::wstring wParameters(parameters.begin(), parameters.end());

													ShellExecute(NULL, L"open", wProgram.c_str(), wParameters.c_str(), NULL, SW_SHOW);

													ExitProcess(69);

													CoTaskMemFree(pszFilePath);
												}

												psiResult->Release();
											}
										}
									}
								}
							}
						}
					}
					pfd->Release();
				}
				CoUninitialize();

			}
			if (isMouseOverButton(openfilebtn, x, y)) {

				OPENFILENAME ofn;
				WCHAR szFile[260] = { 0 };

				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hwnd;
				ofn.lpstrFile = szFile;
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = L"All\0*.*\0MOGEprj\0*.MOGE\0";
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = L"C:\\Users\\Matteo\\Desktop";
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

				if (GetOpenFileName(&ofn) == TRUE)
				{
					char cStrFile[260];
					wcstombs(cStrFile, szFile, wcslen(szFile) + 1);

					std::string program = "C:\\Users\\Matteo\\Desktop\\prjs\\cpp\\MyOwnGameEngine\\MyOwnGameEngine\\x64\\Debug\\MyOwnGameEngine.exe";
					std::string parameters = cStrFile;

					std::wstring wProgram(program.begin(), program.end());
					std::wstring wParameters(parameters.begin(), parameters.end());

					ShellExecute(NULL, L"open", wProgram.c_str(), wParameters.c_str(), NULL, SW_SHOW);

					ExitProcess(69);
				}

				}

		}
		break;

		case WM_CLOSE:
		{
			PostQuitMessage(69);
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

	}

    return DefWindowProc(hwnd, msg, wParam, lParam);

}

// if remember enabled

/*

OPENFILENAME ofn;
				WCHAR szFile[260] = { 0 };

				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hwnd;
				ofn.lpstrFile = szFile;
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = L"All\0*.*\0MOCprj\0*.MOC\0";
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
				SetCurrentDirectory(L"C:\\Users\\Matteo\\Desktop");
				ofn.lpstrInitialDir = L"C:\\Users\\Matteo\\Desktop";

				// Reset the initial directory each time before calling GetSaveFileName
				SetCurrentDirectory(L"C:\\Users\\Matteo\\Desktop");

				// Call GetSaveFileName

				ofn.lpstrInitialDir = L"C:\\Users\\Matteo\\Desktop";

				if (GetSaveFileName(&ofn) == TRUE)
				{

					ofn.lpstrInitialDir = L"C:\\Users\\Matteo\\Desktop";

					// Check the file extension
					std::wstring ws(szFile);
					std::string str(ws.begin(), ws.end());
					size_t ext_pos = str.find_last_of(".");
					std::string ext = (ext_pos == std::string::npos) ? "" : str.substr(ext_pos);

					// If the file has no extension or the user selected "MOCprj", append ".MOC"
					if (ext.empty() || ofn.nFilterIndex == 2) str += ".MOC";

					// Convert back to wide string
					std::wstring wstr(str.begin(), str.end());
					wcscpy(szFile, wstr.c_str());

					HANDLE hFile = CreateFile(szFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
					if (hFile == INVALID_HANDLE_VALUE)
					{
						// Handle the error
					}
					else
					{
						// Use the file
						CloseHandle(hFile);
					}

					char cStrFile[260];
					wcstombs(cStrFile, szFile, wcslen(szFile) + 1);

					DestroyWindow(hwnd);

					std::string program = "C:\\Users\\Matteo\\Desktop\\prjs\\cpp\\MyOwnCompiler\\x64\\Debug\\MyOwnCompiler.exe";
					std::string parameters = cStrFile;

					// Convert to wide strings
					std::wstring wProgram(program.begin(), program.end());
					std::wstring wParameters(parameters.begin(), parameters.end());

					ShellExecute(NULL, L"open", wProgram.c_str(), wParameters.c_str(), NULL, SW_SHOW);

					ExitProcess(69);


*/