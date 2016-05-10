/* 
 * File         : main.h
 * Author       : Main header file
 * Last modified: 13/04/2016
 */

#ifndef FILEOPS_H
#define FILEOPS_H

#include "main.h"

//#define FILE_MORNING "../webinterface/data/morningData.txt"
//#define FILE_NOON "../webinterface/data/noonData.txt"
//#define FILE_AFTERNOON "../webinterface/data/afternoonData.txt"
//#define FILE_EVENING "../webinterface/data/eveningData.txt"

extern FILE *jsonData;
extern char picked_file[];
extern char FILE_MORNING[];
extern char FILE_NOON[];
extern char FILE_AFTERNOON[];
extern char FILE_EVENING[];

void filemanager(int, int);
void fileopen(char *);
void fileclose(char *);

#endif /* FILEOPS_H */
