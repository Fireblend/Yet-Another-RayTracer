#ifndef CYLINDER_H
#define CYLINDER_H

#include "structs.h"
#include "rayTracer.h"

intDataNode *getIntersectionCylinder(long double *anchorPt, long double *directionV, struct object* obj);
rgb *getColorCylinder(long double *anchorPt, long double *directionV, struct intData* inter, struct lightNode* lightList, int rDepth, int tDepth);
long double *getNormalVCylinder(long double *intersection,  struct object* obj);
object *newCylinder(long double x, long double y, long double z, long double r, long double g, long double b, long double kd, long double ks, long double kn, long double ka,long double kt,long double kr,long double o1,long double o2,long double o3,long double o4,long double ratio,long double endPoint1, long double endPoint2, long double Rx, long double Ry, long double Rz,int texCount, objTexture* textures,int planeCount, int*planes, textureNode *cutout, textureNode *bumpmap, long double mount);
long double *getUVCylinder(struct intData *inter);

#endif

