#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "data.h"
#include "quadtree.h"



//--- refer to my work of assignmen 1

// read the data using fopen and strtold (Interpret and store the fourth, fifth, sixth and seventh
// arguments)and make the space for storing node and allocate the longtitude and latitude data.
// read the data of city and allocate the data and read the longtitude and latitude data and
// print out the matched data 
int main(int argc, char *argv[]) {

    char *end;
    FILE* csv_file = fopen(argv[2], "r");
    assert(csv_file);
    FILE* output_file = fopen(argv[3], "w");
    assert(output_file);
    long double bl_x = strtold(argv[4], &end);
    long double bl_y = strtold(argv[5], &end);
    long double tp_x = strtold(argv[6], &end);
    long double tp_y = strtold(argv[7], &end);

    quadtreeNode_t *root_node = create_quadnode();

    assignBLToNode(bl_x, bl_y, root_node);
    assignTLToNode(tp_x, tp_y, root_node);

    city_t *tempCity;
    citySkipHeaderLine(csv_file);
    while((tempCity = cityRead(csv_file)) != NULL){
        assignCityToQuadtree(tempCity, root_node);        
    }
    
    // get matached address and return it if it is true, return NOTFOUND if not.
    char string_x[MAX_SIZE+1], string_y[MAX_SIZE+1];
    // allocate space for the point
    point2D_t *point = create_point();
    while((scanf("%s %s\n", string_x, string_y)) != EOF) {
        double value_x = strtod(string_x, NULL) ;
        double value_y = strtod(string_y, NULL) ;
        fprintf(output_file,"%s %s \n", string_x, string_y);
        fprintf(stdout,"%s %s -->", string_x, string_y) ;
        assignPostoPoint2D(value_x, value_y, point);
        searchPoint(point, root_node, output_file);
        
    }
    freeQuadtree(root_node);
    free(point);
    fclose(csv_file);
    fclose(output_file);
}




