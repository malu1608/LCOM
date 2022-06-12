#pragma once

#include <lcom/lcf.h>

/**
 * Struct for Data
 * 
 */
typedef struct Date{
  uint32_t year; 
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minutes;
  uint8_t seconds;
} Data;

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define SEC_REG 0
#define MIN_REG 2
#define HOUR_REG 4
#define DAY_REG 7
#define MONTH_REG 8
#define YEAR_REG 9
#define REG_A 0x0A
#define REG_B 0X0B
#define BINARY BIT(2)
#define UIP_SET BIT(7)
#define WAIT_UPDATE 244 


/**
 * Checks if rtc's input is written in bcd or in binary.
 */
bool is_binary();

/**
 * Checks if rtc is updating.
 */
bool rtcIsUpdating();

/**
 * Converts a number from bcd to binary.
 */
int bcd_to_bin(int num);

/**
 * Gets a pointer to the desired Data object.
 */
Data * get_date();
