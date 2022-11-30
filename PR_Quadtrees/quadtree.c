#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>


#include "data.h"
#include "quadtree.h"




//---

/* ----------------------- structures ----------------------- */
// 1Stores the location of datapoint (latitude and longtitude)
struct point2D {
    long double x;
    long double y;
};

// 2Specifies a rectangle given an bottom-left 2D point and a upper-right 2D point
struct rectangle2D {
    point2D_t *bottomleft;
    point2D_t *topright;
    dataPoint_t *data_point;
};

// 3Stores the location and associated information (i.e., the footpath information)
struct dataPoint{
    point2D_t *position;
    city_t *info;
    int start_point;
    dataPoint_t *next;
};

// 4Stores a 2D rectangle and 4 pointers referring to the four children SW, NW, NE and SE
struct quadtreeNode {
    rectangle2D_t *rectangle;
    quadtreeNode_t *SW;
    quadtreeNode_t *NW;
    quadtreeNode_t *NE;
    quadtreeNode_t *SE;
    int status;
};

/* ----------------------- function header ----------------------- */
int inRectangle(rectangle2D_t *rectangle, point2D_t *point);
int rectangleOverlap(rectangle2D_t *rec_a, rectangle2D_t *rec_b);
int determineQuadrant(rectangle2D_t *rect, point2D_t *point);
quadtreeNode_t *create_quadnode();
rectangle2D_t *create_rect();
dataPoint_t *create_datapoint();
point2D_t *create_point();
void assignBLToNode(long double x, long double y, quadtreeNode_t *node);
void assignTLToNode(long double x, long double y, quadtreeNode_t *node);
void assignCityToQuadtree(city_t *city, quadtreeNode_t *root_node);
void addPoint(quadtreeNode_t *node, dataPoint_t *point);
void subdivide_function(quadtreeNode_t *parent_node, dataPoint_t *d_point);
void searchPoint(point2D_t *point, quadtreeNode_t *node, FILE *out_file);
void assignPostoPoint2D(double x, double y, point2D_t *point);
int compare2DPoint(point2D_t *point_1, point2D_t *point_2);
int compareSearchPoint(point2D_t *point_1, point2D_t *point_2);
long double lfabs(long double a);
void freeQuadtree(quadtreeNode_t *node);
void freeRec(rectangle2D_t *rec);


/* --------------------------- functions --------------------------- */

// tests whether a given 2D point lies within the rectangle and returns 1 (TRUE) if it does
int inRectangle(rectangle2D_t *rectangle, point2D_t *point) {
    if (point->x < rectangle->bottomleft->x || point->x > rectangle->topright->x) {
         return 0;
    }
    if (point->y < rectangle->bottomleft->y || point->y > rectangle->topright->y) {
         return 0;
    }
    return 1;
}

// tests whether two given rectangles overlap and returns 1 (TRUE) if they do.
// Returns true if two rectangles (a_bl_x, a_tr_x) and (b_bl_x, b_tr_x) overlap
int rectangleOverlap(rectangle2D_t *rect_a, rectangle2D_t *rect_b) {

    // if rectangle has area 0, no overlap
    if (rect_a->bottomleft->x == rect_a->topright->x || rect_a->bottomleft->y == 
    rect_b->topright->y || rect_b->bottomleft->x == rect_b->topright->x || rect_b->bottomleft->y == rect_b->topright->y) {
        return 0;
    }

    // If one rectangle is on left side of other
    if (rect_a->bottomleft->x > rect_b->topright->x || rect_b->bottomleft->x > rect_a->topright->x) {
        return 0;
    }

    // If one rectangle is above other
    if (rect_a->topright->y > rect_b->bottomleft->y || rect_b->topright->y > rect_a->bottomleft->y) {
        return 0;
    }
    return 1;
}

// returns the quadrant of the rectangle that the point lies in.
int determineQuadrant(rectangle2D_t *rect, point2D_t *point) {
    if (point->x <= (rect->topright->x + rect->bottomleft->x) / 2) {
        if (point->y <= (rect->topright->y + rect->bottomleft->y) / 2) {
            return 0; //SW
        }
        return 1; //NW
    }
    else {
        if (point->y <= (rect->bottomleft->y + rect->topright->y) / 2) {
            return 3; //SE
        }
    }
     return 2; //NE
}

// create the space for 
quadtreeNode_t *create_quadnode() {
    // malloc space for node
    quadtreeNode_t *quadnode = malloc(sizeof(*quadnode));
    assert(quadnode);
    quadnode->rectangle = create_rect();
    quadnode->status = 0; // white
    quadnode->SW = NULL;
    quadnode->SE = NULL;
    quadnode->NW = NULL;
    quadnode->NE = NULL;

    return quadnode;

}


