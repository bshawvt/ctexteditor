#include "stdio.h"

//#include "string.h"
//#include "stdlib.h"



#include "ui.h"
#include "util.h"


int main(int argc, char* args[]) {
    /*char a[] = "hello-";
    char b[] = "-world";


    \u19990\u30028\u20320\u22909
    \u12371\u12435\u12395\u12385\u12399\u12289\u19990\u30028
    世界你好\nこんにちは、世界 Hello World"


    CharConcat(a, b);*/

    wchar_t op[] = {0x19990,0x30028,0x20320, 0x22909, 0x12371, 0x12435, 0x12395, 0x12385, 0x12399, 0x12289, 0x19990, 0x30028, 0x00, 0x00};
    //世界你好\nこんにちは、世界 Hello World"};

    UIHInit();
    UIHCreateWindow(10, 10, 250, 200, "Heck");
    //UIHAddLabel(L"世界你好\nこんにちは、世界\nHello World", 10, 10, 150, 50);
    //UIHAddLabel("世界你好\nこんにちは、世界\nHello World!!!?", 10, 100, 150, 50);
    UIHAddLabel("世界你好\nこんにちは、世界\nHello World!!!?", 10, 100, 150, 50);
    UIHAddButton(L"Fek", 100, 100, 50, 25);
    UIHAddEdit("世界你好\nこんにちは、世界", 5, 5, 225, 80);
    UIHShowWindow(1);
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
