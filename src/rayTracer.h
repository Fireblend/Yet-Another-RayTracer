#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "structs.h"
#include "sphere.h"
#include "disc.h"
#include "polygon.h"
#include "cylinder.h"
#include "cone.h"
#include "quadratic.h"
#include "loader.h"

//Display callback para la window a dibujar.
void drawingMachine();

rgb *getColor(long double *eye, long double *vDirection, int rDepth, int tDepth);
rgb *getObjectColor(long double *eye, long double *directionV, struct intData* inter, struct lightNode *lightList, int rDepth, int tDepth);
intDataNode *firstIntersection(long double *eye, long double *vDirection);
void onWithTheShow(long double *eye, int *window);
void loadConfig();
void loadScene();
void loadLights();	
void writeToFile();
intDataNode *createDataNode(long double t, long double xe, long double ye, long double ze, long double xd, long double yd, long double zd, int inOut, struct object *obj);
int checkCutPlanes(intDataNode* dNode, struct object *obj);
int exists(intDataNode* dNode, struct object *obj);
#endif
