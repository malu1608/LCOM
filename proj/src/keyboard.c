#include "keyboard.h"

int(keyboard_subscribe_int)(uint8_t *bit_no){
    * bit_no = key_hook_id;
    if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &key_hook_id) != 0) return 1;
    return 0; 
}
int(keyboard_unsubscribe_int)(){
    if (sys_irqrmpolicy(&key_hook_id) != 0) return 1;
    return 0;
}