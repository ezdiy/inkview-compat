#include <time.h>
#include "inkview.h"

int killme, isold;
int test(int typ, int par1, int par2)
{
    printf("typ=%d par1=%d par2=%d\n",typ,par1,par2);
    if (typ == EVT_POINTERUP) { // tap
        killme = 1;
        if (isold) 
            LeaveInkViewMain();
    }
    return 0;
}

int main(int argc, char **argv)
{
    if ((argc == 2) && (!strcmp(argv[1], "old"))) {
        printf("using InkViewMain()\n");
        isold = 1;
        InkViewMain(test);
        return;
    }
    printf("using poll mode\n");
    PrepareForLoop(&test);
    int tick = 0;
    while (!killme) {
        ProcessEventLoop();
        // 5 * 50Hz
        tick++;
        if ((tick % (50 * 5)) == 0)
            printf("5 sec tick, t=%d\n", time(NULL));
    }
    ClearOnExit();
}

