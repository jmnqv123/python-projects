#include <windows.h>
#include <string>
#include <vector>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Global handles
HWND hEditInput, hEditChat, hButtonSend;

// Very simple chatbot logic
std::string generate_reply(const std::string& input) {
    if (input.find("hello") != std::string::npos) return "Hi there!";
    if (input.find("how are you") != std::string::npos) return "I'm just a bot, but I'm good!";
    if (input.find("bye") != std::string::npos) return "Goodbye!";
    return "I don't understand that.";
}

void append_text(HWND hEdit, const std::string& text) {
    int len = GetWindowTextLength(hEdit);
    SendMessage(hEdit, EM_SETSEL, (WPARAM)len, (LPARAM)len);
    SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)text.c_str());
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    const char CLASS_NAME[] = "ChatBotWindow";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "C++ Chatbot GUI",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400,
        nullptr, nullptr, hInstance, nullptr);

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);

    // Message loop
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

void handle_send() {
    char buffer[256];
    GetWindowText(hEditInput, buffer, sizeof(buffer));
    std::string input(buffer);
    if (input.empty()) return;

    std::string user_line = "You: " + input + "\r\n";
    std::string bot_line = "Bot: " + generate_reply(input) + "\r\n";

    append_text(hEditChat, user_line);
    append_text(hEditChat, bot_line);

    SetWindowText(hEditInput, "");  // Clear input
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        hEditChat = CreateWindow("EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
            10, 10, 460, 250,
            hwnd, nullptr, nullptr, nullptr);

        hEditInput = CreateWindow("EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            10, 270, 360, 25,
            hwnd, nullptr, nullptr, nullptr);

        hButtonSend = CreateWindow("BUTTON", "Send",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            380, 270, 90, 25,
            hwnd, (HMENU)1, nullptr, nullptr);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {  // Send button
            handle_send();
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
