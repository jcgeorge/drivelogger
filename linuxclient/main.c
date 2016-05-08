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
#include "gps.h"
#include "obd.h"

bool DEBUG = true;
static volatile int keep_alive = 1;

/* 
 * Function     : error
 * Arguments    : Pointer to char array
 * Description  : Error handler. Outputs passed error to console.
 */
void error(char *msg)
{
    if (DEBUG == true)
    {
        printf("\n%s", msg);
    }
    exit(EXIT_FAILURE);
}

void int_handler()
{
    keep_alive = 0;
}

/* 
 * Function     : main
 * Arguments    : none
 * Description  : Entry point. Calls other functions which retrieve data
 */
int main()
{
    /* Signal handler, to register ^C */
    signal(SIGINT, int_handler);

    gps_setup();
    obd_setup();   
        
    while(keep_alive)
    {
        gps_poll();
        obd_speed();
        sleep(WAIT_SECONDS);
    }

    if (DEBUG == true)
    {
        printf("\n^C received.");
    }
    gps_end_watch();

    return EXIT_SUCCESS;
}
