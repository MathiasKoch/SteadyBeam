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
#include <netinet/in.h>
#include <linux/i2c-dev.h>
#include "gpio_fcn.h"

#define DESIRED_BAUDRATE B115200

#define NO_COMMANDS 20

#define MAXLEN 80

char COMMAND[NO_COMMANDS] = {"AT!GSTATUS?\r"};
int MAX_RESPONSE_SIZE = 600;

int fd, fd2;


struct parameters{
  int update_rate;
}parameters;

struct parameters *parms;

char * trim (char * s){
    /* Initialize start, end pointers */
    char *s1 = s, *s2 = &s[strlen (s) - 1];

    /* Trim and delimit right side */
    while((isspace(*s2)) && (s2 >= s1))
        s2--;
    *(s2+1) = '\0';

    /* Trim left side */
    while((isspace(*s1)) && (s1 < s2))
        s1++;

    /* Copy finished string */
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

    /* Read next line */
   // dprintf ("Defaulting values to:\n");

    while ((s = fgets (buff, sizeof buff, fp)) != NULL){
        /* Skip blank lines and comments */
        if (buff[0] == '\n' || buff[0] == '#')
            continue;

        /* Parse name/value pair from line */
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

    /* Close file */
    fclose (fp);
}


void configureUart(int baud) {
    char *portname = "/dev/ttyUSB2";

	fd = open (portname, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd < 0){
		printf("error %d opening %s: %s\r\n", errno, portname, strerror(errno));
		return;
	}

	set_interface_attribs(fd, baud, 0);  	// set speed to 115,200 bps, 8n1 (no parity)
	set_blocking(fd, 0);                	// set no blocking
}

void configureUart2(int baud) {
    char *portname = "/dev/ttyS0";

	fd2 = open (portname, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd2 < 0){
		printf("error %d opening %s: %s\r\n", errno, portname, strerror(errno));
		return;
	}

	set_interface_attribs(fd2, baud, 0);  	// set speed to 115,200 bps, 8n1 (no parity)
	set_blocking(fd2, 0);                	// set no blocking
}


void sendAT(int index, char* buf){
	int len = sizeof(COMMAND) / sizeof(COMMAND[index]);
	if(write (fd, &COMMAND[index], len) != len){
		printf("[ERROR] Failed to write to fd in sendAT\r\n");
	}        

	usleep ((len + MAX_RESPONSE_SIZE) * 100); 	// 100 uS per byte of transfer, both directions           
	if(read (fd, buf, MAX_RESPONSE_SIZE) < 0){
		printf("[ERROR] Failed to read fd in sendAT\r\n");
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

	parms->update_rate = 1000;

	parse_config(myConfigVar);

	configureUart(DESIRED_BAUDRATE);
	configureUart2(DESIRED_BAUDRATE);
	char *buf;
	char *cellId;
	
	buf = (char *) malloc(MAX_RESPONSE_SIZE);
	cellId = (char *) malloc(10);
    cellId[0] = 'S';
    cellId[1] = ':';
    int cnt = 0;
    while(1){
        sendAT(0, buf);
        filter(buf, 8, "Cell ID:", 8, cellId);
        if(cnt % (2000/(parms->update_rate)) == 0){
            FILE *fp = fopen("/tmp/steadybeam.log", "w");
            fprintf(fp,"%s\r\n", buf);
            fclose(fp);
        }

		if(write (fd2, cellId, 10) != 10){
			printf("[ERROR] Failed to write to fd2 in main\r\n");
		}
		usleep((parms->update_rate)*1000);
		cnt++;
	}
	

	return  0;

}
