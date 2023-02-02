# Target library
lib := libuthread.a
objs := queue.o
CC := gcc 
CFLAGS := -Wall -Wextra -Werror -MMD

ifneq ($(V), 1)
	Q = @
endif

all: $(lib)
# Used Makefile Discussion notes for the below
deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)

$(lib): $(objs)
	@echo "Generating static library ..."
	$(Q)ar rcs $(@) $(^)

queue.o: queue.c queue.h
	@echo "Generating object files ..."
	$(Q)$(CC) $(CFLAGS) -c -o $(@) $(<)

clean:
	@echo "Cleaning all .a, .o, and .d files ..."
	$(Q)rm -f $(lib) $(objs) $(deps)