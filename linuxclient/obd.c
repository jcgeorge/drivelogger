/* 
 * File         : obd.c
 * Author       : Contains functions for gpsd related operations of the client
 * Last modified: 13/04/2016
 */

#include "obd.h"

int obd_socket = 0;
int obd_socket_status = 0;
int obd_server_port = 0;
int obd_speed_mph = 0;
int obd_speed_weight = 0;

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
    obd_speed_mph = ((int)strtol(obd_speed_hex, NULL, 16)) * 0.621; // kph to mph
    
    if (!SKIP)
    {
        if (GPSD_LOGGED)
        {
            if (obd_speed_mph < 6)
            {
                obd_speed_weight = 100;
            }
            else if (obd_speed_mph > 5 && obd_speed_mph < 11)
            {
                obd_speed_weight = 80;
            }
            else if (obd_speed_mph > 10 && obd_speed_mph < 16)
            {
                obd_speed_weight = 65;
            }
            else if (obd_speed_mph > 15 && obd_speed_mph < 21)
            {
                obd_speed_weight = 50;
            }
            else if (obd_speed_mph > 20 && obd_speed_mph < 31)
            {
                obd_speed_weight = 30;
            }
            else if (obd_speed_mph > 30 && obd_speed_mph < 41)
            {
                obd_speed_weight = 20;
            }
            else if (obd_speed_mph > 40)
            {
                obd_speed_weight = 10;
            }
            
            fprintf(jsonData, "weight: %d},\n", obd_speed_weight);
            
            if (DEBUG == D_ON)
            {
                printf("\nSpeed : %d\nWeight : %d\n", obd_speed_mph, obd_speed_weight);
            }
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
