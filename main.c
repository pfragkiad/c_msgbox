#include <windows.h>
#include "tchar.h" //_trintf

const TCHAR *OK_TEXT = TEXT("YEEAAAHHH!");
const TCHAR *CANCEL_TEXT = TEXT("NOOOOO!");

BOOL areStringsEqual(TCHAR *str1, TCHAR *str2, BOOL caseSensitive)
{
    return caseSensitive ? lstrcmp(str1, str2) == 0 : lstrcmpi(str1, str2) == 0;
}

BOOL isWindowText(HWND hwnd, TCHAR *str)
{
    int textLength = GetWindowTextLength(hwnd);
    TCHAR *text = malloc((textLength + 1) * sizeof(TCHAR));
    GetWindowText(hwnd, text, textLength + 1);
    // int length2 = lstrlen(text); //2
    //_tprintf("'%s'\n", text);
    int result = lstrcmp(text, TEXT("OK")) == 0;
    free(text);
    return result == 0;
}

BOOL isWindowClass(HWND hwnd, TCHAR *str)
{
    TCHAR className[256];
    int length = GetClassName(hwnd, className, 256);
    return lstrcmp(className, str) == 0;
}

BOOL CALLBACK MessageBoxEnumProc(HWND hwnd, LPARAM lParam)
{
    if (isWindowClass(hwnd, TEXT("Button")))
    {
        // if (isWindowText(hwnd, TEXT("OK")))
        //     _tprintf(TEXT("FOUND OK!")); //needs tchar.h

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
    // https://learn.microsoft.com/en-us/windows/win32/winmsg/about-window-classes
    if (msg->message == WM_INITDIALOG && isWindowClass(msg->hwnd, TEXT("#32770")))
        EnumChildWindows(msg->hwnd, MessageBoxEnumProc, 0);
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main()
{
    HHOOK hHook = SetWindowsHookEx(WH_CALLWNDPROC, HookProc, NULL, GetCurrentThreadId());

    MessageBox(NULL,TEXT("The quantum realm has finally come to an end!"), TEXT("KEFTEDAKIAS"), MB_ICONWARNING | MB_OKCANCEL);

    UnhookWindowsHookEx(hHook);

    return 0;
}
