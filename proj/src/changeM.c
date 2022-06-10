#include "changeM.h"

extern xpm_image_t background_menu;
//vbe_mode_info_summary_t vg_info;

changeMstate *createChange(changeType change, unsigned int x, unsigned int y){
    changeMstate *c = (changeMstate *) malloc (sizeof(changeMstate));

  c->x = x;
  c->y = y;
  c->mouseOver = false;
  c->change = change;

  switch (change) {
    case EXIT:
      xpm_load(Exit_xpm, XPM_8_8_8_8, &c->img);
      xpm_load(ExitClicked_xpm, XPM_8_8_8_8, &c->highlightedImg);
      break;
    case SINGLEPLAYER:
      xpm_load(Single_Player_xpm, XPM_8_8_8_8, &c->img);
      xpm_load(Single_PlayerClicked_xpm, XPM_8_8_8_8, &c->highlightedImg);
      break;
    case HELP:
      xpm_load(HelpButton_xpm, XPM_8_8_8_8, &c->img);
      xpm_load(HelpClicked_xpm, XPM_8_8_8_8, &c->highlightedImg);
      break;
  }

  return c;
}

void add_change_to_background(changeMstate *c){
  printf("%x\n",c);
    uint32_t * map, * cmap;

  map = (uint32_t *)background_menu.bytes;

  if (!c->mouseOver) {
    cmap = (uint32_t *)c->img.bytes;

    for(int i = 0; i < c->img.width; i++) {
      for (int j = 0; j < c->img.height; j++) {
        if (*(cmap + i + j*c->img.width) != xpm_transparency_color(XPM_8_8_8_8))
          *(map + (c->x + i) + (c->y + j) * vg_info.x_res) = *(cmap + i + j*c->img.width);
      }
    }
  }
  else {
    cmap = (uint32_t *)c->highlightedImg.bytes;

    for(int i = 0; i < c->highlightedImg.width; i++) {
      for (int j = 0; j < c->highlightedImg.height; j++) {
        if (*(cmap + i + j*c->highlightedImg.width) != xpm_transparency_color(XPM_8_8_8_8))
          *(map + (c->x + i) + (c->y + j) * vg_info.x_res) = *(cmap + i + j*c->img.width);
      }
    }
  }
}
