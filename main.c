#include "stdio.h"

//#include "string.h"
//#include "stdlib.h"



#include "ui.h"
#include "util.h"


void* buttonFunction() {
    printf("found the thing");
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
    UIH_STATE *state2 = UIHMakeState();
    printf("state2 pointer address in %s() = %p\n", __FUNCTION__, state2);
    UIHInit(state2);
    UIHCreateWindow(state2, "世界你好Heck", 10, 10, 250, 200);
    //UIHAddLabel(L"世界你好\nこんにちは、世界\nHello World", 10, 10, 150, 50);
    //UIHAddLabel("世界你好\nこんにちは、世界\nHello World!!!?", 10, 100, 150, 50);
    //UIHAddLabel("世界你好\nこんにちは、世界\nHello World!!!?", 100, 100, 150, 50);

    UIHAddButton(state2, "oddwarg is butts", 100, 100, 125, 25, &buttonFunction);
    //UIHAddEdit("世界你好\nこんにちは、世界", 5, 5, 225, 80);

    UIHShowWindow(state2, 1);
    UIHErr();


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
