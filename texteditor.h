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

#include "ui.h"
#include "util.h"



enum TEXTEDITOR_MENU {
    TEXTEDITOR_MENU_OPEN = UIH_MENU_UUID_RANGE,
    TEXTEDITOR_MENU_SAVE,
    TEXTEDITOR_MENU_SAVEAS,
    TEXTEDITOR_MENU_QUIT
};

void callbackTest(UIH_STATE *state, void *data) {
    printf("henlo\n");
}

void doFileSave(UIH_STATE *state, UIH_CONTROL *editControl) {
    if (editControl!=NULL) {
        printf("saveas datums[0] = %s @ %p\n", state->datums[0], state->datums[0]);
        if (state->datums[0]==NULL) {
            printf("I SHOULD BE HERE REEEE\N");
            OPENFILENAMEW openfile = {0};
            wchar_t filename[512] = {0};

            openfile.lStructSize = sizeof(OPENFILENAMEW);
            openfile.hwndOwner = NULL;
            openfile.lpstrFile = filename;
            openfile.nMaxFile = sizeof(filename);
            openfile.lpstrFilter = L"All files\0*.*\0Text files\0*.TXT\0";
            openfile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            if (GetSaveFileNameW(&openfile)) {
                free(state->datums[0]);
                state->datums[0] = malloc(sizeof(filename));
                //memcpy(state->datums[0], filename, sizeof(filename));
                //sprintf(state->datums[0], "%s", filename);
                memcpy(state->datums[0], filename, sizeof(filename));
            }
        }

        wchar_t *editText = UIHGetString(editControl);
        int editTextSize = WideCharToMultiByte(CP_UTF8, 0, editText, -1, NULL, 0, NULL, NULL);
        char *buffer = UIHWideToChar(editText);
        FILE *file = _wfopen(state->datums[0], L"w+");//fopen("myfilename.txt", "w+");//,ccs=UTF-8");//ccs=UTF-16LE");
        fwrite(buffer, sizeof(char), editTextSize - sizeof(char), file);
        fclose(file);
        free(editText);
        free(buffer);
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
        FILE *file = _wfopen(filename, L"a+");//fopen(buffer, "a+");//("myfilename.txt", "a+");
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
            state->datums[0] = malloc(sizeof(filename));
            memcpy(state->datums[0], filename, sizeof(filename));
            //sprintf(state->datums[0], "%s", filename);

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
    AppendMenuW(child, MF_STRING, TEXTEDITOR_MENU_OPEN, L"&Open");
    AppendMenuW(child, MF_STRING, TEXTEDITOR_MENU_SAVE, L"&Save");
    AppendMenuW(child, MF_STRING, TEXTEDITOR_MENU_SAVEAS, L"&Save As");
    AppendMenuW(child, MF_SEPARATOR, 0, NULL);
    AppendMenuW(child, MF_STRING, TEXTEDITOR_MENU_QUIT, L"&Quit");

    SetMenu(state->hwnd, parent);

}

#endif // _TEXTEDITOR
