#include "mouse_func.h"

bool mouse_ih_error = false, is_mouse_packet_complete = false;
struct packet mouse_parsed_packet;

int16_t mouse_x;
int16_t mouse_y;
bool mouse_pressed;

static uint8_t st, counter = 0;
static uint8_t packet_bytes[3];
static bool found_first_byte = false;


void (mouse_ih)() {
     if (util_sys_inb(READ_ST_REG, &st)) {
        mouse_ih_error = 1;
        uint32_t discarded_byte;
        sys_inb(DATA_PORT, &discarded_byte);
        return;        
    }

    // If either one is set to 1, there's an error
    if (st & (PARITY_ERROR | TIMEOUT_ERROR)) {
        mouse_ih_error = 1;
        uint32_t discarded_byte;
        sys_inb(DATA_PORT, &discarded_byte);
        return;
    }

    if (util_sys_inb(DATA_PORT, &packet_bytes[counter])) {
        mouse_ih_error = 1;
        return;
    }
}

uint8_t (parse_packet)() {
    // Copy raw bytes to the struct
    mouse_parsed_packet.bytes[0] = packet_bytes[0];
    mouse_parsed_packet.bytes[1] = packet_bytes[1];
    mouse_parsed_packet.bytes[2] = packet_bytes[2]; 

    // According to stdbool, for numerical types (aka uint8_t)
    // 0 gets converted to 0 (false) and any other number gets converted
    // to 1 (true), so we don't need to shift the result of the parsed data
    
    /* MOUSE BUTTONS */
    mouse_parsed_packet.lb = packet_bytes[0] & MOUSE_PARSE_LB;     
    mouse_parsed_packet.rb = packet_bytes[0] & MOUSE_PARSE_RB;
    mouse_parsed_packet.mb = packet_bytes[0] & MOUSE_PARSE_MB; 

    if (mouse_parsed_packet.lb)
        mouse_pressed = true;  

    /* MOUSE MOVEMENT DELTAS */
    // Doesn't take into account the sign
    mouse_parsed_packet.delta_x = (uint16_t) packet_bytes[1];
    mouse_parsed_packet.delta_y = (uint16_t) packet_bytes[2];


    // Fixes the sign if it was a negative number
    if (packet_bytes[0] & MOUSE_PARSE_X_DELTA_MSB)
        mouse_parsed_packet.delta_x |= EXTEND_SIGN;

    if (packet_bytes[0] & MOUSE_PARSE_Y_DELTA_MSB)
        mouse_parsed_packet.delta_y |= EXTEND_SIGN;

    mouse_parsed_packet.x_ov = packet_bytes[0] & MOUSE_PARSE_X_OVERFLOW;
    mouse_parsed_packet.y_ov = packet_bytes[0] & MOUSE_PARSE_Y_OVERFLOW;

    if (!mouse_parsed_packet.x_ov) {
        if (abs(mouse_parsed_packet.delta_x) < 50)
            mouse_x += mouse_parsed_packet.delta_x;
    }
    if (!mouse_parsed_packet.y_ov) {
        if (abs(mouse_parsed_packet.delta_y) < 50)
            mouse_y -= mouse_parsed_packet.delta_y;
    }

    if (mouse_x < 0)
        mouse_x = 0;
    if (mouse_y < 0)
        mouse_y = 0;
    if (mouse_x > 1152)
        mouse_x = 1152;
    if (mouse_y > 864)
        mouse_x = 864;

    


    return 0;
}

int mouse_data_handler() {
    if (!found_first_byte) { 
        ++counter;

        if (counter > 2) {
            is_mouse_packet_complete = true;
            
            if (parse_packet())
                return 1;

            counter = 0;
        }

        else 
            is_mouse_packet_complete = false;
        
        return 0;
    }

    else { // We need to find out the first byte
        // Assume that if that bit is 1, it's the first byte/ We need to find out the first byte
        // Assume that if that bit is 1, it's the first byte
        is_mouse_packet_complete = false;
        if (packet_bytes[counter] & MOUSE_PARSE_FIRST_PACKET_IDENTIFIER) {
            packet_bytes[0] = packet_bytes[counter];
            found_first_byte = true;
        }

        return 0;
    }
}
