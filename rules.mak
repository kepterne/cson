
CLEANRELEASESOURCES:= $(patsubst %.c,./release/%.o,$(CLEANSOURCES))
CLEANDEBUGSOURCES:= $(patsubst %.c,./debug/%.o,$(CLEANSOURCES))

ifdef NOLIBS
SRCT = $(SOURCES) 
SRCT += $(LIBSOURCES)
RELEASE_OBJS = $(patsubst %.c,./release/%.o,$(SRCT))
else
RELEASE_OBJS = $(patsubst %.c,./release/%.o,$(SOURCES))
endif
DEBUGSOURCES = $(SOURCES)
DEBUGSOURCES += $(LIBSOURCES)
DEBUG_OBJS = $(patsubst %.c,./debug/%.o,$(DEBUGSOURCES)) 

LIBOBJS = $(patsubst %.c,./release/%.o,$(LIBSOURCES))
LIBOBJSDBG = $(patsubst %.c,./debug/%.o,$(LIBSOURCES))


./debug/%.o: %.c | debug_dir 
	@mkdir -p $(dir $@)
	@printf "Compiling\t$(BOLD)%s$(NORM)\r\n\tto\t$(BLUE)%s$(NORM)\r\n" $? $@
	@$(CC) $(CFLAGS_DEBUG) $(CFLAGS) -c $? -o $@
	@printf '%50s\n' | tr ' ' -

./release/%.o: %.c | release_dir 
	@mkdir -p $(dir $@)
	@printf "Compiling\t$(BOLD)%s$(NORM)\r\n\tto\t$(BLUE)%s$(NORM)\r\n" $? $@
	@$(CC) $(CFLAGS_RELEASE) $(CFLAGS) -c $? -o $@
	@printf '%50s\n' | tr ' ' -

shared:

debug: $(DEBUG_OBJS)  bin debug_dir deploy_dir
	@printf "Linking $(BOLD) ./bin/$(BIN)_dbg $(NORM) in DEBUG mode\r\n"
	@$(CC) $(CFLAGS) $(CFLAGS_DEBUG) $(DEBUG_OBJS) -o ./bin/$(BIN)_dbg $(LIBS)
	@printf '%50s\n' | tr ' ' =


ifdef NOLIBS
release: $(RELEASE_OBJS) bin release_dir shared deploy_dir
	@printf "Linking $(BOLD) $(BINDIR)/$(BIN) $(NORM) in RELEASE mode\r\n"
	@sudo $(CC) $(CFLAGS) $(CFLAGS_RELEASE) \
		$(RELEASE_OBJS)   -o $(BINDIR)/$(BIN) $(LIBS)
	@sudo strip $(BINDIR)/$(BIN) 
	@printf '%50s\n' | tr ' ' =

else
release: $(RELEASE_OBJS) bin release_dir shared deploy_dir
	@printf "Linking $(BOLD) $(BINDIR)/$(BIN) $(NORM) in RELEASE mode\r\n"
	@sudo $(CC) $(CFLAGS) $(CFLAGS_RELEASE) \
		-Wl,-rpath,$(DEPLOYDIR) $(DEPLOYDIR)/lib$(LIBNAME).so.$(LIBVER) \
		$(RELEASE_OBJS)   -o $(BINDIR)/$(BIN) $(LIBS) $(DEPLOYDIR)/lib$(LIBNAME).so.$(LIBVER) 
	@sudo strip $(BINDIR)/$(BIN) 
	@printf '%50s\n' | tr ' ' =
endif

clear:
	@sudo rm -vfr *~ ./bin/$(BIN)_dbg $(BINDIR)/$(BIN) $(CLEANRELEASESOURCES) $(CLEANRELEASESOURCES) 
	@printf '%50s\n' | tr ' ' =
	@printf "CLEAR COMPLETE\n"
	@printf '%50s\n' | tr ' ' =

clean:
	@sudo rm -vfr *~ ./bin/$(BIN)_dbg $(BINDIR)/$(BIN) $(RELEASE_OBJS) $(DEBUG_OBJS) 
	@printf '%50s\n' | tr ' ' =
	@printf "CLEAN COMPLETE\n"
	@printf '%50s\n' | tr ' ' =

all: debug release shared