// 
rectangle2D_t *create_rect() {
    rectangle2D_t *new_rect = malloc(sizeof(*new_rect));
    new_rect->bottomleft = create_point();
    new_rect->topright = create_point();
    new_rect->data_point = NULL;
    return new_rect;
}

// create the space for outer of point and return NULL to initialise
dataPoint_t *create_datapoint() {
    dataPoint_t *point_new = malloc(sizeof(*point_new));
    point_new->position = create_point();
    point_new->next = NULL;
    return point_new;
}


// create the space for new point
point2D_t *create_point() {
    point2D_t *point_new = malloc(sizeof(*point_new));
    return point_new;
}

// allocate longtitude and latitude of bottomleft data 
void assignBLToNode(long double x, long double y, quadtreeNode_t *node) {
    node->rectangle->bottomleft->x = x;
    node->rectangle->bottomleft->y = y;
}

// allocate longtitude and latitude of topright data
void assignTLToNode(long double x, long double y, quadtreeNode_t *node) {
    node->rectangle->topright->x = x;
    node->rectangle->topright->y = y;
}


// create 2 datapoint and assign their info as city that you input
void assignCityToQuadtree(city_t *city, quadtreeNode_t *root_node) {
    
    dataPoint_t *datapoint_1 = create_datapoint();
    dataPoint_t *datapoint_2 = create_datapoint();

    datapoint_1->info = datapoint_2->info = city;
    datapoint_1->position->x = get_start_lon(city);
    datapoint_1->position->y = get_start_lat(city);
    datapoint_1->start_point = 1;

    datapoint_2->position->x = get_end_lon(city);
    datapoint_2->position->y = get_end_lat(city);
    datapoint_2->start_point = 2;

    addPoint(root_node, datapoint_1);
    addPoint(root_node, datapoint_2);
}


// add a datapoint given with its 2D coordinates to the quadtree in case of each white, grey and black
void addPoint(quadtreeNode_t *node, dataPoint_t *point) {
    // white and change to black
    if (node->status == 0) { 
        node->rectangle->data_point = point;
        node->status = 2;
    }
    // grey
    else if (node->status == 1) {
        int quad = determineQuadrant(node->rectangle, point->position);
        switch (quad) {
            case 0: // SW
                if (node->SW == NULL) {
                    subdivide_function(node, point);
                }
                addPoint(node->SW, point);
                break;
            case 1: // NW
                if (node->NW == NULL) {
                    subdivide_function(node, point);
                }
                addPoint(node->NW, point);
                break;
            case 2: // NE
                if (node->NE == NULL) {
                    subdivide_function(node, point);
                }
                addPoint(node->NE, point);
                break;
            case 3: // SE
                if (node->SE == NULL) {
                    subdivide_function(node, point);
                }
                addPoint(node->SE, point);
                break;
        }
    } 
    // black
    else if (node->status == 2) {
        // if the location of datapoint & existing datapoint(old) is the exact same
        if(compare2DPoint(point->position, node->rectangle->data_point->position)) {
            if (getFootpathid(point->info) < getFootpathid(node->rectangle->data_point->info)) {
                // likned point
                point->next = node->rectangle->data_point;
                node->rectangle->data_point = point;  
            } 
            else {
                dataPoint_t *tmp_dtpoint = node->rectangle->data_point->next;
                dataPoint_t *prev = node->rectangle->data_point;
                while(tmp_dtpoint != NULL) {
                    if (getFootpathid(point->info) > getFootpathid(tmp_dtpoint->info)) {
                        prev = tmp_dtpoint;
                        tmp_dtpoint = tmp_dtpoint->next;
                    } else {
                        prev->next = point;
                        point->next = tmp_dtpoint;
                        break;
                    }
                }
                if (tmp_dtpoint == NULL) {
                    prev->next = point;
                }
            }
        } 
        else {
            subdivide_function(node, node->rectangle->data_point);
            // change to grey node
            node->status = 1;
            //re-add the existing datapoint
            addPoint(node, node->rectangle->data_point);
            node->rectangle->data_point = NULL;
            addPoint(node, point);
        }
    }
}

