
#include "disc.h"


intDataNode *getIntersectionDisc(long double *eye, long double *directionV, object *obj){
	intDataNode *newDataNode = NULL;
	long double denom, t;
	long double *intVector;
	DiscData* data = (DiscData*)obj->data;
	
	//Producto punto entre vector de direccion y normal(a, b, c)
	denom = (long double)data->a*directionV[0] + data->b*directionV[1] + data->c*directionV[2];

	//Si no tiene solucion, no interseca
	if(denom > -0.0001 && denom < 0.0001) return NULL;
	
	//Se obtiene distancia a la interseccion.
	t = (long double)-(data->a*eye[0] + data->b*eye[1] + data->c*eye[2] + data->d)/denom; 
	
	//Si t < 0, esta detras del ojo, lo ignoramos.
	if(t < 0.0001) return NULL;
	
	//Se obtiene Xi, Yi, Zi:
	intVector = (long double*) calloc(3, sizeof(long double));
	
	intVector[0] = (long double)eye[0] + t*directionV[0];
	intVector[1] = (long double)eye[1] + t*directionV[1];
	intVector[2] = (long double)eye[2] + t*directionV[2];

	//Distancia al centro
	long double distCenter = sqrt(pow(intVector[0]-obj->position[0],2) + pow(intVector[1]-obj->position[1],2) + pow(intVector[2]-obj->position[2],2));
	
	//Se verifica que este dentro del disco, si no es nulo
	if(data->r2 <= distCenter && distCenter <= data->r){ 
		intData *newData = (intData*) malloc(sizeof(intData));
		//Happiness!
		newData->t = t;
		newData->vector = intVector;
		newData->obj = obj;
		
		//long double *uv = getUVDisc(newData);
		newData->u = 0;
		newData->v = 1;
		
		newDataNode = (intDataNode*) malloc(sizeof(intDataNode));
		newDataNode->data = newData;
		newDataNode->next = NULL;
		//free(uv);
	}
	else{
		free(intVector);
	}
	return newDataNode;		
}

long double *getUVDisc(struct intData *inter){
	long double *result = (long double*) calloc(2, sizeof(long double));
	
	result[0] = 0.5;
	result[1] = 0.5;
	
	return result;
}

rgb *getTexturesColorDisc(struct intData *inter){
	rgb *textureColor = (rgb*) calloc(1, sizeof(rgb));
	
	textureColor->r = 0;
	textureColor->g = 0;
	textureColor->b = 0;
	
		//texturas;
	if(inter->obj->o4 > 0){
		objTexture* indexes = inter->obj->textList;
		int x = inter->obj->texCount;
		while(x != 0){
			//STRETCHED
			if(indexes->mode == 0){
				int column, row;
				textureNode *texture = get(indexes->index, textureList);
				
				column = inter->u * texture->width;
				row = inter->v * texture->height;
				
				textureColor->r = texture->data[(row * texture->width + column)].r;
				textureColor->g = texture->data[(row * texture->width + column)].g;
				textureColor->b = texture->data[(row * texture->width + column)].b;
				
				indexes = indexes->next;
				x--;
			}
		}
	}
	return textureColor;
}

long double *getNormalVDisc(long double *intersection, struct object * obj){
	long double *V = (long double*)calloc(3, sizeof(long double));
	DiscData *data = (DiscData*)obj->data;
	V[0] = data->a;
	V[1] = data->b;
	V[2] = data->c;
	
	return V;
}


DiscData *createDiscData(long double r, long double g, long double b, long double ratio, long double innerRatio, long double qx, long double qy, long double qz, long double x, long double y, long double z){
	DiscData *data = (DiscData*) calloc(1, sizeof(DiscData));
	
	data->color = (rgb*) calloc(1, sizeof(rgb));
	data->color->r = r;
	data->color->g = g;
	data->color->b = b;
	
	data->r = ratio;
	data->r2 = innerRatio;
	
	//Se despeja D:
	long double d = -(qx*x + qy*y + qz*z);
	
	data->a = qx;
	data->b = qy;
	data->c = qz;
	data->d = d;
	
	/*
	//determinar origin, h, w, U, V
	//U, V, h, w y origin
	data->origin = (long double*) calloc(3, sizeof(long double));
	data->origin[0] = points[0].x;
	data->origin[1] = points[0].y;
	data->origin[2] = points[0].z;
	//w
	data->w = sqrt(pow(points[1].x-data->origin[0],2)+pow(points[1].y-data->origin[1],2)+pow(points[1].z-data->origin[2],2));
	
	data->U = (long double*) calloc(3, sizeof(long double));
	data->U[0] = ((points[1].x-data->origin[0])/data->w);
	data->U[1] = ((points[1].y-data->origin[1])/data->w);
	data->U[2] = ((points[1].z-data->origin[2])/data->w);

	
	//h
	data->h = sqrt(pow(points[3].x-data->origin[0],2)+pow(points[3].y-data->origin[1],2)+pow(points[3].z-data->origin[2],2));
		

	data->V = (long double*) calloc(3, sizeof(long double));
	data->V[0] = ((points[3].x-data->origin[0])/data->h);
	data->V[1] = ((points[3].y-data->origin[1])/data->h);
	data->V[2] = ((points[3].z-data->origin[2])/data->h);
	*/
	
	return data;
}

rgb* getObjectColorDisc(struct object *disc){
	DiscData *data = (DiscData*)disc->data;
	
	return data->color;
}

object *newDisc(long double x, long double y, long double z, long double r, long double g, long double b, long double kd, long double ks, long double kn, long double ka, long double kt, long double kr, long double o1,  long double o2,  long double o3,  long double o4,  long double ratio, long double innerRatio, long double qx, long double qy, long double qz, int texCount, objTexture* textures,int planeCount, int*planes){

	object* disc = (object*) calloc(1, sizeof(object));   
	
	//Define funciones:  
	disc->getIntersection = getIntersectionDisc;    
	disc->getNormalV = getNormalVDisc;  
	disc->getObjectColor = getObjectColorDisc; 
	disc->getTexturesColor = getTexturesColorDisc; 
	 
	//Crea datos adicionales de la esfera:
	
	printf("inData\n");
	disc->data = (void*) createDiscData(r, g, b, ratio, innerRatio, qx, qy, qz, x, y, z);  
	 
	printf("outData\n");
	disc->position[0] = x;
	disc->position[1] = y;
	disc->position[2] = z;
	 
	//Define datos adicionales:
	disc->kd = kd;   
	disc->ks = ks;   
	disc->kn = kn;   
	disc->ka = ka; 
	disc->kt = kt;
	disc->kr = kr;
	
	disc->o1 = o1;
	disc->o2 = o2;
	disc->o3 = o3;
	disc->o4 = o4;
	
	disc->hasBumpmap = 0;
	disc->hasCutout = 0;
	
	disc->textList = textures;
	disc->texCount = texCount;
	
	disc->planeCount = planeCount;
	disc->planes = planes;
	
	return disc;  
}

