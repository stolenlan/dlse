/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2025 Tyler Anderson <stolenlan@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

// #define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mmsystem.h>
#include <commctrl.h>
#include "dlse.h"
#include "dlse_gui.h"

// Hmm, this is strange, but required.  Just in case I forget to update this
// add a check in WinMain to ensure that the size is as intended
#define WIN_OFFSET_COUNT 78

char savefile_path[MAX_PATH];

unsigned int uiLineHeight = 32;
unsigned int uiPadding = 10;
unsigned int uiColWidth = 220;
int iVscrollPos = 0;


static void drawLabels(HWND hwnd, HDC hdc, TEXTMETRIC tm) {
	int n_offsets = offset_count();
	char buff[128];

	if(strlen(savefile_path) == 0) {
		return;
	}

	for(int i = 0; i < n_offsets; i++) {
		memset(buff, '\0', 128);
		struct st_offset inst = offset_v15[i];
		char *prop = get_property(savefile_path, inst.key);
		TextOut(hdc, uiPadding, (uiPadding+(uiLineHeight-(tm.tmHeight/2))*i)-iVscrollPos*uiLineHeight, inst.label, strlen(inst.label));
		TextOut(hdc, uiPadding+256, (uiPadding+(uiLineHeight-(tm.tmHeight/2))*i)-iVscrollPos*uiLineHeight, prop, strlen(prop));
		free(prop);
	}
}



LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
PSTR szCmdLine, int iCmdShow) {

	// Common Controls
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_WIN95_CLASSES; //ICC_STANDARD_CLASSES | 
	InitCommonControlsEx(&icex);

	memset(savefile_path, '\0', MAX_PATH);

	static TCHAR szAppName[] = TEXT ("DLSE");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon (GetModuleHandle(NULL), MAKEINTRESOURCE(IDMAIN_ICON));
	wndclass.hCursor = LoadCursor (NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	if (!RegisterClass (&wndclass)) {
		MessageBox (NULL, TEXT ("This program requires Windows NT!"),
			szAppName, MB_ICONERROR);
		return 0;
	}
	hwnd = CreateWindow (szAppName, // window class name
		TEXT ("Dungeon Lords Save Editor"), // window caption
		WS_OVERLAPPEDWINDOW | WS_VSCROLL,
		// window style
		CW_USEDEFAULT,
		// initial x position
		CW_USEDEFAULT,
		// initial y position
		CW_USEDEFAULT,
		// initial x size
		CW_USEDEFAULT,
		// initial y size
		NULL,
		// parent window handle
		NULL,
		// window menu handle
		hInstance,
		// program instance handle
		NULL);

		// Catch the error if the offset 
		if(offset_count() != WIN_OFFSET_COUNT) {
			printf("ERROR: The offset_v15 array length differs from WIN_OFFSET_COUNT\n");
			MessageBox(hwnd, "The WIN_OFFSET_COUNT does not match the actual count.", "ERROR", MB_OK);
			return 1;
		}

		// creation parameters
		ShowWindow (hwnd, iCmdShow);
		UpdateWindow (hwnd);
		while (GetMessage (&msg, NULL, 0, 0)) {
			TranslateMessage (&msg);
			DispatchMessage (&msg);
		}
	
		return msg.wParam; 
	}


LRESULT CALLBACK WndProc (HWND hwnd, UINT message, 
	WPARAM wParam, LPARAM lParam) {
	
	HDC hdc;
	PAINTSTRUCT ps;
	SCROLLINFO si;
	RECT rect;
	static TEXTMETRIC tm;
	static HWND hFileBtn, hValueEdit;
	static int cxClient,cyClient;
	static long int hfbw, hfbh, hfbx, hfby;
	hfbw = 140;
	hfbh = 24;
	static HWND hProps[WIN_OFFSET_COUNT];

	switch (message) {
		case WM_CREATE:
			GetClientRect(hwnd, &rect);
			cxClient = rect.right;
			cyClient = rect.bottom;
			// SetWindowPos(hFileBtn, HWND_BOTTOM, cxClient-hfbw-10, cyClient-hfbh*2-20, hfbw, hfbh, 0);
			// SetWindowPos(hValueEdit, HWND_BOTTOM, cxClient-hfbw-10, cyClient-hfbh-10, hfbw, hfbh, 0);
			printf("The size: %i\n", rect.bottom);
			// printf("Sizeof devcaps: %i\n", sizeof(devcaps)/(sizeof(int)+sizeof(TCHAR)*2));
			hFileBtn = CreateWindow("BUTTON", "Select Save File", 
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 
				cxClient-hfbw-10, cyClient-hfbh*2-20, hfbw, hfbh, hwnd, (HMENU)1, NULL, NULL);

			hValueEdit = CreateWindow("EDIT", "", 
				WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 
				cxClient-hfbw-10, cyClient-hfbh-10, hfbw, hfbh, hwnd, (HMENU)2, NULL, NULL);

			// for(int i = 0; i < WIN_OFFSET_COUNT; i++) {
			// 	hProps[i] = CreateWindow("EDIT", "", 
			// 	WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 
			// 	100, 10+20*i, 150, 20, hwnd, (HMENU)2, NULL, NULL);
			// }

			// PlaySound (MAKEINTRESOURCE(IDWAVE_DROP), NULL, SND_RESOURCE | SND_ASYNC);
			
			return 0;
		case WM_PAINT:
			hdc = BeginPaint (hwnd, &ps);
			GetTextMetrics(hdc, &tm);
			drawLabels(hwnd, hdc, tm);
			EndPaint (hwnd, &ps);

			return 0;
		
		case WM_VSCROLL:
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_ALL;
			si.nMin = 0;
			GetScrollInfo(hwnd, SB_VERT, &si);
			iVscrollPos = si.nPos;

			switch (LOWORD(wParam)) {
			case SB_THUMBTRACK:
				si.nPos = si.nTrackPos;
				break;
			// case SB_THUMBPOSITION:
			// 	si.nPos = si.nTrackPos;
			// 	break;
			case SB_TOP:
				si.nPos = si.nMin;
				break;
			case SB_BOTTOM:
				si.nPos = si.nMax;
				break;
			case SB_LINEUP:
				si.nPos -= 1;
				break;
			case SB_LINEDOWN:
				si.nPos += 1;
				break;
			case SB_PAGEUP:
				si.nPos -= cyClient/uiLineHeight;
				break;
			case SB_PAGEDOWN:
				si.nPos += cyClient/uiLineHeight;
				break;
			default:
				break;
			}

			if(si.nPos < 0) si.nPos = 0;
			if(si.nPos > WIN_OFFSET_COUNT)
				si.nPos = WIN_OFFSET_COUNT;

			si.fMask = SIF_POS;
			SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
			GetScrollInfo(hwnd, SB_VERT, &si);
			if(si.nPos != iVscrollPos) {
				ScrollWindow(hwnd, 0, uiLineHeight * (iVscrollPos - si.nPos), NULL, NULL);
				iVscrollPos = si.nPos;
				// InvalidateRect(hwnd, NULL, TRUE);
				UpdateWindow(hwnd);
			}
			

			return 0;
		
		case WM_SIZE:
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);

			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_RANGE | SIF_PAGE;
			si.nMin = 0;
			si.nMax = WIN_OFFSET_COUNT;
			si.nPage = cyClient/uiLineHeight;
			si.nTrackPos = 0;
			SetScrollInfo(hwnd, SB_VERT, &si, true);
			SetWindowPos(hFileBtn, HWND_BOTTOM, cxClient-hfbw-10, cyClient-hfbh*2-20-iVscrollPos*uiLineHeight, hfbw, hfbh, 0);
			SetWindowPos(hValueEdit, HWND_BOTTOM, cxClient-hfbw-10, cyClient-hfbh-10-iVscrollPos*uiLineHeight, hfbw, hfbh, 0);
			UpdateWindow(hwnd);
			// InvalidateRect(hwnd, NULL, TRUE);
			return 0;

		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case 1: { // File dialog
					OPENFILENAME ofn = {0};
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hwnd;
                    ofn.lpstrFilter = "Save Files\0*.sav\0All Files\0*.*\0";
                    ofn.lpstrFile = savefile_path;
                    ofn.nMaxFile = MAX_PATH;
                    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
                    if(GetOpenFileName(&ofn)) {
	                    int save_version = get_save_major_vers(savefile_path);
						if (save_version != 150) {
							printf("ERROR: Unsupported save/game version.\n");
							MessageBox(hwnd, "Unsupported save/game version", "ERROR", MB_OK);
							memset(savefile_path, '\0', MAX_PATH);
						}                	
                    }
                    InvalidateRect(hwnd, NULL, TRUE);
					PlaySound (MAKEINTRESOURCE(IDWAVE_BLES), NULL, SND_RESOURCE | SND_ASYNC);
					break;
				}
			}
			break;
		case WM_DESTROY:
			PostQuitMessage (0);
			// PlaySound (TEXT ("drop.wav"), NULL, SND_FILENAME);
			// PlaySound (MAKEINTRESOURCE(IDWAVE_DROP), NULL, SND_RESOURCE);
			return 0;
	}
	
	return DefWindowProc (hwnd, message, wParam, lParam);
}
