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
OBJS=$(patsubst !SRC!/%.c,%.o,$(SRCS))

WWW=$(foreach dir, $(SRC_DIRS),$(wildcard $(dir)/*.coffee))
WWW_OBJS=www/index.js

PLUGIN_DIRS = ${shell find !SRC! -type d -print | grep "04-plugins/.*" }
PLUGIN_NAMES=$(foreach dir, $(PLUGIN_DIRS),${shell basename $(dir)})
PLUGINS=$(foreach dir, $(PLUGIN_DIRS),$(wildcard $(dir)/*.c))
PLUGIN_OBJS=$(patsubst !SRC!/%.c,%.o,$(PLUGINS))

TARGET=lunkwill

all: $(OBJS) $(PLUGIN_NAMES) $(WWW_OBJS)
	@echo -e [ BIN ]\\t\"$(TARGET)\"
	@$(CC) $(CFLAGS) -rdynamic -o $(TARGET) $(OBJS) $(LDFLAGS)
	@echo -e [ DAT ]\\t\"www\"
	@cp -r !SRC!/www/* ./www/

$(WWW_OBJS): $(WWW)
	@echo -e [ WWW ]\\t\"$@\"
	@mkdir www -p
	@$(COF) -o $(shell dirname $@) -c -j index.coffee $(WWW)
	@java -jar $(YUI) $@ -o $@ 2>/dev/null 1>/dev/null || true

$(OBJS): $(SRCS)
	@echo -e [ OBJ ]\\t\"$@\"
	@mkdir $(dir $@) -p
	@$(CC) -c $(CFLAGS) $(patsubst %.o,!SRC!/%.c,$@) -o $@
$(PLUGIN_OBJS): $(PLUGINS)
	@echo -e [ OBJ ]\\t\"$@\"
	@mkdir $(dir $@) -p
	@$(CC) -fPIC -c $(CFLAGS) $(patsubst %.o,!SRC!/%.c,$@) -o $@

$(PLUGIN_NAMES): $(PLUGIN_OBJS)
	@echo -e [ LIB ]\\t\"$@\"
	@$(CC) -shared -Wl,-soname,$@.so $(CFLAGS)  -o $@.so $(foreach v,$(PLUGIN_OBJS),$(if $(findstring $@,/$(v)/),$(v),)) $(LDFLAGS)

clean:
	@echo -e Cleaning build directory ...
	@mv Makefile .Makefile
	@mv $(TARGET) .$(TARGET) >/dev/null 2>/dev/null || :
	@rm -rf *
	@mv .Makefile Makefile
	@mv .$(TARGET) $(TARGET) >/dev/null 2>/dev/null || :
