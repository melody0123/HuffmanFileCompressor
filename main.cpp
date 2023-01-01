#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "compression.h"

using namespace std;

int main()
{
    HuffCompression program;
    int sel = 0;
    while(true)
    {
        printf("1：压缩\n2：解压\n3：退出\n请输入希望进行的操作：");
        scanf("%d", &sel);
        if(sel == 1)
            program.compress();
        else if(sel == 2)
            program.decompress();
        else if(sel == 3)
            return 0;
        else
            printf("非法的输入！\n");
    }
    return 0;
}
