#include <ctype.h>
#include <string.h>
#include "autodst.h"


static const char* m_varname[] = { VARNAME_STATUS, VARNAME_FROM, VARNAME_TO, VARNAME_TZSTD, VARNAME_TZDST };
static IniFile g_ini = { 0, NULL };


char* g_ini_file = "@:\\" INI;
char* g_ini_bak  = "@:\\" INI_BAK;
char* g_cfg_file = "@:\\" CFG;

Config g_config =
{
    INVALID,
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    "STD",
    "DST"
};


static Status get_status(const char* val);
static const Rule* set_rule(const char* val, Rule* rule);
static int ini_changed(void);
static int read_cfg_file(void);
static void write_cfg_file(void);


int read_config()
{
    long hdl;
    int  ret = 1;

    if (*g_ini_file < 'A') {
        *g_ini_file = BOOTDRIVE;
        *g_ini_bak  = BOOTDRIVE;
        *g_cfg_file = BOOTDRIVE;
    }

    if (ini_changed()) {
        hdl = Fopen(g_ini_file, FO_READ);

        if (hdl < 0) {
            write_log(TXT_ERR_CONFIG_NOT_READ);
            ret = 0;
        } else {
            char buf[256];

            g_ini.line = 0;
            g_ini.text = buf;

            while (read_line(buf, sizeof(buf) - 1, hdl) != NULL) {
                char* line = skipws(buf);

                ++g_ini.line;

                if (*line != '\0' && *line != '#') {
                    char* crlf;

                    /* Remove trailing CRLF */
                    crlf = strpbrk(line, NEWLINE);
                    if (crlf != NULL) {
                        *crlf = '\0';
                    }

                    if (*line != '\0') {
                        Rule* rule = NULL;
                        char  val[256];
                        int   var;

                        var = get_value(buf, val);

                        switch (var) {
                            case VAR_BEGIN:
                                rule = &g_config.rule_from;
                                break;
                            case VAR_END:
                                rule = &g_config.rule_to;
                                break;
                            default:
                                break;
                        }

                        if (rule != NULL) {
                            if (set_rule(val, rule) == NULL) {
                                ret = 0;
                            }
                        } else {
                            switch (var) {
                                case VAR_STATUS:
                                    g_config.status = get_status(val);
                                    if (g_config.status == INVALID) {
                                        ret = 0;
                                    }
                                    break;
                                case VAR_TZSTD:
                                    strncpy(g_config.tzstd, val, MAXLEN_TZ);
                                    g_config.tzstd[MAXLEN_TZ] = '\0';
                                    break;
                                case VAR_TZDST:
                                    strncpy(g_config.tzdst, val, MAXLEN_TZ);
                                    g_config.tzdst[MAXLEN_TZ] = '\0';
                                    break;
                                    break;
                                default:
                                    write_log(TXT_ERR_SYNTAX_ERROR);
                                    write_log(int2str(g_ini.line));
                                    write_log(":" NEWLINE);
                                    write_log(g_ini.text);
                                    write_log(NEWLINE);
                                    break;
                            }
                        }
                    }
                }
            }

            Fclose(hdl);

            if (ret) {
                if (!g_config.rule_from.found) {
                    ret = 0;
                    write_log(TXT_ERR_BEGIN_RULE_MISSING);
                }

                if (!g_config.rule_to.found) {
                    ret = 0;
                    write_log(TXT_ERR_END_RULE_MISSING);
                }
            }
        }
    } else {
        ret = read_cfg_file();
    }

    if (ret) {
        g_config.rule_from.next_change = get_next_change(&g_config.rule_from);
        g_config.rule_to.next_change   = get_next_change(&g_config.rule_to);
        g_next_change                  = get_next_change(NULL);
        write_cfg_file();
    }

    return ret;
}


int get_value(const char* line, char* dest)
{
    int ret = -1;

    for (int i = 0; i < ARRAYSIZE(m_varname); ++i) {
        const char* var    = m_varname[i];
        size_t      varlen = strlen(var);

        if (strncasecmp(line, var, varlen) == 0) {
            ret  = i;
            line = skipws(line + varlen);

            if (*line == '=') {
                strcpy(dest, skipws(line + 1));
            } else {
                ret = -1;
            }

            break;
        }
    }

    return ret;
}


Status get_status(const char* val)
{
    Status ret = INVALID;

    if (*val == '0' || *val == '1') {
        const char* end = skipws(val + 1);

        if (*end == '\0') {
            ret = *val - '0';
        }
    }

    if (ret < 0) {
        write_log(TXT_ERR_INVALID_STATUS_VAL);
        write_log(int2str(g_ini.line));
        write_log(":" NEWLINE);
        write_log(val);
        write_log(NEWLINE);
    }

    return ret;
}


