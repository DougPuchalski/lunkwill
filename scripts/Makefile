CC?=gcc
COF?=coffee
PREFIX?=/usr/

CFLAGS+= -Wall -g -pthread -Werror -O2 \
		$(shell pkg-config --cflags libconfig) \
		$(shell pkg-config --cflags libgit2) \
		$(shell pkg-config --cflags openssl) \
		$(shell pkg-config --cflags lua 2>/dev/null 1>/dev/null || pkg-config --cflags lua5.1)

LDFLAGS+= -ldl \
		$(shell pkg-config --libs libgit2) \
		$(shell pkg-config --libs libconfig) \
		$(shell pkg-config --libs openssl) \
		$(shell pkg-config --libs lua 2>/dev/null 1>/dev/null || pkg-config --libs lua5.1)

SRC_DIRS = ${shell find !SRC! -type d -print | grep -v "04-plugins" | grep -v "utils" | grep -v "/\." }
SRCS=$(foreach dir, $(SRC_DIRS),$(wildcard $(dir)/*.c))
OBJS=$(patsubst !SRC!/%.c,build/%.o,$(SRCS))

WWW=$(foreach dir, $(SRC_DIRS),$(wildcard $(dir)/*.coffee))
WWW_OBJS=www/index.js

PLUGIN_DIRS = ${shell find !SRC! -type d -print | grep "04-plugins/.*" }
PLUGIN_NAMES=$(foreach dir, $(PLUGIN_DIRS),${shell bash -c 'if [ -f $(dir)/*.c ]; then basename $(dir); fi'})
PLUGINS=$(foreach dir, $(PLUGIN_DIRS),$(wildcard $(dir)/*.c))
PLUGIN_OBJS=$(patsubst !SRC!/%.c,build/%.o,$(PLUGINS))

LUA_PLUGINS_SRC=${shell find !SRC! -print | grep "04-plugins/.*lua$$" }
LUA_PLUGINS=${shell echo $(foreach plug, $(LUA_PLUGINS_SRC), ${shell basename $$(dirname $(plug))}) | sort}

UTILS=$(wildcard !SRC!/utils/*.c)
UTILS_OBJS=$(patsubst !SRC!/%.c,build/%.o,$(UTILS))

TARGET=lunkwill
define RUN_SCRIPT
#!/bin/bash

cd $(PREFIX)/share/lunkwill
./lunkwill $$@
endef

export RUN_SCRIPT

all: $(UTILS_OBJS) $(OBJS) $(PLUGIN_NAMES) $(LUA_PLUGINS) $(WWW_OBJS)
	@echo [ BIN ]    \"$(TARGET)\"
	@$(CC) $(CFLAGS) -rdynamic -o $(TARGET) $(UTILS) $(OBJS) $(LDFLAGS)
	@echo [ DAT ]    \"www\"
	@cp -r $(foreach p,${shell ls -d !SRC!/www/*},$(if $(findstring .coffee,$(p)),,$(p))) ./www/

$(LUA_PLUGINS):$(LUA_PLUGINS_SRC)
	@echo [ LUA ]    \"$@\"
	@mkdir -p plugins/$@/
	@cp $(foreach p,$(LUA_PLUGINS_SRC),$(if $(findstring $@,$(p)),$(p),)) plugins/$@/

$(WWW_OBJS): $(WWW)
	@echo [ WWW ]    \"$@\"
	@mkdir www -p
	@$(COF) -o $(shell dirname $@) -c -j index.coffee $(WWW)
	@java -jar $(YUI) $@ -o $@ &>/dev/null || true

$(OBJS): $(SRCS)
	@echo [ OBJ ]    \"$@\"
	@mkdir $(dir $@) -p
	@$(CC) -c $(CFLAGS) $(patsubst build/%.o,!SRC!/%.c,$@) -o $@

$(UTILS_OBJS): $(UTILS)
	@echo [ OBJ ]    \"$@\"
	@mkdir $(dir $@) -p
	@$(CC) -c $(CFLAGS) $(patsubst build/%.o,!SRC!/%.c,$@) -o $@

$(PLUGIN_OBJS): $(PLUGINS)
	@echo [ OBJ ]    \"$@\"
	@mkdir $(dir $@) -p
	@$(CC) -fPIC -c $(CFLAGS) $(patsubst build/%.o,!SRC!/%.c,$@) -o $@

$(PLUGIN_NAMES): $(PLUGIN_OBJS)
	@echo [ LIB ]    \"$@\"
	@mkdir -p ./plugins/$@
	@$(CC) -shared -Wl,-soname,$@.so $(CFLAGS)  -o ./plugins/$@/$@.so $(foreach v,$(PLUGIN_OBJS),$(if $(findstring $@,/$(v)/),$(v),)) $(LDFLAGS)

plugins: $(UTILS_OBJS) $(PLUGIN_NAMES) $(LUA_PLUGINS)
	@echo

www: $(WWW_OBJS)
	@echo

clean:
	@echo Cleaning build directory ...
	@rm -rf build

distclean:
	@rm -rf .*
	@rm -rf *

install: all
	@rm .build .Makefile -rf
	@mv build .build
	@mv Makefile .Makefile
	@mkdir -p $(PREFIX)/share/lunkwill/
	@cp -r * $(PREFIX)/share/lunkwill/
	@mv .build build
	@mv .Makefile Makefile
	@echo "$$RUN_SCRIPT" > $(PREFIX)/bin/lunkwill
	@chmod 755 $(PREFIX)/bin/lunkwill


