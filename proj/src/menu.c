#include "menu.h"
#include "graphics.h"

xpm_image_t title;
xpm_image_t options;

xpm_image_t youwon;
xpm_image_t winstance;

int option;

void loadMenu() {
  xpm_load((xpm_map_t) title_xpm, XPM_8_8_8_8, &title);
  xpm_load((xpm_map_t) options_xpm, XPM_8_8_8_8, &options);
}

void loadWinMenu() {
  xpm_load((xpm_map_t) youwon_xpm, XPM_8_8_8_8, &youwon);
  xpm_load((xpm_map_t) winstance_xpm, XPM_8_8_8_8, &winstance);
}

void Menu(uint16_t x_res, uint16_t y_res) {
  if (get_date()->hour < 9 || get_date()->hour > 21)
    draw_rectangle(0, 0, x_res, y_res, 0x000000);
  else {
    draw_rectangle(0, 0, x_res, y_res, 0x3b403c);
  }
  draw_loaded_xpm(title, 225, 140);
  draw_loaded_xpm(options, 380, 500);
}

void WinMenu(uint16_t x_res, uint16_t y_res) {
  draw_loaded_xpm(youwon, 95, 140);
  draw_loaded_xpm(winstance, 480, 500);
}

void checkOptions(uint16_t x, uint16_t y) {
  if (x < 380 || x > 768)
    return;
  if (x > 405 && x < 718 && y > 500 && y < 540) {
    draw_rectangle(405, 544, 313, 4, 0xECECEC);
  }
  if (x > 405 && x < 718 && y > 581 && y < 621) {
    draw_rectangle(405, 625, 313, 4, 0xECECEC);
  }
}

int clickOption(uint16_t x, uint16_t y) {
  if (x < 380 || x > 768)
    return 4;
  if (x > 405 && x < 718 && y > 500 && y < 540) {
    draw_rectangle(445, 544, 313, 4, 0xECECEC);
    return 0;
  }
  if (x > 405 && x < 718 && y > 581 && y < 621) {
    draw_rectangle(380, 585, 363, 4, 0xECECEC);
    return 1;
  }
  return 4;
}
