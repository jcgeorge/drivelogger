/* 
 * File         : gps.c
 * Author       : Contains functions for gpsd related operations of the client
 * Last modified: 13/04/2016
 */

#include "gps.h"

int gpsd_socket = 0;
int gpsd_socket_status = 0;
int gpsd_server_port = 0;
int i = 0;

struct sockaddr_in gpsd_sockaddr = {0};
struct hostent *gpsd_server = {0};

char gpsd_data[GPSD_BUFF_LENGTH] = "0";
char latitude[GPSD_VAL_LENGTH] = "0";
char longitude[GPSD_VAL_LENGTH] = "0";

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
        error("\nGPS: Error opening socket!");
    }
    else if (DEBUG == D_MAX)
    {
        printf("\nGPS: Socket established.");
    }

    /* This is unlikely with localhost, but it is good error management */
    if (gpsd_server == NULL)
    {
        error("\nGPS: Error, unknown host!");
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
        error("\nGPS: Error connecting!");
    }
    else if (DEBUG == D_MAX)
    {
        printf("\nGPS: Socket connected.");
    }
    
    gps_send(GPSD_WATCH_START);
    gps_read();
}

void gps_poll()
{
    gps_send(GPSD_POLL);
    gps_read();

    if (gpsd_data[10] == 'P' && gpsd_data[13] == 'L' && gpsd_data[128] == '3')
    {
        for (i = 182; i < 191; i++)
        {
            latitude[i-182] = gpsd_data[i];
        }
                
        for (i = 201; i < 210; i++)
        {
            longitude[i-201] = gpsd_data[i];
        }
        
        if (DEBUG == D_ON)
        {
            printf("\nLatitude : %s\nLongitude : %s", latitude, longitude);
        }
    }
}

/* 
 * Function     : gps_end_watch
 * Arguments    : none
 * Description  : Function called if ^C is detected. Sends end WATCH command
 *                to gpsd so GPS receiver can go in to low power state. 
 */
void gps_end_watch()
{
    gps_send(GPSD_WATCH_END);
}

void gps_send(char *gpsd_command)
{
    gpsd_socket_status = write(gpsd_socket, gpsd_command, strlen(gpsd_command));
    if (gpsd_socket_status < 0)
    {
        error("\nGPS: Error sending to socket!");
    } else if (DEBUG == D_MAX)
    {
        printf("\nGPS: Sent %s", gpsd_command);
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
    memset(gpsd_data, 0, GPSD_BUFF_LENGTH);
    
    gpsd_socket_status = recv(gpsd_socket, gpsd_data, GPSD_BUFF_LENGTH, 0);

    if (gpsd_socket_status < 0)
    {
        error("\nGPS: Error reading from socket!");
    }
    else if (DEBUG == D_MAX)
    {
        printf("\n%s", gpsd_data);
    }
}
