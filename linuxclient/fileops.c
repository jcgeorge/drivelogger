#include "fileops.h"

int filestatus = 0;
FILE *jsonData;

char picked_file[64] = "0";
char FILE_MORNING[] = "../webinterface/data/morningData.json";
char FILE_NOON[] = "../webinterface/data/noonData.json";
char FILE_AFTERNOON[] = "../webinterface/data/afternoonData.json";
char FILE_EVENING[] = "../webinterface/data/eveningData.json";

void filemanager(int filecommand, int hourofday)
{
    if (hourofday <= 11)
    {
        strncpy(picked_file, FILE_MORNING, strlen(FILE_MORNING));
    }
    else if (hourofday > 10 && hourofday < 16)
    {
        strncpy(picked_file, FILE_NOON, strlen(FILE_NOON));
    }
    else if (hourofday > 14 && hourofday < 19)
    {
        strncpy(picked_file, FILE_AFTERNOON, strlen(FILE_AFTERNOON));
    }
    else if (hourofday >= 18)
    {
        strncpy(picked_file, FILE_EVENING, strlen(FILE_EVENING));
    }
    
    if (filecommand == F_OPEN)
    {
        fileopen(picked_file);
    }
    else if (filecommand == F_CLOSE)
    {
        fileclose(picked_file);
    }
}

void fileopen(char *file)
{    
    jsonData = fopen(file, "a+");
    
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
        if (filestatus)
        {
            printf("\nFILE: Error closing %s", file);
            error(NULL);
        }
        else
        {
            printf("\nFILE: Closed %s", file);
        }
    }
}