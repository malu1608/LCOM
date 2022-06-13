#include <lcom/lcf.h>
#include "video_macros.h"

#define TRANSPARENCY_COLOR 254

/** @defgroup Graphics Graphics
 * @{
 *
 * Functions for the using the graphics.
 */

/**
 * @brief A struct that holds informationg about the initialized graphics.
 * 
 */
typedef struct {
	uint16_t x_res, y_res;
	uint8_t bits_per_pixel, bytes_per_pixel, vg_mode;
	uint32_t red_mask, green_mask, blue_mask;
	uint8_t red_field_position, green_field_position, blue_field_position;
	uint8_t red_mask_size, green_mask_size, blue_mask_size;
} vbe_mode_info_summary_t;

extern vbe_mode_info_summary_t vg_info;

/**
 * @brief Initializes the graphics in mode 0x14C. Returning the video memory address.
 * 
 * @return void* 
 */
void *initialize_graphics();

/**
 * @brief Draws an horizontal line starting at position (x, y) with "len" length and "color" color. Returns an 0 if sucessful.
 * 
 * @param x 
 * @param y 
 * @param len 
 * @param color 
 * @return int 
 */
int draw_hline(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a rectangle starting at position (x, y) with "height" height, "width" width and "color" color. Returns an 0 if sucessful.
 * 
 * @param x 
 * @param y 
 * @param height 
 * @param width 
 * @param color 
 * @return int 
 */
int draw_rectangle(int16_t x, int16_t y, uint16_t height, uint16_t width, uint32_t color);

/**
 * @brief Draws a xpm on position (x, y) using the information stored in the img and map parameters.
 * 
 * @param img 
 * @param x 
 * @param y 
 * @param map 
 */
void draw_xpm(xpm_image_t img, int16_t x, int16_t y, uint8_t *map);

/**
 * @brief Similar to the draw_xpm method but draws using only the img information as the xpm has been previously loaded.
 * 
 * @param img 
 * @param x 
 * @param y 
 */
void draw_loaded_xpm(xpm_image_t img, int16_t x, int16_t y);

/**
 * @brief Frees the memory allocated to the second buffer.
 * 
 */
void (destroy_second_buffer)();
/**
 * @brief Draws a pixel with "color" color on position (x, y). 
 * 
 */
int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Copies the second buffer memory and pastes it on the main video memory.
 * 
 */
void (refresh_screen)();
