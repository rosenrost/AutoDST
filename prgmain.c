#include <osbind.h>
#include <string.h>
#include <vt52.h>
#include "autodst.h"


char* g_logfile = "@:\\" LOGFILE;


static void clean_exit(void);
static void output(const char* s);
static void output_time(const char* prompt, const char* timestr, const char* tz);


int main()
{
    int rc = 1;

    *g_logfile = BOOTDRIVE;

    atexit(clean_exit);
    output(NEWLINE REV_ON "AutoDST v" VERSION REV_OFF NEWLINE);

    Fdelete(g_logfile);
    write_log(TXT_EXECUTING_PRG);

    if (read_config()) {
        time_t     nextdst = get_next_rule_time(&g_config.rule_dst);
        time_t     nextstd = get_next_rule_time(&g_config.rule_std);
        time_t     next    = (nextdst < nextstd) ? nextdst : nextstd;
        const char *tz;

        rc = 0;
        tz = (g_config.status == DST_ON) ? g_config.tzdst : g_config.tzstd;

        write_log(TXT_CONFIG_LOADED);
        write_config_log();

        output_time(TXT_CURRENT_TIME, format_time(FMT_WD_DATETIME, time(NULL)), tz);
        output_time(TXT_BEGINNING_OF_DST, format_time(FMT_WD_DATETIME, nextdst), NULL);
        output_time(TXT_END_OF_DST, format_time(FMT_WD_DATETIME, nextstd), NULL);
        output_time(TXT_NEXT_CHANGE, format_time(FMT_WD_DATETIME, next), NULL);

        check_date();
    } else {
        char  errtext[] = TXT_ERR_CONFIG_NOT_LOADED;
        char* drvltr    = strchr(errtext, '@');

        if (drvltr != NULL) {
            *drvltr = Dgetdrv() + 'A';
        }

        output(errtext);
    }

    return rc;
}


void clean_exit()
{
    close_log();
}


void output(const char* s)
{
    while (*s != '\0') {
        Bconout(DEV_CONSOLE, *s);
        ++s;
    }
}


void output_time(const char* prompt, const char* timestr, const char* tz)
{
    output(prompt);
    output(timestr);
    if (tz != NULL) {
        output(" ");
        output(tz);
    }
    output(NEWLINE);
}
