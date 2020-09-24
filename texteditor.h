#ifndef _TEXTEDITOR
#define _TEXTEDITOR

#include "string.h"
#include "stdlib.h"
#include "locale.h"


#include "stdio.h"

#include "windows.h"
#include "mbstring.h"
#include "Commctrl.h"
#include "locale.h"
#include "commdlg.h"
#include "wchar.h"

#include "ui.h"
#include "util.h"



enum TEXTEDITOR_MENU {
    TEXTEDITOR_MENU_OPEN = UIH_MENU_UUID_RANGE,
    TEXTEDITOR_MENU_SAVE,
    TEXTEDITOR_MENU_SAVEAS,
    TEXTEDITOR_MENU_QUIT
};

void editCallback(UIH_STATE *state, UIH_CONTROL *control, void *data) {
    if (state->datums[0] != NULL && state->datums[1] == 0) {
        state->datums[1] = 1;
        printf("i am here\n");

        wchar_t *tmpTitle = malloc(sizeof(wchar_t) * UIH_MAX_FILENAME_SIZE);
        tmpTitle[0] = '\0';
        int titleSize = wcslen(state->hwndTitle);
        int filenameSize = wcslen(state->datums[0]);
        int seperatorSize = wcslen(L" - ");
        int asteriskSize = wcslen(L"*");
        wcsncat(tmpTitle, state->hwndTitle, titleSize);
        wcsncat(tmpTitle, L" - ", seperatorSize);
        wcsncat(tmpTitle, state->datums[0], filenameSize);
        wcsncat(tmpTitle, L"*", asteriskSize);
        tmpTitle[titleSize + filenameSize + seperatorSize + asteriskSize] = '\0';
        SetWindowTextW(state->hwnd, tmpTitle);
        free(tmpTitle);

    }
}

void editOnResizeCallback(UIH_STATE *state, UIH_CONTROL *control, UIH_CONTROL_RECT *rect) {
    printf("editOnResizeCallback control = %p\n", control);
    printf("%i\n", control->hwnd);
    control->rect.width = rect->width - 28;
    control->rect.height = rect->height - 70;
    SetWindowPos(control->hwnd, HWND_TOP, 0, 0, control->rect.width, control->rect.height, SWP_NOMOVE);
}

void doFileSave(UIH_STATE *state, UIH_CONTROL *editControl) {
    if (editControl!=NULL) {
        printf("saveas datums[0] = %s @ %p\n", state->datums[0], state->datums[0]);
        if (state->datums[0]==NULL) {
            printf("I SHOULD BE HERE REEEE\N");
            OPENFILENAMEW openfile = {0};
            wchar_t filename[UIH_MAX_FILENAME_SIZE] = {0};

            openfile.lStructSize = sizeof(OPENFILENAMEW);
            openfile.hwndOwner = NULL;
            openfile.lpstrFile = filename;
            openfile.nMaxFile = sizeof(filename);
            openfile.lpstrFilter = L"All files\0*.*\0Text files\0*.TXT\0";
            openfile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            if (GetSaveFileNameW(&openfile)) {
                free(state->datums[0]);
                state->datums[0] = malloc(sizeof(wchar_t) * UIH_MAX_FILENAME_SIZE);
                memcpy(state->datums[0], filename, sizeof(filename));
            }
            else {
                return;
            }
        }

        wchar_t *tmpTitle = malloc(sizeof(wchar_t) * UIH_MAX_FILENAME_SIZE);
        tmpTitle[0] = '\0';
        int titleSize = wcslen(state->hwndTitle);
        int filenameSize = wcslen(state->datums[0]);
        int seperatorSize = wcslen(L" - ");
        wcsncat(tmpTitle, state->hwndTitle, titleSize);
        wcsncat(tmpTitle, L" - ", seperatorSize);
        wcsncat(tmpTitle, state->datums[0], filenameSize);
        tmpTitle[titleSize + filenameSize + seperatorSize] = '\0';
        SetWindowTextW(state->hwnd, tmpTitle);
        free(tmpTitle);

        wchar_t *editText = UIHGetString(editControl);
        int editTextSize = WideCharToMultiByte(CP_UTF8, 0, editText, -1, NULL, 0, NULL, NULL);
        char *buffer = UIHWideToChar(editText);

        FILE *file = _wfopen(state->datums[0], L"w+");
        fwrite(buffer, sizeof(char), editTextSize - sizeof(char), file);
        fclose(file);
        free(editText);
        free(buffer);

        state->datums[1] = 0;
    }
}


