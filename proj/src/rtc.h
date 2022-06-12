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


int (rtc_write_data)(uint8_t reg);
int (rtc_read_data)(uint8_t *rtc_data);
int (rtc_get_data)(uint8_t reg, uint8_t *rtc_data);
int (rtc_year)();
int (rtc_month)();
int (rtc_day)();
int (rtc_hour)();
int (rtc_minute)();
int (rtc_second)();
void (print_date)(char *date);
void (rtc_subscribe)(uint8_t *rtc_bit_number);
void (rtc_unsubscribe)();
