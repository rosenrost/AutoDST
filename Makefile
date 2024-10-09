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


all:	$(ALL_LANGUAGES)

$(ALL_LANGUAGES):
	@echo "Building $@."
	@$(MAKE) LANGUAGE=$@ $(ACC) $(PRG) $(INI)
	@echo "Done."

clean:	$(CLEAN)

clean-all:
	@for lang in $(ALL_LANGUAGES); do $(MAKE) LANGUAGE=$$lang clean; done

clean-$(LANGUAGE):
	@echo "Removing object files and executables for $(LANGUAGE)."
	@$(RM) $(OBJ_ACC) $(OBJ_PRG)
	@$(RM) -r $(EXECDIR)

depend dep:
	@echo "Generating dependencies."
	@$(CC) $(CFLAGS) -MM $(SRC_ACC) $(SRC_PRG) >$(DEPEND)

release:	all
	@echo "Creating ZIP for release v$(VERSION)"; \
	$(MKDIR) $(RELEASEDIR) && \
	$(CP) $(ALL_LANGUAGES:%=$(EXECDIR)/%) $(RELEASEDIR) && \
	pushd $(RELEASEDIR) >/dev/null && \
	$(ZIP) $(RELEASEFILE) $(ALL_LANGUAGES) && \
	$(RM) -r $(ALL_LANGUAGES) && \
	popd >/dev/null && \
	echo "$(RELEASEDIR)/$(RELEASEFILE) successfully created." || \
	echo "Failed!"

$(ACC):	$(OBJDIR) $(OBJ_ACC)
	@$(MKDIR) -p $(EXECDIR)
	@echo " Linking $(ACC)."
	@$(CC) $(STARTUP) -o $(EXECDIR)/$(ACC) $(OBJ_ACC) $(LDLIBS_ACC) $(LDFLAGS)

$(PRG): $(OBJDIR) $(OBJ_PRG)
	@$(MKDIR) -p $(EXECDIR)
	@echo " Linking $(PRG)."
	@$(CC) $(STARTUP) -o $(EXECDIR)/$(PRG) $(OBJ_PRG) $(LDLIBS_PRG) $(LDFLAGS)

$(INI):
	@echo " Copying $(INI)."
	@$(CP) $(INI) $(EXECDIR)

$(OBJDIR):
	@$(MKDIR) $(OBJDIR)

$(OBJDIR)/%.o:	%.c
	@$(CC) $(CFLAGS) $(OPTFLAGS) -c $< -o $@

-include $(DEPEND)
