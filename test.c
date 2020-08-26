#include "inkview.h"

int killme;
int test(int typ, int par1, int par2)
{
    printf("typ=%d par1=%d par2=%d\n",typ,par1,par2);
    if (typ == 151)
        killme = 1;
    return 0;
}

int main()
{
    printf("try prepare\n");
    PrepareForLoop(&test);
    printf("bloop\n");
    while (!killme) {
        printf("round trip\n");
        ProcessEventLoop();
    }
    printf("exiting, waiting for clear?\n");
    ClearOnExit();
}

