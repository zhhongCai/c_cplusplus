ROOT=..
EXTRALIBS=-pthread
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

BAR =
ifeq "$(PLATFORM)" "macos"
  TLOCK =
  EXTRALIBS=-pthread
else
  TLOCK = timedlock
endif
ifeq "$(PLATFORM)" "linux"
  BAR = barrier
  EXTRALIBS=-pthread -lrt -lbsd
endif
ifeq "$(PLATFORM)" "freebsd"
  BAR = barrier
  EXTRALIBS=-pthread
endif
ifeq "$(PLATFORM)" "solaris"
  BAR = barrier
  EXTRALIBS=-lpthread -lrt
endif

PROGS = ch11_pthread_create ch11_pthread_join ch11_pthread_join2 \
		ch11_pthread_cleanup ch11_pthread_mutex ch11_timedlock \

all:	$(PROGS) $(TLOCK) $(BAR)

$(PROGS):	$(LIBAPUE)
		$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(PROGS) $(TEMPFILES) *.o $(TLOCK) $(BAR)

include $(ROOT)/Make.libapue.inc
