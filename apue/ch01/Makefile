ROOT=..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

ifeq "$(PLATFORM)" "linux"
	ZAP = zap
else
	ZAP =
endif


#PROGS =	access cdpwd changemod devrdev filetype mycd umask unlink $(ZAP)
PROGS =	ch01_ls ch01_copy ch01_rw ch01_printpid ch01_rexec ch01_errno ch01_printugid ch01_signal \
	   	$(ZAP)

all:	$(PROGS)

$(PROGS):	$(LIBAPUE)
		$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(PROGS) $(MOREPROGS) $(TEMPFILES) *.o $(ZAP)

include $(ROOT)/Make.libapue.inc
