#pragma once

 #include <stdbool.h>
 #include <stdint.h>
 #include <minix/driver.h>
 #include <sys/mman.h>
 #include <lcom/lcf.h>


#define ESC_BEAKCODE 0x81


void *(init)(uint16_t mode, vbe_mode_info_t *info);

int R_first(int first, vbe_mode_info_t *info);
int G_first(int first, vbe_mode_info_t *info);
int B_first(int first, vbe_mode_info_t *info);

void (vg_pattern)(vbe_mode_info_t *info, uint16_t mode,  uint8_t no_rectangles, int first, uint8_t step);

void draw_xpm(xpm_image_t img, uint16_t x, uint16_t y);

void erase_xpm(xpm_image_t img, uint16_t x, uint16_t y);

void move_xpm(xpm_image_t img, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate);
