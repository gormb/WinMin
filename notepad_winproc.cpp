#include "notepad_winproc.h"

int notepad_WinProc(HWND hDlg, UINT msg, WPARAM wP, LPARAM lP) {
    int iRet = (msg == WM_INITDIALOG);
    if (msg == WM_COMMAND) {
        if (wP < 3) EndDialog(hDlg, 0); // OK/Cancel
        //    else if (wP == IDC_EDIT1)
        //    {
        //        OutputDebugString(TEXT("273"));
        //    }
        //    else
        //    {// 67109864
        //        OutputDebugString(TEXT("?"));
        //    }
    }
    return iRet;
}
