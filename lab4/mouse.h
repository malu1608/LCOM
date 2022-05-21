#ifndef MOUSE_H
#define MOUSE_H

#include <stdbool.h>
#include <stdint.h>
#include "kbc.h"
#include "constants.h"

int(mouse_subscribe_int)(uint8_t *bit_no);
int(mouse_unsubscribe_int)();
int(keyboard_scan_stop_key)( uint8_t stopkey);
int(keyboard_poll_stop_key)( uint8_t stopkey);
int(keyboard_timed_stop_key)( uint8_t stopkey, uint8_t time);
#endif //KEYBOARD_H