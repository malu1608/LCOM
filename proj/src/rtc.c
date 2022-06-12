#include <stdint.h>
#include <stdio.h>
#include "rtc.h"


bool rtcIsUpdating(){
    uint8_t regA;
    sys_outb(RTC_ADDR_REG, REG_A);
    util_sys_inb(RTC_DATA_REG, &regA);
    return (regA & UIP_SET);
}

bool is_binary(){
    uint8_t regB;
    sys_outb(RTC_ADDR_REG,REG_B);
    util_sys_inb(RTC_DATA_REG,&regB);
    return (regB & BINARY );
}

int bcd_to_bin(int num){
    return  ((num& 0xF0 )>>4 ) * 10 + (num & 0x0F);
}

Data * get_date(){
    //allocate memory
    Data * date = malloc(sizeof(Data));

    //check if rtc is updating to sleep for a while
    if (rtcIsUpdating()) {
        sleep(WAIT_UPDATE * 0.000001);
    }

    //seconds
    sys_outb(RTC_ADDR_REG, SEC_REG);
	util_sys_inb(RTC_DATA_REG, &date->seconds);

    //minutes
	sys_outb(RTC_ADDR_REG, MIN_REG);
	util_sys_inb(RTC_DATA_REG, &date->minutes);

    //hour
	sys_outb(RTC_ADDR_REG, HOUR_REG);
	util_sys_inb(RTC_DATA_REG, &date->hour);

    //day
    sys_outb(RTC_ADDR_REG, DAY_REG);
	util_sys_inb(RTC_DATA_REG, &date->day);

    //month
	sys_outb(RTC_ADDR_REG, MONTH_REG);
	util_sys_inb(RTC_DATA_REG, &date->month);

    //year
	sys_outb(RTC_ADDR_REG, YEAR_REG);
	sys_inb(RTC_DATA_REG, &date->year);

    //convert to binary
    if(!is_binary()){
        date->seconds = bcd_to_bin(date->seconds);
        date->minutes = bcd_to_bin(date->minutes);
        date->hour = bcd_to_bin(date->hour);
        date->day = bcd_to_bin(date->day);
        date->month = bcd_to_bin(date->month);
        date->year = bcd_to_bin(date->year);
    }

    return date;
}
