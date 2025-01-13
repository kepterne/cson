.PHONY: default
default: all

export DEPLOYDIR=/usr/lib
export BINDIR=/usr/bin
export LIBVER=1.0.2
export CC=clang
export LIBNAME=cson

BOLD=\033[1m\e[1;32m
BLUE=\033[1m\e[1;34m
NORM=\033[0m\e[0m

LIBSOURCES = cson/jsonparser.c
LIBSOURCES += cson/cson.c
LIBSOURCES += cson/cobj.c
CLEANSOURCES:= 


CFLAGS= -Wall \
	-I. \
	-DLIBVER=\"$(LIBVER)\" \
	-fPIC \
	-I./lib \
	-Wno-unused-variable -Wno-unused-result -Wno-pointer-sign -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast \
	-Wno-unknown-pragmas \
	-Wno-discarded-qualifiers \
	-Wno-multichar \
	-Wno-misleading-indentation \
	-Wno-strict-aliasing \
	-Wno-enum-conversion \
	-fcommon \
	-Wno-stringop-truncation \
	-Wno-unknown-warning-option \
	-Wno-incompatible-pointer-types-discards-qualifiers \
	-Wno-unused-but-set-variable
	
CFLAGS_DEBUG = -g  \

CFLAGS_RELEASE = -os \
	-Wno-array-bounds \
	-Wno-stringop-overflow 

deploy_dir:
	@sudo mkdir -p $(DEPLOYDIR)

debug_dir:
	@mkdir -p ../debug

release_dir:
	@mkdir -p ../release
	
bin:
	@mkdir -p ../$@

 
