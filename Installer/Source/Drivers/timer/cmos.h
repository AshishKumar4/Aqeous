
#ifndef CMOS_h
#define CMOS_h

#include <system.h>

/**
 The CMOS registers for Real-Time-Clock (RTC) value access.
 */
enum cmos_reg
{
    cmos_rtc_reg_seconds = 0x00,
    cmos_rtc_reg_minutes = 0x02,
    cmos_rtc_reg_hours = 0x04,
    cmos_rtc_reg_weekday = 0x06,
    cmos_rtc_reg_day = 0x07,
    cmos_rtc_reg_month = 0x08,
    cmos_rtc_reg_year = 0x09,
    cmos_rtc_reg_status_a = 0x0a,
    cmos_rtc_reg_status_b = 0x0b,

    /* the following register is not universally available */
    cmos_rtc_reg_century = 0x32,
};


/**
 Initialise the driver for CMOS access.
 */
void init_cmos();

/**
 Checks to see if CMOS is in the process of being updated. It is important
 that accessing of CMOS RTC values is not performed if this is happening.
 */
bool cmos_updating();

/**
 Retrieve the a value from CMOS for the specified register. This also allows
 for specifying whether the CMOS value should be automatically decoded from BCD,
 if it is in bcd.
 */
uint8_t cmos_rtc_get_value(enum cmos_reg r, bool auto_decode_bcd);

/**
 Test to see if CMOS is configured to be 24hour.
 */
bool cmos_24hour();

/**
 Test to see if CMOS is bcd encoded.
 */
bool cmos_bcd_encoded();

#endif
