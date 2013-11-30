CC?=gcc
COF?=coffee

CFLAGS+= -Wall -g -pthread -O2 -Werror \
		$(shell pkg-config --cflags libconfig) \
		$(shell pkg-config --cflags sqlite3) \
		$(shell pkg-config --cflags libgit2) \
		$(shell pkg-config --cflags lua||pkg-config --cflags lua5.1)

LDFLAGS+= -ldl \
		$(shell pkg-config --libs libconfig) \
		$(shell pkg-config --libs sqlite3) \
		$(shell pkg-config --libs libgit2) \
		$(shell pkg-config --libs lua||pkg-config --libs lua5.1)

SRC_DIRS = ${shell find !SRC! -type d -print | grep -v 04-plugins }
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

TARGET=lunkwill

all: $(OBJS) $(PLUGIN_NAMES) $(LUA_PLUGINS) $(WWW_OBJS)
	@echo -e [ BIN ]\\t\"$(TARGET)\"
	@$(CC) $(CFLAGS) -rdynamic -o $(TARGET) $(OBJS) $(LDFLAGS)
	@echo -e [ DAT ]\\t\"www\"
	@cp -r $(foreach p,${shell ls -d !SRC!/www/*},$(if $(findstring .coffee,$(p)),,$(p))) ./www/

$(LUA_PLUGINS):$(LUA_PLUGINS_SRC)
	@echo -e [ LUA ]\\t\"$@\"
	@mkdir -p plugins/$@/
	@cp $(foreach p,$(LUA_PLUGINS_SRC),$(if $(findstring $@,$(p)),$(p),)) plugins/$@/

$(WWW_OBJS): $(WWW)
	@echo -e [ WWW ]\\t\"$@\"
	@mkdir www -p
	@$(COF) -o $(shell dirname $@) -c -j index.coffee $(WWW)
	@java -jar $(YUI) $@ -o $@ &>/dev/null || true

$(OBJS): $(SRCS)
	@echo -e [ OBJ ]\\t\"$@\"
	@mkdir $(dir $@) -p
	@$(CC) -c $(CFLAGS) $(patsubst build/%.o,!SRC!/%.c,$@) -o $@

$(PLUGIN_OBJS): $(PLUGINS)
	@echo -e [ OBJ ]\\t\"$@\"
	@mkdir $(dir $@) -p
	@$(CC) -fPIC -c $(CFLAGS) $(patsubst build/%.o,!SRC!/%.c,$@) -o $@

$(PLUGIN_NAMES): $(PLUGIN_OBJS)
	@echo -e [ LIB ]\\t\"$@\"
	@mkdir -p ./plugins/$@
	@$(CC) -shared -Wl,-soname,$@.so $(CFLAGS)  -o ./plugins/$@/$@.so $(foreach v,$(PLUGIN_OBJS),$(if $(findstring $@,/$(v)/),$(v),)) $(LDFLAGS)

clean:
	@echo -e Cleaning build directory ...
	@rm -rf build

distclean:
	@rm -rf .*
	@rm -rf *

install: all
	@rm .build .Makefile -rf
	@mv build .build
	@mv Makefile .Makefile
	@mkdir -p /usr/share/lunkwill/
	@cp -r * /usr/share/lunkwill/
	@mv .build build
	@mv .Makefile Makefile
	@echo -e "#!/bin/bash\ncd /usr/share/lunkwill/\n./lunkwill \$\$$@" > /usr/bin/lunkwill
	@chmod 755 /usr/bin/lunkwill
