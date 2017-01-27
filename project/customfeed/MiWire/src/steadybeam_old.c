#include <sys/file.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <ctype.h>
#include <unistd.h>
#include <poll.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include <netinet/in.h>
#include <linux/i2c-dev.h>
#include "gpio_fcn.h"

#define DESIRED_BAUDRATE B115200
#define THRANE_BAUDRATE B4800

#define NO_COMMANDS 20

#define MAXLEN 80
#define MAX_BUF 64

#define TURN_CW 0
#define TURN_CCW 1

#define AT_GSTATUS 0

char COMMAND[NO_COMMANDS] = {"AT!GSTATUS?\r"};
int MAX_RESPONSE_SIZE = 600;

int fd_modem, fd_thrane;

pthread_t tid_interrupt;

struct motor_s{
    int fd_pos;    
    int fd_dir;    
    int fd_start;    
    int fd_index;   

    int steps_moved;
    int turn_direction;
    int current_motorstep;
}motor_s;

struct motor_s *motor;

struct parameters{
  int update_rate;
}parameters;

struct parameters *parms;

char * trim (char * s){
    // Initialize start, end pointers
    char *s1 = s, *s2 = &s[strlen (s) - 1];

    // Trim and delimit right side 
    while((isspace(*s2)) && (s2 >= s1))
        s2--;
    *(s2+1) = '\0';

    // Trim left side
    while((isspace(*s1)) && (s1 < s2))
        s1++;

    // Copy finished string
    strcpy(s, s1);
    return s;
}

//void signal_handler_IO (int status);

void parse_config(const char* config){
    char *s, buff[256];
    FILE *fp = fopen (config, "r");
    if (fp == NULL){
        return;
    }

    // Read next line
   // dprintf ("Defaulting values to:\n");

    while ((s = fgets (buff, sizeof buff, fp)) != NULL){
        // Skip blank lines and comments
        if (buff[0] == '\n' || buff[0] == '#')
            continue;

        // Parse name/value pair from line
        char name[MAXLEN], value[MAXLEN];
        int v;
        s = strtok (buff, "=");
        if (s==NULL)
            continue;
        else
            strncpy (name, s, MAXLEN);
        s = strtok (NULL, "=");
        if (s==NULL)
            continue;
        else
            strncpy (value, s, MAXLEN);
        trim (value);
        v = atoi(value);
        if(v!=0){
            if (strcmp(name, "UPDATE_RATE")==0)
                parms->update_rate = v;
            else
                printf ("!!! WARNING: %s/%d: Unknown name/value pair!\n",name, v);
            //dprintf ("\t%s = %d\n",name, v);
        }else{
            /* Copy into correct entry in parameters struct */
            /*if (strcmp(name, "UPDATE_RATE")==0)
                strncpy (parms->update_rate, value, MAXLEN);
            else
                printf ("!!! WARNING: %s/%s: Unknown name/value pair!\n",name, value);*/
            //dprintf ("\t%s = %s\n",name, value);
        }
    }

    // Close file
    fclose (fp);
}

void turnMotor(int turn_dir, int _steps){
    motor->turn_direction = turn_dir;
    if (_steps > 0){
        motor->steps_moved = 0;
        if(motor->turn_direction == TURN_CW){
            write(motor->fd_start, "1", 2);
        }else{
            write(motor->fd_start, "0", 2);
        }
        write(motor->fd_dir, "0", 2);
        int count = 0;
        while (count < 3000 && motor->steps_moved < _steps){
            usleep(5000);
            count++;
        }
        write(motor->fd_dir, "1", 2);
    }
}

void configureMotor(void){
    printf("Configuring motor!\t [");

    int fd;

    // Open file descriptors for value of motor pins
    motor->fd_dir = open("/sys/class/gpio/motor_dir/value", O_RDWR);
    motor->fd_start = open("/sys/class/gpio/motor_start/value", O_RDWR);

    motor->fd_index = open("/sys/class/gpio/motor_index/value", O_RDWR);
    motor->fd_pos = open("/sys/class/gpio/motor_pos/value", O_RDWR);


    // Setup motor_dir directions
    fd = open("/sys/class/gpio/motor_dir/direction", O_WRONLY);
    write(fd, "out", 4); 
    close(fd);

    // Setup motor_start directions
    fd = open("/sys/class/gpio/motor_start/direction", O_WRONLY);
    write(fd, "out", 4); 
    close(fd);

    // Setup motor_index directions
    fd = open("/sys/class/gpio/motor_index/direction", O_WRONLY);
    write(fd, "in", 3); 
    close(fd);

    // Setup motor_pos directions
    fd = open("/sys/class/gpio/motor_pos/direction", O_WRONLY);
    write(fd, "in", 3); 
    close(fd);

    // Stop the motor from start and set turn direction to CW
    write(motor->fd_dir, "1", 2);
    motor->turn_direction = TURN_CW;
    write(motor->fd_start, "1", 2);

    motor->current_motorstep = 0;
    
    // Define Position as interrupt driven, with rising edge trigger
    fd = open("/sys/class/gpio/motor_pos/edge", O_WRONLY);
    write(fd, "rising", 7); 
    close(fd);

    // Define Index as interrupt driven, with rising edge trigger
    fd = open("/sys/class/gpio/motor_index/edge", O_WRONLY);
    write(fd, "falling", 7); 
    close(fd);
    printf("Done]\r\n");

}


