
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "data.h"


/*inspired by https://www.opentechguides.com/
how-to/article/c/142/linked-list-insert.html*/

/*  header function  */
city_t *cityRead(FILE *f);
void cityPrint(FILE *f, city_t *s);
double get_start_lon(city_t *info);
double get_start_lat(city_t *info);
double get_end_lon(city_t *info);
double get_end_lat(city_t *info);
int getFootpathid(city_t *info);
void free_city(city_t *data);

// The details of "struct student" is visible only inside "data.c"
// and cannot be accessed in any other modules 
struct city {
    int footpath_id;
    char *address;
    char *clue_sa;
    char *asset_type;
    double deltaz;
    double distance;
    double grade1in;
    int mcc_id;
    int mccid_int;
    double rlmax;
    double rlmin;
    char *segside;
    int statusid;
    int streetid;
    int street_group;
    double start_lat;
    double start_lon;
    double end_lat;
    double end_lon;
};

typedef struct city city_t;

//reference to week3.7 as fundamental functions
void citySkipHeaderLine(FILE *f) {
	while (fgetc(f) != '\n');
}

// reads a city from file "f" to build a city_t data.
// returns the pointer, or NULL if reading is unsuccessful.
city_t *cityRead(FILE *f) {
	if (feof(f) != 0) {
		return NULL;
	}

	city_t *s = malloc(sizeof(*s));
	int count=1;
	double tempDouble;
	char c;
	char *tempWord = (char *) malloc((MAX_SIZE+1)* sizeof(char)); // malloc(aray_size * sizeof(datatype))
	assert(tempWord);
	while(count <= LEN_DATASET){
		//the case of no information ,,
		if (count==2 || count==3 || count==4 || count==12) {
			c = fgetc(f);	
			if (c == ',') {
				strcpy(tempWord, "\0"); //call null to string	
			}
			else if (c == '"') { 
				fscanf(f, "%[^\"]\",", tempWord);
			}
			else {
				ungetc(c, f);
				fscanf(f, "%[^,],", tempWord);
			}
		} else {
			//the case of end of the file
			if (count == 19){
				fscanf(f, "%lf\n", &tempDouble);
			}
			//the case of continue reading the file
			else {
				fscanf(f, "%lf,", &tempDouble);
			}
		}

		// read 1 st --> value, count
		switch (count) {
		case 1:
			s->footpath_id = (int) tempDouble;
			break;
		case 2:
			s->address = strdup(tempWord);
			break;
		case 3:
			s->clue_sa = strdup(tempWord);
			break;
		case 4:
			s->asset_type = strdup(tempWord);
			break;
		case 5:
			s->deltaz = tempDouble;
			break;
		case 6:
			s->distance = tempDouble;
			break;
		case 7:
			s->grade1in = tempDouble;
			break;
		case 8:
			s->mcc_id = (int) tempDouble;
			break;
		case 9:
			s->mccid_int = (int) tempDouble;
			break;
		case 10:
			s->rlmax = tempDouble;
			break;
		case 11:
			s->rlmin = tempDouble;
			break;
		case 12:
			s->segside = strdup(tempWord);
			break;
		case 13:
			s->statusid = (int) tempDouble;
			break;
		case 14:
			s->streetid = (int) tempDouble;
			break;
		case 15:
			s->street_group = (int) tempDouble;
			break;
		case 16:
			s->start_lat = tempDouble;
			break;
		case 17:
			s->start_lon = tempDouble;
			break;
		case 18:
			s->end_lat = tempDouble;
			break;
		case 19:
			s->end_lon = tempDouble;
			break;

		default:
			printf("Invalid switch case"); 
		}
		count++;
	}
	free(tempWord);
	return s;
}

// prints a city record *s to file "f"
void cityPrint(FILE *f, city_t *s) {
	fprintf(f, "--> footpath_id: %d || address: %s || clue_sa: %s || asset_type: %s "
	"|| deltaz: %.2f || distance: %.2f || grade1in: %.1f || mcc_id: %d || mccid_int: %d "
	"|| rlmax: %.2f || rlmin: %.2f || segside: %s || statusid: %d || streetid: %d "
	"|| street_group: %d || start_lat: %.6f || start_lon: %.6f || end_lat: %.6f || end_lon: %.6f || \n", 
	s->footpath_id, s->address, s->clue_sa, s->asset_type, s->deltaz, s->distance, 
	s->grade1in, s->mcc_id, s->mccid_int, s->rlmax, s->rlmin, s->segside, s->statusid, 
	s->streetid, s->street_group, s->start_lat, s->start_lon, s->end_lat, s->end_lon);
} 

// return start_lon data
double get_start_lon(city_t *info) {
	return info->start_lon;
}

// return start_lat data
double get_start_lat(city_t *info) {
	return info->start_lat;
}

// return end_lon data
double get_end_lon(city_t *info) {
	return info->end_lon;
}

// return end_lat data
double get_end_lat(city_t *info) {
	return info->end_lat;
}

// return footpath_id data
int getFootpathid(city_t *info) {
	return info->footpath_id;
}


// free the memory for city data
void free_city(city_t *data) {
	free(data->address);
	free(data->clue_sa);
	free(data->asset_type);
	free(data->segside);
	free(data);
}


