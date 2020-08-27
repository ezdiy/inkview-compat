#include <sys/syscall.h>
#include <time.h>

#include "inkview.h"
#include "debug.h"

int killme, isold;

#define gettid() (int)syscall(SYS_gettid)

int test(int typ, int par1, int par2)
{
    printf("tid=%d typ=%d par1=%d par2=%d\n",gettid(),typ,par1,par2);
    if (typ == EVT_POINTERUP) { // tap
        killme = 1;
        if (isold) 
            LeaveInkViewMain();
    }
    if (typ == EVT_EXIT) {
        printf("got exit, killme\n");
        killme = 1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    printf("main tid=%d\n", gettid());
    if ((argc == 2) && (!strcmp(argv[1], "old"))) {
        printf("using InkViewMain()\n");
        isold = 1;
        InkViewMain(test);
        return 0;
    }
    printf("using poll mode\n");
    PrepareForLoop(&test);
    int tick = 0;
    while (!killme) {
        ProcessEventLoop();
        // 5 * 50Hz
        tick++;
        if ((tick % (50 * 5)) == 0)
            printf("5 sec tick, t=%d\n", (int)time(NULL));
    }
    ClearOnExit();
    return 0;
}



