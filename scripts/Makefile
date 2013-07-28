CC=gcc
CFLAGS=-Wall -g -pthread -lconfig
SRC_DIRS= core modules modules/tickets modules/git modules/wiki modules/doc
SRCS=$(foreach dir, $(SRC_DIRS),$(wildcard !SRC!/$(dir)/*.c))
OBJS=$(patsubst !SRC!/%.c,%.o,$(SRCS))
TARGET=lunkwill

all: $(OBJS)
	@echo CC $(TARGET)
	@$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

$(OBJS): $(SRCS)
	@echo CC -c $(patsubst %.o,!SRC!/%.c,$@) -o $@
	@mkdir $(dir $@) -p
	@$(CC) -c $(CFLAGS) $(patsubst %.o,!SRC!/%.c,$@) -o $@

clean:
	@echo Cleaning build directory ...
	@mv Makefile .Makefile
	@rm -rf *
	@mv .Makefile Makefile
