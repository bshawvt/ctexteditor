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
    void *fnCallback;
} UIH_CONTROL;

/*typedef struct UIH_CALLBACK {
    void *data1;
    void *data2;
    void *data3;
    void *data4;
} UIH_CALLBACK;*/

typedef struct UIH_STATE {
    long nextUUID;
    int numberOfControls;
    wchar_t *hwndTitle;
    wchar_t *windowClassname;
    HWND hwnd;
    HFONT fonts[UIH_NUM_FONTS];
    //wchar_t *hwndTitle;
    //wchar_t *windowClassname;
    UIH_CONTROL controls[UIH_NUM_CONTROLS];


} UIH_STATE;

/*typedef struct UIH_STATELIST {
    UIH_STATE *addr;
    UIH_STATE *nextAddr;
    UIH_STATE *prevAddr;
    int hasNext;
    int hasPrev;
} UIH_STATELIST;
*/
//UIH_STATELIST UIHStateList;


UIH_STATE *UIHMakeState() {
    UIH_STATE *state = (UIH_STATE*) malloc(sizeof(UIH_STATE));
    if (state == NULL) {
        printf("\n\n****fek****\n\n");
    }
    *state = (UIH_STATE) {0};
    return state;
}


//LRESULT CALLBACK windowProcCallback(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
//HWND UIHCreateWindow(char *text, int x, int y, int width, int height);
//UIH_CONTROL UIHAddLabel(char *text, int x, int y, int w, int h);
//UIH_CONTROL UIHAddButton(char *text, int x, int y, int w, int h);

/** call before any other UIH function */
//void UIHInit();
//void UIHClean();
//void UIHErr();
//int UIHMakeControlText(UIH_CONTROL *ccontrol, char *text);

int UIHErrorCallCount = 1;
#define UIHErr() (printf("\n*****\n - GetLastError#%i = %i\n - in: %s\n - %s @ %i\n*****\n", UIHErrorCallCount++, (int) GetLastError(), __FUNCTION__, __FILE__, __LINE__));

