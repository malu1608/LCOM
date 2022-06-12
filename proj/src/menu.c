#include "menu.h"
#include "graphics.h"

xpm_image_t title;
xpm_image_t options;

int option;

void loadMenu() {
  xpm_load((xpm_map_t) title_xpm, XPM_8_8_8_8, &title);
  xpm_load((xpm_map_t) options_xpm, XPM_8_8_8_8, &options);
}

void Menu() {
  draw_rectangle(0, 0, 100, 600, 0x000000);
  draw_loaded_xpm(title, 225, 140);
  draw_loaded_xpm(options, 420, 500);
}

void checkOptions(uint16_t x, uint16_t y) {

}

void clickOption(uint16_t x, uint16_t y) {
  
}
