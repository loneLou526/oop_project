#include <windows.h>
#include <commdlg.h>
#include <string>
#include "encode_decode.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void openFileDialog(HWND hwnd);

HWND hEditInput; // Глобальный дескриптор строки ввода
std::string selectedFilePath;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "Base64 Encoder/Decoder";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
            0, CLASS_NAME, "Base64 Encoder/Decoder", WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 500, 300, NULL, NULL, hInstance, NULL
    );

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            // Кнопки
            CreateWindow("BUTTON", "Choose File", WS_VISIBLE | WS_CHILD, 20, 30, 150, 40, hwnd, (HMENU)1, NULL, NULL);
            CreateWindow("BUTTON", "Encode", WS_VISIBLE | WS_CHILD, 20, 90, 150, 40, hwnd, (HMENU)2, NULL, NULL);
            CreateWindow("BUTTON", "Decode", WS_VISIBLE | WS_CHILD, 20, 150, 150, 40, hwnd, (HMENU)3, NULL, NULL);

            // Строка ввода
            hEditInput = CreateWindow(
                    "EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                    200, 30, 250, 20, hwnd, NULL, NULL, NULL
            );

            // Метка для строки ввода
            CreateWindow("STATIC", "Enter the name of the modified file:", WS_VISIBLE | WS_CHILD, 200, 10, 250, 20, hwnd, NULL, NULL, NULL);
            return 0;
        }

        case WM_SIZE: {
            // Обработка изменения размеров окна
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);

            // Перераспределение элементов при изменении размеров
            MoveWindow(hEditInput, 200, 30, width - 220, 20, TRUE);
            return 0;
        }

        case WM_PAINT: {
            // Перерисовка окна
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Заливаем фон белым цветом
            HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
            FillRect(hdc, &ps.rcPaint, hBrush);
            DeleteObject(hBrush);

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case 1: // "Choose File"
                    openFileDialog(hwnd);
                    break;

                case 2: { // "Encode"
                    char inputBuffer[1024];
                    GetWindowText(hEditInput, inputBuffer, sizeof(inputBuffer)); // Получаем текст из строки ввода
                    std::string inputText = inputBuffer;

                    if (!selectedFilePath.empty()) {
                        std::string content = readFile(selectedFilePath);
                        std::string encoded = encodeBase64(content);
                        writeFile(inputText, encoded);
                        MessageBox(hwnd, "Text encoded successfully!", "Success", MB_OK | MB_ICONINFORMATION);
                    } else {
                        MessageBox(hwnd, "Please enter text to encode.", "Error", MB_OK | MB_ICONERROR);
                    }
                    break;
                }

                case 3: { // "Decode"
                    char inputBuffer[1024];
                    GetWindowText(hEditInput, inputBuffer, sizeof(inputBuffer)); // Получаем текст из строки ввода
                    std::string inputText = inputBuffer;

                    if (!selectedFilePath.empty()) {
                        std::string content = readFile(selectedFilePath);
                        std::string decoded = decodeBase64(content);
                        writeFile(inputText, decoded);
                        MessageBox(hwnd, "Text decoded successfully!", "Success", MB_OK | MB_ICONINFORMATION);
                    } else {
                        MessageBox(hwnd, "Please enter text to decode.", "Error", MB_OK | MB_ICONERROR);
                    }
                    break;
                }
            }
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void openFileDialog(HWND hwnd) {
    OPENFILENAME ofn;
    char fileName[MAX_PATH] = "";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = "Text Files\0*.txt\0All Files\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    ofn.lpstrTitle = "Select a File";

    if (GetOpenFileName(&ofn)) {
        selectedFilePath = fileName;
        MessageBox(hwnd, ("Selected file: " + selectedFilePath).c_str(), "File Selected", MB_OK | MB_ICONINFORMATION);
    }
}
