#include <mouse.h>

int key_hook_id = MOUSE_IRQ;


int (mouse_subscribe_int)(uint8_t *bit_no){
    *bit_no = key_hook_id;
    if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &key_hook_id)) return 1;
    return 0;
}

int (mouse_unsubscribe_int)() {
    if (sys_irqrmpolicy(&key_hook_id) != 0) return 1;
    return 0;   //ALL OK
}

int mouse_scan_stop_key(uint8_t stopkey){
    uint8_t irq_set;
    keyboard_subscribe_int(&irq_set);
    int ipc_status;
    message msg;
    int r;

    //Ciclo dado no enunciado
    while(scancode != stopkey ) {
    /* Get a request message. */
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: /* hardware interrupt notification */
                    if (msg.m_notify.interrupts &BIT(irq_set)) { /* subscribed interrupt */
                        util_sys_inb(KBC_ST_REG, &stat); //assuming it returns OK
                        //loop while 8042 output buffer is empty
                        kbc_ih();
                        #ifdef LAB2
                        if(scancode == EXT_MASK) {
                            kbd_print_scancode(IS_MAKE_CODE, 2, &scancode);
                        } else{
                            kbd_print_scancode(IS_MAKE_CODE, 1, &scancode);
                        }
                        #endif // LAB2
                    }
                    break;
                default:
                    break; /* no other notifications expected: do nothing */
            }
        } else {
            /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
        }
    }
    sys_outb(KBC_OUT_BUF,0x01);
    if(mouse_unsubscribe_int()) return 2;
    return 0; //All OK
}