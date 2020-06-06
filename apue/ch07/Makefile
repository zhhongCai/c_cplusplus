ROOT=..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

ifeq "$(PLATFORM)" "linux"
	ZAP = 
else
	ZAP =
endif


#PROGS =	access cdpwd changemod devrdev filetype mycd umask unlink $(ZAP)
PROGS =	ch07_atexit ch07_setjmp ch07_jmp ch07_resource \
	   	$(ZAP)

all:	$(PROGS)

$(PROGS):	$(LIBAPUE)
		$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(PROGS) $(MOREPROGS) $(TEMPFILES) *.o $(ZAP)

include $(ROOT)/Make.libapue.inc
