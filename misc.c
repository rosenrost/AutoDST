#include <ctype.h>
#include <string.h>
#include "autodst.h"


static const char* m_wday_abbr[] =
{
    WDAY_SUN_ABBR,
    WDAY_MON_ABBR,
    WDAY_TUE_ABBR,
    WDAY_WED_ABBR,
    WDAY_THU_ABBR,
    WDAY_FRI_ABBR,
    WDAY_SAT_ABBR
};

static const char* m_wday_name[] =
{
    WDAY_SUNDAY,
    WDAY_MONDAY,
    WDAY_TUESDAY,
    WDAY_WEDNESDAY,
    WDAY_THURSDAY,
    WDAY_FRIDAY,
    WDAY_SATURDAY
};

static const char* m_mday_abbr[] =
{
    MDAY_JAN_ABBR,
    MDAY_FEB_ABBR,
    MDAY_MAR_ABBR,
    MDAY_APR_ABBR,
    MDAY_MAY_ABBR,
    MDAY_JUN_ABBR,
    MDAY_JUL_ABBR,
    MDAY_AUG_ABBR,
    MDAY_SEP_ABBR,
    MDAY_OCT_ABBR,
    MDAY_NOV_ABBR,
    MDAY_DEC_ABBR
};

static const char* m_mday_name[] =
{
    MDAY_JANUARY,
    MDAY_FEBRUARY,
    MDAY_MARCH,
    MDAY_APRIL,
    MDAY_MAY,
    MDAY_JUNE,
    MDAY_JULY,
    MDAY_AUGUST,
    MDAY_SEPTEMBER,
    MDAY_OCTOBER,
    MDAY_NOVEMBER,
    MDAY_DECEMBER
};


static size_t mystrftime(char *s, size_t smax, const char *fmt, const struct tm *tp);


char* skipws(const char* s)
{
    while (isspace(*s)) {
        ++s;
    }

    return (char*)s;
}


char* read_line(char* s, long n, long handle)
{
    char* ptr = s;
    char  c   = '\0';
    int   eof = 0;

    while (--n > 0 && !(eof = (Fread(handle, 1, &c) < 1))) {
        *ptr++ = c;

        if (c == '\n') {
            break;
        }
    }

    if (eof && ptr == s) {
        s = NULL;
    } else {
        *ptr = '\0';
    }

    return s;
}


const char* format_time(const char *fmt, time_t t)
{
    static char buf[80];
    struct tm* tm = localtime(&t);
    mystrftime(buf, sizeof(buf) - 1, fmt, tm);
    return buf;
}


size_t mystrftime(char *s, size_t smax, const char *fmt, const struct tm *tp)
{
    /** Original source "stolen" from libcmini.
     *  https://github.com/freemint/libcmini
     */

    char*  ptr   = s;
    size_t count = 0;

    do {
        if (*fmt == '%') {
            const char *addstr = NULL;
            int         addlen = -1;
            char        addval[80];

            switch (*++fmt) {
                case 'a':
                case 'A':
                    if (tp->tm_wday < 0 || tp->tm_wday > 6) {
                        addstr = "?";
                    } else if (*fmt == 'a') {
                        addstr = m_wday_abbr[tp->tm_wday];
                    } else {
                        addstr = m_wday_name[tp->tm_wday];
                    }
                    break;
                case 'b':
                    if (tp->tm_mon < 0 || tp->tm_mon > 11) {
                        addstr = "?";
                    } else if (*fmt == 'a') {
                        addstr = m_mday_abbr[tp->tm_mon];
                    } else {
                        addstr = m_mday_name[tp->tm_mon];
                    }
                    break;
                case 'd':
                    addval[0] = '0' + (tp->tm_mday / 10);
                    addval[1] = '0' + (tp->tm_mday % 10);
                    addval[2] = '\0';
                    break;
                case 'H':
                    addval[0] = '0' + (tp->tm_hour / 10);
                    addval[1] = '0' + (tp->tm_hour % 10);
                    addval[2] = '\0';
                    break;
                case 'I':
                    {
                        int hour12 = tp->tm_hour % 12;
                        if (hour12 == 0) {
                            hour12 = 12;
                        }
                        addval[0] = '0' + (hour12 / 10);
                        addval[1] = '0' + (hour12 % 10);
                        addval[2] = '\0';
                    }
                    break;
                case 'm':
                    addval[0] = '0' + ((tp->tm_mon + 1) / 10);
                    addval[1] = '0' + ((tp->tm_mon + 1) % 10);
                    addval[2] = '\0';
                    break;
                case 'M':
                    addval[0] = '0' + (tp->tm_min / 10);
                    addval[1] = '0' + (tp->tm_min % 10);
                    addval[2] = '\0';
                    break;
                case 'p':
                    if (tp->tm_hour > 0 && tp->tm_hour < 13) {
                        addstr = "AM";
                    } else {
                        addstr = "PM";
                    }
                    break;
                case 'S':
                    addval[0] = '0' + (tp->tm_sec / 10);
                    addval[1] = '0' + (tp->tm_sec % 10);
                    addval[2] = '\0';
                    break;
                case 'y':
                    addval[0] = '0' + ((tp->tm_year % 100) / 10);
                    addval[1] = '0' + ((tp->tm_year % 100) % 10);
                    addval[2] = '\0';
                    break;
                case 'Y':
                    addval[0] = '0' + ((tp->tm_year + 1900) / 1000);
                    addval[1] = '0' + (((tp->tm_year + 1900) % 1000) / 100);
                    addval[2] = '0' + (((tp->tm_year + 1900) % 100) / 10);
                    addval[3] = '0' + ((tp->tm_year + 1900) % 10);
                    addval[4] = '\0';
                    break;
                case '%':
                    addstr = "%";
                    break;
                case 'c':
                case 'j':
                case 'U':
                case 'W':
                case 'w':
                case 'X':
                case 'x':
                case 'Z':
                default:
                    addval[0] = '%';
                    addval[1] = *fmt;
                    addval[2] = '\0';
                    break;
            }

            if (addstr == NULL) {
                addstr = addval;
            }

            if (*addstr != '\0') {
                size_t len;

                if (addlen < 0) {
                    len = strlen(addstr);
                } else {
                    len = addlen;
                }

                if (count + len > smax) {
                    len = smax - count;
                }

                strncpy(ptr, addstr, len);

                ptr   += len;
                count += len;
            }

            ++fmt;
        } else {
            if (count <= smax) {
                *ptr++ = *fmt;
            }

            ++fmt;
        }
    } while (fmt[-1] != '\0');

    if (count > smax) {
        count = 0;
    }

    return count;
}
