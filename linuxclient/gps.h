/* 
 * File         : gps.h
 * Author       : Header file for gps.c
 * Last modified: 13/04/2016
 */

#ifndef GPS_H
#define GPS_H

#include "main.h"

#define GPSD_HOST "localhost"
#define GPSD_PORT "2947"
#define GPSD_BUFF_LENGTH 2048
#define GPSD_WATCH_START "?WATCH={\"enable\":true}"
#define GPSD_POLL "?POLL;"
#define GPSD_WATCH_END "?WATCH={\"enable\":false}"

extern int gpsd_socket;
extern int gpsd_socket_status;
extern int gpsd_server_port;

extern struct sockaddr_in gpsd_sockaddr;
extern struct hostent *gpsd_server;

extern char gpsd_data[];

void gps_setup();
void gps_poll();
void gps_end_watch();
void gps_send(char *);
void gps_read();

#endif /* GPS_H */
