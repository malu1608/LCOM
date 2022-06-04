#pragma once
#include <lcom/lcf.h>
#include "i8042.h"

#include <stdbool.h>
#include <stdint.h>

extern uint8_t scancode;
extern uint8_t is_scancode_complete;

int (get_scancode)();

void (analyse_scancode)();

int kbc_reenable_default_int();

int kbc_print_scancode_wrapper();

