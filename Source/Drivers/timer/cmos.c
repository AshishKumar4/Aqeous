#include <cmos.h>
#include <sys.h>

#define CMOS_REGISTER_PORT 0x70
#define CMOS_DATA_PORT 0x71

////////////////////////////////////////////////////////////////////////////////
// Date Names
////////////////////////////////////////////////////////////////////////////////

const char *__cmos_get_short_day(uint8_t day)
{
    switch (day) {
        case 7: return "Sat";
        case 6: return "Fri";
        case 5: return "Thu";
        case 4: return "Wed";
        case 3: return "Tue";
        case 2: return "Mon";
        default:
        case 1: return "Sun";
    }
};

const char *__cmos_get_day(uint8_t day)
{
    switch (day) {
        case 7: return "Saturday";
        case 6: return "Friday";
        case 5: return "Thursday";
        case 4: return "Wednesday";
        case 3: return "Tuesday";
        case 2: return "Monday";
        default:
        case 1: return "Sunday";
    }
};

const char *__cmos_get_short_month(uint8_t month)
{
    switch (month) {
        case 12: return "Dec";
        case 11: return "Nov";
        case 10: return "Oct";
        case 9: return "Sep";
        case 8: return "Aug";
        case 7: return "Jul";
        case 6: return "Jun";
        case 5: return "May";
        case 4: return "Apr";
        case 3: return "Mar";
        case 2: return "Feb";
        default:
        case 1: return "Jan";
    }
};

const char *__cmos_get_month(uint8_t month)
{
    switch (month) {
        case 12: return "December";
        case 11: return "November";
        case 10: return "October";
        case 9: return "September";
        case 8: return "August";
        case 7: return "July";
        case 6: return "June";
        case 5: return "May";
        case 4: return "April";
        case 3: return "March";
        case 2: return "February";
        default:
        case 1: return "January";
    }
};


////////////////////////////////////////////////////////////////////////////////
// Doomsday
////////////////////////////////////////////////////////////////////////////////

uint8_t __cmos_do_doomsday(uint8_t day, uint8_t month, uint8_t year)
{
    if (month < 3) {
        month += 13;
        --year;
    }
    uint16_t dow = day + (13 * month - 27)/5
                       + year
                       + year/4
                       - year/100
                       + year/400;
    return ((uint8_t)(dow % 7) + 1);
};


////////////////////////////////////////////////////////////////////////////////
// BCD Decoding
////////////////////////////////////////////////////////////////////////////////

uint8_t decode_bcd(uint8_t bcd)
{
    return ((bcd & 0xF0) >> 1) + ((bcd & 0xF0) >> 3) + (bcd & 0x0F);
};


////////////////////////////////////////////////////////////////////////////////
// CMOS Core
////////////////////////////////////////////////////////////////////////////////

struct
{
    bool is_24h;
    bool bcd_encoded;
} cmos;

void init_cmos()
{
    uint8_t v = cmos_rtc_get_value(cmos_rtc_reg_status_b, false);
    cmos.is_24h = v & (1 << 1);
    cmos.bcd_encoded = (v & (1 << 2)) ? false : true;
};

bool cmos_24hour()
{
    return cmos.is_24h;
};

bool cmos_bcd_encoded()
{
    return cmos.bcd_encoded;
};

bool cmos_updating()
{
    uint8_t v = cmos_rtc_get_value(cmos_rtc_reg_status_a, false);
    return v & 0x80;
};

inline unsigned int ReadFromCMOS()
{
    outb(0x70,0x00);
    unsigned int time=inb(0x71);
    return time;
}

////////////////////////////////////////////////////////////////////////////////

uint8_t cmos_rtc_get_value(enum cmos_reg r, bool auto_decode_bcd)
{
    asm volatile("cli");
    outb(CMOS_REGISTER_PORT, r);
    uint8_t ret_val = inb(CMOS_DATA_PORT);
    asm volatile("sti");

    /* we should only decode the value if it is in BCD and we've specified */
    /* automatic decoding */
    if (cmos_bcd_encoded() && auto_decode_bcd) {
        switch (r) {
            case cmos_rtc_reg_seconds:
            case cmos_rtc_reg_minutes:
            case cmos_rtc_reg_hours:
            case cmos_rtc_reg_weekday:
            case cmos_rtc_reg_day:
            case cmos_rtc_reg_month:
            case cmos_rtc_reg_year:
            case cmos_rtc_reg_century:
                ret_val = decode_bcd(ret_val);
                break;
            default:
                break;
        }
    }

    return ret_val;
};
