# Target library
lib := libuthread.a
objs := queue.o uthread.o context.o sem.o
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

uthread.o: uthread.c uthread.h
	@echo "Generating object files ..."
	$(Q)$(CC) $(CFLAGS) -c -o $(@) $(<)

context.o: context.c private.h
	@echo "Generating object files ..."
	$(Q)$(CC) $(CFLAGS) -c -o $(@) $(<)

queue.o: queue.c queue.h
	@echo "Generating object files ..."
	$(Q)$(CC) $(CFLAGS) -c -o $(@) $(<)

sem.o: sem.c sem.h
	@echo "Generating object files ..."
	$(Q)$(CC) -pthread $(CFLAGS) -c -o $(@) $(<)

clean:
	@echo "Cleaning all .a, .o, and .d files ..."
	$(Q)rm -f $(lib) $(objs) $(deps)
