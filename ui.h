#ifndef _UIH_GUARD
#define _UIH_GUARD

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include "string.h"
#include "stdlib.h"

#include "windows.h"
#include "mbstring.h"
#include "Commctrl.h"
#include "locale.h"
#include "richedit.h"
#include "commdlg.h"

/// utf tests
#define UIH_HelloWorld_Hanzi "世界你好"
#define UIH_HelloWorld_Kanji "こんにちは、世界"
#define UIH_HelloWorld_English "Hello world"

#define UIH_NUM_FONTS               10
#define UIH_NUM_CONTROLS            10
#define UIH_NUM_STATE_DATUMS        10
#define UIH_NUM_CALLBACK_DATUMS     10
#define UIH_CONTROL_UUID_RANGE      1000
#define UIH_MENU_UUID_RANGE         10000
#define UIH_PROPNAME_STATE          L"uihprop1"
#define UIH_PROPNAME_CALLBACK       L"uihcallback1"
#define UIH_PROPNAME_MENUCALLBACK   L"uihcallback2"
#define UIH_MAX_FILENAME_SIZE       1024

typedef struct UIH_CONTROL_RECT {
    int x;
    int y;
    int width;
    int height;
} UIH_CONTROL_RECT;

typedef struct UIH_CONTROL {
    long uuid;
    HWND hwnd;
    wchar_t *text;
    UIH_CONTROL_RECT rect;
    void *fnControlCallback;
    void *fnResizeCallback;
} UIH_CONTROL;

typedef struct UIH_CALLBACK {
    void *datums[UIH_NUM_CALLBACK_DATUMS];
} UIH_CALLBACK;

typedef struct UIH_STATE {
    int isRunning; // if main state then 0 tells main loop to stop
    long nextUUID;
    int numberOfControls;
    int numberOfFonts;
    int numberOfMenuItems;
    wchar_t *hwndTitle;
    wchar_t *windowClassname;
    HACCEL accelTable;
    void *datums[UIH_NUM_STATE_DATUMS]; // holds misc data like current open filenames
    void *fnMenuCallback;
    HWND hwnd;
    HFONT fonts[UIH_NUM_FONTS];
    UIH_CONTROL controls[UIH_NUM_CONTROLS];
} UIH_STATE;



UIH_STATE *UIHMakeState() {
    UIH_STATE *state = (UIH_STATE*) malloc(sizeof(UIH_STATE));
    if (state == NULL) {
        UIHDisplayError("Bad things have happened and couldn't allocate memory for state", __FUNCTION__, 1, "Error");
        return NULL;
    }
    *state = (UIH_STATE) {0};
    state->accelTable = NULL;
    return state;
}


int UIHErrorCallCount = 1;
#define UIHErr() (printf("\n*****\n - GetLastError#%i = %i\n - in: %s\n - %s @ %i\n*****\n", UIHErrorCallCount++, (int) GetLastError(), __FUNCTION__, __FILE__, __LINE__));

wchar_t *UIHCharToWide(char *text) {
    int textSize = MultiByteToWideChar(CP_UTF8, 0, text, -1, NULL, 0);

    wchar_t *wText = malloc(sizeof(wchar_t) * textSize);
    sprintf(wText, "%s", text);

    MultiByteToWideChar(CP_UTF8, 0, text, -1, wText, textSize);
    return wText;
}
void UIHDisplayError(char *body, char *gcc_function_macro, int showMessageBox, char* title) {
    printf("Error in %s():\n %s\n", gcc_function_macro, body);
    if (showMessageBox) {
        int bodySize = MultiByteToWideChar(CP_UTF8, 0, body, -1, NULL, 0);
        int titleSize = MultiByteToWideChar(CP_UTF8, 0, title, -1, NULL, 0);

        wchar_t tmpBodyText[bodySize];
        wchar_t tmpTitleText[titleSize];

        sprintf(tmpBodyText, "%s", body);
        sprintf(tmpTitleText, "%s", title);

        MultiByteToWideChar(CP_UTF8, 0, body, -1, tmpBodyText, bodySize);
        MultiByteToWideChar(CP_UTF8, 0, title, -1, tmpTitleText, titleSize);

        MessageBoxW(NULL, tmpBodyText, tmpTitleText, 0);
    }
};
void UIHDoMenuCallbackFromId(UIH_STATE *state, int id) {
    printf("id = %i\n", id);
    if (state->fnMenuCallback != NULL) {
        HANDLE fHandle = GetPropW(state->hwnd, UIH_PROPNAME_MENUCALLBACK);
        void (*fnMenuCallback)(UIH_STATE *, int, void *) = state->fnMenuCallback;
        fnMenuCallback(state, id, fHandle);
    }
}
void UIHDoControlCallbackFromId(UIH_STATE *state, int id) {
    printf("id = %i\n", id);
    for(int i = 0; i < state->numberOfControls; i++) {
        UIH_CONTROL *control = &state->controls[i];
        if (id == control->uuid) {
            if (control->fnControlCallback != NULL) {
                HANDLE tHandle = GetPropW(control->hwnd, UIH_PROPNAME_CALLBACK);
                void (*fnControlCallback)(UIH_STATE*, UIH_CONTROL*, void*) = control->fnControlCallback;
                fnControlCallback(state, control, tHandle);
            }
            break;
        }
    }
}


