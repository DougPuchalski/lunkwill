CC?=gcc
CFLAGS+= -Wall -g -pthread -O2 -Werror
LDFLAGS+= -lconfig -lsqlite3 -lgit2 -ldl
SRC_DIRS = ${shell find !SRC! -type d -print | grep -v 04-plugins }
SRCS=$(foreach dir, $(SRC_DIRS),$(wildcard $(dir)/*.c))
OBJS=$(patsubst !SRC!/%.c,%.o,$(SRCS))

PLUGIN_DIRS = ${shell find !SRC! -type d -print | grep "04-plugins/.*" }
PLUGIN_NAMES=$(foreach dir, $(PLUGIN_DIRS),${shell basename $(dir)})
PLUGINS=$(foreach dir, $(PLUGIN_DIRS),$(wildcard $(dir)/*.c))
PLUGIN_OBJS=$(patsubst !SRC!/%.c,%.o,$(PLUGINS))

TARGET=lunkwill

all: $(OBJS) plugins
	@echo CC $(TARGET)
	@$(CC) $(CFLAGS) $(LDFLAGS) -rdynamic -o $(TARGET) $(OBJS)
	@cp -r !SRC!/www ./www

$(OBJS): $(SRCS)
	@echo CC -c $(patsubst %.o,!SRC!/%.c,$@) -o $@
	@mkdir $(dir $@) -p
	@$(CC) -c $(CFLAGS) $(patsubst %.o,!SRC!/%.c,$@) -o $@
	
$(PLUGIN_OBJS): $(PLUGINS)
	@echo CC -c $(patsubst %.o,!SRC!/%.c,$@) -o $@
	@mkdir $(dir $@) -p
	@$(CC) -fPIC -c $(CFLAGS) $(patsubst %.o,!SRC!/%.c,$@) -o $@
	
$(PLUGIN_NAMES): $(PLUGIN_OBJS)
	@echo CC $@.so
	@$(CC) -shared -Wl,-soname,$@.so $(CFLAGS) $(LDFLAGS) -o $@.so $(foreach v,$(PLUGIN_OBJS),$(if $(findstring $@,/$(v)/),$(v),))
	
plugins: $(PLUGIN_NAMES)
	@echo

clean:
	@echo Cleaning build directory ...
	@mv Makefile .Makefile
	@mv $(TARGET) .$(TARGET) >/dev/null 2>/dev/null || :
	@rm -rf *
	@mv .Makefile Makefile
	@mv .$(TARGET) $(TARGET) >/dev/null 2>/dev/null || :
