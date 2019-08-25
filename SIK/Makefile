TARGET: czekamnaudp ileczekam

CC	= cc
CFLAGS	= -Wall -O2
LFLAGS	= -Wall


czekamnaudp: czekamnaudp.o err.o
	$(CC) $(LFLAGS) $^ -o $@

ileczekam: ileczekam.o err.o
	$(CC) $(LFLAGS) $^ -o $@

.PHONY: clean TARGET
clean:
	rm -f czekamnaudp ileczekam *.o *~ *.bak
