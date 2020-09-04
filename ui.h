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
    wchar_t *text;
} UIH_CONTROL;

typedef struct UIH_STATE {
    long controlCount;
    HWND hwnd;
    wchar_t *hwndTitle;//[256];
    //wchar_t hwndTitle[100];
    HFONT font[UIH_NUM_FONTS];
    char propName[100]; //
    wchar_t windowClassname[256];
    UIH_CONTROL controls[UIH_NUM_CONTROLS];
    int numberOfControls;
} UIH_STATE;

UIH_STATE UIHState;


//LRESULT CALLBACK windowProcCallback(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
//HWND UIHCreateWindow(char *text, int x, int y, int width, int height);
//UIH_CONTROL UIHAddLabel(char *text, int x, int y, int w, int h);
//UIH_CONTROL UIHAddButton(char *text, int x, int y, int w, int h);

/** call before any other UIH function */
//void UIHInit();
//void UIHClean();
//void UIHErr();
//int UIHMakeControlText(UIH_CONTROL *ccontrol, char *text);


void UIHErr() {
    printf("\n** GetLastError: %i **\n", (int) GetLastError());
}

LRESULT CALLBACK windowProcCallback(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
    int id = (unsigned short) wparam;
    int event = ((unsigned short)(((unsigned long) wparam >> 16) & 0xFFFF));
    switch (umsg) {
        case WM_COMMAND:
            printf("\n** window proc callback:\n id=%i event=%i umsg=%x", id, event, umsg);
            break;
        /*case WM_SIZE:
            break;
        case WM_SIZING:
            break;*/
        case WM_CLOSE:
            printf("close");
            //UIH_STATE UIHState;//.propName;
            //char *propName = UIHState.propName;
            RemovePropW(hwnd, UIHState.propName);
            UIHClean();
            break;
        default:
            //printf("\n** window proc callback:\n id=%i event=%i umsg=%x", id, event, umsg);
            break;
    }
    return DefWindowProcW(hwnd, umsg, wparam, lparam);
}

