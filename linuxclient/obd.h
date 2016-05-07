/* 
 * File         : obd.h
 * Author       : Header file for obd.c
 * Last modified: 13/04/2016
 */

#ifndef OBD_H
#define OBD_H

#include "main.h"

#define OBD_HOST "192.168.100.135"
#define OBD_PORT "35000"
#define OBD_INFO "ATI\r"
#define OBD_ECHO_OFF "ATE0\r"
#define OBD_PROTO_AUTO "ATSP0\r"
#define OBD_SPEED "010D1\r"
#define OBD_BUFF_LENGTH 32

extern int obd_socket;
extern int obd_socket_status;
extern int obd_server_port;
extern int obd_speed_dec;

extern struct sockaddr_in obd_sockaddr;
extern struct hostent *obd_server;

extern char obd_buffer[];
extern char obd_speed_hex[];

void obd_setup();
void obd_speed();
void obd_send(char*);
void obd_read();

#endif /* OBD_H */
