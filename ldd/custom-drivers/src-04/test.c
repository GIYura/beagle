#include <stdio.h>

struct ST
{
    int a;
    int b;
    int c;
    int d;
};

struct ST data;

#define CONTAINER_OF(ptr, type, member) ((type*)( (char*)ptr - (char*)&((type*)0)->member))

int main(int argc, char* argv[])
{
    data.a = 10;
    data.b = 20;
    data.c = 30;
    data.d = 40;

    struct ST* data1 = CONTAINER_OF(&data.b, struct ST, b);

    printf("a = %d\n", data1->a);
    printf("b = %d\n", data1->b);
    printf("c = %d\n", data1->c);
    printf("d = %d\n", data1->d);

    return 0;
}

