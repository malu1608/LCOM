
#include <keyboard.h>
#include "lcom/timer.h"
#include <stdbool.h>
#include <stdint.h>

//GLOBAL HooK_ID
int key_hook_id = KEYBOARD_IRQ;

extern uint8_t scancode;
extern uint8_t stat;
extern uint8_t counter;


int (keyboard_subscribe_int)(uint8_t *bit_no) {
    //inicializar com IRQ1
    * bit_no = key_hook_id;
    if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &key_hook_id) != 0) return 1;
    return 0;    //ALL OK
}

int (keyboard_unsubscribe_int)() {
    if (sys_irqrmpolicy(&key_hook_id) != 0) return 1;
    return 0;   //ALL OK
}

int keyboard_scan_stop_key(uint8_t stopkey){
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
    if(keyboard_unsubscribe_int()) return 2;
    return 0; //All OK
}

int keyboard_poll_stop_key(uint8_t stopkey) {
    //Ciclo dado no enunciado
    uint8_t cmd=0;
    while (scancode != stopkey) {
        util_sys_inb(KBC_ST_REG, &stat); /* assuming it returns OK */
        /* loop while 8042 output buffer is empty */
        if (stat & KBC_OBF) {
            //loop while 8042 output buffer is empty
            if ((stat & KBC_AUX)) continue;
            kbc_ih();
            if (scancode == EXT_MASK) {
                kbd_print_scancode(IS_MAKE_CODE, 2, &scancode);
            } else {
                kbd_print_scancode(IS_MAKE_CODE, 1, &scancode);
            }
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }
    sys_outb(KBC_OUT_BUF, KEYBOARD_ENABLE);
    sys_outb(KBC_CMD_REG,0x20);
    util_sys_inb(KBC_OUT_BUF,&cmd);
    sys_outb(KBC_CMD_REG,0x60);
    sys_outb(KBC_OUT_BUF,(cmd | 0x01));
    return 0; //All OK
}

int keyboard_timed_stop_key(uint8_t stopkey, uint8_t time){
    uint8_t irq_set_key;
    uint8_t irq_set_time;

    keyboard_subscribe_int(&irq_set_key);
    timer_subscribe_int(&irq_set_time);

    int ipc_status;
    message msg;
    int r;
    counter=0;
    uint8_t seg=0;

    //Ciclo dado no enunciado
    while((scancode != stopkey )&&(seg < time)) {
        /* Get a request message. */
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: /* hardware interrupt notification */
                    if (msg.m_notify.interrupts & BIT(irq_set_time)){
                        //Incrementa a variavel global counter em 1
                        timer_int_handler();
                        if(counter % 60 == 0){
                            seg++;
                            counter=0;
                        }
                    }
                    if (msg.m_notify.interrupts & BIT(irq_set_key)) { /* subscribed interrupt */
                        util_sys_inb(KBC_ST_REG, &stat); //assuming it returns OK
                        //loop while 8042 output buffer is empty
                        kbc_ih();
                        if(scancode == EXT_MASK) {
                            kbd_print_scancode(IS_MAKE_CODE, 2, &scancode);
                        } else{
                            kbd_print_scancode(IS_MAKE_CODE, 1, &scancode);
                        }
                        counter=0;
                        seg=0;
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
    if(keyboard_unsubscribe_int()) return 2;
    return 0; //All OK
}
