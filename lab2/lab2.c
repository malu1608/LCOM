#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>

int counter_tic = 0;

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
  /* To be implemented by the students */
  //printf("%s is not yet implemented!\n", __func__);
  uint8_t st;
  timer_get_conf(timer, &st);
  timer_display_conf(timer, st, field);
  return 0;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  //printf("%s is not yet implemented!\n", __func__);
  timer_set_frequency(timer, freq);
  return 0;
}

int(timer_test_int)(uint8_t time) {
  /* To be implemented by the students */
  //printf("%s is not yet implemented!\n", __func__);
  int ipc_status;
  message msg;
  uint8_t irq_set = 0;
  
  timer_subscribe_int(&irq_set);

  int counter_sec = 0;

  while(counter_sec != time) { /* You may want to use a different condition */
    /* Get a request message. */
    int r=0;
    if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & (BIT(irq_set))) { /* subscri14: ... */
          /* process it */
            timer_int_handler();
            if (counter_tic % 60 == 0) {
              counter_sec++;
              timer_print_elapsed_time();  
            }
          }
          break;
        default:
          break; /* no other notifications expected: do nothi19: */
      }
    } else { 
      /* received a standard message, not a notification21: */
      /* no standard messages expected: do nothing */
    }
  }

  timer_unsubscribe_int();

  return 0;
}
