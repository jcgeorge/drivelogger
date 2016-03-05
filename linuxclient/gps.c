/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "gps.h"

void error(char *msg)
{
    perror(msg);
    exit(0);
}

void gps_setup()
{
    /* gpsd servers dumps data here by default */
    server = gethostbyname(GPSD_HOST);
    portno = atoi(GPSD_PORT);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("ERROR opening socket");
    }

    /* Unlikely with localhost, but error management is good */
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    /* some network stuff */
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server -> h_addr, (char *)&serv_addr.sin_addr.s_addr, server -> h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("ERROR connecting");
    }
}

void gps_start_watch()
{
    /* Tells daemon to watch all devices, report back in JSON */
    watch_start = GPSD_WATCH_START;
    n = write(sockfd, watch_start, strlen(watch_start));
    if (n < 0)
    {
        error("ERROR writing to socket");
    } else 
    {
        printf("\n==================================");
        printf("\nWATCH command sent to gpsd daemon.");
        printf("\n==================================\n\n");
    }
}

void gps_read()
{
    /* Zeroes buffer before reading stuff back */
    bzero(buffer, 1024);
    n = read(sockfd, buffer, 1023);
    if (n < 0)
    {
        error("ERROR reading from socket");
    }
    printf("%s\n\n", buffer);
}

void gps_end_watch()
{
    /* Tells daemon to stop watching devices */
    watch_end = GPSD_WATCH_END;
    n = write(sockfd, watch_end, strlen(watch_end));
    if (n < 0)
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
