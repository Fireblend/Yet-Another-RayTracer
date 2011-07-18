#ifndef QUADRATIC_H
#define QUADRATIC_H

#include "structs.h"
#include "rayTracer.h"

intDataNode *getIntersectionQuadratic(long double *anchorPt, long double *directionV, struct object* obj);
rgb *getColorQuadratic(long double *anchorPt, long double *directionV, struct intData* inter, struct lightNode* lightList, int rDepth, int tDepth);
long double *getNormalVQuadratic(long double *intersection,  struct object* obj);
object *newQuadratic(long double A, long double B, long double C, long double D, long double E, long double F, long double G, long double H, long double J, long double K,long double r, long double g, long double b, long double kd, long double ks, 
long double kn, long double ka, long double kt, long double kr, long double o1, long double o2, long double o3, long double o4, int planeCount, int *planes);


#endif
