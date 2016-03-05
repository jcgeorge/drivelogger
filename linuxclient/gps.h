/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   gps.h
 * Author: John George
 *
 * Created on 05 March 2016, 12:33
 */

#ifndef GPS_H
#define GPS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>

#define GPSD_HOST "localhost"
#define GPSD_PORT "2947"
#define GPSD_WATCH_START "?WATCH={\"enable\":true,\"json\":true}"
#define GPSD_WATCH_END "?WATCH={\"enable\":false}"

int sockfd, portno, n;

struct sockaddr_in serv_addr;
struct hostent *server;

char buffer[1024], *watch_start, *watch_end;

void error(char*);
void gps_setup();
void gps_start_watch();
void gps_read();
void gps_end_watch();

#endif /* GPS_H */
