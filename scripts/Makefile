CC?=gcc
CFLAGS+= -Wall -g -pthread -O2 -Werror -I/usr/include/lua5.1 
LDFLAGS+= -lconfig -lsqlite3 -lgit2 -llua5.1 -ldl
SRC_DIRS = ${shell find !SRC! -type d -print | grep -v 04-plugins }
SRCS=$(foreach dir, $(SRC_DIRS),$(wildcard $(dir)/*.c))
OBJS=$(patsubst !SRC!/%.c,%.o,$(SRCS))

PLUGIN_DIRS = ${shell find !SRC! -type d -print | grep "04-plugins/.*" }
PLUGIN_NAMES=$(foreach dir, $(PLUGIN_DIRS),${shell basename $(dir)})
PLUGINS=$(foreach dir, $(PLUGIN_DIRS),$(wildcard $(dir)/*.c))
PLUGIN_OBJS=$(patsubst !SRC!/%.c,%.o,$(PLUGINS))

TARGET=lunkwill

all: $(OBJS) $(PLUGIN_NAMES)
	@echo -e [ BIN ]\\t\"$(TARGET)\"
	@$(CC) $(CFLAGS) -rdynamic -o $(TARGET) $(OBJS) $(LDFLAGS)
	@echo -e [ DAT ]\\t\"www\"
	@cp -r !SRC!/www ./www

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
