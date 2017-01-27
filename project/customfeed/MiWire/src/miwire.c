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



struct parameters{
  int update_rate;
}parameters;

struct parameters *parms;


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
    

    mpu9250_init(0, true);    

	
    while(1){
        
	}
	
    exit(EXIT_SUCCESS);
	return  0;

}
