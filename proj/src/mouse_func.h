#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "mouse_macros.h"
#include "utils.h"


extern bool mouse_ih_error, is_mouse_packet_complete;
extern struct packet mouse_parsed_packet;

extern int16_t mouse_x;
extern int16_t mouse_y;
extern bool mouse_pressed;

/** @defgroup Mouse Mouse
 * @{
 *
 * Functions for the using the mouse.
 */

/**
 * @brief Handles the mouse's interrupts.
 * 
 */
void (mouse_ih)();

/**
 * @brief Handles the data received by the mouse's interrupt.
 * 
 */
int (mouse_data_handler)();

/**
 * @brief Parses the packets received by the mouse's interrupt.
 * 
 */
uint8_t (parse_packet)();
