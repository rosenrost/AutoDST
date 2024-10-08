#include <string.h>
#include "autodst.h"


const char* g_conf_wday[7]  = { "sun", "mon", "tue", "wed", "thu", "fri", "sat" };
const char* g_conf_mday[12] = { "jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec" };

Status g_status               = INVALID;
char   g_tzstd[MAXLEN_TZ + 1] = "STD";
char   g_tzdst[MAXLEN_TZ + 1] = "DST";
Rule   g_rule_from            = { 0, 0, 0, 0, 0, 0, 0 };
Rule   g_rule_to              = { 0, 0, 0, 0, 0, 0, 0 };
time_t g_next_change          = -1L;

static time_t m_last_change = -1L;

/* Year range is from 1980 to 2099, so no century can occur which is not a leap year
#define is_leap(y)  ((y) % 4 == 0 && ((y) % 100 != 0 || (y) % 400 == 0))
*/
#define is_leap(y)  ((y) % 4 == 0)


void update_clock(Status new_status)
{
    if (g_status != new_status) {
        time_t         offset;
        time_t         t;
        struct tm*     tm;
        unsigned short tosdate;
        unsigned short tostime;
        Rule*          rule;

        if (new_status == DST_ON) {
            write_log(TXT_SWITCHING_TO_DST);
            rule   = &g_rule_from;
            offset = 3600L;
        } else {
            write_log(TXT_SWITCHING_TO_STD);
            rule   = &g_rule_to;
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

        g_status          = new_status;
        g_next_change     = get_next_change(NULL);
        rule->next_change = get_next_change(rule);

        write_rules_log();

        /* Write new status to config file */
        Fdelete(g_config_bak);

        if (Frename(0, g_config_file, g_config_bak) == 0) {
            long inhdl = Fopen(g_config_bak, FO_READ);

            if (inhdl < 0) {
                write_log(TXT_ERR_CANNOT_CREATE_CONFIG_BAK);
                (void)Frename(0, g_config_bak, g_config_file);
            } else {
                long outhdl = Fcreate(g_config_file, 0);

                if (outhdl < 0) {
                    write_log(TXT_ERR_CANNOT_CREATE_NEW_CONFIG);
                    Fclose(inhdl);
                    (void)Frename(0, g_config_bak, g_config_file);
                } else {
                    char buf[256];

                    while (read_line(buf, sizeof(buf) - 1, inhdl) != NULL) {
                        size_t len  = strlen(buf);
                        char*  line = skipws(buf);

                        if (*line == '\0' || *line == '#') {
                            Fwrite(outhdl, len, buf);
                        } else {
                            char val[256];
                            int  var;

                            var = get_value(buf, val);

                            if (var == VAR_STATUS) {
                                char ststr[] = "0";

                                ststr[0] += g_status;

                                strcpy(buf, "status=");
                                strcat(buf, ststr);
                                strcat(buf, NEWLINE);
                            }

                            Fwrite(outhdl, strlen(buf), buf);
                        }
                    }

                    Fclose(outhdl);
                    Fclose(inhdl);
                }
            }

            Fdelete(g_config_bak);
        } else {
            write_log(TXT_ERR_CANNOT_CREATE_CONFIG_BAK);
        }
    }
}


time_t get_next_change(const Rule* rule)
{
    time_t     now;
    struct tm* tm;
    int        year;
    time_t     tr;

    if (rule == NULL) {
        rule = (g_status == DST_OFF) ? &g_rule_from : &g_rule_to;
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
    int       mdays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    struct tm tm;
    time_t    t;

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

    t = mktime(&tm);
    memcpy(&tm, localtime(&t), sizeof(tm));

    /* Find matching weekday */
    for (int i = 0; i < 7; ++i) {
        if (tm.tm_wday == rule->weekday) {
            int day[5] = { 0, 0, 0, 0, 0 };
            int md     = mdays[tm.tm_mon];
            int d      = tm.tm_mday;
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
                tm.tm_mday = day[rule->which - 1];
            } else {
                /* Find n'th to last weekday */
                tm.tm_mday = day[weeks + rule->which];
            }

            break;
        } else {
            ++tm.tm_mday;
            ++tm.tm_wday;
            tm.tm_wday %= 7;
        }
    }

    t = mktime(&tm) + rule->hour * 3600L + rule->minute;

    return t;
}
