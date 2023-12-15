#include "minipad.h"
#include "resource.h"

#define SHORTCUT_KEYS

HWND hDlg, hEdit;
#ifdef SHORTCUT_KEYS
    WNDPROC pOrgEdit; int __stdcall EditProc(HWND hwnd, UINT msg, WPARAM wP, LPARAM lP);
#endif
CHAR szFn[MAX_PATH] = "";
OPENFILENAMEA ofn = { 0 };
bool bIgnoreKeys = false;

__inline int WmInit(HWND hDlgl) {
    hDlg = hDlgl;
    hEdit = GetDlgItem(hDlg, IDC_TEXTBOX);
#ifdef SHORTCUT_KEYS
    pOrgEdit = (WNDPROC)SetWindowLongPtr(hEdit, GWLP_WNDPROC, (LONG_PTR)EditProc);
#endif
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hDlg;
    ofn.lpstrFile = szFn;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = "Text (*.txt)\0*.txt\0All (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    return CDefault();
}

__inline int CNew() { *szFn = 0; return SetWindowTextA(hEdit, ""); }

int CLoad() {
    HANDLE hFile = CreateFileA(szFn, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hFile == INVALID_HANDLE_VALUE)
        return FALSE;
    else {
        DWORD dwRead, dwFs = GetFileSize(hFile, NULL);
        LPSTR buf = (LPSTR)GlobalAlloc(GPTR, dwFs + 1);
        if (ReadFile(hFile, buf, dwFs, &dwFs, 0)) {
            buf[dwFs] = 0;
            SetWindowTextA(hEdit, buf);
        }
        GlobalFree(buf);
        CloseHandle(hFile);
    }
    return TRUE;
}

int CSave() {
    if (!*szFn)
        return CSaveas();
    HANDLE hFile = CreateFileA(szFn, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (hFile == INVALID_HANDLE_VALUE)
        return FALSE;
    else {
        DWORD dwWritten;
        DWORD dwLen = GetWindowTextLength(hEdit);
        LPSTR buf = (LPSTR)GlobalAlloc(GPTR, dwLen + 1);
        GetWindowTextA(hEdit, buf, dwLen + 1);
        WriteFile(hFile, buf, dwLen * sizeof(WCHAR), &dwWritten, NULL);
        GlobalFree(buf);
        CloseHandle(hFile);
    }
    return TRUE;
}

int CDefault() {
    LPWSTR arg = GetCommandLineW();
    while (*arg == ' ') arg++; // skip trailing spaces
    WCHAR wEnd = *arg == '\"' ? *arg++ : ' ';
    char* pFn = szFn;
    for (int i = 0; i < MAX_PATH - 1 && *arg && *arg != wEnd; i++) 
        *pFn++ = (char)*arg++;
    *((DWORD*)(pFn - 3)) = 0x00747874; // end with "txt\0"
    return CLoad() || CSave();
}

int COpen() {
    ofn.lpstrTitle = "Open";
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    if (GetOpenFileNameA(&ofn))
        return CLoad();
    return FALSE;
}

int CSaveas() {
    ofn.lpstrTitle = "Save";
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
    if (GetSaveFileNameA(&ofn))
        CSave();
    return TRUE;
}

__inline int CCut() { SendMessage(hEdit, WM_CUT, 0, 0); return TRUE; }
__inline int CCopy() { SendMessage(hEdit, WM_COPY, 0, 0); return TRUE; }
__inline int CPaste() { SendMessage(hEdit, WM_PASTE, 0, 0); return TRUE; }
__inline int CSelectAll() { SendMessage(hEdit, EM_SETSEL, 0, -1); return TRUE; }

__inline int CEnd() { return EndDialog(hDlg, 0);  }

__inline int WmCommand(WPARAM c, LPARAM lP) {
    switch (c) {
    case ID_FILE_DEFAULT: return CDefault();
    case ID_FILE_NEW: return CNew();
    case ID_FILE_OPEN: return COpen();
    case ID_FILE_SAVE: return CSave();
    case ID_FILE_SAVEAS: return CSaveas();
    case IDCANCEL:
    case ID_FILE_EXIT: return CEnd();
    case ID_EDIT_SELECTALL: return CSelectAll();
    case ID_EDIT_CUT: return CCut();
    case ID_EDIT_COPY: return CCopy();
    case ID_EDIT_PASTE: return CPaste();
    }
    return FALSE;
}

__inline int WmSize(__int16 w, __int16 h) { return MoveWindow(hEdit, 2, 2, w - 4, h - 4, TRUE);}

#ifdef SHORTCUT_KEYS
int WmKeydown(int k) {
    if (!bIgnoreKeys && GetKeyState(VK_CONTROL) & 0x800) {
        bIgnoreKeys = true;
        switch (k) { // DNOSW AXCV
        case 'D': return CDefault();
        case 'N': return CNew();
        case 'O': return COpen();
        case 'S': if (!*szFn) return CSaveas();
                else return CSave();
        case 'W': return CEnd();
        case 'A': return CSelectAll();
        case 'X': return CCut();
        case 'C': return CCopy();
        case 'V': return CPaste();
        }
        bIgnoreKeys = false;
    }
    return FALSE;
}
#endif

__inline int _t_WinProc(HWND hDlgl, UINT uM, WPARAM wP, LPARAM lP) {
    switch (uM) {
    case WM_INITDIALOG: return WmInit(hDlgl);
    case WM_SIZE: return WmSize(lP, lP>>16);
    #ifdef SHORTCUT_KEYS
        case WM_KEYDOWN: return WmKeydown(wP);
    #endif
    case WM_COMMAND: return WmCommand(wP, lP); // pass-through; FALSE
    default: return FALSE;
    }
}

#ifdef SHORTCUT_KEYS
    int __stdcall EditProc(HWND hwnd, UINT msg, WPARAM wP, LPARAM lP) { return msg&WM_KEYDOWN & !WmKeydown(wP) | CallWindowProc(pOrgEdit, hwnd, msg, wP, lP); }
#endif

 
#pragma region Plumbing winmain/winproc debug=x64/release=x32 

#ifndef _WIN64 // 32-bit
    __inline __int32 CALLBACK aWinProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp) { return _t_WinProc(hDlg, msg, wp, lp); }
    #ifdef NDEBUG // 32-bit release
        //#pragma comment(linker, "/MERGE:.data=.text") // hangs
        #pragma comment(linker,"/align:0x40")
        #pragma comment(linker, "/MERGE:.rdata=.text") 
        #pragma comment(linker, "/MERGE:.data=.text") 
        #pragma comment(linker, "/ENTRY:aWinMain")
        int APIENTRY aWinMain() { return DialogBoxParamA(0, (char*)103, 0, aWinProc, 0); }
    #else // 32-bit debug
        int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) { return DialogBoxParamA(0, (char*)103, 0, aWinProc, 0); }
    #endif
#else // 64-bit release/debug
    __int64 CALLBACK wWinProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp) { return _t_WinProc(hDlg, msg, wp, lp); }
    __int32 APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) { return (__int32)DialogBoxParamW(0, (wchar_t*)IDD_ABOUTBOX, 0, wWinProc, 0); }
#endif


#pragma endregion