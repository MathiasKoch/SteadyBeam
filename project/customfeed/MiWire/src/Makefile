
CFLAGS	+= -Wall -Os

# build miwire executable when user executes "make"

############################################################
# This MAKEFIL still needs to be written!!!!!!!!!!!!!!!!!!!#
############################################################


all: miwire

gpio_fcn.o: gpio_fcn.c
	$(CC) $(CFLAGS) $(INCLUDE) -c gpio_fcn.c -o gpio_fcn.o

miwire.o: miwire.c
	$(CC) $(CFLAGS) $(INCLUDE) -c miwire.c -o miwire.o

miwire: miwire.o gpio_fcn.o
	$(CC) -pthread $(LDFLAGS) -o $@ $^ $(LIBS)


# remove object files and executable when user executes "make clean"
clean:
	rm *.o miwire