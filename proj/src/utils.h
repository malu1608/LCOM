#include <lcom/lcf.h>
#include <stdint.h>

/** @defgroup Utils Utils
 * @{
 *
 * Functions for the utils.
 */

/**
 * @brief Gives the "lsb" value the val's least significant two bytes.
 * 
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb);

/**
 * @brief Gives the "msb" value the val's most significant two bytes.
 * 
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb);

/**
 * @brief Gives the "value" variable the value obtained through the sys_inb call to the "port" in uint8_t.
 * 
 */
int(util_sys_inb)(int port, uint8_t *value);
