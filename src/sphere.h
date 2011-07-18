#ifndef SPHERE_H
#define SPHERE_H

#include "structs.h"
#include "rayTracer.h"

intDataNode *getIntersectionSphere(long double *anchorPt, long double *directionV, struct object* obj);
rgb *getColorSphere(long double *anchorPt, long double *directionV, struct intData* inter, struct lightNode* lightList, int rDepth, int tDepth);
long double *getNormalVSphere(long double *intersection,  struct object* obj);
object *newSphere(long double x, long double y, long double z, long double ratio, long double r, long double g, long double b, long double kd, long double ks, long double kn, long double ka, long double kt, long double kr, long double o1, long double o2, long double o3, long double o4, int planeCount, int*planes, int countText, objTexture *textures, textureNode *cutout, textureNode *bumpmap, long double mount);
rgb* getTexturesColorSphere(struct intData *inter);
#endif