void UIHDoControlResizeCallback(UIH_STATE *state) {
    UIH_CONTROL_RECT rect = {0};
    RECT *tmpRect = malloc(sizeof(RECT));
    GetWindowRect(state->hwnd, tmpRect);
    rect.width = tmpRect->right - tmpRect->left;
    rect.height = tmpRect->bottom - tmpRect->top;
    for(int i = 0; i < UIH_NUM_CONTROLS; i++) {
        UIH_CONTROL *control = &state->controls[i];
        if (control->fnResizeCallback != NULL) {
            printf("UIHDoControlResizeCallback control addr = %p\n", control);
            void (*fnResizeCallback)(UIH_STATE*, UIH_CONTROL*, UIH_CONTROL_RECT*) = control->fnResizeCallback;
            fnResizeCallback(state, control, &rect);
        }
    }
    free(tmpRect);
}

LRESULT CALLBACK windowProcCallback(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
    int id = (unsigned short) wparam;
    int event = ((unsigned short)(((unsigned long) wparam >> 16) & 0xFFFF));
    HANDLE handle = GetPropW(hwnd, UIH_PROPNAME_STATE);
    UIH_STATE *state = NULL;
    if (handle != NULL) {
        state = (UIH_STATE *) handle;
        switch (umsg) {
            case WM_COMMAND: {
                if (id < UIH_CONTROL_UUID_RANGE + UIH_NUM_CONTROLS) {
                    UIHDoControlCallbackFromId(state, id);
                }
                else {
                    UIHDoMenuCallbackFromId(state, id);
                }
                break;
            }
            case WM_SIZE: {
                printf("size\n");
                UIHDoControlResizeCallback(state);
                break;
            }
            case WM_SIZING: {
                printf("size2\n");
                break;
            }
            case WM_CLOSE: {
                printf("closeasdasdasd");
                UIHClean(state);
                break;
            }
            default: {
                //printf("\n** window proc callback:\n id=%i event=%i umsg=%x", id, event, umsg);
                break;
            }
        }
    }
    else {
        UIHDisplayError("handle is null!", __FUNCTION__, 0, "");
    }
    return DefWindowProcW(hwnd, umsg, wparam, lparam);
}

char *UIHWideToChar(char *text) {
    int textSize = WideCharToMultiByte(CP_UTF8, 0, text, -1, NULL, 0, NULL, NULL);
    char *buffer = malloc(sizeof(char) * (textSize + 1));
    WideCharToMultiByte(CP_UTF8, 0, text, -1, buffer, textSize, NULL, NULL);
    return buffer;
}

void UIHSetString(UIH_CONTROL *control, char *text) {
    int textSize = MultiByteToWideChar(CP_UTF8, 0, text, -1, NULL, 0);
    wchar_t *tmpBuffer = malloc(textSize * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, text, -1, tmpBuffer, textSize);
    printf("hwnd= %i\ncontrol addr = %p\n", control, control);
    SendMessageW(control->hwnd, WM_SETTEXT, 0, tmpBuffer);
    free(tmpBuffer);
}

