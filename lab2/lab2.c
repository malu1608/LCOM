#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>

extern int counter;


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
    uint8_t st;
    if(timer_get_conf(timer, &st)) return 1; //ERROR handller 1
    if(timer_display_conf(timer, st, field)) return 2; //ERROR handler 2
    return 0;   //ALL OK
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
    if(timer_set_frequency(timer,freq)) return 1;
    return 0;   //ALL OK
}


int(timer_test_int)(uint8_t time) {
    uint8_t irq_set;
    //faz set do
    timer_subscribe_int(&irq_set);

    // Variaveis do excerto de código dado pelo enunciado
    int ipc_status;
    message msg;
    int r;

    //Ciclo dado no enunciado
    while( counter < time * 60 ) {
        /* Get a request message. */
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: /* hardware interrupt notification */
                    if (msg.m_notify.interrupts & BIT(irq_set)) { /* subscribed interrupt */
                        //Incrementa a variavel global counter em 1
                        timer_int_handler();
                        //Quando passar 1 segundo isto é 60hz faz a função timer_print_elapsed_time()
                        if(counter % 60 == 0){
                            timer_print_elapsed_time();
                        }
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
    if (timer_unsubscribe_int())  return 1;
    return 0;   //ALL OK
}
