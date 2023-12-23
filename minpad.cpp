#include "minpad.h"
#include "resource.h"

HWND hDlg, hEdit;
#ifdef MINPAD_SHORTCUT_KEYS
    WNDPROC pOrgEdit; int __stdcall EditProc(HWND hwnd, UINT msg, WPARAM wP, LPARAM lP);
#endif
CHAR szFn[MAX_PATH] = "";
#if defined(MINPAD_SHORTCUT_KEYS) || defined(MINPAD_MENU)
    OPENFILENAMEA ofn = { 0 };
#endif
bool bIgnoreKeys = false;
LPSTR buf = NULL;

__inline int WmInit(HWND hDlgl) {
    hDlg = hDlgl;
    hEdit = GetDlgItem(hDlg, IDC_TEXTBOX);
#if defined(MINPAD_SHORTCUT_KEYS) || defined(MINPAD_MENU)
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hDlgl;
        ofn.lpstrFile = szFn;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrFilter = "Text (*.txt)\0*.txt\0All (*.*)\0*.*\0";
        ofn.nFilterIndex = 1;
    #endif
    #ifdef MINPAD_SHORTCUT_KEYS
        pOrgEdit = (WNDPROC)SetWindowLongPtr(hEdit, GWLP_WNDPROC, (LONG_PTR)EditProc); // Subclass edit control to preview keyboard
    #endif
    return CDefault();
}

__inline void SetFn() {
    SetWindowTextA(hDlg, szFn);
}

__inline int CNew() { *szFn = 0; SetFn(); return SetWindowTextA(hEdit, ""); }

int CLoad() {
    HANDLE hFile = CreateFileA(szFn, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hFile == INVALID_HANDLE_VALUE)
        return FALSE;
    else {
        DWORD dwFs = GetFileSize(hFile, NULL);
        buf = (buf == NULL)
            ? (LPSTR)GlobalAlloc(0, dwFs + 1)
            : (LPSTR)GlobalReAlloc(buf, dwFs + 1, GMEM_MOVEABLE);
        if (buf && ReadFile(hFile, buf, dwFs, &dwFs, 0)) {
            buf[dwFs] = 0;
            SetWindowTextA(hEdit, buf);
        }
        //GlobalFree(buf);
        CloseHandle(hFile);
    }
    SetFn();
    return TRUE;
}

int CSave(byte bAskName = 0) {
    #if defined(MINPAD_SHORTCUT_KEYS) || defined(MINPAD_MENU)
        if (!*szFn || bAskName) {
            ofn.lpstrTitle = "Sav";
            ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
            if (!GetSaveFileNameA(&ofn))
                return FALSE;
        }
    #endif
    HANDLE hFile = CreateFileA(szFn, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (hFile == INVALID_HANDLE_VALUE)
        return FALSE;
    else {
        DWORD dwWritten;
        DWORD dwLen = GetWindowTextLength(hEdit);
        buf = (LPSTR)GlobalReAlloc(buf, dwLen + 1, 0);
        GetWindowTextA(hEdit, buf, dwLen + 1);
        WriteFile(hFile, buf, dwLen * sizeof(WCHAR), &dwWritten, NULL);
        //GlobalFree(buf);
        CloseHandle(hFile);
        SetFn();
    }
    return TRUE;
}

int CDefault() {
    LPWSTR arg = GetCommandLineW();
    while (*arg == ' ') arg++; // skip trailing spaces
    WCHAR wEnd = *arg == '\"' ? *arg++ : ' ';
    char* pFn = szFn;
    for (int i = 0; i < MAX_PATH && *arg && *arg != wEnd; i++) 
        *pFn++ = (char)*arg++;
    *((DWORD*)(pFn - 3)) = 0x00747874; // end with "txt\0"
    return CLoad() || CSave();
}

int COpen() {
    #if defined(MINPAD_SHORTCUT_KEYS) || defined(MINPAD_MENU)
        ofn.lpstrTitle = "Open";
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
        if (GetOpenFileNameA(&ofn))
            return CLoad();
    #endif
    return FALSE;
}

__inline int CSelectAll() { SendMessage(hEdit, EM_SETSEL, 0, -1); return TRUE; }

int CCutCopyPasteNewUndo(int i) { SendMessage(hEdit, i- 0x9946, 0, -1); return TRUE; }

__inline int CEnd() { return EndDialog(hDlg, 0);  }

__inline int WmCommand(WPARAM c, LPARAM lP) {
    if (c >= ID_FILE_NEW && c <= ID_EDIT_SELECTALL) {
        if (c < ID_FILE_OPEN) return CNew();
        else if (c < ID_FILE_SAVEAS) return COpen();
        else if (c < ID_FILE_EXIT) return CSave(c & 1);
        else if (c < ID_EDIT_CUT) return CEnd();
        else if (c <= ID_EDIT_UNDO) return CCutCopyPasteNewUndo(c);
        else if (c < ID_EDIT_SELECTALL) return CDefault();
        else return CSelectAll();
    }
    return FALSE;
}

__inline int WmSize(__int16 w, __int16 h) { return MoveWindow(hEdit, 2, 2, w - 4, h - 4, TRUE);}

#ifdef MINPAD_SHORTCUT_KEYS
int WmKeydown(int k) {
    int iRet = FALSE;
    if (!bIgnoreKeys && GetKeyState(VK_CONTROL) & 0x800) {
        bIgnoreKeys = true;
        switch (k) { // DNOSW AXCV
        case 'D': iRet = CDefault(); break;
        case 'N': iRet = CNew(); break;
        case 'O': iRet = COpen(); break;
        case 'S': iRet = CSave((*szFn) ? 0 : 1);  break;
        case 'W': iRet = CEnd(); break;
        case 'A': iRet = CSelectAll(); break;
        case 'X': iRet = CCutCopyPasteNewUndo(WM_CUT); break;
        case 'C': iRet = CCutCopyPasteNewUndo(WM_COPY); break;
        case 'V': iRet = CCutCopyPasteNewUndo(WM_PASTE); break;
        case 'Z': iRet = CCutCopyPasteNewUndo(WM_UNDO); break;
        }
        bIgnoreKeys = false;
    }
    return iRet;
}
#endif

__inline int _t_WinProc(HWND hDlgl, UINT uM, WPARAM wP, LPARAM lP) {
    switch (uM) {
    case WM_INITDIALOG: return WmInit(hDlgl);
    case WM_SIZE: return WmSize(lP, lP>>16);
    #ifdef MINPAD_SHORTCUT_KEYS
        case WM_KEYDOWN: return WmKeydown(wP);
    #endif
    #ifdef MINPAD_MENU
        case WM_COMMAND: return WmCommand(wP, lP);
    #else
        case WM_COMMAND: if (wP == IDCANCEL) return CEnd();
    #endif
    default: return FALSE;
    }
}

#ifdef MINPAD_SHORTCUT_KEYS
    int __stdcall EditProc(HWND hwnd, UINT msg, WPARAM wP, LPARAM lP) { return msg&WM_KEYDOWN & !WmKeydown(wP) | CallWindowProc(pOrgEdit, hwnd, msg, wP, lP); }
#endif

 
#pragma region Plumbing winmain/winproc debug=x64/release=x32 

#ifndef _WIN64 // 32-bit
    __inline __int32 CALLBACK aWinProc(HWND hDlgl, UINT msg, WPARAM wp, LPARAM lp) { return _t_WinProc(hDlgl, msg, wp, lp); }
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