/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: John George
 *
 * Created on 04 March 2016, 22:33
 */

#include "gps.h"

static volatile int keep_alive = 1;

void int_handler(int dummy)
{
    keep_alive = 0;
}

int main()
{
    /* Signal handler, to register ^C */
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