LRESULT CALLBACK windowProcCallback(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {

    int id = (unsigned short) wparam;
    int event = ((unsigned short)(((unsigned long) wparam >> 16) & 0xFFFF));
    switch (umsg) {
        case WM_COMMAND: {
            wchar_t propName[32];
            sprintf(propName, "%i", hwnd);
            HANDLE handle = GetPropW(hwnd, "feck");
            UIH_STATE *state = (UIH_STATE *) handle;
            printf("state->hwnd=%i\n", state->hwnd);
            //printf("\n** window proc callback:\n id=%i event=%i umsg=%x state.hwnd=%i hwnd=%i\n", id, event, umsg, state->hwnd, hwnd);
            for(int i = 0; i < state->numberOfControls; i++) {
                UIH_CONTROL control = state->controls[i];
                printf("id=%i, control.uuid=%i, state->numberOfControls=%i, control.hwnd=%i\n", id, control.uuid, state->numberOfControls, control.hwnd);
                if (id == control.uuid) {
                    //printf("found the thing");

                    void (*UIHFNCallback)(UIH_CALLBACK) = control.fnCallback;
                    (*UIHFNCallback)();
                    break;
                }
            }
            break;
        }
        /*case WM_SIZE:
            break;
        case WM_SIZING:
            break;*/
        case WM_CLOSE:
            printf("close");
            //RemovePropW(hwnd, UIHState.propName);
            //UIHClean();
            break;
        default:
            //printf("\n** window proc callback:\n id=%i event=%i umsg=%x", id, event, umsg);

            //return DefWindowProcW(hwnd, umsg, wparam, lparam);
            break;
    }
    //free(state);
    return DefWindowProcW(hwnd, umsg, wparam, lparam);
}

void UIHCreateWindow(UIH_STATE *state, char *title, int x, int y, int width, int height) {

    int classnameSize = MultiByteToWideChar(CP_UTF8, 0, title, -1, NULL, 0);
    int titleSize = MultiByteToWideChar(CP_UTF8, 0, title, -1, NULL, 0);

    state->windowClassname = malloc(classnameSize * sizeof(wchar_t));
    state->hwndTitle = malloc(titleSize * sizeof(wchar_t));

    char tmpClassname[classnameSize];
    sprintf(tmpClassname, "%p%s", state, title);
    MultiByteToWideChar(CP_UTF8, 0, tmpClassname, -1, state->windowClassname, classnameSize);
    MultiByteToWideChar(CP_UTF8, 0, title, -1, state->hwndTitle, titleSize);

    //printf("state->windowClassname = %s\nstate->hwndTitle = %s\n", state->windowClassname, state->hwndTitle);

    HINSTANCE hInstance = GetModuleHandleW(NULL);
    WNDCLASSEXW window;
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
    window.lpszClassName = state->windowClassname;//windowClassname;
    window.hIconSm = NULL;

    RegisterClassExW(&window);

    HWND hwnd = CreateWindowExW(NULL, state->windowClassname, state->hwndTitle, WS_TILEDWINDOW, x, y, width, height, NULL, NULL, hInstance, NULL);
    state->hwnd = hwnd;
    //printf("hwnd=%i, state->hwnd=%i\n", hwnd, state->hwnd);

    wchar_t tmpName[32];
    sprintf(tmpName, "%i", state->hwnd);
    //printf("%s", tmpName);

    //printf("tmpPropName=%s\n", tmpName);

    SetPropW(state->hwnd, "feck", state);
    //UIHErr();


}

void UIHShowWindow(UIH_STATE *state, int hidden) {
    printf("state pointer address = %p\nstate->hwnd = %i\n", state, state->hwnd);

    if (hidden>0) {
        ShowWindow(state->hwnd, SW_SHOWDEFAULT);
    } else {
        ShowWindow(state->hwnd, SW_HIDE);
    }
    UpdateWindow(state->hwnd);
};

/** modifies UIHState.controls to include a new control */
int UIHMakeControl(UIH_STATE *state, char *text) {

    int index = state->numberOfControls++;
    printf("index = %i\n", index);
    state->controls[index].uuid = ++state->nextUUID;
    int textSize = MultiByteToWideChar(CP_UTF8, 0, text, -1, NULL, 0);
    state->controls[index].text = malloc(sizeof(wchar_t) * textSize);
    MultiByteToWideChar(CP_UTF8, 0, text, -1, state->controls[index].text, textSize);

    return index;
}

/*void UIHAddLabel(char *text, int x, int y, int w, int h) {
    UIH_CONTROL control;
    //control.uuid = ++UIHState.controlCount;
    int index = UIHMakeControl(text);

    //control.text = malloc(sizeof(wchar_t) + 2);
    //int nsize = MultiByteToWideChar(CP_UTF8, 0, text, -1, NULL, 0);
    //int nnsize = MultiByteToWideChar(CP_UTF8, 0, text, -1, control.text, nsize);

    //printf("\n** %i %i **\n", nsize, nnsize);
    printf("\n** control.text = %i **\n", UIHState.controls[index].uuid);

    UIHState.controls[index].hwnd = CreateWindowExW(NULL, L"STATIC", UIHState.controls[index].text, WS_VISIBLE | WS_CHILD, x, y, w, h, UIHState.hwnd, (HMENU) UIHState.controls[index].uuid, NULL, NULL);

    printf("** UIHAddLabel: UIHState.controlCount = %i, control.uuid = %i\n", UIHState.controlCount, UIHState.controls[index].uuid);
    SendMessageW(UIHState.controls[index].hwnd,WM_SETFONT,
                 MAKEWPARAM(UIHState.font[0], 0),
                 MAKELPARAM(1, 0));
    //return control;
}*/

void UIHAddButton(UIH_STATE *state, char *text, int x, int y, int w, int h, void* callback) {

    int index  = UIHMakeControl(state, text);
    state->controls[index].fnCallback = callback;
    printf("addbutton: uuid = %i\n", state->controls[index].uuid);
    state->controls[index].hwnd = CreateWindowExW(NULL, L"BUTTON", state->controls[index].text, WS_VISIBLE | WS_CHILD, x, y, w, h, state->hwnd, (HMENU) state->controls[index].uuid, NULL, NULL);
    SendMessageW(state->controls[index].hwnd,
                 WM_SETFONT,
                 MAKEWPARAM(state->fonts[0], 0),
                 MAKELPARAM(1, 0));
    //return control;
}

/*void UIHAddEdit(char *text, int x, int y, int w, int h) {

    int index = UIHMakeControl(text);

    UIHState.controls[index].hwnd = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", UIHState.controls[index].text, ES_MULTILINE | ES_AUTOVSCROLL | WS_VISIBLE | WS_CHILD, x, y, w, h, UIHState.hwnd, (HMENU) UIHState.controls[index].uuid, NULL, NULL);
    printf("** UIHAddEdit: UIHState.controlCount = %i, control.uuid = %i\n", UIHState.controlCount, UIHState.controls[index].uuid);
    SendMessageW(UIHState.controls[index].hwnd,WM_SETFONT,
                 MAKEWPARAM(UIHState.font[0], 0),
                 MAKELPARAM(1, 0));
}

void UIHDebugControl() {

    int index = UIHMakeControl("FK");
    free(UIHState.controls[index].text);
    UIHState.controls[index].text = malloc(20000000);
}*/

void UIHInit(UIH_STATE *state) {

    state->fonts[0] = CreateFontW(14, 0, 0, 0, FW_REGULAR, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, TEXT("Microsoft Sans Serif"));
    state->nextUUID = 1000;

    INITCOMMONCONTROLSEX c;
    c.dwICC = ICC_TAB_CLASSES;
    c.dwSize = sizeof(INITCOMMONCONTROLSEX);
    InitCommonControlsEx(&c);

    _wsetlocale(LC_ALL, "en_US.utf8");

}

void UIHClean(UIH_STATE *state) {

    int i = 0;
    while(i < UIH_NUM_FONTS) {
        DeleteObject(state->fonts[i++]);
    }
    i = 0;
    while (i < state->numberOfControls) {
        //UIH_CONTROL control = UIHState.controls[i++];
        free(state->controls[i++].text);
    }


    free(state->hwndTitle);
    free(state->windowClassname);
    //UIHState.
}



#endif // _GUARD
