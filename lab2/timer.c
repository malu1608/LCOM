#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int hook_id;
extern int counter_tic;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  //printf("%s is not yet implemented!\n", __func__);
  if (!(timer == 0 || timer == 1 || timer == 2)) {
     return 1;
  }
  if (freq <= 0)
    return 1;
  
  uint8_t st;
  timer_get_conf(timer, &st); //ler os valores do timer

  uint8_t ctrlwrd = st & (BIT(3)|BIT(2)|BIT(1)|BIT(0));
  ctrlwrd |= (BIT(5)|BIT(4));//st & (BIT(5)|BIT(4));
  if (timer == 1) {
    ctrlwrd |= BIT(6);
  } else if (timer == 2) {
      ctrlwrd |= BIT(7);
  }

  if (sys_outb(TIMER_CTRL, ctrlwrd) != 0) {
    return 1;
  }
  
  uint16_t div =  TIMER_FREQ / freq;
  uint8_t lsb, msb;


  util_get_LSB(div, &lsb);
  sys_outb(TIMER_0 + timer, lsb);
  util_get_MSB(div, &msb);
  sys_outb(TIMER_0 + timer, msb);
  /*switch ((st & (BIT(5)|BIT(4))) >> 4) {
    case 1:
      util_get_LSB(div, &lsb);
      sys_outb(TIMER_0 + timer, lsb);
      break;
    case 2:
      util_get_MSB(div, &msb);
      sys_outb(TIMER_0 + timer, msb);
      break;
    case 3:
      util_get_LSB(div, &lsb);
      sys_outb(TIMER_0 + timer, lsb);
      util_get_MSB(div, &msb);
      sys_outb(TIMER_0 + timer, msb);
      break;
    default:
      return 1;
  }*/
  
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  /* To be implemented by the students */
  //printf("%s is not yet implemented!\n", __func__);
  hook_id = (int)*bit_no;
  sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&hook_id);
  return 0;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  //printf("%s is not yet implemented!\n", __func__);
  sys_irqrmpolicy(&hook_id);
  return 0;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  //printf("%s is not yet implemented!\n", __func__);
  counter_tic++;

}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  /* To be implemented by the students */
  uint8_t rbcm = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer)); //Read-Back Command with status configuration depending on timer
  
  if (sys_outb(TIMER_CTRL, rbcm) != 0) {
    return 1;
  }

  switch (timer) {
    case 0:
    {
      if (util_sys_inb(TIMER_0, st) != 0) {return 1;}
      break;
    }
    case 1: 
    {
      if (util_sys_inb(TIMER_1, st) != 0) {return 1;}
      break;
    }
    case 2:
    {
      if (util_sys_inb(TIMER_2, st) != 0) {return 1;}
      break;
    }
    default:
      return 1;
    }

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  /* To be implemented by the students */
  union timer_status_field_val config;

  switch (field) {

    case tsf_all:
      config.byte = st;
      break;

    case tsf_initial:
    //the & logic gets the st values in 5th and 4th bit, and all the other bits have 0 value
      config.in_mode = (st & TIMER_LSB_MSB) >> 4;
      if (config.in_mode == 1) {
        config.in_mode = LSB_only;
      }
      else if (config.in_mode == 2) {
        config.in_mode = MSB_only;
      }
      else if (config.in_mode == 3) {
        config.in_mode = MSB_after_LSB;
      } else {
        config.in_mode = INVAL_val;
      }
      break;

    case tsf_mode:

      config.count_mode = (st & (BIT(3)|BIT(2)|BIT(1))) >> 1;
      if (config.count_mode == 6) {
          config.count_mode = 2;
      } else if (config.count_mode == 7) {
          config.count_mode = 3;
      }
      break;

    case tsf_base:

      config.bcd = (st & BIT(0));
      break;
  }
  
  timer_print_config(timer, field, config);

  return 0;
}
