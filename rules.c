#include <string.h>
#include "autodst.h"


const char* g_conf_wday[7]  = { "sun", "mon", "tue", "wed", "thu", "fri", "sat" };
const char* g_conf_mday[12] = { "jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec" };

time_t g_next_change = -1L;

static time_t m_last_change = -1L;

/* Year range is from 1980 to 2099, so no century can occur which is not a leap year
#define is_leap(y)  ((y) % 4 == 0 && ((y) % 100 != 0 || (y) % 400 == 0))
*/
#define is_leap(y)  ((y) % 4 == 0)


void update_clock(Status new_status)
{
    if (g_config.status != new_status) {
        time_t         offset;
        time_t         t;
        struct tm*     tm;
        unsigned short tosdate;
        unsigned short tostime;
        Rule*          rule;

        if (new_status == DST_ON) {
            write_log(TXT_SWITCHING_TO_DST);
            rule   = &g_config.rule_from;
            offset = 3600L;
        } else {
            write_log(TXT_SWITCHING_TO_STD);
            rule   = &g_config.rule_to;
            offset = -3600L;
        }

        m_last_change = g_next_change;
        t             = time(NULL) + offset;
        tm            = localtime(&t);
        tosdate       = (tm->tm_mday & 0x1f)
                      + (((tm->tm_mon + 1) & 0xf) << 5)
                      + ((tm->tm_year - 80) << 9)
                      ;
        tostime       = ((tm->tm_sec / 2) & 0x1f)
                      + ((tm->tm_min & 0x3f) << 5)
                      + ((tm->tm_hour & 0x1f) << 11)
                      ;

        Tsetdate(tosdate);
        Tsettime(tostime);

        g_config.status   = new_status;
        g_next_change     = get_next_change(NULL);
        rule->next_change = get_next_change(rule);

        write_rules_log();
        write_new_status();
    }
}


time_t get_next_change(const Rule* rule)
{
    time_t     now;
    struct tm* tm;
    int        year;
    time_t     tr;

    if (rule == NULL) {
        rule = (g_config.status == DST_OFF) ? &g_config.rule_from : &g_config.rule_to;
    }

    if (m_last_change >= 0L) {
        /* Search from one day after last change */
        now = m_last_change + 86400L;
    } else {
        now = time(NULL);
    }


    tm   = localtime(&now);
    year = tm->tm_year + 1900;
    tr   = get_rule_time(rule, year);

    if (tr < now) {
        /* Rule will match next year */
        tr = get_rule_time(rule, year + 1);
    }

    return tr;
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
