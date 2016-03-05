/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: John George
 *
 */

#include "main.h"
#include "gps.h"
// #include "obd.h"

static volatile int keep_alive = 1;

void error(char *msg)
{
    printf("%s\n", msg);
    exit(0);
}

void int_handler()
{
    keep_alive = 0;
}

int main()
{
    // Signal handler, to register ^C
    signal(SIGINT, int_handler);

    gps_setup();
    
    gps_start_watch();
    
    while(keep_alive)
    {
        gps_read();
    }

    gps_end_watch();

    return 0;
}
