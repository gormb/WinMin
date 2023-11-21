#include <windows.h>

// release x86
// debug x64

int _t_WinProc(HWND hDlg, UINT msg, WPARAM wP, LPARAM) {
    if (msg == WM_COMMAND)
        if (wP < 3) EndDialog(hDlg, 0);
    return (msg == WM_INITDIALOG);
}

#ifdef NDEBUG
#pragma comment(linker, "/ENTRY:aWinMain")
#pragma comment(linker, "/MERGE:.text=.rdata") 

__int32 CALLBACK aWinProc(HWND hDlg, UINT msg, WPARAM wP, LPARAM) { return _t_WinProc(hDlg, msg, wP, 0); }
int APIENTRY aWinMain() { return DialogBoxParamA(0, (char*)103, 0, aWinProc, 0); }

#else

__int64 CALLBACK wWinProc(HWND hDlg, UINT msg, WPARAM wP, LPARAM) { return _t_WinProc(hDlg, msg, wP, 0); }
__int32 APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int) { return (__int32)DialogBoxParamW(0, (wchar_t*)103, 0, wWinProc, 0); }

#endif
