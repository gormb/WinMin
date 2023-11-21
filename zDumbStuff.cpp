//#include <windows.h>
////#include <commctrl.h>//#pragma comment(lib, "comctl32.lib")
////#include "resource.h"
//
//__int64 CALLBACK WinProc(HWND hDlg, UINT msg, WPARAM wP, LPARAM) {
//    __int64 iRet = 1;
//    if (msg == WM_COMMAND && wP < 3)
//        EndDialog(hDlg, 0);
//    else if (msg != WM_INITDIALOG)
//        iRet = 0;
//    return iRet;
//}
//
//#ifdef DEBUG
//__int32 APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int) {   //InitCommonControls(); //__int64 iRet = DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_ABOUTBOX), 0, About);
//    DialogBox(0, (wchar_t*)103, 0, WinProc); //IDD_ABOUTBOX: About dialog //IDD_OPENFILE : Open File dialog //IDD_SAVEFILE : Save File dialog //IDD_PRINTDLG : Print dialog //IDD_PRINTSETUPDLG : Print Setup dialog //IDD_FIND //: Find dialog //IDD_REPLACE : Replace dialog //IDD_FONT : Font dialog //IDD_COLOR : Color dialog
//    return 0;
//}
//#else
//int APIENTRY aWinMain() {
//    DialogBoxA(0, (char*)103, 0, WinProc);
//    return 0;
//}
//#pragma comment(linker, "/ENTRY:aWinMain")
//#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
//#pragma comment(linker, "/MERGE:.rdata=.text") // Merge .rdata into .text
//#endif // DEBUG
//