wchar_t *UIHGetString(UIH_CONTROL *control) {
    int editSize = SendMessageW(control->hwnd, WM_GETTEXTLENGTH, 0, 0);
    wchar_t *buffer = malloc(sizeof(wchar_t) * (editSize + sizeof(wchar_t)));
    SendMessageW(control->hwnd, WM_GETTEXT, editSize + sizeof(wchar_t), buffer);
    return buffer;
}

void UIHCreateWindow(UIH_STATE *state, char *title, int x, int y, int width, int height) {
    int classnameSize = MultiByteToWideChar(CP_UTF8, 0, title, -1, NULL, 0);
    int titleSize = MultiByteToWideChar(CP_UTF8, 0, title, -1, NULL, 0);

    state->windowClassname = malloc(sizeof(wchar_t) * (classnameSize + sizeof(wchar_t)));
    state->hwndTitle = malloc(sizeof(wchar_t) + (titleSize * sizeof(wchar_t)));

    char tmpClassname[classnameSize];
    sprintf(tmpClassname, "%p%s", state, title);
    MultiByteToWideChar(CP_UTF8, 0, tmpClassname, -1, state->windowClassname, classnameSize);
    MultiByteToWideChar(CP_UTF8, 0, title, -1, state->hwndTitle, titleSize);
    UIHErr();
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
    window.lpszClassName = state->windowClassname;
    window.hIconSm = NULL;

    RegisterClassExW(&window);
    UIHErr();

    state->hwnd = CreateWindowExW(NULL, state->windowClassname, state->hwndTitle, WS_TILEDWINDOW, x, y, width, height, NULL, NULL, hInstance, NULL);

    wchar_t tmpName[32];
    sprintf(tmpName, "%i", state->hwnd);

    SetPropW(state->hwnd, UIH_PROPNAME_STATE, state);

    UIHErr();
}

void UIHShowWindow(UIH_STATE *state, int hidden) {
    if (hidden) {
        ShowWindow(state->hwnd, SW_SHOWDEFAULT);
    } else {
        ShowWindow(state->hwnd, SW_HIDE);
    }
    UpdateWindow(state->hwnd);
};

int UIHMakeControl(UIH_STATE *state, char *text, int x, int y, int width, int height) {
    if (state->numberOfControls < UIH_NUM_CONTROLS) {
        int index = state->numberOfControls++;
        state->controls[index].uuid = ++state->nextUUID;
        int textSize = MultiByteToWideChar(CP_UTF8, 0, text, -1, NULL, 0);
        state->controls[index].text = malloc(sizeof(wchar_t) * (textSize + sizeof(wchar_t)));
        MultiByteToWideChar(CP_UTF8, 0, text, -1, state->controls[index].text, textSize);

        state->controls[index].rect.x = x;
        state->controls[index].rect.y = y;
        state->controls[index].rect.width = width;
        state->controls[index].rect.height = height;

        return index;
    }
    else {
        UIHDisplayError("exceeded UIH_NUM_CONTROLS", __FUNCTION__, 0, "");
        return -1;
    }
}

void UIHRegisterMenuCallback(UIH_STATE *state, void *callback, void *data) {
    state->fnMenuCallback = callback;
    SetPropW(state->hwnd, UIH_PROPNAME_MENUCALLBACK, data);
}

UIH_CONTROL *UIHAddLabel(UIH_STATE *state, char *text, int fontid, int x, int y, int w, int h) {
    int index  = UIHMakeControl(state, text, x, y, w, h);

    printf("%s: uuid = %i\n", __FUNCTION__, state->controls[index].uuid);
    state->controls[index].hwnd = CreateWindowExW(NULL, L"STATIC", state->controls[index].text, WS_VISIBLE | WS_CHILD, x, y, w, h, state->hwnd, (HMENU) state->controls[index].uuid, NULL, NULL);
    SendMessageW(state->controls[index].hwnd,
                 WM_SETFONT,
                 MAKEWPARAM(state->fonts[fontid], 0),
                 MAKELPARAM(1, 0));
    return &state->controls[index];
}

