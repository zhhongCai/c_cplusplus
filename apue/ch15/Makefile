ROOT=..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

ifeq "$(PLATFORM)" "linux"
	ZAP = zap
else
	ZAP =
endif


#PROGS =	access cdpwd changemod devrdev filetype mycd umask unlink $(ZAP)
PROGS =	ch15_pipe ch15_pipe_page ch15_popen ch15_tolower ch15_popen_pclose \
		ch15_add ch15_pipe_add \
	   	$(ZAP)

all:	$(PROGS)

$(PROGS):	$(LIBAPUE)
		$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(PROGS) $(MOREPROGS) $(TEMPFILES) *.o $(ZAP)

include $(ROOT)/Make.libapue.inc
