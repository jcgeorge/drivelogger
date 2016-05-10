/*
 * Program      : DriveLogger Linux Client
 * Author       : John George
 * Description  : This program is executed on system startup. It listens to 
 *                GPS data in JSON format from the gpsd daemon, and OBD data
 *                as raw hex values from the OBD socket. Parsed values
 *                (latitude, longitude, and vehicle speed) are then saved to
 *                a JSON file locally.
 */

/* 
 * File         : main.c
 * Author       : Entry point
 * Last modified: 13/04/2016
 */

#include "main.h"

int DEBUG = D_ON;
int GPSD_LOGGED = 0;
int SKIP_SAVE = 1;
static volatile int keepalive = 1;

/* 
 * Function     : error
 * Arguments    : Pointer to char array
 * Description  : Error handler. Outputs passed error to console.
 */
void error(char *msg)
{
    if (DEBUG == D_ON || DEBUG == D_MAX)
    {
        printf("\n%s", msg);
    }
    exit(EXIT_FAILURE);
}

void inthandler()
{
    keepalive = 0;
}

int gethourofday()
{
    char outstr[200];
    time_t t;
    struct tm *tmp;

    t = time(NULL);
    tmp = localtime(&t);
    if (tmp == NULL) {
        error("TIMEL: localtime() failed!");
    }
    
    if (strftime(outstr, sizeof(outstr), "%H", tmp) == 0) {
        error("TIME: strftime() returned 0!");
    }
    
    return ((int)strtol(outstr, NULL, 10));
}

/* 
 * Function     : main
 * Arguments    : none
 * Description  : Entry point. Calls other functions which retrieve data
 */
int main()
{
    /* Signal handler, to register ^C */
    signal(SIGINT, inthandler);
                    
    gps_setup();
    obd_setup();    
    filemanager(F_OPEN, gethourofday());
        
    while(keepalive)
    {
        /* Discard first four sets of data */
        gps_poll(SKIP_SAVE);
        obd_speed(SKIP_SAVE);
        sleep(WAIT_SECONDS);
        
        gps_poll(SKIP_SAVE);
        obd_speed(SKIP_SAVE);
        sleep(WAIT_SECONDS);
        
        gps_poll(SKIP_SAVE);
        obd_speed(SKIP_SAVE);
        sleep(WAIT_SECONDS);
        
        gps_poll(SKIP_SAVE);
        obd_speed(SKIP_SAVE);
        SKIP_SAVE = 0;
        sleep(WAIT_SECONDS);
        
        gps_poll(SKIP_SAVE);
        obd_speed(SKIP_SAVE);
        GPSD_LOGGED = 0;
        SKIP_SAVE = 1;
        sleep(WAIT_SECONDS);
    }

    if (DEBUG == D_ON || DEBUG == D_MAX)
    {
        printf("\n\n^C received.");
    }
    filemanager(F_CLOSE, gethourofday());
    gps_end_watch();

    return EXIT_SUCCESS;
}
