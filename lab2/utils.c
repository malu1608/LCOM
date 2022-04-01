#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (util_sys_inb)(int port, uint8_t *value) {
  /* To be implemented by the students */
  //we have info on the port (32-bit) and bring it to a known reference (&var), then cast info (var) and store in value
  uint32_t var = 0x00;
  if (sys_inb(port, &var) != 0) //var is updated here
    {
      printf("Error");
      return 1;
    }
  *value = (uint8_t) var;

  return 0;

}
