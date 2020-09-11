#include "stdio.h"

//#include "string.h"
//#include "stdlib.h"



#include "ui.h"
#include "util.h"


void buttonFunction(UIH_STATE *state, void *data) {
    printf("button activate fnCallback state address = %p\n", state);
    printf("button activate fnCallback data address = %p\n", data);

    UIH_CONTROL *control = (UIH_CONTROL*) data;

    //printf("control->text = %s\n", control->text);
    //MessageBoxW(state->hwnd, control->text, L"THING", 0);
    UIHGetString(control);


}

int main(int argc, char* args[]) {
    /*char a[] = "hello-";
    char b[] = "-world";


    \u19990\u30028\u20320\u22909
    \u12371\u12435\u12395\u12385\u12399\u12289\u19990\u30028
    世界你好\nこんにちは、世界 Hello World"


    CharConcat(a, b);*/

    //wchar_t op[] = {0x19990,0x30028,0x20320, 0x22909, 0x12371, 0x12435, 0x12395, 0x12385, 0x12399, 0x12289, 0x19990, 0x30028, 0x00, 0x00};
    //世界你好\nこんにちは、世界 Hello World"};

    /*UIH_STATE *state = UIHMakeState();
    printf("state pointer address in %s() = %p\n", __FUNCTION__, state);
    UIHInit(state);
    UIHCreateWindow(state, "世界你好Heck", 10, 10, 250, 200);
    //UIHAddLabel(L"世界你好\nこんにちは、世界\nHello World", 10, 10, 150, 50);
    //UIHAddLabel("世界你好\nこんにちは、世界\nHello World!!!?", 10, 100, 150, 50);
    //UIHAddLabel("世界你好\nこんにちは、世界\nHello World!!!?", 100, 100, 150, 50);

    UIHAddButton(state, "世界你好Fek", 100, 100, 75, 25, &buttonFunction);
    //UIHAddEdit("世界你好\nこんにちは、世界", 5, 5, 225, 80);

    UIHShowWindow(state, 1);*/

    // second window i guess idk
    UIH_STATE *state1 = UIHMakeState();
    if (state1 != NULL) {
        printf("state pointer address in %s() = %p\n", __FUNCTION__, state1);
        UIHInit(state1);//(UIH_STATE*)0x0);
        UIHCreateWindow(state1, "世界你好Heck", 10, 10, 250, 200);
        //UIHAddLabel(L"世界你好\nこんにちは、世界\nHello World", 10, 10, 150, 50);
        UIH_CONTROL *label1 = UIHAddLabel(state1, "世界你好\nこんにちは、世界\nHello World!!!?", 0, 10, 100, 150, 50);
        UIH_CONTROL *edit1 = UIHAddEdit(state1, "世界你好\nこんにちは、世界", 0, 5, 5, 225, 80);
        //UIHAddLabel("世界你好\nこんにちは、世界\nHello World!!!?", 100, 100, 150, 50);

        UIHAddButton(state1, "基佬", 0, 100, 100, 125, 25, &buttonFunction, edit1);
        //UIHAddButton(state1, "no callback", 0, 100, 70, 125, 25, NULL, NULL);


        UIHShowWindow(state1, 1);
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
