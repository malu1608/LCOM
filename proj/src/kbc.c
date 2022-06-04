#include "kbc.h"
#include "utils.h"

//GLOBAL Status Register
uint8_t stat;
uint8_t scancode = 0;

void (kbc_ih)() {
    if (stat & (KBC_PAR_ERR | KBC_TO_ERR)) return;
    util_sys_inb(KBC_OUT_BUF, &scancode);
}
