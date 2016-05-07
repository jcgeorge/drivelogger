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
#define GPSD_BUFF_LENGTH 1024
#define GPSD_WATCH_START "?WATCH={\"enable\":true,\"json\":true}"
#define GPSD_WATCH_END "?WATCH={\"enable\":false}"

extern int gpsd_socket;
extern int gpsd_socket_status;
extern int gpsd_server_port;

extern struct sockaddr_in gpsd_sockaddr;
extern struct hostent *gpsd_server;

extern char gps_data[];

void gps_setup();
void gps_start_watch();
void gps_read();
void gps_end_watch();

#endif /* GPS_H */
