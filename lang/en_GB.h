#ifndef LANG_EN_UK_H
#define LANG_EN_UK_H


/**
 * British English
 */


#include "lang/en_US.h"


/**
 * Date and time formats:
 * Date: DD/MM/YYYY
 * Time: 12-hour with trailing AM/PM
 */

#undef FMT_DATE
#define FMT_DATE  "%d/%m/%Y"

#undef FMT_WD_DATE
#define FMT_WD_DATE  "%a %d/%m/%Y"

#undef FMT_TIME
#define FMT_TIME  "%I:%M:%S %p"

#undef FMT_DATETIME
#define FMT_DATETIME  FMT_DATE " " FMT_TIME

#undef FMT_WD_DATETIME
#define FMT_WD_DATETIME  FMT_WD_DATE " " FMT_TIME


/* common */

#undef ABBREV_DST
#define ABBREV_DST  "ST"

#undef ABBREV_STD
#define ABBREV_STD  "MT"


/* accmain.c */
#undef TXT_ALERT_ACC_AS_PRG
#define TXT_ALERT_ACC_AS_PRG  "[3][ This programme can only be | started as an accessory. ][Cancel]"


/* prgmain.c and log.c */
#undef TXT_CURRENT_TIME
#define TXT_CURRENT_TIME      "Current time:             "

#undef TXT_BEGINNING_OF_DST
#define TXT_BEGINNING_OF_DST  "Beginning of Summer Time: "

#undef TXT_END_OF_DST
#define TXT_END_OF_DST        "End of Summer Time:       "

#undef TXT_NEXT_CHANGE
#define TXT_NEXT_CHANGE       "Next change:              "


/* config.c */
#undef TXT_ERR_BEGIN_RULE_MISSING
#define TXT_ERR_BEGIN_RULE_MISSING  ERROR_PREFIX "Rule for beginning of Summer Time missing." NEWLINE

#undef TXT_ERR_END_RULE_MISSING
#define TXT_ERR_END_RULE_MISSING    ERROR_PREFIX "Rule for end of Summer Time missing." NEWLINE


/* rules.c */
#undef TXT_SWITCHING_TO_DST
#define TXT_SWITCHING_TO_DST  "Switching to Summer Time." NEWLINE

#undef TXT_SWITCHING_TO_STD
#define TXT_SWITCHING_TO_STD  "Switching to Mean Time." NEWLINE


#endif /* LANG_EN_UK_H */
