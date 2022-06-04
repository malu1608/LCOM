#ifndef I8042_H
#define I8042_H

#include <lcom/lcf.h>
#define DELAY_US 20000

//Keyboard IRQ Number
#define KEYBOARD_IRQ 1 /**< @brief Keyboard IRQ line */
#define KEYBOARD_ENABLE 0x01
//KBC Ports
#define KBC_OUT_BUF 0x60
#define KBC_ST_REG  0x64
#define KBC_CMD_REG 0x64

//STATUS REGISTER BITs
#define KBC_OBF BIT(0)
#define KBC_IBF BIT(1)
#define KBC_AUX BIT(5)
#define KBC_TO_ERR BIT(6)
#define KBC_PAR_ERR BIT(7)


//Keyboard KEYs
#define ESC_BEAKCODE 0x81

#define IS_MAKE_CODE !(scancode & BIT(7))

//Extended MASK
#define EXT_MASK 0xE0


#endif // I8042_H
