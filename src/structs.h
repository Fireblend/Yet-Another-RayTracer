
#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "math.h"
#include "string.h"

typedef struct intData{
	
	long double t;
	long double *vector;
	struct object *obj;
	long double u, v;
	
} intData;

typedef struct intDataNode{
	struct intData *data;
	struct intDataNode *next;
} intDataNode;

typedef struct ptsNode {
	
	long double x, y, z;
	
} ptsNode;

typedef struct cutPlane{
	
	long double a,b,c,d;
	struct cutPlane *next;
	
} cutPlane;

typedef struct PolygonData {
	
	long double a, b, c, d, h, w;
	long double *V;
	long double *U;
	int vertexCount, unusedAxis;
	struct rgb* color;
	struct ptsNode* vertexes;
	struct ptsNode* square;
	struct ptsNode* squashed;
	
} PolygonData;

typedef struct DiscData {
	
	long double a, b, c, d, r, r2, w,h;
	long double *origin;
	long double *V;
	long double *U;
	struct rgb* color;
	
} DiscData;

typedef struct SphereData {
	
	long double r, r2, d;  
	struct rgb* color;
	long double *north, *green;
	
} SphereData;

typedef struct CylinderData {
	
	long double r, r2, h;  
	struct rgb* color;
	long double *ray, *G;
	long double *endPoints;
	
} CylinderData;


typedef struct QuadraticData {
	
	long double A, B, C, D, E, F, G, H, J, K;  
	struct rgb* color;
	
} QuadraticData;



typedef struct ConeData {
	  
	struct rgb* color;
	long double *ray;
	long double *prop;
	long double *green;
	long double *endPoints;
	
} ConeData;

typedef struct rgb {
	
	float r;
	float g;
	float b;
	
} rgb;

typedef struct lightNode {
	
	long double *data;
	long double c1, c2, c3, r, g, b;
	struct lightNode *next; 
	
} lightNode;

typedef struct objTexture {
	
	long double thRes, tvRes, rightAdjust, leftAdjust, upperAdjust, bottomAdjust, talto, tancho;
	int mode, type, index;
	
	struct objTexture *next; 
	
} objTexture;

typedef struct object {
	//Posicion X, Y, Z
	long double position[3];
	
	//Funcion para informacion de la interseccion
	intDataNode *(* getIntersection)(long double *eye, long double *directionV, struct object* obj);
	//Funcion para color en un punto dado
	rgb *(* getColorAt)(long double *eye, long double *directionV, struct intData* obj, struct lightNode* lights, int rDepth, int tDepth);
	//Funcion de calculo de la normal
	long double *(* getNormalV)(long double *intersection,  struct object* obj);
	rgb*(* getObjectColor)(struct object* obj);
	rgb*(* getTexturesColor)(struct intData* inter);
	long double **(* getBumpmapVects)(struct intData* inter, struct object *sphere);
	
	//Informacion adicional para un tipo especifico de objeto
	void* data;
	
	//ka -> coeficiente de absorcion de luz ENTRE 0 - 1
	//kd -> coeficiente de reflexion difusa   ENTRE 0 - 1
	//ks -> coeficiente de reflexion especular ENTRE 0 - 1
	long double ka, kd, ks, kt, o1,o2,o3,o4, mount;
	
	int texCount;
	objTexture *textList;
	
	int planeCount;
	int *planes;
	
	int hasCutout, hasBumpmap;
	struct textureNode *cutout;
	struct textureNode *bumpmap;
	
	
	//kn -> area de la mancha blanca NUMERO ENTERO >= 1
	int kn;
	
	long double kr;
	 
} object;

typedef struct node {
	
	object *data;
	struct node *next; 
	
} node;

typedef struct textureNode {
	
	rgb *data; 
	int width;
	int height;
	int mode;
	
	struct textureNode *next; 
	
} textureNode;


intDataNode *addIntersection(intDataNode **previous, intData *newData);
node *add(node **previous, object *newData);
void addToTexture(char file[50]);
void trial(textureNode* texture);
objTexture *addObjTexture(objTexture **previous, int index, int mode, int type, long double leftAdjust, long double rightAdjust, long double upperAdjust, long double bottomAdjust, long double tancho, long double talto);
textureNode *addTexture(textureNode **previous, rgb *newData, int width, int height);
lightNode *addLight(lightNode **previous, long double x, long double y, long double z, long double r, long double g, long double b, long double i, long double c1, long double c2, long double c3);
textureNode *get(int count, textureNode *txt);
cutPlane *addCutPlane(cutPlane **previous, ptsNode *points);
cutPlane *getCutPlane(int count, cutPlane *planes);
textureNode * grabTexture(char file[50]);

long double eye[3]; 	//Coordenada del ojo X, Y, Z.
int window[4];			//Coordenadas de la ventana en plano X, Y: Xmin, Xmax, Ymin, Ymax.
rgb bgColor;
node *objectList; 
lightNode *lightList;
textureNode *textureList;
cutPlane *cutPlaneList;

#endif

