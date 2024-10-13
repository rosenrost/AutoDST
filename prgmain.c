#include <osbind.h>
#include <string.h>
#include <vt52.h>
#include "autodst.h"


char* g_logfile = "@:\\" LOGFILE;


static void clean_exit(void);
static void output(const char* s);
static void output_time(const char* prompt, const char* timestr, const char* tz);
static void pause(unsigned int sec);


int main()
{
    int rc = 1;

    *g_logfile = BOOTDRIVE;

    atexit(clean_exit);
    output(NEWLINE REV_ON "AutoDST v" VERSION REV_OFF NEWLINE);

    Fdelete(g_logfile);
    write_log(TXT_EXECUTING_PRG);

    if (read_config()) {
        const char* tz;
        Status      curr_status;

        rc = 0;
        tz = (g_status == DST_ON) ? g_tzdst : g_tzstd;

        write_log(TXT_CONFIG_LOADED);
        write_config_log();

        output_time(TXT_CURRENT_TIME, format_time(FMT_WD_DATETIME, time(NULL)), tz);
        output_time(TXT_BEGINNING_OF_DST, format_time(FMT_WD_DATETIME, g_rule_from.next_change), NULL);
        output_time(TXT_END_OF_DST, format_time(FMT_WD_DATETIME, g_rule_to.next_change), NULL);
        output_time(TXT_NEXT_CHANGE, format_time(FMT_WD_DATETIME, get_next_change(NULL)), NULL);

        if (g_next_change == g_rule_from.next_change) {
            /* Next change will switch to DST => current time is standard time */
            curr_status = DST_OFF;
        } else if (g_next_change == g_rule_to.next_change) {
            /* Next change will switch to standard time => current time is DST */
            curr_status = DST_ON;
        } else {
            curr_status = g_status;
        }

        update_clock(curr_status);
    } else {
        char  errtext[] = TXT_ERR_CONFIG_NOT_LOADED;
        char* drvltr    = strchr(errtext, '@');

        if (drvltr != NULL) {
            *drvltr = Dgetdrv() + 'A';
        }

        output(errtext);
    }

    pause(1);

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


void pause(unsigned int sec)
{
    clock_t end = clock() + (sec * CLOCKS_PER_SEC);
    do {} while (clock() < end);
}
