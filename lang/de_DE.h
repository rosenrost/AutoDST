#ifndef LANG_DE_DE_H
#define LANG_DE_DE_H


/**
 * Standard German (Germany)
 */


/**
 * Date and time formats:
 * Date: DD.MM.YYYY
 * Time: 24-hour
 */
#define FMT_DATE                          "%d.%m.%Y"
#define FMT_WD_DATE                       "%a %d.%m.%Y"
#define FMT_TIME                          "%H:%M:%S"
#define FMT_DATETIME                      FMT_DATE " " FMT_TIME
#define FMT_WD_DATETIME                   FMT_WD_DATE " " FMT_TIME

/* common */
#define ERROR_PREFIX                      "FEHLER: "
#define ABBREV_DST                        "So.z."
#define ABBREV_STD                        "Wi.z."
#define WDAY_SUNDAY                       "Sonntag"
#define WDAY_MONDAY                       "Montag"
#define WDAY_TUESDAY                      "Dienstag"
#define WDAY_WEDNESDAY                    "Mittwoch"
#define WDAY_THURSDAY                     "Donnerstag"
#define WDAY_FRIDAY                       "Freitag"
#define WDAY_SATURDAY                     "Samstag"
#define WDAY_SUN_ABBR                     "So"
#define WDAY_MON_ABBR                     "Mo"
#define WDAY_TUE_ABBR                     "Di"
#define WDAY_WED_ABBR                     "Mi"
#define WDAY_THU_ABBR                     "Do"
#define WDAY_FRI_ABBR                     "Fr"
#define WDAY_SAT_ABBR                     "Sa"
#define MDAY_JANUARY                      "Januar"
#define MDAY_FEBRUARY                     "Februar"
#define MDAY_MARCH                        "MÑrz"
#define MDAY_APRIL                        "April"
#define MDAY_MAY                          "Mai"
#define MDAY_JUNE                         "Juni"
#define MDAY_JULY                         "Juli"
#define MDAY_AUGUST                       "August"
#define MDAY_SEPTEMBER                    "September"
#define MDAY_OCTOBER                      "Oktober"
#define MDAY_NOVEMBER                     "November"
#define MDAY_DECEMBER                     "Dezember"
#define MDAY_JAN_ABBR                     "Jan"
#define MDAY_FEB_ABBR                     "Feb"
#define MDAY_MAR_ABBR                     "Mrz"
#define MDAY_APR_ABBR                     "Apr"
#define MDAY_MAY_ABBR                     "Mai"
#define MDAY_JUN_ABBR                     "Jun"
#define MDAY_JUL_ABBR                     "Jul"
#define MDAY_AUG_ABBR                     "Aug"
#define MDAY_SEP_ABBR                     "Sep"
#define MDAY_OCT_ABBR                     "Okt"
#define MDAY_NOV_ABBR                     "Nov"
#define MDAY_DEC_ABBR                     "Dez"

/* accmain.c */
#define TXT_STARTING_ACC                  "Starte AUTODST.ACC." NEWLINE
#define TXT_STATUS_CURR_DATE              "Aktuelles Datum"
#define TXT_STATUS_SWITCH_TO              "Umstellung auf"
#define TXT_STATUS_SWITCH_BACK_TO         "RÅckstellung auf"
#define TXT_STATUS_OK_BUTTON              "  OK  "
#define TXT_ERR_AES_INIT                  ERROR_PREFIX "AES-Initialisierung fehlgeschlagen" NEWLINE
#define TXT_ERR_ACC_REGISTER              ERROR_PREFIX "Accessory kann nicht registriert werden." NEWLINE
#define TXT_ALERT_ACC_AS_PRG              "[3][ Dieses Programm kann nur als | Accessory gestart werden. ][Abbruch]"
#define TXT_ALERT_CONFIG_ERROR            "[1][ Konfigurationsfehler | | Siehe " LOGFILE " | fÅr Meldungen. ][Abbruch]"

/* accmain.c and prgmain.c */
#define TXT_CONFIG_LOADED                 "Einstellungen erfolgreich gelesen." NEWLINE

/* prgmain.c */
#define TXT_EXECUTING_PRG                 "FÅhre " AUTO_PRG " aus." NEWLINE
#define TXT_ERR_CONFIG_NOT_LOADED         "Einstellungen kînnen nicht gelesen werden. Fehlermeldungen in @:\\" LOGFILE "." NEWLINE

/* prgmain.c and log.c */
#define TXT_CURRENT_TIME                  "Aktuelle Zeit:       "
#define TXT_BEGINNING_OF_DST              "Anfang Sommerzeit:   "
#define TXT_END_OF_DST                    "RÅckkehr Winterzeit: "
#define TXT_NEXT_CHANGE                   "NÑchste Umstellung:  "

/* config.c */
#define TXT_ERR_CONFIG_NOT_READ           ERROR_PREFIX "Konfigurationsdatei " INI " kann nicht gelesen werden." NEWLINE
#define TXT_ERR_SYNTAX_ERROR              ERROR_PREFIX "Syntax-Fehler in Zeile "
#define TXT_ERR_BEGIN_RULE_MISSING        ERROR_PREFIX "Regel fÅr Anfang der Sommerzeit fehlt." NEWLINE
#define TXT_ERR_END_RULE_MISSING          ERROR_PREFIX "Regel fÅr Ende der Sommerzeit fehlt." NEWLINE
#define TXT_ERR_INVALID_STATUS_VAL        ERROR_PREFIX "UngÅltiger Status-Wert in Zeile "
#define TXT_ERR_INVALID_WEEKDAY           ERROR_PREFIX "UngÅltiger Wochentag"
#define TXT_ERR_INVALID_WEEKDAY_IND       ERROR_PREFIX "UngÅltiges Wochentag-Kennzeichen"
#define TXT_ERR_INVALID_MONTH             ERROR_PREFIX "UngÅltiger Monat"
#define TXT_ERR_INVALID_MINUTE_VAL        ERROR_PREFIX "UngÅltige Minutenangabe"
#define TXT_ERR_MISSING_MINUTE_VAL        ERROR_PREFIX "Fehlende Minutenangabe"
#define TXT_ERR_TOO_MANY_VALUES           ERROR_PREFIX "Zu viele Werte"
#define TXT_ERR_IN_RULE_IN_LINE           " in Regel in Zeile "

/* rules.c */
#define TXT_SWITCHING_TO_DST              "Stelle auf Sommerzeit." NEWLINE
#define TXT_SWITCHING_TO_STD              "Stelle auf Winterzeit." NEWLINE
#define TXT_ERR_CANNOT_CREATE_CONFIG_BAK  ERROR_PREFIX "Kann " INI_BAK " nicht anlegen." NEWLINE
#define TXT_ERR_CANNOT_CREATE_NEW_CONFIG  ERROR_PREFIX "Kann neue " INI " nicht anlegen." NEWLINE
#define TXT_ERR_CANNOT_CREATE_CFG         ERROR_PREFIX "Kann " CFG " nicht anlegen." NEWLINE


#endif /* LANG_DE_DE_H */
