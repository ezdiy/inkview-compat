#include <ucontext.h>
#include "inkview.h"

#define COMPAT __attribute__((weak))

static ucontext_t inner, outer;
static char stack[128 * 1024];
static iv_handler handler;
static int init, exiting;
static iv_mtinfo *gtcache;

static int inner_handler(int typ, int par1, int par2) {
    if (!init) {
        init = 1;
        swapcontext(&inner, &outer);
    }
    int ret = handler(typ, par1, par2);
    if (exiting)
        return ret;
    swapcontext(&inner, &outer);
    return ret;
}

COMPAT void PrepareForLoop(iv_handler h) {
    exiting = init = 0;
    handler = h;
    getcontext(&inner);
    inner.uc_stack.ss_sp = stack;
    inner.uc_stack.ss_size = sizeof(stack);
    inner.uc_link = &outer;
    makecontext(&inner, (void (*)(void)) &InkViewMain, 1, inner_handler);
    swapcontext(&outer, &inner);
}

COMPAT void ProcessEventLoop() {
    swapcontext(&outer, &inner);
}

COMPAT void ClearOnExit() {
    exiting = 1;
    LeaveInkViewMain();
}

COMPAT iv_mtinfo *GetTouchInfoI(unsigned int i) {
    if (!i) gtcache = GetTouchInfo();
    return gtcache + i;
}

