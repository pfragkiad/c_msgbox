#include <windows.h>

HHOOK hHook = NULL;

const TCHAR* OKTEXT =TEXT("YEEAAAHHH!");
const TCHAR* CANCELTEXT = TEXT("NOOOOO!") ;

BOOL CALLBACK MessageBoxEnumProc(HWND hwnd, LPARAM lParam)
{
    TCHAR className[256]; // = {0};
    GetClassName(hwnd, className, 256);
    if (lstrcmp(className, TEXT("Button")) == 0)
    {
        // TCHAR text[256];
        // GetWindowText(hwnd,text,256); //contains OK!

        int ctlId = GetDlgCtrlID(hwnd);
        switch (ctlId)
        {
        case IDOK:
            SetWindowText(hwnd, OKTEXT );
            break;
        case IDCANCEL:
            SetWindowText(hwnd, CANCELTEXT);
            break;
        }
    }
    return TRUE;
}

LRESULT CALLBACK MyMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    CWPSTRUCT *msg = (CWPSTRUCT *)lParam;

    if (msg->message == WM_INITDIALOG)
    {
        HWND hwnd = msg->hwnd;
        TCHAR className[256];
        GetClassName(hwnd, className, 256);

        //https://learn.microsoft.com/en-us/windows/win32/winmsg/about-window-classes
        if (lstrcmp(className, TEXT("#32770")) == 0)
            EnumChildWindows(hwnd,MessageBoxEnumProc, lParam);
    }

    // Call the next hook in the chain
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main()
{
    hHook = SetWindowsHookEx(WH_CALLWNDPROC, MyMsgProc, NULL, GetCurrentThreadId());

    MessageBox(NULL, "The quantum realm has finally come to an end!", "KEFTEDAKIAS", MB_ICONWARNING | MB_OKCANCEL);

    UnhookWindowsHookEx(hHook);

    return 0;
}
