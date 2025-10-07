#ifndef LANG_EN_US_H
#define LANG_EN_US_H


/**
 * American English
 */


/**
 * Date and time formats:
 * Date: Mon DD YYYY
 * Time: 12-hour with trailing AM/PM
 */
#define FMT_DATE                          "%b %d %Y"
#define FMT_WD_DATE                       "%a %b %d %Y"
#define FMT_TIME                          "%I:%M:%S %p"
#define FMT_DATETIME                      FMT_DATE " " FMT_TIME
#define FMT_WD_DATETIME                   FMT_WD_DATE " " FMT_TIME

/* common */
#define ERROR_PREFIX                      "ERROR: "
#define ABBREV_DST                        "DST"
#define ABBREV_STD                        "STD"
#define WDAY_SUNDAY                       "Sunday"
#define WDAY_MONDAY                       "Monday"
#define WDAY_TUESDAY                      "Tuesday"
#define WDAY_WEDNESDAY                    "Wednesday"
#define WDAY_THURSDAY                     "Thursday"
#define WDAY_FRIDAY                       "Friday"
#define WDAY_SATURDAY                     "Saturday"
#define WDAY_SUN_ABBR                     "Sun"
#define WDAY_MON_ABBR                     "Mon"
#define WDAY_TUE_ABBR                     "Tue"
#define WDAY_WED_ABBR                     "Wed"
#define WDAY_THU_ABBR                     "Thu"
#define WDAY_FRI_ABBR                     "Fri"
#define WDAY_SAT_ABBR                     "Sat"
#define MDAY_JANUARY                      "January"
#define MDAY_FEBRUARY                     "February"
#define MDAY_MARCH                        "March"
#define MDAY_APRIL                        "April"
#define MDAY_MAY                          "May"
#define MDAY_JUNE                         "June"
#define MDAY_JULY                         "July"
#define MDAY_AUGUST                       "August"
#define MDAY_SEPTEMBER                    "September"
#define MDAY_OCTOBER                      "October"
#define MDAY_NOVEMBER                     "November"
#define MDAY_DECEMBER                     "December"
#define MDAY_JAN_ABBR                     "Jan"
#define MDAY_FEB_ABBR                     "Feb"
#define MDAY_MAR_ABBR                     "Mar"
#define MDAY_APR_ABBR                     "Apr"
#define MDAY_MAY_ABBR                     "May"
#define MDAY_JUN_ABBR                     "Jun"
#define MDAY_JUL_ABBR                     "Jul"
#define MDAY_AUG_ABBR                     "Aug"
#define MDAY_SEP_ABBR                     "Sep"
#define MDAY_OCT_ABBR                     "Oct"
#define MDAY_NOV_ABBR                     "Nov"
#define MDAY_DEC_ABBR                     "Dec"

/* accmain.c */
#define TXT_STARTING_ACC                  "Starting AUTODST.ACC." NEWLINE
#define TXT_STATUS_CURR_DATE              "Current date"
#define TXT_STATUS_SWITCH_TO              "Switch to"
#define TXT_STATUS_SWITCH_BACK_TO         "Switch back to"
#define TXT_STATUS_OK_BUTTON              "  OK  "
#define TXT_ERR_AES_INIT                  ERROR_PREFIX "AES initialization failed" NEWLINE
#define TXT_ERR_ACC_REGISTER              ERROR_PREFIX "Accessory cannot be registered." NEWLINE
#define TXT_ALERT_ACC_AS_PRG              "[3][ This program can only be | started as an accessory. ][Cancel]"
#define TXT_ALERT_CONFIG_ERROR            "[1][ Configuration error | | See " LOGFILE " | for details. ][Cancel]"

/* accmain.c and prgmain.c */
#define TXT_CONFIG_LOADED                 "Configuration successfully loaded." NEWLINE

/* prgmain.c */
#define TXT_EXECUTING_PRG                 "Executing " AUTO_PRG "." NEWLINE
#define TXT_ERR_CONFIG_NOT_LOADED         "Configuration could not be loaded. See @:\\" LOGFILE " for error messages." NEWLINE

/* prgmain.c and log.c */
#define TXT_CURRENT_TIME                  "Current time:     "
#define TXT_BEGINNING_OF_DST              "Beginning of DST: "
#define TXT_END_OF_DST                    "End of DST:       "
#define TXT_NEXT_CHANGE                   "Next change:      "

/* config.c */
#define TXT_ERR_CONFIG_NOT_READ           ERROR_PREFIX "Configuration file " INI " cannot be read." NEWLINE
#define TXT_ERR_SYNTAX_ERROR              ERROR_PREFIX "Syntax error in configuration line "
#define TXT_ERR_BEGIN_RULE_MISSING        ERROR_PREFIX "Rule for beginning of DST missing." NEWLINE
#define TXT_ERR_END_RULE_MISSING          ERROR_PREFIX "Rule for end of DST missing." NEWLINE
#define TXT_ERR_INVALID_STATUS_VAL        ERROR_PREFIX "Invalid status value in line "
#define TXT_ERR_INVALID_WEEKDAY           ERROR_PREFIX "Invalid weekday"
#define TXT_ERR_INVALID_WEEKDAY_IND       ERROR_PREFIX "Invalid weekday indicator"
#define TXT_ERR_INVALID_MONTH             ERROR_PREFIX "Invalid month"
#define TXT_ERR_INVALID_MINUTE_VAL        ERROR_PREFIX "Invalid minute value"
#define TXT_ERR_MISSING_MINUTE_VAL        ERROR_PREFIX "Missing minute value"
#define TXT_ERR_TOO_MANY_VALUES           ERROR_PREFIX "Too many values"
#define TXT_ERR_IN_RULE_IN_LINE           " in rule in line "

/* rules.c */
#define TXT_SWITCHING_TO_DST              "Switching to DST." NEWLINE
#define TXT_SWITCHING_TO_STD              "Switching to STD." NEWLINE
#define TXT_ERR_CANNOT_CREATE_CONFIG_BAK  ERROR_PREFIX "Cannot create " INI_BAK "." NEWLINE
#define TXT_ERR_CANNOT_CREATE_NEW_CONFIG  ERROR_PREFIX "Cannot create new " INI "." NEWLINE
#define TXT_ERR_CANNOT_CREATE_CFG         ERROR_PREFIX "Cannot create " CFG "." NEWLINE


#endif /* LANG_EN_US_H */
