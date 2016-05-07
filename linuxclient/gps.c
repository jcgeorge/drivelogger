/* 
 * File         : gps.c
 * Author       : Contains functions for gpsd related operations of the client
 * Last modified: 13/04/2016
 */

#include "gps.h"

int gpsd_socket = 0;
int gpsd_socket_status = 0;
int gpsd_server_port = 0;

struct sockaddr_in gpsd_sockaddr = {0};
struct hostent *gpsd_server = {0};

char gps_data[GPSD_BUFF_LENGTH] = "0";

/* 
 * Function     : gps_setup
 * Arguments    : none
 * Description  : sets up a client socket for gpsd
 */
void gps_setup()
{
    gpsd_server = gethostbyname(GPSD_HOST);
    gpsd_server_port = atoi(GPSD_PORT);

    /* Establishing a socket */
    gpsd_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if (gpsd_socket < 0)
    {
        error("\ngpsd: Error opening socket!");
    }
    else if (DEBUG == true)
    {
        printf("\ngpsd: Socket established.");
    }

    /* This is unlikely with localhost, but it is good error management */
    if (gpsd_server == NULL)
    {
        error("\ngpsd: Error, unknown host!");
    }

    /* Getting host information */
    gpsd_sockaddr.sin_family = AF_INET;
    memcpy((char *)gpsd_server -> h_addr, (char *)
            &gpsd_sockaddr.sin_addr.s_addr, gpsd_server -> h_length);
    gpsd_sockaddr.sin_port = htons(gpsd_server_port);
    
    /* Connecting to the socket */
    gpsd_socket_status = connect(gpsd_socket, (struct sockaddr *)
            &gpsd_sockaddr, sizeof(gpsd_sockaddr));
    
    if (gpsd_socket_status < 0)
    {
        error("\ngpsd: Error connecting!");
    }
    else if (DEBUG == true)
    {
        printf("\ngpsd: Socket connected.");
    }
}

/* 
 * Function     : gps_start_watch
 * Arguments    : none
 * Description  : Function called only if socket successfully set up. 
 *                Sends WATCH command to gpsd.
 */
void gps_start_watch()
{
    /* Tell daemon to watch all devices */
    gpsd_socket_status = send(gpsd_socket, GPSD_WATCH_START,
            strlen(GPSD_WATCH_START), 0);
    
    if (gpsd_socket_status < 0)
    {
        error("\ngpsd: Error writing to socket!");
    } else if (DEBUG == true)
    {
        printf("\ngpsd: WATCH command sent.");
    }
}

/* 
 * Function     : gps_read
 * Arguments    : none
 * Description  : Function called only if WATCH command sent successfully. 
 *                Reads JSON data from the socket and outputs to console.
 */
void gps_read()
{
    /* Zero out buffer to prepare for receiving data */
    memset(gps_data, 0, GPSD_BUFF_LENGTH);
    
    gpsd_socket_status = recv(gpsd_socket, gps_data, GPSD_BUFF_LENGTH, 0);
    
    if (gpsd_socket_status < 0)
    {
        error("\ngpsd: Error reading from socket!");
    }
    
    printf("\n%s", gps_data);
}

/* 
 * Function     : gps_end_watch
 * Arguments    : none
 * Description  : Function called if ^C is detected. Sends end WATCH command
 *                to gpsd so GPS receiver can go in to low power state. 
 */
void gps_end_watch()
{
    /* Tell daemon to stop watching devices */
    gpsd_socket_status = send(gpsd_socket, GPSD_WATCH_END,
            strlen(GPSD_WATCH_END), 0);
    
    if (gpsd_socket_status < 0)
    {
        error("\ngpsd: Error writing to socket!");
    }
    else if (DEBUG == true)
    {
        printf("\ngpsd: End WATCH command sent.");
    }
}
