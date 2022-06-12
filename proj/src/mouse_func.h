#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "mouse_macros.h"
#include "utils.h"


extern bool mouse_ih_error, is_mouse_packet_complete;
extern struct packet mouse_parsed_packet;

extern int16_t mouse_x;
extern int16_t mouse_y;

void (mouse_ih)();

int (mouse_data_handler)();

uint8_t (parse_packet)();
