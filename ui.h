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
#include "locale.h"

#include "windows.h"
#include "mbstring.h"
#include "Commctrl.h"
#include "locale.h"





/// utf tests
#define UIH_HelloWorld_Hanzi "世界你好"
#define UIH_HelloWorld_Kanji "こんにちは、世界"
#define UIH_HelloWorld_English "Hello world"

#define UIH_NUM_FONTS           10
#define UIH_NUM_CONTROLS        100
#define UIH_PROPNAME_STATE      L"uihprop1"
#define UIH_PROPNAME_CALLBACK   L"uihcallback1"

typedef struct UIH_CONTROL {
    long uuid;
    HWND hwnd;
    wchar_t *text;
    void *fnCallback;
} UIH_CONTROL;

typedef struct UIH_CALLBACK {
    void *data1;
    void *data2;
    void *data3;
    void *data4;
} UIH_CALLBACK;

typedef struct UIH_STATE {
    long nextUUID;
    int numberOfControls;
    int numberOfFonts;
    wchar_t *hwndTitle;
    wchar_t *windowClassname;
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
    return state;
}


int UIHErrorCallCount = 1;
#define UIHErr() (printf("\n*****\n - GetLastError#%i = %i\n - in: %s\n - %s @ %i\n*****\n", UIHErrorCallCount++, (int) GetLastError(), __FUNCTION__, __FILE__, __LINE__));

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

LRESULT CALLBACK windowProcCallback(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
    int id = (unsigned short) wparam;
    int event = ((unsigned short)(((unsigned long) wparam >> 16) & 0xFFFF));
    HANDLE handle = GetPropW(hwnd, UIH_PROPNAME_STATE);
    UIH_STATE *state = NULL;
    if (handle != NULL) {
        state = (UIH_STATE *) handle;
        switch (umsg) {
            case WM_COMMAND: {
                for(int i = 0; i < state->numberOfControls; i++) {
                    UIH_CONTROL control = state->controls[i];
                    if (id == control.uuid) {
                        if (control.fnCallback != NULL) {
                            HANDLE tHandle = GetPropW(control.hwnd, UIH_PROPNAME_CALLBACK);
                            void (*UIH_fnCallback)(UIH_STATE*, void*) = control.fnCallback;
                            UIH_fnCallback(state, tHandle);
                        }
                        break;
                    }
                }
                break;
            }
            case WM_SIZE: {
                break;
            }
            case WM_SIZING: {
                break;
            }
            case WM_CLOSE: {
                printf("close");
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


void UIHSetString(UIH_CONTROL *control, char *text) {
    int textSize = MultiByteToWideChar(CP_UTF8, 0, text, -1, NULL, 0);
    wchar_t *tmpBuffer = malloc(textSize * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, text, -1, tmpBuffer, textSize);
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

    HWND hwnd = CreateWindowExW(NULL, state->windowClassname, state->hwndTitle, WS_TILEDWINDOW, x, y, width, height, NULL, NULL, hInstance, NULL);
    state->hwnd = hwnd;

    wchar_t tmpName[32];
    sprintf(tmpName, "%i", state->hwnd);

    SetPropW(state->hwnd, UIH_PROPNAME_STATE, state);
}

void UIHShowWindow(UIH_STATE *state, int hidden) {
    if (hidden) {
        ShowWindow(state->hwnd, SW_SHOWDEFAULT);
    } else {
        ShowWindow(state->hwnd, SW_HIDE);
    }
    UpdateWindow(state->hwnd);
};

/** \brief Private function which allocates memory and converts char to widechar
 *
 * \param UIH_STATE pointer
 * \param array of chracters
 * \return state->controls index of newly created control
 *
 */
static int UIHMakeControl(UIH_STATE *state, char *text) {
    if (state->numberOfControls < UIH_NUM_CONTROLS) {
        int index = state->numberOfControls++;
        state->controls[index].uuid = ++state->nextUUID;
        int textSize = MultiByteToWideChar(CP_UTF8, 0, text, -1, NULL, 0);
        state->controls[index].text = malloc(sizeof(wchar_t) * (textSize + sizeof(wchar_t)));
        MultiByteToWideChar(CP_UTF8, 0, text, -1, state->controls[index].text, textSize);
        return index;
    }
    else {
        UIHDisplayError("exceeded UIH_NUM_CONTROLS", __FUNCTION__, 0, "");
        return -1;
    }
}

UIH_CONTROL *UIHAddLabel(UIH_STATE *state, char *text, int fontid, int x, int y, int w, int h) {
    int index  = UIHMakeControl(state, text);

    printf("%s: uuid = %i\n", __FUNCTION__, state->controls[index].uuid);
    state->controls[index].hwnd = CreateWindowExW(NULL, L"STATIC", state->controls[index].text, WS_VISIBLE | WS_CHILD, x, y, w, h, state->hwnd, (HMENU) state->controls[index].uuid, NULL, NULL);
    SendMessageW(state->controls[index].hwnd,
                 WM_SETFONT,
                 MAKEWPARAM(state->fonts[fontid], 0),
                 MAKELPARAM(1, 0));
    return &state->controls[index];
}

UIH_CONTROL *UIHAddButton(UIH_STATE *state, char *text, int fontid, int x, int y, int w, int h, void* callback, void* data) {
    int index  = UIHMakeControl(state, text);

    printf("%s: uuid = %i\n", __FUNCTION__, state->controls[index].uuid);
    state->controls[index].hwnd = CreateWindowExW(NULL, L"BUTTON", state->controls[index].text, WS_VISIBLE | WS_CHILD, x, y, w, h, state->hwnd, (HMENU) state->controls[index].uuid, NULL, NULL);

    if (callback != NULL) {
        state->controls[index].fnCallback = callback;
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
    int index  = UIHMakeControl(state, text);

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
    state->nextUUID = 1000;

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

    free(state->hwndTitle);
    free(state->windowClassname);

    RemovePropW(state->hwnd, UIH_PROPNAME_STATE);

}



#endif // _GUARD
