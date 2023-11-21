#include <windows.h>
// release x86
// debug x64

#define _i __int64
#ifdef NDEBUG
#define _i __int32
#pragma comment(linker, "/ENTRY:aWinMain")
#pragma comment(linker, "/MERGE:.text=.rdata") 
#endif

_i CALLBACK WinProc(HWND hDlg, UINT msg, WPARAM wP, LPARAM) {
    if (msg == WM_COMMAND)
        if (wP < 3) EndDialog(hDlg, 0);
    return (msg == WM_INITDIALOG);
}

#ifdef NDEBUG
int APIENTRY aWinMain() {
    return DialogBoxParamA(0, (char*)103, 0, WinProc, 0);
}
#else
__int32 APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int) {
    DialogBox(0, (wchar_t*)103, 0, WinProc);
    return 0;
}

#endif // NDEBUG
