#ifndef DISC_H
#define DISC_H

#include "structs.h"
#include "rayTracer.h"

intDataNode *getIntersectionDisc(long double *anchorPt, long double *directionV, struct object* obj);
rgb *getColorDisc(long double *anchorPt, long double *directionV, struct intData* inter, struct lightNode* lightList, int rDepth, int tDepth);
long double *getNormalVDisc(long double *intersection,  struct object* obj);
rgb *getObjectColorDisc(struct object* obj);
object *newDisc(long double x, long double y, long double z, long double r, long double g, long double b, long double kd, long double ks, long double kn, long double ka, long double kt, long double kr, long double o1,  long double o2,  long double o3,  long double o4,  long double ratio, long double innerRatio, long double qx, long double qy, long double qz, int texCount, objTexture* textures,int planeCount, int*planes);
long double *getUVDisc(struct intData *inter);
rgb *getTexturesColorDisc(struct intData *inter);

#endif
