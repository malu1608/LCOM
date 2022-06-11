 
#pragma once

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "i8254.h"
#include "gamemanager.h"
#include "img/help.xpm"
#include "img/background.xpm"
#include "img/o_s.xpm"
#include "changeM.h"

void load_background();

/**
 *  @brief Apresenta no ecrã o Fundo do Nível
 */
void draw_menu_background();

/**
 *  @brief Apresenta no ecrã o Fundo do Menu Help
 */
void draw_help_background();

/**
 *  @brief Remove do ecrã o Fundo do Menu Help
 */
void clean_help_background();

void add_to_background(xpm_image_t * img, int x, int y);

void Menu();