const Rule* set_rule(const char* val, Rule* rule)
{
    rule->found = 0;

    for (int i = 0; i < ARRAYSIZE(g_conf_wday); ++i) {
        const char* wday    = g_conf_wday[i];
        size_t      wdaylen = strlen(wday);

        if (strncasecmp(val, wday, wdaylen) == 0) {
            rule->weekday = i;
            val += wdaylen;
            break;
        }
    }

    if (rule->weekday < 0) {
        write_log(TXT_ERR_INVALID_WEEKDAY);
        goto rule_error;
    } else {
        if (*val == '\0' || isspace(*val)) {
            rule->which = 1;
        } else {
            int fromlast = 0;

            switch (*val) {
                case '-':
                    fromlast = 1;
                    /* FALLTHROUGH */
                case '+':
                    ++val;
                    break;
                default:
                    break;
            }

            switch (*val) {
                case '1':
                case '2':
                case '3':
                case '4':
                    rule->which = *val++ - '0';
                    if (fromlast) {
                        rule->which = -rule->which;
                    }
                    break;
                default:
                    write_log(TXT_ERR_INVALID_WEEKDAY_IND);
                    goto rule_error;
            }
        }

        val = skipws(val);

        for (int i = 0; i < ARRAYSIZE(g_conf_mday); ++i) {
            const char* mname    = g_conf_mday[i];
            size_t      mnamelen = strlen(mname);

            if (strncasecmp(val, mname, mnamelen) == 0) {
                rule->month = i;
                val = skipws(val + mnamelen);
                break;
            }
        }

        if (rule->month < 0) {
            write_log(TXT_ERR_INVALID_MONTH);
            goto rule_error;
        } else {
            rule->hour = 0;

            while (isdigit(*val)) {
                rule->hour = rule->hour * 10 + (*val++ - '0');
            }

            if (*val++ == ':') {
                rule->minute = 0;

                while (isdigit(*val)) {
                    rule->minute = rule->minute * 10 + (*val++ - '0');
                }

                if (rule->minute > 59) {
                    rule->minute = -1;
                } else if (isspace(*val)) {
                    val = skipws(val);

                    if (*val != '\0') {
                        write_log(TXT_ERR_TOO_MANY_VALUES);
                        goto rule_error;
                    }
                } else if (*val != '\0') {
                    rule->minute = -1;
                }

                if (rule->minute < 0) {
                    write_log(TXT_ERR_INVALID_MINUTE_VAL);
                    goto rule_error;
                } else {
                    rule->found = 1;
                }
            } else {
                write_log(TXT_ERR_MISSING_MINUTE_VAL);
                goto rule_error;
            }
        }
    }

    return rule;

rule_error:

    write_log(TXT_ERR_IN_RULE_IN_LINE);
    write_log(int2str(g_ini.line));
    write_log(":" NEWLINE);
    write_log(g_ini.text);
    write_log(NEWLINE);

    return NULL;
}


int ini_changed()
{
    _DTA  dta;
    _DTA* olddta = Fgetdta();
    int   ret    = 1;

    Fsetdta(&dta);

    if (Fsfirst(g_ini_file, ~FA_LABEL & ~FA_DIR) == 0) {
        unsigned short ini_date = dta.dta_date;
        unsigned short ini_time = dta.dta_time;

        if (Fsfirst(g_cfg_file, ~FA_LABEL & ~FA_DIR) == 0) {
            unsigned short cfg_date = dta.dta_date;
            unsigned short cfg_time = dta.dta_time;

            if (ini_date < cfg_date) {
                ret = 0;
            } else if (ini_date == cfg_date && ini_time < cfg_time) {
                ret = 0;
            }
        }
    }

    Fsetdta(olddta);

    return ret;
}


int read_cfg_file()
{
    long hdl = Fopen(g_cfg_file, FO_READ);

    if (hdl < 0) {
        return 0;
    } else {
        long cnt = Fread(hdl, sizeof(g_config), &g_config);
        Fclose(hdl);

        if (cnt != sizeof(g_config)) {
            return 0;
        }
    }

    return 1;
}


void write_cfg_file()
{
    long hdl = Fcreate(g_cfg_file, 0);

    if (hdl < 0) {
        write_log(TXT_ERR_CANNOT_CREATE_CFG);
    } else {
        long cnt = Fwrite(hdl, sizeof(g_config), &g_config);
        Fclose(hdl);

        if (cnt != sizeof(g_config)) {
            Fdelete(g_cfg_file);
        }
    }
}


void write_new_status()
{
    Fdelete(g_ini_bak);

    if (Frename(0, g_ini_file, g_ini_bak) == 0) {
        long inhdl = Fopen(g_ini_bak, FO_READ);

        if (inhdl < 0) {
            write_log(TXT_ERR_CANNOT_CREATE_CONFIG_BAK);
            (void)Frename(0, g_ini_bak, g_ini_file);
        } else {
            long outhdl = Fcreate(g_ini_file, 0);

            if (outhdl < 0) {
                write_log(TXT_ERR_CANNOT_CREATE_NEW_CONFIG);
                Fclose(inhdl);
                (void)Frename(0, g_ini_bak, g_ini_file);
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

                            ststr[0] += g_config.status;

                            strcpy(buf, "status=");
                            strcat(buf, ststr);
                            strcat(buf, NEWLINE);
                        }

                        Fwrite(outhdl, strlen(buf), buf);
                    }
                }

                Fclose(outhdl);
                Fclose(inhdl);

                write_cfg_file();
            }
        }

        Fdelete(g_ini_bak);
    } else {
        write_log(TXT_ERR_CANNOT_CREATE_CONFIG_BAK);
    }
}
