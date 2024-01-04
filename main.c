#include <windows.h>
        #include "tchar.h"

const TCHAR *OK_TEXT = TEXT("YEEAAAHHH!");
const TCHAR *CANCEL_TEXT = TEXT("NOOOOO!");

BOOL CALLBACK MessageBoxEnumProc(HWND hwnd, LPARAM lParam)
{
    TCHAR className[256];
    GetClassName(hwnd, className, 256);
    if (lstrcmp(className, TEXT("Button")) == 0)
    {
        //TCHAR text[256];
        int textLength = GetWindowTextLength(hwnd);
        TCHAR* text = malloc((textLength+1) * sizeof(TCHAR));
        GetWindowText(hwnd,text,textLength+1);
        _tprintf("'%s'\n",text);
        if(lstrcmp(text,TEXT("OK"))==0) _tprintf(TEXT("FOUND OK!"));
        free(text);


        int ctlId = GetDlgCtrlID(hwnd);
        switch (ctlId)
        {
        case IDOK:
            SetWindowText(hwnd, OK_TEXT);
            break;
        case IDCANCEL:
            SetWindowText(hwnd, CANCEL_TEXT);
            break;
        }
    }
    return TRUE;
}

LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    CWPSTRUCT *msg = (CWPSTRUCT *)lParam;
    if (msg->message == WM_INITDIALOG)
    {
        HWND hwnd = msg->hwnd;
        TCHAR className[256];
        GetClassName(hwnd, className, 256);

        // https://learn.microsoft.com/en-us/windows/win32/winmsg/about-window-classes
        if (lstrcmp(className, TEXT("#32770")) == 0)
            EnumChildWindows(hwnd, MessageBoxEnumProc, 0);
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main()
{
    HHOOK hHook = SetWindowsHookEx(WH_CALLWNDPROC, HookProc, NULL, GetCurrentThreadId());

    MessageBox(NULL, "The quantum realm has finally come to an end!", "KEFTEDAKIAS", MB_ICONWARNING | MB_OKCANCEL);

    UnhookWindowsHookEx(hHook);

    return 0;
}
