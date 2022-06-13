 
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

/** @defgroup Menu Menu
 * @{
 *
 * Functions for the menu.
 */

/**
 * @brief Loads every xpm needed to draw the main menu.
 * 
 */
void loadMenu();

/**
 * @brief Loads every xpm needed to draw the win condition menu.
 * 
 */
void loadWinMenu();

/**
 * @brief Draws the main menu window. Uses the x_res and y_res to draw the whole window with the desired background color.
 * 
 * @param x_res 
 * @param y_res 
 */
void Menu(uint16_t x_res, uint16_t y_res);

/**
 * @brief Draws the win condition menu window.
 * 
 * @param x_res 
 * @param y_res 
 */
void WinMenu(uint16_t x_res, uint16_t y_res);

/**
 * @brief Checks if the mouse on position (x, y) is overlapping a menu option, underlining it if so.
 * 
 * @param x 
 * @param y 
 */
void checkOptions(uint16_t x, uint16_t y);

/**
 * @brief Checks if the mouse on position (x, y) is overlapping a menu option when the left button pressed, returning an integer with the options selected, 4 if no valid option was pressed.
 * 
 * @param x 
 * @param y 
 * @return int 
 */
int clickOption(uint16_t x, uint16_t y);
