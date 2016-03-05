/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   gps.h
 * Author: John George
 *
 */

#ifndef GPS_H
#define GPS_H

#include "main.h"

#define GPSD_HOST "localhost"
#define GPSD_PORT "2947"
#define GPSD_WATCH_START "?WATCH={\"enable\":true,\"json\":true}"
#define GPSD_WATCH_END "?WATCH={\"enable\":false}"

extern int gpsd_socket;
extern int gpsd_socket_status;
extern int gpsd_server_port;

struct sockaddr_in gpsd_sockaddr;
struct hostent *gpsd_hostent;

char gps_data[1024];

void gps_setup();
void gps_start_watch();
void gps_read();
void gps_end_watch();

#endif /* GPS_H */
