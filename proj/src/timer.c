#include "timer.h"


int timer_subscribe(int *hook_id) {
    sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, hook_id);
    return 0;
}

int timer_unsubscribe(int *hook_id) {
    sys_irqrmpolicy(hook_id);
    return 0;
}
