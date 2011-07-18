#ifndef CONE_H
#define CONE_H

#include "structs.h"
#include "rayTracer.h"

intDataNode *getIntersectionCone(long double *anchorPt, long double *directionV, struct object* obj);
rgb *getColorCone(long double *anchorPt, long double *directionV, struct intData* inter, struct lightNode* lightList, int rDepth, int tDepth);
long double *getNormalVCone(long double *intersection,  struct object* obj);
object *newCone(long double x, long double y, long double z, long double r, long double g, long double b, long double kd, long double ks, long double kn, long double ka,long double kt,long double kr, long double o1,long double o2,long double o3,long double o4,long double endPoint1, long double endPoint2, long double Rx, long double Ry, long double Rz, long double greenx, long double greeny, long double greenz, long double prop1, long double prop2, int texCount, objTexture * textures, int planeCount, int*planes, textureNode *cutout, textureNode *bumpmap, long double mount);

#endif
