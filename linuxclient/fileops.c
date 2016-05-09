/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "fileops.h"

int filestatus = 0;

FILE *jsonData;

void filemanager(int filecommand)
{
    if (filecommand == F_OPEN)
    {
        fileopen(FILE_EVENING);
    }
    else if (filecommand == F_CLOSE)
    {
        fileclose(FILE_EVENING);
    }
}

void filewrite()
{
    
}

void fileopen(char *file)
{    
    jsonData = fopen(file, "w+");
    
    if (DEBUG == D_ON || DEBUG == D_MAX)
    {
        printf("\nFILE: Opened %s", file);
    }
}

void fileclose(char *file)
{
    filestatus = fclose(jsonData);
    
    if (DEBUG == D_ON || DEBUG == D_MAX)
    {
        if (!filestatus)
        {
            printf("\nFILE: Closed %s", file);
        }
        else
        {
            printf("\nFILE: Error closing %s", file);
        }
    }
}