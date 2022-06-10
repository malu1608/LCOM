#include "menu.h"

extern vbe_mode_info_summary_t vg_info;
xpm_image_t background_menu;
xpm_image_t clean_background_menu;
//static changeMstate ** mainMenuButtons;

void load_background(){
    xpm_load(background_xpm, XPM_8_8_8_8, &background_menu);
    xpm_load(background_xpm, XPM_8_8_8_8, &clean_background_menu);
}

void draw_menu_background() {

  uint32_t *map = (uint32_t*)background_menu.bytes;

  for(unsigned int i = 0; i < background_menu.width; i++) {
    for (unsigned int j = 0; j < background_menu.height; j++) {
      //uint32_t color = map[j*background_menu.width+i];
      //printf("Drawing to: %d%d %x\n", i, j, color);
      draw_pixel(i,j,*(map + i + j*vg_info.x_res));
      //printf("Done.");
    }
  }
}

void draw_help_background() {
  xpm_image_t img;
  xpm_load(Help_xpm, XPM_8_8_8_8, &img);

  uint32_t* map = (uint32_t*) img.bytes;

  for(unsigned int i = 0; i < img.width; i++) {
    for (unsigned int j = 0; j < img.height; j++) {
      draw_pixel(i,j,*(map + i + j*vg_info.x_res));
    }
  }
}

void clean_help_background() {
  uint32_t* map = (uint32_t*) background_menu.bytes;

  for (int i = 0; i < (int)vg_info.x_res; i++) {
    for (int j = 0; j < (int)vg_info.y_res; j++) {
      if (i < (int)vg_info.x_res - 1 && j < (int)vg_info.y_res - 1)
       draw_pixel(i,j,*(map + i + j * vg_info.x_res));
    }
  }
}

void add_to_background(xpm_image_t * img, int x, int y) {
  uint32_t * map, * imgMap;

  map = (uint32_t *)background_menu.bytes;

  imgMap = (uint32_t *)img->bytes;

  for(int i = 0; i < img->width; i++) {
    for (int j = 0; j < img->height; j++) {
      if (*(imgMap + i + j*img->width) != xpm_transparency_color(XPM_8_8_8_8))
        *(map + (x + i) + (y + j) * vg_info.x_res) = *(imgMap + i + j*img->width);
    }
  }
}

void Menu(){
  
 /* load_background();
  
  mainMenuContainer = (changeMstate **) malloc(3 * sizeof(changeMstate *));

  mainMenuContainer[0] = createChange(SINGLEPLAYER,96,384);
  mainMenuContainer[2] = createChange(HELP,96,576);
  mainMenuContainer[3] = createChange(EXIT,544,576);

  static xpm_image_t logotip;
  xpm_load(OneShot_xpm, XPM_8_8_8_8, &logotip);

  add_change_to_background(mainMenuContainer[0]);
  add_change_to_background(mainMenuContainer[2]);
  add_change_to_background(mainMenuContainer[3]);
  add_to_background(&logotip,50,170);
  draw_menu_background();*/
}
