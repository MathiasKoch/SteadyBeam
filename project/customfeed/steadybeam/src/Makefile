
CFLAGS	+= -Wall -Os

# build steadybeam executable when user executes "make"

all: steadybeam

gpio_fcn.o: gpio_fcn.c
	$(CC) $(CFLAGS) $(INCLUDE) -c gpio_fcn.c -o gpio_fcn.o

steadybeam.o: steadybeam.c
	$(CC) $(CFLAGS) $(INCLUDE) -c steadybeam.c -o steadybeam.o

nmea.o: nmea/nmea.c
	$(CC) $(CFLAGS) $(INCLUDE) -c nmea/nmea.c -o nmea/nmea.o

parser.o: nmea/parser.c
	$(CC) $(CFLAGS) $(INCLUDE) -c nmea/parser.c -o nmea/parser.o

gpgga.o: parsers/gpgga.c
	$(CC) $(CFLAGS) $(INCLUDE) -c parsers/gpgga.c -o parsers/gpgga.o

gpgll.o: parsers/gpgll.c
	$(CC) $(CFLAGS) $(INCLUDE) -c parsers/gpgll.c -o parsers/gpgll.o

gprmc.o: parsers/gprmc.c
	$(CC) $(CFLAGS) $(INCLUDE) -c parsers/gprmc.c -o parsers/gprmc.o

parse.o: parsers/parse.c
	$(CC) $(CFLAGS) $(INCLUDE) -c parsers/parse.c -o parsers/parse.o

steadybeam: steadybeam.o gpio_fcn.o #nmea/nmea.o nmea/parser.o parsers/gpgga.o parsers/gpgll.o parsers/gprmc.o parsers/parse.o
	$(CC) -pthread $(LDFLAGS) -o $@ $^ $(LIBS)


# remove object files and executable when user executes "make clean"
clean:
	rm *.o steadybeam