#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>
#include "kbc.h"
#include "i8042.h"

int(keyboard_subscribe_int)(uint8_t *bit_no);
int(keyboard_unsubscribe_int)();
int(keyboard_scan_stop_key)( uint8_t stopkey);
int(keyboard_poll_stop_key)( uint8_t stopkey);
int(keyboard_timed_stop_key)( uint8_t stopkey, uint8_t time);
#endif //KEYBOARD_H
