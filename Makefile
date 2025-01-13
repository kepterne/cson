include cson.mak

BIN=cson-tool

SOURCES = cson-tool.c

LIBS = -lpthread -lm

include rules.mak

all: debug release shared

shared: $(LIBOBJS) bin release_dir deploy_dir
	@printf "Linking $(BOLD) $(DEPLOYDIR)/lib$(LIBNAME).so.$(LIBVER)  $(NORM) in RELEASE mode\r\n"
	@sudo $(CC) -shared -fPIC -Wl,-soname,lib$(LIBNAME).so.$(LIBVER) -o $(DEPLOYDIR)/lib$(LIBNAME).so.$(LIBVER) $(CFLAGS) $(LIBOBJS)   $(LIBS) -lc
	@printf "Stripping $(BOLD) $(DEPLOYDIR)/lib$(LIBNAME).so.$(LIBVER)  $(NORM)\r\n"
	@sudo strip $(DEPLOYDIR)/lib$(LIBNAME).so.$(LIBVER) 
	@sudo cp $(DEPLOYDIR)/lib$(LIBNAME).so.$(LIBVER) ../bin
	@printf '%50s\n' | tr ' ' =

clean:
	@sudo rm -vfr *~ $(DEPLOYDIR)/lib$(LIBNAME).so.$(LIBVER)  *.o ../bin/$(BIN)_dbg  $(RELEASE_OBJS) $(DEBUG_OBJS) $(LIBOBJS) $(LIBOBJSDBG)
	@printf '%50s\n' | tr ' ' =
	@printf "CLEAN COMPLETE\n"
	@printf '%50s\n' | tr ' ' =