UIH_CONTROL *UIHAddButton(UIH_STATE *state, char *text, int fontid, int x, int y, int w, int h, void* callback, void* data) {
    int index  = UIHMakeControl(state, text, x, y, w, h);
    printf("%s: uuid = %i\n", __FUNCTION__, state->controls[index].uuid);
    state->controls[index].hwnd = CreateWindowExW(NULL, L"BUTTON", state->controls[index].text, WS_VISIBLE | WS_CHILD, x, y, w, h, state->hwnd, (HMENU) state->controls[index].uuid, NULL, NULL);

    if (callback != NULL) {
        state->controls[index].fnControlCallback = callback;
        if (data != NULL) {
            SetPropW(state->controls[index].hwnd, UIH_PROPNAME_CALLBACK, data);
        }
    }

    SendMessageW(state->controls[index].hwnd,
                 WM_SETFONT,
                 MAKEWPARAM(state->fonts[fontid], 0),
                 MAKELPARAM(1, 0));
    return &state->controls[index];
}

UIH_CONTROL *UIHAddEdit(UIH_STATE *state, char *text, int fontid, int x, int y, int w, int h) {
    int index  = UIHMakeControl(state, text, x, y, w, h);

    printf("%s: uuid = %li\n", __FUNCTION__, state->controls[index].uuid);
    state->controls[index].hwnd = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", state->controls[index].text, ES_MULTILINE | ES_AUTOVSCROLL | WS_VISIBLE | WS_CHILD, x, y, w, h, state->hwnd, (HMENU) state->controls[index].uuid, NULL, NULL);
    SendMessageW(state->controls[index].hwnd,
                 WM_SETFONT,
                 MAKEWPARAM(state->fonts[fontid], 0),
                 MAKELPARAM(1, 0));
    return &state->controls[index];
}

int UIHLoadFont(UIH_STATE *state, char *fontName) {
    if (state->numberOfFonts < UIH_NUM_FONTS) {
        int index = state->numberOfFonts++;
        int fontNameSize = MultiByteToWideChar(CP_UTF8, 0, fontName, -1, NULL, 0);
        wchar_t tmpFontName[fontNameSize];
        MultiByteToWideChar(CP_UTF8, 0, fontName, -1,tmpFontName, fontNameSize);
        state->fonts[index] = CreateFontW(14, 0, 0, 0, FW_REGULAR, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, tmpFontName);//TEXT("Microsoft Sans Serif"));
        return index;
    }
    else {
        UIHDisplayError("exceeded UIH_NUM_FONTS", __FUNCTION__, 0, "");
        return -1;
    }
}

void UIHInit(UIH_STATE *state) {
    setlocale(LC_ALL, "");

    UIHLoadFont(state, "Microsoft Sans Serif"); // make state->fonts[0] the default font
    state->nextUUID = UIH_CONTROL_UUID_RANGE;

    INITCOMMONCONTROLSEX c;
    c.dwICC = ICC_TAB_CLASSES;
    c.dwSize = sizeof(INITCOMMONCONTROLSEX);
    InitCommonControlsEx(&c); // force load comctl32 even though it's linked because sometimes it just dies to death without this
}

void UIHClean(UIH_STATE *state) {
    printf("numberOfFonts = %i\n", state->numberOfFonts);
    int i = 0;
    while(i < state->numberOfFonts) {
        DeleteObject(state->fonts[i++]);
    }
    i = 0;
    while (i < state->numberOfControls) {
        RemovePropW(state->controls[i++].hwnd, UIH_PROPNAME_CALLBACK);
        free(state->controls[i++].text);
    }
    i = 0;
    while (i < UIH_NUM_STATE_DATUMS) {
        free(state->datums[i++]);
    }
    RemovePropW(state->hwnd, UIH_PROPNAME_STATE);
    RemovePropW(state->hwnd, UIH_PROPNAME_MENUCALLBACK);

    free(state->hwndTitle);
    free(state->windowClassname);
    DestroyAcceleratorTable(state->accelTable);
    state->isRunning = 0;

}

void UIHEventUpdate(UIH_STATE *state) {
    MSG msg;
    if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (state->accelTable != NULL)
            TranslateAcceleratorW(state->hwnd, state->accelTable, &msg);
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

#endif // _GUARD
