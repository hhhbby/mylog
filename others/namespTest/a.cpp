// #include "var.h"
#include <stdio.h>
__attribute__((weak)) int a = 10;

int main()
{
    printf("%d\n", a);
    return 0;
}
