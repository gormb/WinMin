#pragma once

#ifndef RESOURCE_H
#define RESOURCE_H


#include <Windows.h>
#include <winnt.rh>

#define IDD_ABOUTBOX                    103
#define IDC_STATIC                      -1

#define IDC_TEXTBOX                     1001
#define IDC_OK_BUTTON                   1002
#define IDC_CANCEL_BUTTON               1003

#define ID_FILE_NEW                     40001
#define ID_FILE_OPEN                    40002
#define ID_FILE_SAVEAS                  40003
#define ID_FILE_SAVE                    40004
#define ID_FILE_EXIT                    40005
#define ID_EDIT_CUT                     40006 // 0x0300 + 0x9946
#define ID_EDIT_COPY                    40007 // 0x0301 + 0x9946
#define ID_EDIT_PASTE                   40008 // 0x0302 + 0x9946
#define ID_EDIT_CLEAR                   40009 // 0x0303 + 0x9946
#define ID_EDIT_UNDO                    40010 // 0x0300 + 0x9946
#define ID_FILE_DEFAULT                 40011
#define ID_EDIT_SELECTALL				40012

// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        109
#define _APS_NEXT_COMMAND_VALUE         40099
#define _APS_NEXT_CONTROL_VALUE         1010
#define _APS_NEXT_SYMED_VALUE           107
#endif
#endif

#endif // RESOURCE_H
