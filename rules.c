#include <limits.h>
#include <string.h>
#include "autodst.h"


const char* g_conf_wday[7]  = { "sun", "mon", "tue", "wed", "thu", "fri", "sat" };
const char* g_conf_mday[12] = { "jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec" };

/* Year range is from 1980 to 2099, so no century can occur which is not a leap year
#define is_leap(y)  ((y) % 4 == 0 && ((y) % 100 != 0 || (y) % 400 == 0))
*/
#define is_leap(y)  ((y) % 4 == 0)


static time_t get_rule_time(const Rule* rule, int year);


/* Check for change and - if not - return seconds until next change */
long check_date()
{
    static time_t next_check = LONG_MIN;

    time_t now = time(NULL);
    long   ret = 0L;

    if (now > next_check) {
        Status    status = g_config.status;
        struct tm *tm;
        int       year;
        time_t    todst;
        time_t    tostd;

        tm    = localtime(&now);
        year  = tm->tm_year + 1900;
        todst = get_rule_time(&g_config.rule_dst, year);
        tostd = get_rule_time(&g_config.rule_std, year);

        if (todst < tostd) {
            /* Northern hemisphere: DST starts in spring and ends in autumn */
            if (now >= todst && now < tostd) {
                status = DST_ON;
            } else {
                status = DST_OFF;
            }
        } else {
            /* Southern hemisphere: DST starts in autumn and ends in spring */
            if (now >= tostd && now < todst) {
                status = DST_OFF;
            } else {
                status = DST_ON;
            }
        }

        if (status != g_config.status) {
            time_t         offset;
            time_t         t;
            struct tm*     tm;
            unsigned short tosdate;
            unsigned short tostime;

            if (status == DST_ON) {
                write_log(TXT_SWITCHING_TO_DST);
                offset = 3600L;
            } else {
                write_log(TXT_SWITCHING_TO_STD);
                offset = -3600L;
            }

            t       = time(NULL) + offset;
            tm      = localtime(&t);
            tosdate = (tm->tm_mday & 0x1f)
                    + (((tm->tm_mon + 1) & 0xf) << 5)
                    + ((tm->tm_year - 80) << 9)
                    ;
            tostime = ((tm->tm_sec / 2) & 0x1f)
                    + ((tm->tm_min & 0x3f) << 5)
                    + ((tm->tm_hour & 0x1f) << 11)
                    ;

            Tsetdate(tosdate);
            Tsettime(tostime);

            g_config.status = status;

            write_rules_log(0);
            write_new_status();

            next_check = now;

            if (offset < 0L) {
                ret = -offset;
            }
        } else {
            time_t nextdst = get_next_rule_time(&g_config.rule_dst);
            time_t nextstd = get_next_rule_time(&g_config.rule_std);
            time_t next    = (nextdst < nextstd) ? nextdst : nextstd;

            ret = next - now;
        }
    }

    return ret;
}


time_t get_rule_time(const Rule* rule, int year)
{
    int        mdays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    struct tm  tm;
    struct tm* ltm;
    time_t     t;

    if (is_leap(year)) {
        /* leap year */
        ++mdays[1];
    }

    tm.tm_sec  = 0;
    tm.tm_min  = 0;
    tm.tm_hour = 0;
    tm.tm_mday = 1;
    tm.tm_mon  = rule->month;
    tm.tm_year = year - 1900;

    t   = mktime(&tm);
    ltm = localtime(&t);

    /* Find matching weekday */
    for (int i = 0; i < 7; ++i) {
        if (ltm->tm_wday == rule->weekday) {
            int day[5] = { 0, 0, 0, 0, 0 };
            int md     = mdays[ltm->tm_mon];
            int d      = ltm->tm_mday;
            int weeks  = 0;

            for (int j = 0; j < ARRAYSIZE(day); ++j, d += 7, ++weeks) {
                if (d <= md) {
                    day[j] = d;
                } else {
                    break;
                }
            }

            if (rule->which > 0) {
                /* Find n'th weekday */
                ltm->tm_mday = day[rule->which - 1];
            } else {
                /* Find n'th to last weekday */
                ltm->tm_mday = day[weeks + rule->which];
            }

            break;
        } else {
            ++ltm->tm_mday;
            ++ltm->tm_wday;
            ltm->tm_wday %= 7;
        }
    }

    t = mktime(ltm) + rule->hour * 3600L + rule->minute;

    return t;
}


time_t get_next_rule_time(const Rule* rule)
{
    time_t now  = time(NULL);
    int    year = localtime(&now)->tm_year + 1900;
    time_t next = get_rule_time(rule, year);

    if (next < now) {
        next = get_rule_time(rule, year + 1);
    }

    return next;
}