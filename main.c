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
#include "texteditor.h"



int main(int argc, char* args[]) {


    // UIH
    //menuCallbacks(NULL, NULL, NULL);
    UIH_STATE *mainState = UIHMakeState();
    if (mainState != NULL) {
        printf("state pointer address in %s() = %p\n", __FUNCTION__, mainState);
        UIHInit(mainState);
        UIHCreateWindow(mainState, "Worlds Worst Text Editor™", 10, 10, 350, 200);

        //UIH_CONTROL *label1 = UIHAddLabel(state, "世界你好\nこんにちは、世界\nHello World!!!?", 0, 10, 100, 150, 50);
        UIH_CONTROL *edit1 = UIHAddEdit(mainState, "heck世界你好こんにちは、世界feck", 0, 5, 5, 325, 80);

        //(state, "基佬", 0, 100, 90, 125, 25, NULL, edit1);
        //UIHAddButton(state, "poop", 0, 100, 120, 125, 25, &callbackTest, NULL);

        UIH_CALLBACK tmpCallbackStruct;
        tmpCallbackStruct.datums[0] = edit1;
        UIHRegisterMenuCallback(mainState, &onMenuCallback, &tmpCallbackStruct);

        MakeTextEditorMenu(mainState);
        UIHShowWindow(mainState, 1);
        UIHErr();

        mainState->isRunning = 1;
        while(mainState->isRunning) {
            Sleep(1);
            UIHEventUpdate();
        }

    }
    else {
        UIHDisplayError("Feckin' memory didn't allocate yo", __FUNCTION__, 1, "Error");
        return -1;
    }

    //UIClean();
    return 0;
}