void configureModem(int baud) {
    printf("Configuring modem!\t [");
    char *portname = "/dev/ttyUSB2";

    fd_modem = open (portname, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd_modem < 0){
        printf("Error %d opening %s: %s]\r\n", errno, portname, strerror(errno));
        return 0;
    }

    set_interface_attribs(fd_modem, baud, 0);   // set speed to 115,200 bps, 8n1 (no parity)
    set_blocking(fd_modem, 0);                  // set no blocking
    printf("Done]\r\n");
}

void configureThrane(int baud) {
    printf("Configuring Thrane!\t [");

    char *portname = "/dev/ttyS1";

	fd_thrane = open (portname, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd_thrane < 0){
		printf("Error %d opening %s: %s]\r\n", errno, portname, strerror(errno));
		return;
	}

	set_interface_attribs(fd_thrane, baud, 0);
	set_blocking(fd_thrane, 0);                	// set no blocking
    printf("Done]\r\n");

}


void sendAT(int index, char* buf){
	int len = sizeof(COMMAND) / sizeof(COMMAND[index]);
	if(write (fd_modem, &COMMAND[index], len) != len){
		printf("[ERROR] Failed to write to fd_modem in sendAT\r\n");
	}        

	usleep ((len + MAX_RESPONSE_SIZE) * 100); 	// 100 uS per byte of transfer, both directions           
	if(read (fd_modem, buf, MAX_RESPONSE_SIZE) < 0){
		printf("[ERROR] Failed to read fd_modem in sendAT\r\n");
	}
}

int filter(char* buf, int searchLen, char* search, int returnLen, char* returnVal){
	char *s = strstr(buf, search);
	if(s != NULL){
		int index = s - buf + searchLen;
		while(buf[index] == ' '){
			index++;
		}
		memcpy(returnVal+2, &buf[index], returnLen);
		return 1;
	}else{
		memcpy(returnVal+2, "00000000", 8);
		return 0;
	}
}

void* interrupt_thread(void *arg){
    struct pollfd fdset[2];
    int dummy, rc, rd;

    while (1) {
        memset((void*)fdset, 0, sizeof(fdset));

        // Prepare motor_pos for interrupt polling
        fdset[0].fd = motor->fd_pos;
        fdset[0].events = POLLPRI;
        fdset[0].revents = 0;
        
        lseek(fdset[0].fd, 0, SEEK_SET);
        (void*)read(fdset[0].fd, &dummy, sizeof(dummy));


        // Prepare motor_index for interrupt polling
        fdset[1].fd = motor->fd_index;
        fdset[1].events = POLLPRI;
        fdset[1].revents = 0;
        
        lseek(fdset[1].fd, 0, SEEK_SET);
        (void*)read(fdset[1].fd, &dummy, sizeof(dummy));


        rc = poll(fdset, 2, -1);      

        if (rc < 0) {
            printf("\npoll() failed!\n");
            return 0;
        }


        if (fdset[0].revents & POLLPRI) {   // POS
            char *value[MAX_BUF];
            lseek(fdset[0].fd, 0, SEEK_SET);
            rd = read(fdset[0].fd, value, sizeof(value));

            // INTERRUPT TRIGGERED.. Code here:
            printf("POS Triggered\r\n");
            motor->steps_moved++;
            if(motor->turn_direction == TURN_CW){
                motor->current_motorstep++;
            }else{
                motor->current_motorstep--;
            }

            if (motor->current_motorstep < 0){
                motor->current_motorstep = 200 - motor->current_motorstep;
            }
        }
        if (fdset[1].revents & POLLPRI) {   // INDEX
            char *value[MAX_BUF];
            lseek(fdset[1].fd, 0, SEEK_SET);
            rd = read(fdset[1].fd, value, sizeof(value));

            // INTERRUPT TRIGGERED.. Code here:
            printf("HOME Reached! \r\n");

            motor->current_motorstep = 0;
        }
    }
}


int main (int argc, char *argv[]){

	const char* myConfigVar;
	int i;

    // if the variable wasn't defined, initialize to hardcoded default
    if (!myConfigVar)
        myConfigVar = "/etc/steadybeam/steadybeam.conf";

    // parse commandline arguments
    // start at 1, because argv[0] contains the name of the program
    for (i = 1; i < argc; ++i) {
        if (strcmp("-C", argv[i]) == 0) {
            if (i + 1 < argc)
                myConfigVar = argv[i + 1];
            else
                printf("missing value for config argument [-C]\n");
        }
    }

    parms = (struct parameters *) malloc(sizeof(struct parameters));
	motor = (struct motor_s *) malloc(sizeof(struct motor_s));

	parms->update_rate = 1000;

	parse_config(myConfigVar);
    

    configureModem(DESIRED_BAUDRATE);
	configureThrane(THRANE_BAUDRATE);
    configureMotor();

    int err = pthread_create(&tid_interrupt, NULL, &interrupt_thread, NULL);
    if (err != 0)
        printf("\ncan't create interrupt thread"); 

    printf("Done with initialize!\r\n");

	
    while(1){
        //sendAT(AT_GSTATUS, buf);
        //filter(buf, 8, "Cell ID:", 8, cellId);
        turnMotor(TURN_CW, 50);
        //usleep((parms->update_rate)*1000);
		sleep(1);
		//cnt++;
	}
	
    pthread_join(tid_interrupt, NULL);
    exit(EXIT_SUCCESS);
	return  0;

}