HWND UIHCreateWindow(char *title, int x, int y, int width, int height) {

    //wchar_t *p = malloc(sizeof(wchar_t) + 200);

    //strcpy(UIHState.hwndTitle, "feck");
    int nsize = MultiByteToWideChar(CP_UTF8, 0, title, -1, NULL, 0);
    UIHState.hwndTitle = malloc(sizeof(wchar_t) + 2);
    //unsigned short poop[256];
    int nnsize = MultiByteToWideChar(CP_UTF8, 0, title, -1, UIHState.hwndTitle, nsize);


    //printf("** nsize = %i, nnsize = %i **", nsize, nnsize);

    itoa(&UIHState.propName, UIHState.windowClassname, 16);
    strcat(UIHState.windowClassname, title);
    strcpy(UIHState.propName, UIHState.windowClassname);
    printf("\n** UIHState.propName: %s **\n** UIHState.windowClassname: %s **\n** UIHState.hwndTitle: %s **\n** title: %s **\n", UIHState.propName, UIHState.windowClassname, (wchar_t*)UIHState.hwndTitle, title);


    int nsize1 = MultiByteToWideChar(CP_UTF8, 0, UIHState.windowClassname, -1, NULL, 0);
    //UIHState.hwndTitle = malloc(sizeof(wchar_t) * nsize);
    int nnsize1 = MultiByteToWideChar(CP_UTF8, 0, UIHState.windowClassname, -1, UIHState.windowClassname, nsize);

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
    window.lpszClassName = UIHState.windowClassname;
    window.hIconSm = NULL;

    RegisterClassExW(&window);

    UIHState.hwnd  = CreateWindowExW(NULL, UIHState.windowClassname, UIHState.hwndTitle, WS_TILEDWINDOW, x, y, width, height, NULL, NULL, hInstance, NULL);
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


int UIHMakeControl(UIH_CONTROL *control, char *text) {

    control->uuid = ++UIHState.controlCount;
    UIHState.numberOfControls++;
    printf("\n** trace 1 **\n");
    control->text = malloc(256);
    printf("\n** trace 2 **\n");

    int nsize = MultiByteToWideChar(CP_UTF8, 0, text, -1, NULL, 0);
    printf("\n** trace 3 **\n");
    int nnsize = MultiByteToWideChar(CP_UTF8, 0, text, -1, control->text, nsize);
    printf("\n** trace 4 **\n");
    return 0;//nnsize;
}

UIH_CONTROL UIHAddLabel(char *text, int x, int y, int w, int h) {
    UIH_CONTROL control;
    //control.uuid = ++UIHState.controlCount;
    UIHMakeControl(&control, text);
    printf("\n** trace 5 **\n");
    //control.text = malloc(sizeof(wchar_t) + 2);
    //int nsize = MultiByteToWideChar(CP_UTF8, 0, text, -1, NULL, 0);
    //int nnsize = MultiByteToWideChar(CP_UTF8, 0, text, -1, control.text, nsize);

    //printf("\n** %i %i **\n", nsize, nnsize);
    printf("\n** control.text = %i **\n", control.uuid);

    control.hwnd = CreateWindowExW(NULL, L"STATIC", control.text, WS_VISIBLE | WS_CHILD, x, y, w, h, UIHState.hwnd, (HMENU) control.uuid, NULL, NULL);
    printf("\n** trace 6 **\n");

    printf("** UIHAddLabel: UIHState.controlCount = %i, control.uuid = %i\n", UIHState.controlCount, control.uuid);
    SendMessageW(control.hwnd,WM_SETFONT,
                 MAKEWPARAM(UIHState.font[0], 0),
                 MAKELPARAM(1, 0));
    return control;
}

UIH_CONTROL UIHAddButton(char *text, int x, int y, int w, int h) {
    UIH_CONTROL control;
    UIHMakeControl(&control, text);
    //control.uuid = ++UIHState.controlCount;

    control.hwnd = CreateWindowExW(NULL, L"BUTTON", control.text, WS_VISIBLE | WS_CHILD, x, y, w, h, UIHState.hwnd, (HMENU) control.uuid, NULL, NULL);
    printf("** UIHAddButton: UIHState.controlCount = %i, control.uuid = %i\n", UIHState.controlCount, control.uuid);
    SendMessageW(control.hwnd,WM_SETFONT,
                 MAKEWPARAM(UIHState.font[0], 0),
                 MAKELPARAM(1, 0));
    return control;
}

UIH_CONTROL UIHAddEdit(char *text, int x, int y, int w, int h) {
    UIH_CONTROL control;
    //control.uuid = ++UIHState.controlCount;
    UIHMakeControl(&control, text);

    control.hwnd = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", control.text, ES_MULTILINE | ES_AUTOVSCROLL | WS_VISIBLE | WS_CHILD, x, y, w, h, UIHState.hwnd, (HMENU) control.uuid, NULL, NULL);
    printf("** UIHAddEdit: UIHState.controlCount = %i, control.uuid = %i\n", UIHState.controlCount, control.uuid);
    SendMessageW(control.hwnd,WM_SETFONT,
                 MAKEWPARAM(UIHState.font[0], 0),
                 MAKELPARAM(1, 0));
    return control;
}

void UIHInit() {
    //UIHState = malloc(sizeof(UIH_STATE));
    UIHState.font[0] = CreateFontW(14, 0, 0, 0, FW_REGULAR, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, TEXT("Microsoft Sans Serif"));
    UIHState.controlCount = 1000;
    INITCOMMONCONTROLSEX c;
    c.dwICC = ICC_TAB_CLASSES;
    c.dwSize = sizeof(INITCOMMONCONTROLSEX);
    InitCommonControlsEx(&c);

    _wsetlocale(LC_ALL, "en_US.utf8");

}

void UIHClean() {
    //free(UIHState.propName);
    int i = 0;
    while(i < UIH_NUM_FONTS) {
        DeleteObject(UIHState.font[i++]);
    }
    i = 0;
    while (i < UIHState.numberOfControls) {
        free(UIHState.controls[i].text);
        i++;
    }


    free(UIHState.hwndTitle);
    //UIHState.
}



#endif // _GUARD
