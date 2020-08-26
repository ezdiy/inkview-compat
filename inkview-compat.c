#include <ucontext.h>
#include "inkview.h"

#define COMPAT __attribute__((weak))
#define TICK_MS 20

static ucontext_t inner, outer;
static char stack[128 * 1024];
static iv_handler handler;
static int exiting;
static iv_mtinfo *gtcache;
static int abc[3];

static void inner_timer() {
    SetWeakTimer("compat-poll", inner_timer, TICK_MS); // re-arm
    abc[0] = -1; // signals expiry
    swapcontext(&inner, &outer);
}

static int inner_handler(int a, int b, int c) {
    if (a == EVT_INIT)
        SetWeakTimer("compat-poll", inner_timer, TICK_MS);
    abc[0] = a;
    abc[1] = b;
    abc[2] = c;
    if (a != EVT_EXIT)
        swapcontext(&inner, &outer);
    return 0;
}

COMPAT void PrepareForLoop(iv_handler h) {
    exiting = 0;
    handler = h;
    getcontext(&inner);
    inner.uc_stack.ss_sp = stack;
    inner.uc_stack.ss_size = sizeof(stack);
    inner.uc_link = &outer;
    makecontext(&inner, (void (*)(void)) &InkViewMain, 1, inner_handler);
}

COMPAT void ProcessEventLoop() {
    swapcontext(&outer, &inner);
    if (abc[0] != -1)
        handler(abc[0], abc[1], abc[2]);
}

COMPAT void ClearOnExit() {
    abc[0] = -1;
    ClearTimer(inner_timer);
    LeaveInkViewMain();
    if (abc[0] != -1)
        handler(abc[0], abc[1], abc[2]);
}

COMPAT iv_mtinfo *GetTouchInfoI(unsigned int i) {
    if (!i) gtcache = GetTouchInfo();
    return gtcache + i;
}

