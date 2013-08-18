CC?=gcc
CFLAGS+= -Wall -g -pthread -O2
LDFLAGS+= -lconfig -lsqlite3 -lgit2
SRC_DIRS = ${shell find !SRC! -type d -print}
SRCS=$(foreach dir, $(SRC_DIRS),$(wildcard $(dir)/*.c))
OBJS=$(patsubst !SRC!/%.c,%.o,$(SRCS))
TARGET=lunkwill

all: $(OBJS)
	@echo CC $(TARGET)
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS)
	@cp -r !SRC!/www ./www

$(OBJS): $(SRCS)
	@echo CC -c $(patsubst %.o,!SRC!/%.c,$@) -o $@
	@mkdir $(dir $@) -p
	@$(CC) -c $(CFLAGS) $(patsubst %.o,!SRC!/%.c,$@) -o $@

clean:
	@echo Cleaning build directory ...
	@mv Makefile .Makefile
	@mv $(TARGET) .$(TARGET) >/dev/null 2>/dev/null || :
	@rm -rf *
	@mv .Makefile Makefile
	@mv .$(TARGET) $(TARGET) >/dev/null 2>/dev/null || :
