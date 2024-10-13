#include <ctype.h>
#include <string.h>
#include "autodst.h"


static const char* m_varname[] = { VARNAME_STATUS, VARNAME_FROM, VARNAME_TO, VARNAME_TZSTD, VARNAME_TZDST };


Config g_config      = { 0, NULL };
char*  g_config_file = "@:\\" CONFIG;
char*  g_config_bak  = "@:\\" CONFIG_BAK;


static Status get_status(const char* val);
static const Rule* set_rule(const char* val, Rule* rule);


int read_config()
{
    long hdl;
    int  ret = 1;

    *g_config_file = BOOTDRIVE;
    *g_config_bak  = BOOTDRIVE;

    hdl = Fopen(g_config_file, FO_READ);

    if (hdl < 0) {
        write_log(TXT_ERR_CONFIG_NOT_READ);
        ret = 0;
    } else {
        char buf[256];

        g_config.line = 0;
        g_config.text = buf;

        while (read_line(buf, sizeof(buf) - 1, hdl) != NULL) {
            char* line = skipws(buf);

            ++g_config.line;

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
                            rule = &g_rule_from;
                            break;
                        case VAR_END:
                            rule = &g_rule_to;
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
                                g_status = get_status(val);
                                if (g_status == INVALID) {
                                    ret = 0;
                                }
                                break;
                            case VAR_TZSTD:
                                strncpy(g_tzstd, val, MAXLEN_TZ);
                                g_tzstd[MAXLEN_TZ] = '\0';
                                break;
                            case VAR_TZDST:
                                strncpy(g_tzdst, val, MAXLEN_TZ);
                                g_tzdst[MAXLEN_TZ] = '\0';
                                break;
                                break;
                            default:
                                write_log(TXT_ERR_SYNTAX_ERROR);
                                write_log(int2str(g_config.line));
                                write_log(":" NEWLINE);
                                write_log(g_config.text);
                                write_log(NEWLINE);
                                break;
                        }
                    }
                }
            }
        }

        Fclose(hdl);

        if (ret) {
            if (!g_rule_from.found) {
                ret = 0;
                write_log(TXT_ERR_BEGIN_RULE_MISSING);
            }

            if (!g_rule_to.found) {
                ret = 0;
                write_log(TXT_ERR_END_RULE_MISSING);
            }

            if (ret) {
                g_rule_from.next_change = get_next_change(&g_rule_from);
                g_rule_to.next_change   = get_next_change(&g_rule_to);
                g_next_change           = get_next_change(NULL);
            }
        }
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
        write_log(int2str(g_config.line));
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
    write_log(int2str(g_config.line));
    write_log(":" NEWLINE);
    write_log(g_config.text);
    write_log(NEWLINE);

    return NULL;
}
