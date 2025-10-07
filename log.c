#include <string.h>
#include "autodst.h"


long g_loghdl = -1L;


static void open_log(void);
static void write_time_log(const char* prompt, time_t t, const char* tz, const char* suffix);


void open_log()
{
    if (g_loghdl < 0L) {
        g_loghdl = Fopen(g_logfile, FO_RW);
    }

    if (g_loghdl < 0L) {
        g_loghdl = Fcreate(g_logfile, 0);
    } else {
        Fseek(0, g_loghdl, SEEK_END);
    }
}


void close_log()
{
    if (g_loghdl >= 0L) {
        Fclose(g_loghdl);
        g_loghdl = -1L;
    }
}


void write_log(const char* s)
{
    static unsigned char newline = 1;
    int isopen = (g_loghdl >= 0L);

    open_log();

    if (g_loghdl >= 0L) {
        if (newline) {
            newline = 0;
            write_time_log("", time(NULL), NULL, ": ");
        }

        Fwrite(g_loghdl, strlen(s), s);
        newline = (strstr(s, NEWLINE) != NULL);

        if (!isopen) {
            close_log();
        }
    }
}


void write_time_log(const char* prompt, time_t t, const char* tz, const char* suffix)
{
    write_log(prompt);
    write_log(format_time(FMT_WD_DATETIME, t));
    if (tz != NULL && g_config.status != INVALID) {
        write_log(" ");
        write_log(tz);
    }
    if (suffix != NULL) {
        write_log(suffix);
    }
}


void write_config_log()
{
    write_rules_log();
}


void write_rules_log()
{
    write_time_log(TXT_BEGINNING_OF_DST, g_config.rule_from.next_change, NULL, NEWLINE);
    write_time_log(TXT_END_OF_DST, g_config.rule_to.next_change, NULL, NEWLINE);
    write_time_log(TXT_NEXT_CHANGE, g_next_change, NULL, NEWLINE);
}


const char* int2str(int n)
{
    static char buf[10];

    char  tmp[sizeof(buf)];
    char* ptr = tmp;
    int   len = 0;
    char  neg = '\0';

    if (n < 0) {
        neg = '-';
        n = -n;
    }

    do {
        *ptr++ = '0' + (n % 10);
        n /= 10;
        ++len;
    } while (n > 0);

    ptr = buf;

    if (neg != '\0') {
        *ptr++ = neg;
    }

    for (int i = 0; i < len; ++i) {
        *ptr++ = tmp[len - i - 1];
    }

    *ptr = '\0';

    return buf;
}
