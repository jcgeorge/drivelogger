/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "gps.h"

int gpsd_socket = 0;
int gpsd_socket_status = 0;

void gps_setup()
{
    gpsd_hostent = gethostbyname(GPSD_HOST);
    int gpsd_server_port = atoi(GPSD_PORT);

    // Establishing a socket
    gpsd_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if (gpsd_socket < 0)
    {
        error("ERROR opening socket!");
    }

    // This is unlikely with localhost, but it is good error management
    if (gpsd_hostent == NULL)
    {
        error("ERROR: unknown host!");
    }

    // Getting host information
    gpsd_sockaddr.sin_family = AF_INET;
    memcpy((char *)gpsd_hostent -> h_addr, (char *)&gpsd_sockaddr.sin_addr.s_addr, gpsd_hostent -> h_length);
    gpsd_sockaddr.sin_port = htons(gpsd_server_port);
    
    // Connecting to the socket
    gpsd_socket_status = connect(gpsd_socket, (struct sockaddr *)&gpsd_sockaddr, sizeof(gpsd_sockaddr));
    
    if (gpsd_socket_status < 0)
    {
        error("ERROR connecting!");
    }
}

void gps_start_watch()
{
    /* Tells daemon to watch all devices, report back in JSON */
    gpsd_socket_status = send(gpsd_socket, GPSD_WATCH_START, strlen(GPSD_WATCH_START), 0);
    
    if (gpsd_socket_status < 0)
    {
        error("ERROR sending to socket!");
    } else 
    {
        printf("\n==================================");
        printf("\nWATCH command sent to gpsd daemon.");
        printf("\n==================================\n\n");
    }
}

void gps_read()
{
    /* Zeroes buffer to prepare for receiving data */
    memset(gps_data, 0, 1024);
    
    gpsd_socket_status = recv(gpsd_socket, gps_data, 1023, 0);
    
    if (gpsd_socket_status < 0)
    {
        error("ERROR reading from socket!");
    }
    printf("%s\n\n", gps_data);
}

void gps_end_watch()
{
    /* Tells daemon to stop watching devices */
    gpsd_socket_status = send(gpsd_socket, GPSD_WATCH_END, strlen(GPSD_WATCH_END), 0);
    
    if (gpsd_socket_status < 0)
    {
        error("ERROR writing to socket");
    } else 
    {
        printf("\n======================================");
        printf("\n^C received.");
        printf("\nEnd WATCH command sent to gpsd daemon.");
        printf("\n======================================\n\n");
    }
}
