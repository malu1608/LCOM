 
#pragma once

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "i8254.h"
#include "img/title.xpm"
#include "img/options.xpm"

void loadMenu();

void Menu();

void checkOptions(uint16_t x, uint16_t y);

void clickOption(uint16_t x, uint16_t y);
