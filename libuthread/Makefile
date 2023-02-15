# Target library
lib := libuthread.a
objs := queue.o uthread.o context.o sem.o preempt.o
CC := gcc 
CFLAGS := -Wall -Wextra -Werror -MMD -c -o

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

%.o: %.c
	@echo "Generating $@ ..."
	$(Q)$(CC) $(CFLAGS) $(@) $(<)

clean:
	@echo "Cleaning all .a, .o, and .d files ..."
	$(Q)rm -f $(lib) $(objs) $(deps)
