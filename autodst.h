#ifndef DST_H
#define DST_H


#ifndef LANGFILE
#define LANGFILE  "lang/en_US.h"
#endif /* !defined LANGFILE */


#include <time.h>
#include LANGFILE


/* C library */
extern short _app;


#define ARRAYSIZE(x)    (sizeof(x) / sizeof(x[0]))
#define NEWLINE         "\r\n"
#define IS_NEWLINE(c)   ((c) == '\r' || (c) == '\n')
#define AUTO_PRG        "AUTODST.PRG"
#define CONFIG          "AUTODST.INI"
#define CONFIG_BAK      "AUTODST.BAK"
#define LOGFILE         "AUTODST.LOG"
#define MENU_TITLE      "Auto DST"
#define VARNAME_STATUS  "status"
#define VARNAME_FROM    "from"
#define VARNAME_TO      "to"
#define VARNAME_TZSTD   "tzstd"
#define VARNAME_TZDST   "tzdst"
#define MAXLEN_TZ       31
#define BOOTDRIVE       ('A' + Dgetdrv())


typedef struct
{
    unsigned int weekday;
    #define SUNDAY    0
    #define MONDAY    1
    #define TUESDAY   2
    #define WEDNESDAY 3
    #define THURSDAY  4
    #define FRIDAY    5
    #define SATURDAY  6

    int which;

    unsigned int month;
    #define JANUARY   0
    #define FEBRUARY  1
    #define MARCH     2
    #define APRIL     3
    #define MAY       4
    #define JUNE      5
    #define JULY      6
    #define AUGUST    7
    #define SEPTEMBER 8
    #define OCTOBER   9
    #define NOVEMBER  10
    #define DECEMBER  11

    unsigned int hour;
    unsigned int minute;
    unsigned int found;
    time_t next_change;
} Rule;

typedef struct
{
     unsigned int line;
     const char* text;
} Config;


#define DST_OFF  0
#define DST_ON   1
#define INVALID  0xff
typedef unsigned char Status;
extern Status g_status;

extern char g_tzstd[MAXLEN_TZ + 1];
extern char g_tzdst[MAXLEN_TZ + 1];

extern Rule g_rule_from;
extern Rule g_rule_to;

extern long g_loghdl;

extern time_t g_next_change;

extern const char* g_conf_wday[7];
extern const char* g_conf_mday[12];

enum { VAR_STATUS, VAR_BEGIN, VAR_END, VAR_TZSTD, VAR_TZDST };

extern Config g_config;

extern char*  g_config_file;
extern char*  g_config_bak;
extern char*  g_logfile;


void update_clock(Status new_status);
time_t get_rule_time(const Rule* rule, int year);
time_t get_next_change(const Rule* rule);
int read_config(void);
int get_value(const char* line, char* dest);
char* skipws(const char* s);
char* read_line(char* s, long n, long handle);
const char* format_time(const char *fmt, time_t t);

void close_log(void);
void write_log(const char* s);
void write_config_log(void);
void write_rules_log(void);
const char* int2str(int n);


#endif /* DST_H */
