#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

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


void buttonFunction(UIH_STATE *state, void *data) {
    printf("button activate fnCallback state address = %p\n", state);
    printf("button activate fnCallback data address = %p\n", data);

    UIH_CONTROL *control = (UIH_CONTROL*) data;
    if (data!=NULL) {

        //char editText[] = "heck世界你好こんにちは、世界feck";
        wchar_t *editText = UIHGetString(control);
        //int editTextSize = wcslen(editText) * sizeof(wchar_t);

        //printf("sizeof editText = %i\n", editTextSize);
        //printf("sizeof wchar_t = %i\n", sizeof(wchar_t));


        int editTextSize = WideCharToMultiByte(CP_UTF8, 0, editText, -1, NULL, 0, NULL, NULL);
        //printf("sizeof editTextSize2 = %i\n", editTextSize2);

        //char *buffer = malloc(sizeof(char) * (editTextSize2 + 1));
        //WideCharToMultiByte(CP_UTF8, 0, editText, -1, buffer, editTextSize2, NULL, NULL);

        char *buffer = UIHWideToChar(editText);
        FILE *file = fopen("myfilename.txt", "w+");//,ccs=UTF-8");//ccs=UTF-16LE");
        fwrite(buffer, sizeof(char), editTextSize - sizeof(char), file);
        fclose(file);
        free(editText);
        free(buffer);
    }
}


void buttonFunction2(UIH_STATE *state, void *data) {
    FILE *file = fopen("myfilename.txt", "a+");

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buffer = malloc(sizeof(char) * (size + 1));
    size_t r = fread(buffer, sizeof(char), size, file);
    buffer[size] = '\0';

    UIH_CONTROL *control = (UIH_CONTROL*) data;
    if (data!=NULL) {
        UIHSetString(control, buffer);
    }
    fclose(file);
    free(buffer);
    printf("asdasd\n");
}

void menuCallbacks(UIH_STATE *state, int menuId, void *data) {

    switch(menuId) {
        //case UIH_MENU_OPEN: {
        //  printf("asdasd\n");
        default: {
            printf("asddcxxx");
            break;
        }
    }

    /*OPENFILENAMEW openfile = {0}
    char filename[512] = {0};

    openfile.lStructSize = sizeof(OPENFILENAMEW);
	openfile.hwndOwner = NULL;
	openfile.lpstrFile = filename;
	openfile.nMaxFile = sizeof(filename);
	openfile.lpstrFilter = L"All files\0*.*\0Text files\0*.TXT\0";
	openfile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


    if (GetOpenFileNameW(&openfile)) {
        char *buffer = UIHWideToChar(filename);
        printf("buffer = %s\n", buffer);
        free(buffer);
    }


    UIHErr();*/

}



int main(int argc, char* args[]) {
    //menuCallbacks(NULL, NULL, NULL);
    UIH_STATE *state = UIHMakeState();
    if (state != NULL) {
        printf("state pointer address in %s() = %p\n", __FUNCTION__, state);
        UIHInit(state);
        UIHCreateWindow(state, "世界你好Heck", 10, 10, 250, 200);


        HMENU child = CreateMenu();
        HMENU parent = CreateMenu();

        enum UIH_MENU_ITEMS {
            UIH_MENU_OPEN = UIHGetNextMenuUUID(state),
            UIH_MENU_SAVE,
            UIH_MENU_QUIT
        };

        AppendMenuW(parent, MF_POPUP, child, L"File");
        AppendMenuW(child, MF_STRING, UIHGetNextMenuUUID(state), L"&Open");
        AppendMenuW(child, MF_STRING, UIHGetNextMenuUUID(state), L"&Save");
        AppendMenuW(child, MF_SEPARATOR, 0, NULL);
        AppendMenuW(child, MF_STRING, UIHGetNextMenuUUID(state), L"&Quit");

        SetMenu(state->hwnd, parent);

        UIH_CONTROL *label1 = UIHAddLabel(state, "世界你好\nこんにちは、世界\nHello World!!!?", 0, 10, 100, 150, 50);
        UIH_CONTROL *edit1 = UIHAddEdit(state, "heck世界你好こんにちは、世界feck", 0, 5, 5, 225, 80);
        UIHAddButton(state, "基佬", 0, 100, 90, 125, 25, &buttonFunction, edit1);
        UIHAddButton(state, "poop", 0, 100, 120, 125, 25, &buttonFunction2, edit1);

        UIH_CALLBACK tmpCallbackStruct;
        tmpCallbackStruct.data1 = &edit1;
        tmpCallbackStruct.data2 = &label1;
        printf("tmpCallbackStruct = %p\n", &tmpCallbackStruct);
        UIHRegisterMenu(state, &menuCallbacks, &tmpCallbackStruct);


        UIHShowWindow(state, 1);
        UIHErr();

        while(1) {
            MSG msg;
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

    }
    else {
        UIHDisplayError("Feckin' memory didn't allocate yo", __FUNCTION__, 1, "Error");
        return -1;
    }


    //UIClean();
    return 0;
}
