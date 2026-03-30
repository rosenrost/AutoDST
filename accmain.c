#include <gem.h>
#include <osbind.h>
#include <string.h>
#include "autodst.h"


#define DEFAULT_SLEEP  60L


char* g_logfile  = "@:\\" LOGFILE;
char* m_auto_prg = "@:\\AUTO\\" AUTO_PRG;


static int auto_prg_exists(void);
static void clean_exit(void);
static void dst_loop(short menu_id);
static void err_loop(short menu_id);
static void show_status(void);

static void (*acc_loop)(short menu_id) = err_loop;


int main()
{
    short app_id  = appl_init();
    int   rc      = 1;
    short menu_id = -1;

    wind_update(BEG_UPDATE);
    *g_logfile  = BOOTDRIVE;
    *m_auto_prg = BOOTDRIVE;

    if (!auto_prg_exists()) {
        /* Delete log file if AUTODST.PRG does not exist in AUTO folder */
        Fdelete(g_logfile);
    }

    write_log(TXT_STARTING_ACC);

    if (app_id < 0) {
        /* AES init failed */
        write_log(TXT_ERR_AES_INIT);
    } else {
        atexit(clean_exit);

        if (_app) {
            /* Cannot run as PRG */
            form_alert(1, TXT_ALERT_ACC_AS_PRG);
        } else {
            static const char* menu_title = "  " MENU_TITLE;

            menu_id = menu_register(app_id, menu_title);

            if (menu_id < 0) {
                write_log(TXT_ERR_ACC_REGISTER);
            } else if (read_config()) {
                rc = 0;
                write_log(TXT_CONFIG_LOADED);
                write_config_log();
                acc_loop = dst_loop;
            } else {
                g_config.status = INVALID;
            }
        }
    }

    close_log();
    wind_update(END_UPDATE);
    acc_loop(menu_id);

    return rc;
}


int auto_prg_exists()
{
    /* Check if AUTODST.PRG exists in AUTO folder */

    long autohdl = Fopen(m_auto_prg, FO_READ);

    if (autohdl < 0L) {
        return 0;
    }

    return 1;
}


void clean_exit()
{
    close_log();
    appl_exit();
}


void dst_loop(short menu_id)
{
    for (;;) {
        long   sleep = DEFAULT_SLEEP * 1000L; /* Default: sleep for one minute */
        short  msg[8];
        short  evret;

        if (check_date() <= 120L) {
            /* Next change will be within two minutes, so only sleep for one second */
            sleep = 1000L;
        }

        evret = evnt_multi(MU_MESAG | MU_TIMER,
                           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                           msg, sleep, NULL, NULL, NULL, NULL, NULL, NULL
                          );

        if ((evret & MU_MESAG) != 0) {
            if (msg[0] == AC_OPEN && msg[4] == menu_id) {
                /* Accessory was called */
                show_status();
            }
        }
    }
}


void err_loop(short menu_id)
{
    for (;;) {
        short msg[8];

        evnt_mesag(msg);

        if (menu_id >= 0 && msg[0] == AC_OPEN && msg[4] == menu_id) {
            /* Accessory was called */
            show_status();
        }
    }
}


void show_status()
{
    char text[256];

    if (g_config.status == INVALID) {
        strcpy(text, TXT_ALERT_CONFIG_ERROR);
    } else {
        time_t     nextchg;
        const char *currtz;
        const char *nexttz;
        const char *swline;

        check_date();

        if (g_config.status == DST_ON) {
            nextchg = get_next_rule_time(&g_config.rule_std);
            currtz  = g_config.tzdst;
            nexttz  = g_config.tzstd;
            swline  = TXT_STATUS_SWITCH_BACK_TO " ";
        } else {
            nextchg = get_next_rule_time(&g_config.rule_dst);
            currtz  = g_config.tzstd;
            nexttz  = g_config.tzdst;
            swline  = TXT_STATUS_SWITCH_TO " ";
        }

        strcpy(text, "[0][" TXT_STATUS_CURR_DATE ":  |");
        strcat(text, format_time(FMT_DATETIME, time(NULL)));
        strcat(text, " ");
        strcat(text, currtz);
        strcat(text, "  | |");
        strcat(text, swline);
        strcat(text, nexttz);
        strcat(text, ":  |");
        strcat(text, format_time(FMT_DATETIME, nextchg));
        strcat(text, "  ][" TXT_STATUS_OK_BUTTON "]");
    }

    form_alert(1, text);
}
