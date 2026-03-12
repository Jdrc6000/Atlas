#include "rtc.h"

static inline unsigned char inb(unsigned short port) {
    unsigned char val;
    __asm__ volatile ("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

static inline void outb(unsigned short port, unsigned char val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

#define CMOS_CMD 0x70
#define CMOS_DATA 0x71

#define RTC_REG_SECOND 0x00
#define RTC_REG_MINUTE 0x02
#define RTC_REG_HOUR 0x04
#define RTC_REG_DAY 0x07
#define RTC_REG_MONTH 0x08
#define RTC_REG_YEAR 0x09
#define RTC_STATUS_A 0x0A

static int rtc_updating() {
    outb(CMOS_CMD, RTC_STATUS_A);
    return inb(CMOS_DATA) & 0x80;
}

static unsigned char rtc_reg(unsigned char reg) {
    outb(CMOS_CMD, reg);
    return inb(CMOS_DATA);
}

static unsigned char bcd_to_bin(unsigned char bcd) {
    return (bcd >> 4) * 10 + (bcd & 0x0F);
}

void rtc_read(rtc_time_t *t) {
    while (rtc_updating());

    t->second = bcd_to_bin(rtc_reg(RTC_REG_SECOND));
    t->minute = bcd_to_bin(rtc_reg(RTC_REG_MINUTE));
    t->hour = bcd_to_bin(rtc_reg(RTC_REG_HOUR));
    t->day = bcd_to_bin(rtc_reg(RTC_REG_DAY));
    t->month = bcd_to_bin(rtc_reg(RTC_REG_MONTH));
    t->year = bcd_to_bin(rtc_reg(RTC_REG_YEAR));
}