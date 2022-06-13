#include "menu.h"
#include "graphics.h"

xpm_image_t title;
xpm_image_t options;

int option;

void loadMenu() {
  xpm_load((xpm_map_t) title_xpm, XPM_8_8_8_8, &title);
  xpm_load((xpm_map_t) options_xpm, XPM_8_8_8_8, &options);
}

void Menu(uint16_t x_res, uint16_t y_res) {
  draw_rectangle(0, 0, x_res, y_res, 0x000000);
  draw_loaded_xpm(title, 225, 140);
  draw_loaded_xpm(options, 420, 500);
}

void checkOptions(uint16_t x, uint16_t y) {
  if (x < 420 || x > 808)
    return;
  if (x > 445 && x < 758 && y > 500 && y < 540) {
    draw_rectangle(445, 544, 313, 4, 0xECECEC);
  }
  if (x > 445 && x < 758 && y > 581 && y < 621) {
    draw_rectangle(445, 625, 313, 4, 0xECECEC);
  }
}

int clickOption(uint16_t x, uint16_t y) {
  if (x < 420 || x > 808)
    return 4;
  if (x > 445 && x < 758 && y > 500 && y < 540) {
    draw_rectangle(445, 544, 313, 4, 0xECECEC);
    return 0;
  }
  if (x > 445 && x < 758 && y > 581 && y < 621) {
    draw_rectangle(420, 585, 363, 4, 0xECECEC);
    return 1;
  }
  return 4;
}