// create the space for further node and checking which area it is 
void subdivide_function(quadtreeNode_t *parent_node, dataPoint_t *d_point) {
    quadtreeNode_t *subnode = create_quadnode();
    int quadrant = determineQuadrant(parent_node->rectangle, d_point->position);

    switch (quadrant) {
        case 0: // SW
            subnode->rectangle->bottomleft->x = parent_node->rectangle->bottomleft->x;
            subnode->rectangle->bottomleft->y = parent_node->rectangle->bottomleft->y;
            subnode->rectangle->topright->x = (parent_node->rectangle->bottomleft->x + parent_node->rectangle->topright->x) / 2;
            subnode->rectangle->topright->y = (parent_node->rectangle->topright->y + parent_node->rectangle->bottomleft->y) / 2;
            parent_node->SW = subnode;
            break;
        case 1: // NW
            subnode->rectangle->bottomleft->x = parent_node->rectangle->bottomleft->x;
            subnode->rectangle->bottomleft->y = (parent_node->rectangle->bottomleft->y + parent_node->rectangle->topright->y) / 2;
            subnode->rectangle->topright->x = (parent_node->rectangle->topright->x + parent_node->rectangle->bottomleft->x) / 2;
            subnode->rectangle->topright->y = (parent_node->rectangle->topright->y);
            parent_node->NW = subnode;
            break;
        case 2: // NE
            subnode->rectangle->bottomleft->x = (parent_node->rectangle->bottomleft->x + parent_node->rectangle->topright->x) / 2;
            subnode->rectangle->bottomleft->y = (parent_node->rectangle->topright->y + parent_node->rectangle->bottomleft->y) / 2;
            subnode->rectangle->topright->x = (parent_node->rectangle->topright->x) ;
            subnode->rectangle->topright->y = parent_node->rectangle->topright->y;
            parent_node->NE = subnode;
            break;
        case 3: //SE
            subnode->rectangle->bottomleft->x = (parent_node->rectangle->bottomleft->x + parent_node->rectangle->topright->x) / 2;
            subnode->rectangle->bottomleft->y = parent_node->rectangle->bottomleft->y;
            subnode->rectangle->topright->x = (parent_node->rectangle->topright->x) ;
            subnode->rectangle->topright->y = (parent_node->rectangle->topright->y + parent_node->rectangle->bottomleft->y) / 2;
            parent_node->SE = subnode;
            break;
    }
 
}


// search and print the position in case of white, grey and black
// tests whether a datapoint given by its 2D coordinates lies within the rectangle 
// and returns the datapoint (and its stored information) if it does and NULL otherwise.
void searchPoint(point2D_t *point, quadtreeNode_t *node, FILE *out_file) {
    // black
    if (node->status == 2) {
        // compare point & datapoint->position whether its match or not
        dataPoint_t *tmp_dtpoint = node->rectangle->data_point;
        while(tmp_dtpoint != NULL) {
            if (compareSearchPoint(point, tmp_dtpoint->position)) {
                cityPrint(out_file, tmp_dtpoint->info);
            }
            tmp_dtpoint = tmp_dtpoint->next;
        }
        printf("\n");
    } 
    // grey
    else if (node->status == 1) {
        int quad = determineQuadrant(node->rectangle, point);
        switch (quad) {
            case 0: // SW
                printf(" SW");
                searchPoint(point, node->SW, out_file);
                break;
            case 1: // NW
                printf(" NW");
                searchPoint(point, node->NW, out_file);
                break;
            case 2: // NE
                printf(" NE");
                searchPoint(point, node->NE, out_file);
                break;
            case 3: // SE
                printf(" SE");
                searchPoint(point, node->SE, out_file);
                break;
        }
    } 
    // white
    else if (node->status == 0) {
        printf("NOTFOUND\n");
    }
}


// assign Position to Point2D
void assignPostoPoint2D(double x, double y, point2D_t *point) {
    point->x = x;
    point->y = y;
}

// compare 2 point if they are placed at the same place and return true if it is true for line 233
int compare2DPoint(point2D_t *point_1, point2D_t *point_2) {
    if ((point_1->x == point_2->x) && (point_1->y == point_2->y)) {
        return 1;
    }
    return 0;
}

// compare 2 point if they are partialy different for line 312
int compareSearchPoint(point2D_t *point_1, point2D_t *point_2) {
    if ((lfabs(point_1->x - point_2->x) < EPSLON) && (lfabs(point_1->y - point_2->y) < EPSLON)) {
        return 1;
    }
    return 0;
}

// helper function for changing negative number to positive number to compare the point 
long double lfabs(long double a) {
    if (a < 0) {
        return -a;
    }
    return a;
}

// free the memmory of tree node
void freeQuadtree(quadtreeNode_t *node) {
    if (node->status == 1) {
        if (node->SW != NULL) {
            freeQuadtree(node->SW);
        }
        if (node->NW != NULL) {
            freeQuadtree(node->NW);
        }
        if (node->NE != NULL) {
            freeQuadtree(node->NE);
        }
        if (node->SE != NULL) {
            freeQuadtree(node->SE);
        }
    }
    freeRec(node->rectangle);
    free(node);
}

// free the memmory of rectangle 
void freeRec(rectangle2D_t *rec){
    free(rec->bottomleft);
    free(rec->topright);
    dataPoint_t *tmp = rec->data_point;
    dataPoint_t *prenode;
    while (tmp != NULL) {
        free(tmp->position);
        if (tmp->start_point == 1) {
            free_city(tmp->info);
        }
	    prenode = tmp;
        tmp = tmp->next;
	    free(prenode);
	}
    free(rec);
}


