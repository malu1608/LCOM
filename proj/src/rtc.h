//#pragma once

#include <lcom/lcf.h>

#define RTC_IRQ 8 //hookid
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71 

#define RTC_SECONDS 0   
#define RTC_MINUTES 2   
#define RTC_HOURS 4   
#define RTC_DAYOFWEEK 6   
#define RTC_DAYOFMONTH 7   
#define RTC_MONTH 8   
#define RTC_YEAR 9

#define PERIODIC_INTERRUPT_ENABLE BIT(6)

#define RTC_REG_A 10
#define RTC_REG_B 11
#define RTC_REG_C 12
#define RTC_REG_D 13

#define RTC_UIP BIT(7)
#define RTC_PF BIT(6)

/**
 * @brief Struct for Date
 * 
 */
typedef struct Date{
  uint32_t year; 
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minutes;
  uint8_t second;
} Date;

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
 * @brief Check if the input from the rtc is written in binary or in bcd.
 * 
 * @return true - It's written in binary
 * @return false - It's written in bcd
 */
bool is_binary();

/**
 * @brief Checks if rtc is updating.
 * 
 * @return true - It's updating
 * @return false - It's not updating
 */
bool RTC_is_Updating();

/**
 * @brief Converts the int passed in the parameter from bcd to binary.
 * 
 * @param num - Number to be converted
 * @return int - The converted number
 */
int bcd_to_binary(int num);

/**
 * @brief Get the date object.
 * 
 * @return Date* - Date pointer to Date object
 */
Date * get_date();
