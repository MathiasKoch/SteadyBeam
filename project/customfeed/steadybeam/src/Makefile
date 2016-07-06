
CFLAGS	+= -Wall -Os

# build steadybeam executable when user executes "make"

all: steadybeam

gpio_fcn.o: gpio_fcn.c
	$(CC) $(CFLAGS) $(INCLUDE) -c gpio_fcn.c -o gpio_fcn.o

steadybeam.o: steadybeam.c
	$(CC) $(CFLAGS) $(INCLUDE) -c steadybeam.c -o steadybeam.o

steadybeam: steadybeam.o gpio_fcn.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

# remove object files and executable when user executes "make clean"
clean:
	rm *.o steadybeam