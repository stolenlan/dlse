#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

// Replace this with your actual implementation
int set_property(const char *savefile_path, const char *value_string) {
    int value = atoi(value_string);
    if (value < 0 || value > 9999) return 1;
    // TODO: Add binary patch logic here
    MessageBoxA(NULL, "Property set successfully!", "Success", MB_OK);
    return 0;
}

char savefile_path[MAX_PATH] = "";

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hFileBtn, hValueEdit, hPatchBtn, hLabel;

    switch (msg) {
        case WM_CREATE:
            hFileBtn = CreateWindow("BUTTON", "Select Save File", 
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 
                10, 10, 150, 30, hwnd, (HMENU)1, NULL, NULL);

            hValueEdit = CreateWindow("EDIT", "", 
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 
                10, 50, 150, 25, hwnd, (HMENU)2, NULL, NULL);

            hPatchBtn = CreateWindow("BUTTON", "Apply Patch", 
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 
                10, 90, 150, 30, hwnd, (HMENU)3, NULL, NULL);

            hLabel = CreateWindow("STATIC", "HOLY FUCKITY",
                WS_VISIBLE | WS_CHILD, 10, 180, 100,25, hwnd, NULL,NULL, NULL);
            break;

        case WM_GETMINMAXINFO:
            MINMAXINFO *mmi = (MINMAXINFO *)lParam;
            mmi->ptMinTrackSize.x = 400;
            mmi->ptMinTrackSize.y = 400;
            mmi->ptMaxSize.x = 400;
            mmi->ptMaxSize.y = 400;
            return 0;
            break;
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
                    if (GetOpenFileName(&ofn)) {
                        MessageBox(hwnd, savefile_path, "Selected", MB_OK);
                        printf("The filename: %s\n", ofn.lpstrFile);
                    }
                    break;
                }

                case 3: { // Apply patch
                    if (savefile_path[0] == '\0') {
                        MessageBox(hwnd, "No file selected!", "Error", MB_ICONERROR);
                        break;
                    }

                    char value[64];
                    GetWindowText(hValueEdit, value, sizeof(value));

                    if (set_property(savefile_path, value) != 0) {
                        MessageBox(hwnd, "Failed to patch file.", "Error", MB_ICONERROR);
                    }
                    break;
                }
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_VSCROLL:
            printf("Yeah we scrolled man\n");
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {

    const char g_szClassName[] = "SaveEditWindowClass";

    WNDCLASSEX wc = {
        .cbSize = sizeof(WNDCLASSEX),
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = WndProc,
        .hInstance = hInstance,
        .hCursor = LoadCursor(NULL, IDC_ARROW),
        .hbrBackground = (HBRUSH)(COLOR_WINDOW+1),
        .lpszClassName = g_szClassName
    };

    RegisterClassEx(&wc);

    HWND hwnd = CreateWindowEx(
        0, g_szClassName, "Savegame Editor",
        WS_OVERLAPPEDWINDOW | WS_VSCROLL,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 600,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) return 1;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG Msg;
    while (GetMessage(&Msg, NULL, 0, 0) > 0) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    return Msg.wParam;
}


// #include <windows.h>

// int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
// 	MessageBox(NULL,"Savegame editor executed successfully!", "Hello Windows", MB_OK);
// 	return 0;
// }
