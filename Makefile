ifeq (,$(LIBCMINI_DIR))
 LIBCMINI_DIR = /usr/local/lib/libcmini
endif

PREFIX      = m68k-atari-mint-

CSTD        = -std=c99

CFLAGS_2    = -x c -Werror -fno-builtin
LDFLAGS_2   = -Wl,--relax -Wl,--no-whole-archive
STARTUP     =

OPTFLAGS   = -Os

ifneq (,$(LIBCMINI_DIR))
 LIBCMINI_DIR  = /usr/local/lib/libcmini
 CFLAGS_2  += -nostdlib -nodefaultlibs -DUSE_LIBCMINI -I$(LIBCMINI_DIR)/include
 LDFLAGS_2 += -nostdlib -nodefaultlibs -L$(LIBCMINI_DIR)/lib -lgcc -lcmini -lgcc
 STARTUP    = $(LIBCMINI_DIR)/lib/crt0.o
endif

CC	    = $(PREFIX)gcc.exe
AR	    = $(PREFIX)ar.exe
RANLIB	    = $(PREFIX)ranlib.exe
MAKE	    = make --no-print-directory
CP          = cp -a
MV          = mv
RM          = rm -f
MKDIR       = mkdir -p
ECHO		= echo
ECHON		= echo -n	# no linefeed
TOUPPER		= | tr "[:lower:]" "[:upper:]"
TOLOWER		= | tr "[:upper:]" "[:lower:]"
ZIP         = zip -rq
DEPEND      = .depend
CFLAGS      = -Wall -pedantic -fomit-frame-pointer $(CFLAGS_2) $(CSTD)
LDFLAGS	    = -s $(LDFLAGS_2)

LDLIBS_ACC	 = -lgem
LDLIBS_PRG	 =

ACC      = autodst.acc
PRG		 = autodst.prg
INI		 = autodst.ini
ALL_LANG_INC	= $(wildcard lang/*.h)
ALL_LANGUAGES	= $(ALL_LANG_INC:lang/%.h=%)
SRC_ACC	 = accmain.c config.c log.c misc.c rules.c
SRC_PRG	 = prgmain.c config.c log.c misc.c rules.c
LANG_H	 = lang.h

VERSION		= $(file < VERSION)
RELEASEDIR	= release
RELEASEFILE	= autodst-$(VERSION).zip

ifeq (,$(VERSION))
 VERSION = 0
endif

CFLAGS += -DVERSION="\"$(VERSION)\""

ifeq (,$(LANGUAGE))
	EXECDIR = bins
	OBJDIR = objs
	CLEAN = clean-all
else
    EXECDIR = bins/$(LANGUAGE)
	OBJDIR = objs/$(LANGUAGE)
	CLEAN = clean-$(LANGUAGE)
	CFLAGS += -DLANGFILE="\"lang/$(LANGUAGE).h\""
endif

OBJ_ACC	 = $(SRC_ACC:%.c=$(OBJDIR)/%.o)
OBJ_PRG	 = $(SRC_PRG:%.c=$(OBJDIR)/%.o)


.PHONY:	all clean clean-all depend dep release $(ALL_LANGUAGES) $(INI) $(OBJDIR) clean-$(LANGUAGE)
.SILENT:


all:	$(ALL_LANGUAGES)

$(ALL_LANGUAGES):
	$(ECHON) "Building $@: "
	$(MAKE) LANGUAGE=$@ depend-$@ $(ACC) $(PRG) $(INI)
	$(ECHO)

clean:	$(CLEAN)

clean-all:
	for lang in $(ALL_LANGUAGES); do $(MAKE) LANGUAGE=$$lang clean; done

clean-$(LANGUAGE):
	$(ECHON) "Removing object files for $(LANGUAGE). "
	$(RM) $(OBJ_ACC) $(OBJ_PRG)
	$(RM) $(LANG_H)
	$(ECHO)

depend dep:
	$(ECHON) "Generating dependencies. "
	>$(LANG_H)
	$(CC) $(CFLAGS) -MM $(SRC_ACC) $(SRC_PRG) >$(DEPEND)
	$(RM) $(LANG_H)
	$(ECHO)

depend-$(LANGUAGE):
	$(CP) lang/$(LANGUAGE).h $(LANG_H)
	$(CC) $(CFLAGS) -MM $(SRC_ACC) $(SRC_PRG) >$(DEPEND)

release:	all
	$(ECHON) "Creating ZIP for release v$(VERSION). "; \
	$(MKDIR) $(RELEASEDIR) && \
	$(CP) $(ALL_LANGUAGES:%=$(EXECDIR)/%) $(RELEASEDIR) && \
	pushd $(RELEASEDIR) >/dev/null && \
	find -mindepth 1 -type d | while read d; do mv -T $$d `echo $$d $(TOUPPER)`; done && \
	find -type f | while read f; do mv $$f `echo $$f $(TOUPPER)`; done && \
	$(ZIP) $(RELEASEFILE) `echo $(ALL_LANGUAGES) $(TOUPPER)` && \
	$(RM) -r $(ALL_LANGUAGES) && \
	popd >/dev/null && \
	$(ECHO) "-> $(RELEASEDIR)/$(RELEASEFILE)" || \
	$(ECHO) "failed!"

$(ACC):	$(OBJDIR) $(OBJ_ACC)
	$(MKDIR) -p $(EXECDIR)
	$(RM) $(EXECDIR)/$(ACC)
	$(ECHON) "$(ACC) "
	$(CC) $(STARTUP) -o $(EXECDIR)/$(ACC) $(OBJ_ACC) $(LDLIBS_ACC) $(LDFLAGS)

$(PRG): $(OBJDIR) $(OBJ_PRG)
	$(MKDIR) -p $(EXECDIR)
	$(RM) $(EXECDIR)/$(PRG)
	$(ECHON) "$(PRG) "
	$(CC) $(STARTUP) -o $(EXECDIR)/$(PRG) $(OBJ_PRG) $(LDLIBS_PRG) $(LDFLAGS)

$(INI):
	$(ECHON) "$(INI) "
	$(CP) $(INI) $(EXECDIR)

$(OBJDIR):
	$(MKDIR) $(OBJDIR)

$(OBJDIR)/%.o:	%.c
	$(CC) $(CFLAGS) $(OPTFLAGS) -c $< -o $@

-include $(DEPEND)
