#include <stdint.h>
#include <stdio.h>
#include "rtc.h"

int rtc_hookid = RTC_IRQ;

/*
* Writes in the register.
*/
int (rtc_write_data)(uint8_t reg){

    if(sys_outb(RTC_ADDR_REG, reg) != 0) 
        return 1; 
    else
        return 0;

}

/*
* Reads from register.
*/
int (rtc_read_data)(uint8_t *rtc_data){
    
    if(util_sys_inb(RTC_ADDR_REG, rtc_data) != 0) 
        return 1; 
    else
        return 0;

}

/*
* Writes in the register and reads from register.
*/
int (rtc_get_data)(uint8_t reg, uint8_t *rtc_data){

    sys_outb(RTC_ADDR_REG, reg);
    util_sys_inb(RTC_ADDR_REG, rtc_data);  
    return 1;
}

/*
* Gets year.
*/
int (rtc_year)(){
  uint8_t year;
  rtc_get_data(RTC_YEAR, &year);

  return year;
}

/*
* Gets month.
*/
int (rtc_month)(){
  uint8_t month;
  rtc_get_data(RTC_MONTH, &month);

  return month;
}

/*
* Gets day.
*/
int (rtc_day)(){
  uint8_t day;
  rtc_get_data(RTC_DAYOFMONTH, &day);

  return day;
}

/*
* Gets hours.
*/
int (rtc_hour)(){
  uint8_t hour;
  rtc_get_data(RTC_HOURS, &hour);

  return hour;
}

/*
* Gets minutes.
*/
int (rtc_minute)(){
  uint8_t minute;
  rtc_get_data(RTC_MINUTES, &minute);

  return minute;
}

/*
* Gets seconds.
*/
int (rtc_second)(){
  uint8_t seconds;
  rtc_get_data(RTC_SECONDS, &seconds);

  return seconds;
}

void (print_date)(char *date){

    uint8_t sec, min, hour, day, month, year;

    sec = rtc_second();
 
    min = rtc_minute();

    hour = rtc_hour();

    day = rtc_day();
 
    month = rtc_month();

    year = rtc_year();

    printf("%02d %02d 20%02d", day, month, year);
}

void (rtc_subscribe)(uint8_t *rtc_bit_number) {

	*rtc_bit_number = BIT(rtc_hookid);

	unsigned char reg;
	sys_outb(RTC_ADDR_REG, RTC_REG_C);
	util_sys_inb(RTC_DATA_REG, &reg);	

	printf("%x",reg);

	if(sys_irqsetpolicy(RTC_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &rtc_hookid) != 0) {
		printf("RTC: subscribe: couldn't set policy\n");
		return;
	}
}

void (rtc_unsubscribe)() { 
    if(sys_irqrmpolicy(&rtc_hookid) != 0) {
    	printf("RTC: unsubscribe: couldn't disable interrupts\n");
    	return;
    }
}

