/* 
 * File         : obd.c
 * Author       : Contains functions for gpsd related operations of the client
 * Last modified: 13/04/2016
 */

#include "obd.h"

int obd_socket = 0;
int obd_socket_status = 0;
int obd_server_port = 0;
int obd_speed_dec = 0;

struct sockaddr_in obd_sockaddr = {0};
struct hostent *obd_server = {0};

char obd_buffer[OBD_BUFF_LENGTH] = "0";
char obd_speed_hex[2] = "0";

/* 
 * Function     : obd_setup
 * Arguments    : none
 * Description  : sets up a client socket for OBD
 */
void obd_setup()
{
    obd_server_port = atoi(OBD_PORT);
    obd_socket = socket(AF_INET, SOCK_STREAM, 0);    
    if (obd_socket < 0)
    {
        error("\nOBD: Error opening socket!");
    }
    else if (DEBUG == D_MAX)
    {
        printf("\nOBD: Socket established.");
    }
    
    obd_server = gethostbyname(OBD_HOST);    
    if (obd_server == NULL)
    {
        error("\nOBD: Error, no such host!");
    }
    
    bzero((char *) &obd_sockaddr, sizeof(obd_sockaddr));
    obd_sockaddr.sin_family = AF_INET;
    bcopy((char *)obd_server -> h_addr,
        (char *)&obd_sockaddr.sin_addr.s_addr,
        obd_server -> h_length);
    obd_sockaddr.sin_port = htons(obd_server_port);
    
    obd_socket_status = connect(obd_socket,(struct sockaddr *)&obd_sockaddr,
            sizeof(obd_sockaddr));
    if (obd_socket_status < 0)
    {
        error("\nOBD: Error connecting!");
    }
    else if (DEBUG == D_MAX)
    {
        printf("\nOBD: Socket connected.");
    }
    
    obd_send(OBD_ECHO_OFF);
    obd_read();
    obd_send(OBD_PROTO_AUTO);
    obd_read();
}

void obd_speed(int SKIP)
{
    obd_send(OBD_SPEED);
    obd_read();
    obd_speed_hex[0] = obd_buffer[6];
    obd_speed_hex[1] = obd_buffer[7];
    obd_speed_dec = (int)strtol(obd_speed_hex, NULL, 16);
    
    if (!SKIP)
    {
        if (GPSD_LOGGED)
        {
            fprintf(jsonData, "weight: %d},\n", obd_speed_dec);
        }
        
        if (DEBUG == D_ON)
        {
            printf("\nSpeed : %d\n", obd_speed_dec);
        }
    }
}

/* 
 * Function     : obd_send
 * Arguments    : 
 * Description  : Function to send data to OBD socket
 */
void obd_send(char *obd_command)
{
    bzero(obd_buffer, OBD_BUFF_LENGTH);
    strncpy(obd_buffer, obd_command, strlen(obd_command));
    
    obd_socket_status = write(obd_socket, obd_buffer, strlen(obd_buffer));
    if (obd_socket_status < 0)
    {
        error("\nOBD: Error sending to socket!");
    } else if (DEBUG == D_MAX)
    {
        printf("\nOBD: Sent %s", obd_command);
    }
}

/* 
 * Function     : obd_read
 * Arguments    : none
 * Description  : Function to read data from OBD socket
 */
void obd_read()
{
    bzero(obd_buffer, OBD_BUFF_LENGTH);
    obd_socket_status = read(obd_socket, obd_buffer, OBD_BUFF_LENGTH);
    if (obd_socket_status < 0)
    {
        error("\nOBD: Error reading from socket!");
    }
    if (DEBUG == D_MAX)
    {
        printf("\n%s", obd_buffer);
    }
    
    while (obd_buffer[0] == 'S' && obd_buffer[2] == 'A')
    {
        obd_socket_status = read(obd_socket, obd_buffer, OBD_BUFF_LENGTH);
        if (obd_socket_status < 0)
        {
                error("\nOBD: Error reading from socket!");
        }
        if (DEBUG == D_MAX)
        {
            printf("\n%s", obd_buffer);
        }
    }
}
