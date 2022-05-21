#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <keyboard.h>
#include <i8042.h>

#include "lcom/utils.h"


#include <stdbool.h>
#include <stdint.h>


extern uint8_t keyboard_counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
    //Read keyboard until ESC key
    if(keyboard_scan_stop_key(ESC_BEAKCODE)) return 1;
    //print sys_inb count
    if(kbd_print_no_sysinb(keyboard_counter)) return 2;
    return 0;   //ALL OK
}

int(kbd_test_poll)() {
    if(keyboard_poll_stop_key(ESC_BEAKCODE)) return 1;
    //print sys_inb count
    if(kbd_print_no_sysinb(keyboard_counter)) return 2;
    return 0;   //ALL OK
}

int(kbd_test_timed_scan)(uint8_t n) {
    if(keyboard_timed_stop_key(ESC_BEAKCODE, n)) return 1;
    //print sys_inb count
    if(kbd_print_no_sysinb(keyboard_counter)) return 2;
    return 0;   //ALL OK
}

