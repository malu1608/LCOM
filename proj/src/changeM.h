#pragma once

#include <lcom/lcf.h>
#include "graphics.h"

#include "img/exit.xpm"
#include "img/exit_c.xpm"
#include "img/sp.xpm"
#include "img/sp_click.xpm"
#include "img/help_c.xpm"
#include "img/help_click.xpm"

typedef enum {EXIT,SINGLEPLAYER, HELP} changeType;

/** @struct Button
 *  @brief Struct relativa a um Botão
 *  @var Button::x
 *  Posição dos x do Botão
 *  @var Button::y
 *  Posição dos y do Botão
 *  @var Button::buttonImg
 *  XPM do Botão Normal (Cursor Fora dele)
 *  @var Button::highlightedImg
 *  XPM do Botão Destacado (Cursor Sobre ele)
 *  @var Button::buttonType
 *  Tipo de Botão
 *  @var Button::mouseOver
 *  Boleano: 1 se o Cursor estiver sobre o Botão e 0 caso contrário
 */
typedef struct {
  unsigned int x, y;

  xpm_image_t img; //cursor normal
  xpm_image_t highlightedImg; //cursor em cima

  changeType change;

  bool mouseOver;

} changeMstate;


changeMstate *createChange(changeType change, unsigned int x, unsigned int y);

void add_change_to_background(changeMstate *c);
