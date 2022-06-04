#include <lcom/lcf.h>
#include <stdint.h>
#include <lcom/utils.h>

uint8_t keyboard_counter=0;

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
    //Podia ser implementado tambem da seguinte forma -> *lsb = val & 0x00FF;
    *lsb = (uint8_t) val;
    return 0;   //ALL OK
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
    //Podia ser implementado tambem da seguinte forma -> *msb = val & 0xFF00;
    *msb = (uint8_t)(val >> 8);
    return 0;   //ALL OK
}



int (util_sys_inb)(int port, uint8_t *value) {
    // Variavel onde irá ficar o valor no formato uint de 32 bit
    // necessario para chamada de função sys_inb dado que o valor
    // é retornado em 32 bits e a função util_sys_inb retorna em 8 bit
    uint32_t value32;
    if(sys_inb(port, &value32)) return 1; //ERROR 1
    *value = value32;
    return 0;   //ALL OK
}
