#include "keyboard.h"

static uint8_t scancode_no_bytes;
static uint8_t scancode_bytes[2];
static uint8_t valid_scancode = 0;
static uint8_t character;
uint8_t scancode;
uint8_t is_scancode_complete = 1;

#define DELAY_US 20000


int get_scancode(){
  //This makes the call to the system returning to character the scancode, if it returns 1 then it failed so I signal that by making valid_scancode 1.
    if (util_sys_inb(STAT_REG, &character)) {
      valid_scancode = 0;
      return 1;
    }

    if (!(character & ST_OUT_BUF)) {
        valid_scancode = 0;
        return 1;
    }

    if (util_sys_inb(OUT_BUF, &scancode)) {
        valid_scancode = 0;
        return 1;
    }

    if (character & (KBC_PAR_ERR | KBC_TO_ERR | KBC_MOUSE_DATA)) {
        valid_scancode = 0;
        return 1;
    }

    valid_scancode = 1;
    return 0;
}

void analyse_scancode() {
    if (valid_scancode) {
        if (is_scancode_complete) {
            scancode_bytes[0] = scancode;
            if (scancode == SCANCODE_TWO_BYTES) {
                is_scancode_complete = 0;
                scancode_no_bytes = 2;
            }
            else {
                is_scancode_complete = 1;
                scancode_no_bytes = 1;
            }
        }
        else {
            scancode_bytes[1] = scancode;
            is_scancode_complete = 1;
        }
    }
}


int kbc_send_cmd(uint8_t port, uint8_t cmd) {
  uint8_t timeouts = TIMEOUT_ATTEMPTS;

  while(timeouts) {
    if (util_sys_inb(STAT_REG, &character)) {
      return 1;
    }

    if (character & ST_IN_BUF) {
      --timeouts;
      if (tickdelay(micros_to_ticks(KBC_WAIT))) {
        return 1;
      }
      continue;
    }
    else {
      break;
    }
  }

  if (timeouts == 0) return 1;

  if (sys_outb(port, cmd)) return 1;

  return 0;
}

int kbc_read_outbf(uint8_t port, uint8_t *content) {
  uint8_t timeouts = TIMEOUT_ATTEMPTS;

  while (timeouts) {
    if (util_sys_inb(STAT_REG, &character)) {
      return 1;
    }

    if (character & ST_OUT_BUF) {
      break;
    }
    else {
      --timeouts;
      if(tickdelay(micros_to_ticks(KBC_WAIT))) {
        return 1;
      }
      continue;
    }
  }

  if (timeouts == 0) {
    return 1;
  }

  if (util_sys_inb(OUT_BUF, content)) {
    return 1;
  }

  return 0;
}


int kbc_reenable_default_int() {
  if (kbc_send_cmd(IN_BUF_CMD, READ_COMMAND_BYTE)) {
    return 1;
  }

  uint8_t cmd;
  if(kbc_read_outbf(OUT_BUF, &cmd)) {
    return 1;
  }

  cmd |= CMD_BYTE_ENABLE_INT_KBD;

  if (kbc_send_cmd(IN_BUF_CMD, WRITE_COMMAND_BYTE)) {
    return 1;
  }

  if (kbc_send_cmd(IN_BUF_ARGS, cmd)) {
    return 1;
  }

  return 0;
}


int kbc_print_scancode_wrapper() {
    if (kbd_print_scancode(scancode < MAKE_TO_BREAK, scancode_no_bytes, scancode_bytes))
        return 1;


    return 0;
}
