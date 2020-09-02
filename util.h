#ifndef _UTILH_GUARD
#define _UTILH_GUARD

#include "string.h"
#include "stdlib.h"

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include "windows.h"
#include "mbstring.h"
#include "Commctrl.h"
#include "locale.h"

wchar_t *CharToWChar(wchar_t *text) {
    _wsetlocale(LC_ALL, "C");
    //_wsetlocale(LC_ALL, "en_US.utf8");
    int s = _mbstrlen(text);
    wchar_t *dst = malloc(sizeof(wchar_t));
    mbstowcs(dst, text, s);
    return dst;
}

void CharConcat(char *a, char *b) {
    char *p = a;
    strcat(p, b);
    printf("* a=%s *\n", p);
    printf("* a=%s *\n", b);
}

char *CharConcat2(char *a, char *b) {
    char *p = a;
    int l1 = strlen(a);
    int l2 = strlen(b);

    char *pout = malloc((l1+l2+1) * sizeof(char));

    memcpy(pout, b, l2);
    int i = l2;
    for(i = l2; i < l1+l2; i++) {
        pout[i] = *p++;
    }
    pout[i] = '\0';
    return pout;
}

void TestA(char **a, char *b, char c[], char **d[]) {
    char *p = b;
    printf("%c\n", *p++);
    printf("%c\n", *p++);
    printf("%c\n", *p++);
}

#endif // _UTILH_GUARD
