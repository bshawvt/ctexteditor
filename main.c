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

#include "ui.h"
#include "util.h"


void buttonFunction(UIH_STATE *state, void *data) {
    printf("button activate fnCallback state address = %p\n", state);
    printf("button activate fnCallback data address = %p\n", data);

    UIH_CONTROL *control = (UIH_CONTROL*) data;
    if (data!=NULL) {

        //char editText[] = "heck世界你好こんにちは、世界feck";
        wchar_t *editText = UIHGetString(control);
        int editTextSize = wcslen(editText) * sizeof(wchar_t);

        printf("sizeof editText = %i\n", editTextSize);
        printf("sizeof wchar_t = %i\n", sizeof(wchar_t));


        int editTextSize2 = WideCharToMultiByte(CP_UTF8, 0, editText, -1, NULL, 0, NULL, NULL);
        printf("sizeof editTextSize2 = %i\n", editTextSize2);
        char *buffer = malloc(sizeof(char) * editTextSize2 + 10);
        WideCharToMultiByte(CP_UTF8, 0, editText, -1, buffer, editTextSize2, NULL, NULL);

        FILE *file = fopen("myfilename.txt", "w+");//,ccs=UTF-8");//ccs=UTF-16LE");
        printf("??? = %c\n", buffer[editTextSize2]);
        fwrite(buffer, sizeof(char), editTextSize2 - 1, file);
        fclose(file);
        free(editText);
        free(buffer);
    }
}


void buttonFunction2(UIH_STATE *state, void *data) {

    //FILE *file = fopen("myfilename.txt", "a+");

    UIH_CONTROL *control = (UIH_CONTROL*) data;

    if (data!=NULL) {
        UIHSetString(control, "henlo世界你好");
    }


}

int main(int argc, char* args[]) {

    UIH_STATE *state = UIHMakeState();
    if (state != NULL) {
        printf("state pointer address in %s() = %p\n", __FUNCTION__, state);
        UIHInit(state);
        UIHCreateWindow(state, "世界你好Heck", 10, 10, 250, 200);

        UIH_CONTROL *label1 = UIHAddLabel(state, "世界你好\nこんにちは、世界\nHello World!!!?", 0, 10, 100, 150, 50);
        UIH_CONTROL *edit1 = UIHAddEdit(state, "heck世界你好こんにちは、世界feck", 0, 5, 5, 225, 80);
        UIHAddButton(state, "基佬", 0, 100, 90, 125, 25, &buttonFunction, edit1);
        UIHAddButton(state, "poop", 0, 100, 120, 125, 25, &buttonFunction2, edit1);

        UIHShowWindow(state, 1);
        UIHErr();

    }
    else {
        //UIHDisplayError();
        UIHDisplayError("Feckin' memory didn't allocate yo", __FUNCTION__, 1, "Error");
        return -1;
    }

    while(1) {
        MSG msg;
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    //UIClean();
    return 0;
}
