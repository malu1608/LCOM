#include <lcom/lcf.h>
#include "video_macros.h"



typedef struct {
	uint16_t x_res, y_res;
	uint8_t bits_per_pixel, bytes_per_pixel, vg_mode;
	uint32_t red_mask, green_mask, blue_mask;
	uint8_t red_field_position, green_field_position, blue_field_position;
	uint8_t red_mask_size, green_mask_size, blue_mask_size;
} vbe_mode_info_summary_t;

extern vbe_mode_info_summary_t vg_info;

void *initialize_graphics();

int draw_pixel(uint16_t x, uint16_t y, uint32_t color);

int draw_hline(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int draw_rectangle(uint16_t x, uint16_t y, uint16_t height, uint16_t width, uint32_t color);

void draw_xpm(xpm_image_t img, uint16_t x, uint16_t y, uint8_t *map);

void erase_xpm(xpm_image_t img, uint16_t x, uint16_t y);


void graphics_handling();
