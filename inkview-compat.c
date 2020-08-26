#include <ucontext.h>
#include "inkview.h"

#define COMPAT __attribute__((weak))

static ucontext_t inner, outer;
static char stack[128 * 1024];
static iv_handler handler;
static int init, exiting;
static iv_mtinfo *gtcache;
static int abc[3];

static int inner_handler(int a, int b, int c) {
    abc[0] = a;
    abc[1] = b;
    abc[2] = c;
    if (!exiting)
        swapcontext(&inner, &outer);
    return 0;
}

COMPAT void PrepareForLoop(iv_handler h) {
    exiting = init = 0;
    handler = h;
    getcontext(&inner);
    inner.uc_stack.ss_sp = stack;
    inner.uc_stack.ss_size = sizeof(stack);
    inner.uc_link = &outer;
    makecontext(&inner, (void (*)(void)) &InkViewMain, 1, inner_handler);
}

COMPAT void ProcessEventLoop() {
    swapcontext(&outer, &inner);
    handler(abc[0], abc[1], abc[2]);
}

COMPAT void ClearOnExit() {
    exiting = 1;
    abc[0] = -1;
    LeaveInkViewMain();
    if (abc[0] != -1)
        handler(abc[0], abc[1], abc[2]);
}

COMPAT iv_mtinfo *GetTouchInfoI(unsigned int i) {
    if (!i) gtcache = GetTouchInfo();
    return gtcache + i;
}

