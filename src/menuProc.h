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

//this file contains functions used in the main file

//	window procedure / behavior
LRESULT CALLBACK WndProcMenuWindow(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	HDC hdc = GetDC(hwnd);
	HCURSOR arrow_cursor = LoadCursorW(0, IDC_ARROW);
	HCURSOR hand_cursor = LoadCursorW(0, IDC_HAND);

	MenuButton newfilebtn = MenuButton(200, 50, NULL, NULL, 600, 600,
		1, 1, NULL, NULL,
		16, 0, L"Comic Sans MS",
		RGB(40, 30, 199), RGB(100, 50, 20), RGB(210, 240, 255), hdc, L"meow");

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

			newfilebtn.drawBox();
			for (int i = 0; i < 600; i++) SetPixel(hdc, i, 290, RGB(0, 0, 0));
			for (int i = 0; i < 600; i++) SetPixel(hdc, 290, i, RGB(0, 0, 0));

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

			if (
					x > newfilebtn.getPosX() && 
					y > newfilebtn.getPosY() &&
					x < (newfilebtn.getPosX() + newfilebtn.getWidth()) &&
					y < (newfilebtn.getPosY() + newfilebtn.getHeight())
				)
			{

				SetWindowText(hwnd, L"uwu");
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

			if (
				x > newfilebtn.getPosX() &&
				y > newfilebtn.getPosY() &&
				x < (newfilebtn.getPosX() + newfilebtn.getWidth()) &&
				y < (newfilebtn.getPosY() + newfilebtn.getHeight())
				)
			{

				IFileSaveDialog* pfd;

				// CoCreate the File Open Dialog object.
				HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&pfd));

				if (SUCCEEDED(hr))
				{
					// Set the options on the dialog.
					DWORD dwFlags;

					// Before setting, always get the options first in order 
					// not to override existing options.
					hr = pfd->GetOptions(&dwFlags);
					if (SUCCEEDED(hr))
					{
						// In this case, get shell items only for file system items.
						hr = pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
						if (SUCCEEDED(hr))
						{
							// Set the file types to display only. Notice that, this is a 1-based array.
							COMDLG_FILTERSPEC rgSpec[] =
							{
								{L"All Files", L"*.*"},
								{L"MOCprj Files", L"*.MOC"}
							};
							hr = pfd->SetFileTypes(ARRAYSIZE(rgSpec), rgSpec);
							if (SUCCEEDED(hr))
							{
								// Set the selected file type index to MOCprj for this example.
								hr = pfd->SetFileTypeIndex(2);
								if (SUCCEEDED(hr))
								{
									// Set the default extension to be ".MOC" file.
									hr = pfd->SetDefaultExtension(L"MOC");
									if (SUCCEEDED(hr))
									{
										// Set the default folder to the Desktop.
										IShellItem* psi;
										SHCreateItemFromParsingName(L"C:\\Users\\Matteo\\Desktop", NULL, IID_PPV_ARGS(&psi));
										pfd->SetFolder(psi);
										psi->Release();

										// Show the dialog
										hr = pfd->Show(NULL);
										if (SUCCEEDED(hr))
										{
											// Obtain the result once the user clicks 
											// the 'Save' button.
											// The result is an IShellItem object.
											IShellItem* psiResult;
											hr = pfd->GetResult(&psiResult);
											if (SUCCEEDED(hr))
											{
												// We are just going to print out the 
												// name of the file for sample sake.
												PWSTR pszFilePath = NULL;
												hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

												if (SUCCEEDED(hr))
												{
													// Create the file
													std::wofstream file(pszFilePath);
													file.close();

													char cStrFile[260];
													wcstombs(cStrFile, pszFilePath, wcslen(pszFilePath) + 1);

													DestroyWindow(hwnd);

													std::string program = "C:\\Users\\Matteo\\Desktop\\prjs\\cpp\\MyOwnCompiler\\x64\\Debug\\MyOwnCompiler.exe";
													std::string parameters = cStrFile;

													// Convert to wide strings
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