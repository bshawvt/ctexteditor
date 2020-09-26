#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#undef __STRICT_ANSI__

#include "string.h"
#include "stdlib.h"
#include "locale.h"
#include "wchar.h"


#include "stdio.h"

#include "windows.h"
#include "mbstring.h"
#include "Commctrl.h"
#include "locale.h"
#include "commdlg.h"

#include "../cprotogui/ui.h"
#include "texteditor.h"



int main(int argc, char* args[]) {


    // UIH
    //menuCallbacks(NULL, NULL, NULL);
    UIH_STATE *mainState = UIHMakeState();
    if (mainState != NULL) {

        UIHInit(mainState);
        UIHCreateWindow(mainState, "Text Editor", 10, 10, 350, 200);

        UIH_CONTROL *edit1 = UIHAddEdit(mainState, "", 0, 5, 5, 325, 80);
        edit1->fnResizeCallback = &editOnResizeCallback;
        edit1->fnControlCallback = &editCallback;
        printf("edit1 = %p\n", edit1);

        UIH_CALLBACK tmpCallbackStruct;
        tmpCallbackStruct.datums[0] = edit1;
        //tmpCallbackStruct.datums[1] = label1;
        UIHRegisterMenuCallback(mainState, &onMenuCallback, &tmpCallbackStruct);

        MakeTextEditorMenu(mainState);
        UIHShowWindow(mainState, 1);
        //UIHErr();

        mainState->isRunning = 1;
        while(mainState->isRunning) {
            Sleep(1);
            UIHEventUpdate(mainState);
        }

    }
    else {
        UIHDisplayError("Feckin' memory didn't allocate yo", __FUNCTION__, 1, "Error");
        return -1;
    }

    //UIClean();
    return 0;
}
