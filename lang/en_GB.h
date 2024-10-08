#ifndef LANG_EN_UK_H
#define LANG_EN_UK_H


/**
 * British English
 */


#include "en_US.h"


/**
 * Date and time formats:
 * Date: DD/MM/YYYY
 * Time: 12-hour with trailing AM/PM
 */

#ifdef FMT_DATE
# undef FMT_DATE
#endif /* defined FMT_DATE */
#define FMT_DATE  "%d/%m/%Y"

#ifdef FMT_WD_DATE
# undef FMT_WD_DATE
#endif /* defined FMT_WD_DATE */
#define FMT_WD_DATE  "%a %d/%m/%Y"

#ifdef FMT_TIME
# undef FMT_TIME
#endif /* defined FMT_TIME */
#define FMT_TIME  "%I:%M:%S %p"

#ifdef FMT_DATETIME
# undef FMT_DATETIME
#endif /* defined FMT_DATETIME */
#define FMT_DATETIME  FMT_DATE " " FMT_TIME

#ifdef FMT_WD_DATETIME
# undef FMT_WD_DATETIME
#endif /* defined FMT_WD_DATETIME */
#define FMT_WD_DATETIME  FMT_WD_DATE " " FMT_TIME


/* common */

#ifdef ABBREV_DST
# undef ABBREV_DST
#endif /* defined ABBREV_DST */
#define ABBREV_DST  "ST"

#ifdef ABBREV_STD
# undef ABBREV_STD
#endif /* defined ABBREV_STD */
#define ABBREV_STD  "MT"


/* accmain.c */
#ifdef TXT_ALERT_ACC_AS_PRG
# undef TXT_ALERT_ACC_AS_PRG
#endif /* defined TXT_ALERT_ACC_AS_PRG */
#define TXT_ALERT_ACC_AS_PRG  "[3][ This programme can only be | started as an accessory. ][Cancel]"


/* prgmain.c and log.c */
#ifdef TXT_CURRENT_TIME
# undef TXT_CURRENT_TIME
#endif /* defined TXT_CURRENT_TIME */
#ifdef TXT_BEGINNING_OF_DST
# undef TXT_BEGINNING_OF_DST
#endif /* defined TXT_BEGINNING_OF_DST */
#ifdef TXT_END_OF_DST
# undef TXT_END_OF_DST
#endif /* defined TXT_END_OF_DST */
#ifdef TXT_NEXT_CHANGE
# undef TXT_NEXT_CHANGE
#endif /* defined TXT_NEXT_CHANGE */
#define TXT_CURRENT_TIME      "Current time:             "
#define TXT_BEGINNING_OF_DST  "Beginning of Summer Time: "
#define TXT_END_OF_DST        "End of Summer Time:       "
#define TXT_NEXT_CHANGE       "Next change:              "


/* config.c */
#ifdef TXT_ERR_BEGIN_RULE_MISSING
# undef TXT_ERR_BEGIN_RULE_MISSING
#endif /* defined TXT_ERR_BEGIN_RULE_MISSING */
#ifdef TXT_ERR_END_RULE_MISSING
# undef TXT_ERR_END_RULE_MISSING
#endif /* defined TXT_ERR_END_RULE_MISSING */
#define TXT_ERR_BEGIN_RULE_MISSING  ERROR_PREFIX "Rule for beginning of Summer Time missing." NEWLINE
#define TXT_ERR_END_RULE_MISSING    ERROR_PREFIX "Rule for end of Summer Time missing." NEWLINE


/* rules.c */
#ifdef TXT_SWITCHING_TO_DST
# undef TXT_SWITCHING_TO_DST
#endif /* defined TXT_SWITCHING_TO_DST */
#ifdef TXT_SWITCHING_TO_STD
# undef TXT_SWITCHING_TO_STD
#endif /* defined TXT_SWITCHING_TO_STD */
#define TXT_SWITCHING_TO_DST  "Switching to Summer Time." NEWLINE
#define TXT_SWITCHING_TO_STD  "Switching to Mean Time." NEWLINE


#endif /* LANG_EN_UK_H */
