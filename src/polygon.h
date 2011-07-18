#ifndef POLYGON_H
#define POLYGON_H

#include "structs.h"
#include "rayTracer.h"

intDataNode *getIntersectionPolygon(long double *anchorPt, long double *directionV, struct object* obj);
rgb *getColorPolygon(long double *anchorPt, long double *directionV, struct intData* inter, struct lightNode* lightList, int rDepth, int tDepth);
long double *getNormalVPolygon(long double *intersection,  struct object* obj);
object *newPolygon(long double r, long double g, long double b, long double kd, long double ks, long double kn, long double ka,long double kt, long double kr, long double o1,  long double o2,  long double o3,  long double o4,  long double ptsCount, ptsNode* points, int texCount, objTexture* textures,int planeCount, int*planes, textureNode *cutout, textureNode *bumpmap, long double mount);
long double *getUVPolygon(struct intData *inter);
rgb *getTexturesColorPolygon(struct intData *inter);
void translatePolygon(PolygonData *data, long double x, long double y, long double z);
void rotatePolygon(PolygonData *data, long double mx, long double my, long double mz, long double dx,long double dy,long double dz);

#endif
