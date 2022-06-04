#include <lcom/lcf.h>
#include <lcom/timer.h>
#include "util.h"

#include <stdint.h>

#include "i8254.h"

//GLOBAL HooK_ID
int hook_id = TIMER0_IRQ;
//GLOBAL Counter
int counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
    //Variavel da command word
    uint8_t cmd=0;
    //Variavel de status para utilizar no REED_BACK
    uint8_t  st;
    //Variavel para guardar o enfdereço do timer
    int port;
    // Set de st utilizando a chamada de reed_back command
    if (timer_get_conf(timer, &st)) return 1;
    //Case para verificar qual o timer indicado e atribuir à
    // comand word e ao port o valor correspondente
    switch(timer) {
        case 0: {
            cmd |= TIMER_SEL0;
            port = TIMER_0;
            break;
        }
        case 1: {
            cmd |= TIMER_SEL1;
            port = TIMER_1;
            break;
        }
        case 2: {
            cmd |= TIMER_SEL2;
            port = TIMER_2;
            break;
        }
        default: return 2;
    }
    // Formar a command word com o LSB antes de MSB
    cmd |= (TIMER_LSB_MSB);
    cmd |= (st & (TIMER_M_MASK | TIMER_BCD));

    //
    if(sys_outb(TIMER_CTRL, (uint32_t) cmd)) return 3;

    //Calculo da nova frequencia
    int n_freq =  TIMER_FREQ / freq;

    uint8_t  lsb;
    uint8_t  msb;
    //Obter o LSB depois de aplicar a nova frequencia
    util_get_LSB(n_freq, &lsb);
    //Obter o MSB depois de aplicar a nova frequencia
    util_get_MSB(n_freq, &msb);
    //Enviar os novos LSB e MSB
    sys_outb(port, lsb);
    sys_outb(port, msb);

    return 0;   //ALL OK
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    //inicializar com IRQ0
    * bit_no = hook_id;

    if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != 0) return 1;
    return 0;    //ALL OK
}

int (timer_unsubscribe_int)() {

    if (sys_irqrmpolicy(&hook_id) != 0) return 1;
    return 0;   //ALL OK
}

void (timer_int_handler)() {
    //Incrementar a variavel global counter
    counter++;
}


//!
int (timer_get_conf)(uint8_t timer, uint8_t *st) {
    uint8_t cmd=0;
    int port;

    cmd |= TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

    if(sys_outb(TIMER_CTRL, (uint32_t) cmd)) return 1;
    // Switch timer
    switch (timer) {
        case 0 : {port = TIMER_0; break;} //00
        case 1 : {port = TIMER_1; break;} //01
        case 2 : {port = TIMER_2; break;} //11
        default: return 2;
    }
    if(util_sys_inb(port, st)) return 3;
    return 0;   //ALL OK
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
    // Declaration of conf CLassGuide 7.1 info at end
    union timer_status_field_val conf;
    uint8_t in_mode;

    //  Case to check field type CLassGuide 7.1 info at begin
    // Defined at timer.h
     switch(field){
        case tsf_all: {
            conf.byte = st;
            break;
        }
        case tsf_initial: {
            in_mode = (st & TIMER_IN_MASK) >> TIMER_POS_IN_MASK;
            switch (in_mode) {
                case 0 : { conf.in_mode = INVAL_val;
                    break;}
                case 1 : { conf.in_mode = LSB_only;
                    break;}
                case 2 : { conf.in_mode = MSB_only;
                    break;}
                case 3 : { conf.in_mode = MSB_after_LSB;
                    break;}
                default: return 2;      // If ERROR exit code 2
            }
            break;
        }
        case tsf_mode: {
            conf.count_mode = (st & TIMER_M_MASK) >> TIMER_POS_M_MASK;
            // verificação do valor 110 ou 111 que deve ser igual a 010 ou 011 dado que x10 ou x11 para count mode 2 ou 3
            if(conf.count_mode == 6 || conf.count_mode == 7 ) conf.count_mode -= 4;
            break;
        }
        case tsf_base: {
            conf.byte = st & TIMER_BCD;
            break;
        }
        default: return 1;      // If ERROR exit code 1
    }
    if(timer_print_config(timer, field, conf)) return 3;       // If ERROR exit code 3
    return 0;   //ALL OK
}

