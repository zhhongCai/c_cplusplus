ROOT=..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

ifeq "$(PLATFORM)" "linux"
	ZAP = zap
else
	ZAP =
endif


#PROGS =	access cdpwd changemod devrdev filetype mycd umask unlink $(ZAP)
PROGS =	ch10_signal ch10_signal2 ch10_sigchld ch10_pause ch10_alarm \
		ch10_sigpending ch10_sigjmp_func ch10_sigsuspend ch10_sigsuspend_global \
		ch10_signal_fork ch10_abort ch10_system ch10_system2 ch10_sleep \
		ch10_jobctl \
	   	$(ZAP)

all:	$(PROGS)

$(PROGS):	$(LIBAPUE)
		$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(PROGS) $(MOREPROGS) $(TEMPFILES) *.o $(ZAP)

include $(ROOT)/Make.libapue.inc
