 
#pragma once

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "i8254.h"
#include "rtc.h"
#include "img/title.xpm"
#include "img/options.xpm"
#include "img/youwon.xpm"
#include "img/winstance.xpm"

void loadMenu();

void loadWinMenu();

void Menu(uint16_t x_res, uint16_t y_res);

void WinMenu(uint16_t x_res, uint16_t y_res);

void checkOptions(uint16_t x, uint16_t y);

int clickOption(uint16_t x, uint16_t y);
