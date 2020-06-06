ROOT=..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

ifeq "$(PLATFORM)" "linux"
	ZAP = ch05_getspnam_linux 
else
	ZAP =
endif


#PROGS =	access cdpwd changemod devrdev filetype mycd umask unlink $(ZAP)
PROGS =	ch05_getpwnam ch05_getgrname ch05_getgroups ch05_uname ch05_strftime \
	   	$(ZAP)

all:	$(PROGS)

$(PROGS):	$(LIBAPUE)
		$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(PROGS) $(MOREPROGS) $(TEMPFILES) *.o $(ZAP)

include $(ROOT)/Make.libapue.inc
