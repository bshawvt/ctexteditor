#ifndef _UIH_GUARD
#define _UIH_GUARD

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include "windows.h"
#include "Commctrl.h"
#include "stdint.h"

#include "util.h"

/// utf tests
#define UIH_HelloWorld_Hanzi "世界你好"
#define UIH_HelloWorld_Kanji "こんにちは、世界"
#define UIH_HelloWorld_English "Hello world"

#define UIH_NUM_FONTS 10
#define UIH_NUM_CONTROLS 100

typedef struct UIH_CONTROL {
    long uuid;
    HWND hwnd;
} UIH_CONTROL;

typedef struct UIH_STATE {
    long controlCount;
    HWND hwnd;
    HFONT font[UIH_NUM_FONTS];
    char propName[100]; //
    char windowClassname[100];
    UIH_CONTROL controls[UIH_NUM_CONTROLS];
    int controlsInUseBecauseImLazy[UIH_NUM_CONTROLS];
} UIH_STATE;

UIH_STATE UIHState;


LRESULT CALLBACK windowProcCallback(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
HWND UIHCreateWindow(int x, int y, int width, int height, char title[]);
//UIH_CONTROL UIHAddLabel(wchar_t *text, int x, int y, int w, int h);
UIH_CONTROL UIHAddButton(wchar_t *text, int x, int y, int w, int h);

/** call before */
void UIHInit();
void UIHClean();
void UIHErr();


void UIHErr() {
    printf("\n** GetLastError: %i **\n", (int) GetLastError());
}

LRESULT CALLBACK windowProcCallback(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
    int id = (unsigned short) wparam;
    int event = ((unsigned short)(((unsigned long) wparam >> 16) & 0xFFFF));
    switch (umsg) {
        case WM_COMMAND:
            break;
        /*case WM_SIZE:
            break;
        case WM_SIZING:
            break;*/
        case WM_CLOSE:
            printf("close");
            //UIH_STATE UIHState;//.propName;
            char *propName = UIHState.propName;
            RemovePropW(hwnd, UIHState.propName);
            UIHClean();
            break;
        default:
            //printf("\n** window proc callback:\n id=%i event=%i umsg=%x", id, event, umsg);
            break;
    }
    return DefWindowProc(hwnd, umsg, wparam, lparam);
}

HWND UIHCreateWindow(int x, int y, int width, int height, char windowTitle[]) {

    char windowClassname[100];
    itoa(&UIHState.propName, &UIHState.windowClassname, 16);
    strcat(UIHState.windowClassname, windowTitle);
    strcpy(&UIHState.propName, &UIHState.windowClassname);
    printf("\n** UIHState.propName: %s **\n** UIHState.windowClassname: %s **\n** windowTitle: %s **\n", UIHState.propName, UIHState.windowClassname, windowTitle);

    HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASSEX window;
    window.cbSize = sizeof(WNDCLASSEX);
    window.style = CS_HREDRAW | CS_VREDRAW;
    window.lpfnWndProc = windowProcCallback;
    window.cbClsExtra = 0;
    window.cbWndExtra = 0;
    window.hInstance = hInstance;
    window.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    window.hCursor = LoadCursor(NULL, IDC_ARROW);
    window.hbrBackground = (HBRUSH) COLOR_WINDOW;
    window.lpszMenuName = NULL;
    window.lpszClassName = &UIHState.windowClassname;
    window.hIconSm = NULL;

    RegisterClassExW(&window);

    UIHState.hwnd  = CreateWindowExW(NULL, &UIHState.windowClassname, windowTitle, WS_TILEDWINDOW, x, y, width, height, NULL, NULL, hInstance, NULL);
    SetPropW(UIHState.hwnd, UIHState.propName, &window);

    return UIHState.hwnd;
}

void UIHShowWindow(int hidden) {
    if (hidden>0)
        ShowWindow(UIHState.hwnd, SW_SHOWDEFAULT);
    else
        ShowWindow(UIHState.hwnd, SW_HIDE);
    UpdateWindow(UIHState.hwnd);
};

UIH_CONTROL UIHAddLabel(char *text, int x, int y, int w, int h) {
    UIH_CONTROL control;
    control.uuid = UIHState.controlCount++;
    int nsize = MultiByteToWideChar(CP_UTF8, 0, text, -1, NULL, 0);

    wchar_t *p = malloc(sizeof(wchar_t) + 2);
    int nnsize = MultiByteToWideChar(CP_UTF8, 0, text, -1, p, nsize);

    printf("\n** %i %i **\n", nsize, nnsize);

    control.hwnd = CreateWindowExW(NULL, L"STATIC", p, WS_VISIBLE | WS_CHILD, x, y, w, h, UIHState.hwnd, (HMENU) control.uuid, NULL, NULL);
    printf("** AddLabel: UIHState.controlCount = %i\n", UIHState.controlCount);
    SendMessageW(control.hwnd,WM_SETFONT,
                 MAKEWPARAM(UIHState.font[0], 0),
                 MAKELPARAM(1, 0));
    return control;
}

UIH_CONTROL UIHAddButton(wchar_t *text, int x, int y, int w, int h) {
    UIH_CONTROL control;
    control.uuid = UIHState.controlCount++;

    control.hwnd = CreateWindowExW(NULL, L"BUTTON", text, WS_VISIBLE | WS_CHILD, x, y, w, h, UIHState.hwnd, (HMENU) control.uuid, NULL, NULL);
    printf("** AddLabel: UIHState.controlCount = %i\n", UIHState.controlCount);
    SendMessageW(control.hwnd,WM_SETFONT,
                 MAKEWPARAM(UIHState.font[0], 0),
                 MAKELPARAM(1, 0));
    return control;
}

UIH_CONTROL UIHAddEdit(char *text, int x, int y, int w, int h) {
    UIH_CONTROL control;
    control.uuid = UIHState.controlCount++;

    control.hwnd = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", text, ES_AUTOHSCROLL | WS_VISIBLE | WS_CHILD, x, y, w, h, UIHState.hwnd, (HMENU) control.uuid, NULL, NULL);
    printf("** AddLabel: UIHState.controlCount = %i\n", UIHState.controlCount);
    SendMessageW(control.hwnd,WM_SETFONT,
                 MAKEWPARAM(UIHState.font[0], 0),
                 MAKELPARAM(1, 0));
    return control;
}

void UIHInit() {
    //UIHState = malloc(sizeof(UIH_STATE));
    UIHState.font[0] = CreateFontW(14, 0, 0, 0, FW_REGULAR, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, TEXT("Microsoft Sans Serif"));
    INITCOMMONCONTROLSEX c;
    c.dwICC = ICC_TAB_CLASSES;
    c.dwSize = sizeof(INITCOMMONCONTROLSEX);
    InitCommonControlsEx(&c);

    _wsetlocale(LC_ALL, "en_US.utf8");

}

void UIHClean() {
    free(UIHState.propName);
    int i = 0;
    while(i < UIH_NUM_FONTS) {
        DeleteObject(UIHState.font[i++]);
    }


    //free(UIHState);
}



#endif // _GUARD
