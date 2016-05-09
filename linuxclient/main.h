/* 
 * File         : main.h
 * Author       : Main header file
 * Last modified: 13/04/2016
 */

#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "gps.h"
#include "obd.h"
#include "fileops.h"

#define WAIT_SECONDS 1

#define D_OFF 0
#define D_ON 1
#define D_MAX 2

#define F_OPEN 1
#define F_CLOSE 0

extern int DEBUG;
extern int GPSD_LOGGED;
extern int SKIP_SAVE;

void error(char*);
void inthandler();
int main();

#endif /* MAIN_H */