void doFileOpen(UIH_STATE *state, UIH_CONTROL *editControl) {
    OPENFILENAMEW openfile = {0};
    wchar_t filename[512] = {0};

    openfile.lStructSize = sizeof(OPENFILENAMEW);
	openfile.hwndOwner = NULL;
	openfile.lpstrFile = filename;
	openfile.nMaxFile = sizeof(filename);
	openfile.lpstrFilter = L"All files\0*.*\0Text files\0*.TXT\0";
	openfile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&openfile)) {
        char *buffer = UIHWideToChar(filename);
        FILE *file = _wfopen(filename, L"a+");
        if (file!=NULL) {
            fseek(file, 0, SEEK_END);
            long size = ftell(file);
            fseek(file, 0, SEEK_SET);
            char *contentBuffer = malloc(sizeof(char) * (size + 1));
            size_t r = fread(contentBuffer, sizeof(char), size, file);
            contentBuffer[size] = '\0';

            if (editControl!=NULL) {
                UIHSetString(editControl, contentBuffer);
            }
            // keep a copy of the filename to use as the filename in doFileSave
            free(state->datums[0]);
            state->datums[0] = malloc(sizeof(wchar_t) * UIH_MAX_FILENAME_SIZE);
            memcpy(state->datums[0], filename, sizeof(filename));

            wchar_t *tmpTitle = malloc(sizeof(wchar_t) * UIH_MAX_FILENAME_SIZE);
            tmpTitle[0] = '\0';
            int titleSize = wcslen(state->hwndTitle);
            int filenameSize = wcslen(state->datums[0]);
            int seperatorSize = wcslen(L" - ");
            wcsncat(tmpTitle, state->hwndTitle, titleSize);
            wcsncat(tmpTitle, L" - ", seperatorSize);
            wcsncat(tmpTitle, state->datums[0], filenameSize);
            tmpTitle[titleSize + filenameSize + seperatorSize] = '\0';
            SetWindowTextW(state->hwnd, tmpTitle);
            free(tmpTitle);

            fclose(file);
            free(contentBuffer);
            free(buffer);
        }
    }
}

void onMenuCallback(UIH_STATE *state, int menuId, void *data) {
    UIH_CALLBACK *tmpStruct = (UIH_CALLBACK*) data;
    UIH_CONTROL *control = NULL;
    if (data != NULL) {
        control = (UIH_CONTROL*) tmpStruct->datums[0];
        printf("address for 0 = %p\naddress for 1 = %p\n", tmpStruct->datums[0], tmpStruct->datums[1]);
    }
    switch(menuId) {
        case TEXTEDITOR_MENU_OPEN: {
            if (control!=NULL) {
                doFileOpen(state, control);
            }
          break;
        }
        case TEXTEDITOR_MENU_SAVE: {
            if (control != NULL) {
                doFileSave(state, control);
            }
            break;
        }
        case TEXTEDITOR_MENU_SAVEAS: {
            if (control != NULL) {
                free(state->datums[0]);
                state->datums[0] = NULL;
                printf("menu callback state-datums[0] = %s @ %p\n", state->datums[0], state->datums[0]);
                //memcpy(state->datums[0], NULL, 1);

                doFileSave(state, control);
            }
            break;
        }
        case TEXTEDITOR_MENU_QUIT: {
            SendMessageW(state->hwnd, WM_CLOSE, 0, 0);
            break;
        }
        default: {
            printf("asddcxxx");
            break;
        }
    }

    /**/

}

void MakeTextEditorMenu(UIH_STATE *state) {

    HMENU child = CreateMenu();
    HMENU parent = CreateMenu();

    AppendMenuW(parent, MF_POPUP, child, L"File");
    AppendMenuW(child, MF_STRING, TEXTEDITOR_MENU_OPEN, L"&Open\tCtrl+O");
    AppendMenuW(child, MF_STRING, TEXTEDITOR_MENU_SAVE, L"&Save\tCtrl+S");
    AppendMenuW(child, MF_STRING, TEXTEDITOR_MENU_SAVEAS, L"Save As\tCtrl+Shift+S");
    AppendMenuW(child, MF_SEPARATOR, 0, NULL);
    AppendMenuW(child, MF_STRING, TEXTEDITOR_MENU_QUIT, L"&Quit\tCtrl+Q");

    SetMenu(state->hwnd, parent);

    ACCEL accels[4];
    accels[0].fVirt = FCONTROL | FVIRTKEY;
    accels[0].cmd = TEXTEDITOR_MENU_OPEN;
    accels[0].key = 'O';

    accels[1].fVirt = FCONTROL | FVIRTKEY;
    accels[1].cmd = TEXTEDITOR_MENU_SAVE;
    accels[1].key = 'S';

    accels[2].fVirt = FCONTROL | FSHIFT | FVIRTKEY;
    accels[2].cmd = TEXTEDITOR_MENU_SAVEAS;
    accels[2].key = 'S';

    accels[3].fVirt = FCONTROL | FVIRTKEY;
    accels[3].cmd = TEXTEDITOR_MENU_QUIT;
    accels[3].key = 'Q';

    state->accelTable = CreateAcceleratorTableW(accels, 4);
    UIHErr();

}

#endif // _TEXTEDITOR
