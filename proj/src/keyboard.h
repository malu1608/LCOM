#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"

#include <stdbool.h>
#include <stdint.h>

extern uint8_t scancode;
extern uint8_t is_scancode_complete;

/** @defgroup Keyboard Keyboard
 * @{
 *
 * Functions for the using the keyboard.
 */

/**
 * @brief Gets the scancode from the keyboard out buffer. Does not parse it.
 * 
 */
int (get_scancode)();

/**
 * @brief Parses the scancode if it is ready to.
 * 
 */
void (analyse_scancode)();

/**
 * @brief Prints the scancode that completed reading. Returns 0 if sucessful.
 * 
 * @return int 
 */
int kbc_print_scancode_wrapper();

