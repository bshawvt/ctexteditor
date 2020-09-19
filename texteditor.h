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
    TEXTEDITOR_MENU_QUIT
};

void callbackTest(UIH_STATE *state, void *data) {
    printf("henlo\n");
}

void doFileSave(UIH_STATE *state, UIH_CONTROL *editControl) {
    /* todo: broken
        new file
    */
    //UIH_CONTROL *control = (UIH_CONTROL*) data;
    if (editControl!=NULL) {
        if (state->datums[0] == NULL) {
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
        printf("t1\n");
        if (file!=NULL) {
            printf("t2\n");
            fseek(file, 0, SEEK_END);
            long size = ftell(file);
            fseek(file, 0, SEEK_SET);
            char *contentBuffer = malloc(sizeof(char) * (size + 1));
            size_t r = fread(contentBuffer, sizeof(char), size, file);
            contentBuffer[size] = '\0';

            //UIH_CONTROL *control = (UIH_CONTROL*) data;
            if (editControl!=NULL) {
                printf("t3\n");
                UIHSetString(editControl, contentBuffer);
            }
            free(state->datums[0]);
            state->datums[0] = malloc(sizeof(filename));
            memcpy(state->datums[0], filename, sizeof(filename));
            printf("%s\n", state->datums[0]);
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
    }
    switch(menuId) {
        case TEXTEDITOR_MENU_OPEN: {
          //if (data!=NULL) {
            //UIH_CALLBACK *tmpStruct = (UIH_CALLBACK*) data;
            //UIH_CONTROL *control = (UIH_CONTROL*) tmpStruct->data[0];

            if (control!=NULL) {
                printf("onopen\n");
                doFileOpen(state, control);
            }
          //}
          break;
        }
        case TEXTEDITOR_MENU_SAVE: {
            //UIH_CALLBACK *tmpStruct = (UIH_CALLBACK*) data;
            //UIH_CONTROL *control = (UIH_CONTROL*) tmpStruct->data[0];
            if (control != NULL) {
                //printf("am i here\n");
                //printf("onsave filename = %s\n", state->datums[0]);
                doFileSave(state, control);
            }
            break;
        }
        case TEXTEDITOR_MENU_QUIT: {

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
    AppendMenuW(child, MF_SEPARATOR, 0, NULL);
    AppendMenuW(child, MF_STRING, TEXTEDITOR_MENU_QUIT, L"&Quit");

    SetMenu(state->hwnd, parent);

}

#endif // _TEXTEDITOR
