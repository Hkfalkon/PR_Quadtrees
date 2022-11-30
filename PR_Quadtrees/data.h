

#ifndef _DATA_H_
#define _DATA_H_
#include <stdio.h>
typedef struct city city_t;

// constant defnitions
#define LEN_DATASET 19
#define MAX_SIZE 128
#define MAX_NAME_LEN 256
#define MAX_LINE_LEN 512


/*------------ function definitions -----------------------------*/

// skip the header line of .csv file "f"
void citySkipHeaderLine(FILE *f);

// reads a student from file "f" to build a student_t data
// returns the pointer, or NULL if reading is unsuccessful
city_t *cityRead(FILE *f);

// prints the header line for the list of students
//void studentPrintHeader(FILE *f);

// prints a student record *s to file "f"
void cityPrint(FILE *f, city_t *s);

// return start_lon data
double get_start_lon(city_t *info);

// return start_lat data
double get_start_lat(city_t *info);

// return end_lon data
double get_end_lon(city_t *info);

// return end_lat data
double get_end_lat(city_t *info);

// return footpath_id data
int getFootpathid(city_t *info);

// free the memory for city data
void free_city(city_t *data);

#endif


