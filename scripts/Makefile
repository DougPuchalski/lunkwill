CC=gcc
CFLAGS=-Wall -g -pthread -lconfig
SRC_DIRS := ${shell find !SRC! -type d -print}
SRCS=$(foreach dir, $(SRC_DIRS),$(wildcard $(dir)/*.c))
OBJS=$(patsubst !SRC!/%.c,%.o,$(SRCS))
TARGET=lunkwill

all: $(OBJS)
	@echo CC $(TARGET)
	@$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
	@cp -r !SRC!/www ./www
	@touch default.conf

$(OBJS): $(SRCS)
	@echo CC -c $(patsubst %.o,!SRC!/%.c,$@) -o $@
	@mkdir $(dir $@) -p
	@$(CC) -c $(CFLAGS) $(patsubst %.o,!SRC!/%.c,$@) -o $@

clean:
	@echo Cleaning build directory ...
	@mv Makefile .Makefile
	@rm -rf *
	@mv .Makefile Makefile
