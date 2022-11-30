
#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include <stdio.h>
#include <math.h>
#include <float.h>
#include "data.h"
#include "quadtree.h"

#define EPSLON 10e-12

/* implement by implement sheets */

/*  defimition structure */
typedef struct point2D point2D_t;

typedef struct rectangle2D rectangle2D_t;

typedef struct dataPoint dataPoint_t;

typedef struct quadtreeNode quadtreeNode_t;


// function header
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


#endif

