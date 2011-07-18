
#include "sphere.h"

//Iluminacion ambiente
long double threshold = 0.0000001;



long double *getUVSphere(struct intData *inter){
	
	SphereData *data = (SphereData*)inter->obj->data;
	
	long double *result = (long double*) calloc(2, sizeof(long double));
	long double *normal = getNormalVSphere(inter->vector, inter->obj);
	long double *north = data->north;
	
	long double u, v, a, b, c, d, xm, ym, zm, hx, hy, hz, hl;
	
	//Calcula V
	v = normal[0]*north[0] + normal[1]*north[1] + normal[2]*north[2];
	
	v = acosf(v)/M_PI;

	//Calcula U
	a = north[0];
	b = north[1];
	c = north[2];
	
	d = a * inter->vector[0] + b * inter->vector[1] + c * inter->vector[2] + (-(a * inter->obj->position[0]) - (b * inter->obj->position[1]) - (c * inter->obj->position[2]));
	
	xm = inter->vector[0] - d*a;
	ym = inter->vector[1] - d*b;
	zm = inter->vector[2] - d*c;
	
	hx = xm - inter->obj->position[0];
	hy = ym - inter->obj->position[1];
	hz = zm - inter->obj->position[2];
	
	hl = sqrtl(pow(hx,2)+pow(hy,2)+pow(hz,2));
	
	hx = hx/hl;
	hy = hy/hl;
	hz = hz/hl;
	
	u = hx*data->green[0] + hy*data->green[1] + hz*data->green[2];

	u = acosf(u)/(M_PI*2);
	
	long double jx = data->green[1]*data->north[2] - data->green[2]*data->north[1];
	long double jy = data->green[2]*data->north[0] - data->green[0]*data->north[2];
	long double jz = data->green[0]*data->north[1] - data->green[1]*data->north[0];
	
	a = jx;
	b = jy;
	c = jz;
	
	d = (jx*inter->obj->position[0] + jy*inter->obj->position[1] + jz*inter->obj->position[2]);
	
	if(a*inter->vector[0] + b*inter->vector[1] + c*inter->vector[2] + d > 0) u = 1-u;
	
	result[0] = u;
	result[1] = v;

	free(normal);
	return result;
}


intDataNode *getIntersectionSphere(long double *eye, long double *directionV, object *obj){
	
	SphereData *data = (SphereData*)obj->data;
	long double b, c, xc, yc, zc, xd, yd, zd, xe, ye, ze;
	
	//Coordenadas del centro de la esfera.
	xc = obj->position[0];
	yc = obj->position[1];
	zc = obj->position[2];
	
	//Vector de direccion de entrada.
	xd = directionV[0];
	yd = directionV[1];
	zd = directionV[2];
	
	//Posicion del ojo.
	xe = eye[0];
	ye = eye[1];
	ze = eye[2];
	
	//Valores de B y C para la ecuacion de la esfera (A = 1 pues el vector de direccion esta normalizado)
	b = 2*xd * (xe - xc) + 2*yd * (ye - yc) + 2*zd * (ze - zc);
	c = pow(xe - xc, 2) + pow(ye - yc, 2) + pow(ze - zc, 2) - data->r2;
	
	//Calcula discriminante
	long double disc = pow(b, 2)-(4*c);
	long double t;
	//Si da > 0:
	if(disc > 0.0001){
		t = ((-b) - sqrt(disc))/(2);
		intDataNode *newDataNode = createDataNode(t, xe, ye, ze, xd, yd, zd, 1, obj);
		
		long double *uv = getUVSphere(newDataNode->data);
		newDataNode->data->u = uv[0];
		newDataNode->data->v = uv[1];
		free(uv);
		
		t = ((-b) + sqrt(disc))/(2);
		intDataNode *newDataNode2 = createDataNode(t, xe, ye, ze, xd, yd, zd, 1, obj);
		
		uv = getUVSphere(newDataNode2->data);
		newDataNode2->data->u = uv[0];
		newDataNode2->data->v = uv[1];
		
		free(uv);
		
		//Si no se agrega la 1...
		if(checkCutPlanes(newDataNode, obj)){
			//Si no se agrega la 2....
			if(checkCutPlanes(newDataNode2, obj)){
				return NULL;
			}
			//Se agrega la 2...
			else{
				return newDataNode2;
			}
		}
		//Si se agrega la 1...
		else{
			//Si no se agrega la 2...
			if(checkCutPlanes(newDataNode2, obj)){
				return newDataNode;
			}
			//Si se agrega la 2...
			else{
				newDataNode->next = newDataNode2;
				return newDataNode;
			}
		}
	}
	
	//Si da =  0:
	else if (disc < threshold && disc > -threshold){
		t = (-b)/(2);
		intDataNode *newDataNode = createDataNode(t, xe, ye, ze, xd, yd, zd, 1, obj);
		
		return newDataNode;
	}
	return NULL;
}

rgb* getTexturesColorSphere(struct intData *inter){
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

long double *getNormalVSphere(long double *intersection,  struct object* obj){
	long double *normal = (long double*) calloc(3, sizeof(long double));
	SphereData *data = (SphereData*)obj->data;
	
	normal[0] = (long double)(intersection[0]-obj->position[0])/data->r; 
	normal[1] = (long double)(intersection[1]-obj->position[1])/data->r;
	normal[2] = (long double)(intersection[2]-obj->position[2])/data->r;
	
	long double l = sqrt(pow(normal[0],2)+pow(normal[1],2)+pow(normal[2],2));
	
	normal[0] = normal[0]/l;
	normal[1] = normal[1]/l;
	normal[2] = normal[2]/l;
	
	return normal;
}

SphereData *createSphereData(long double ratio, long double r, long double g, long double b, long double northX, long double northY, long double northZ, long double greenX, long double greenY, long double greenZ){
	SphereData *data = (SphereData*) calloc(1, sizeof(SphereData));
	
	data->r = ratio;
	data->r2 = pow(ratio, 2);
	data->d = (long double)ratio*2;
	
	rgb *ncolor = (rgb*) calloc(1, sizeof(rgb));
	ncolor->r = r;
	ncolor->g = g;
	ncolor->b = b;
	
	data->north = (long double*) calloc (3, sizeof(long double));
	data->green = (long double*) calloc (3, sizeof(long double));
	
	data->north[0] = northX;
	data->north[1] = northY;
	data->north[2] = northZ;
	
	data->green[0] = greenX;
	data->green[1] = greenY;
	data->green[2] = greenZ;
	
	data->color = ncolor;
	
	return data;
}

rgb* getObjectColorSphere(struct object *sphere){
	SphereData *data = (SphereData*)sphere->data;
	
	return data->color;
}

long double ** getBumpmapVectsSphere(struct intData *inter, struct object *sphere){
	long double **result = (long double**)calloc(2, sizeof(long double*));
	SphereData *data = (SphereData*)sphere->data;
	long double *n = getNormalVSphere(inter->vector, sphere);
	long double *U = (long double*)calloc(3, sizeof(long double));
	long double *V = (long double*)calloc(3, sizeof(long double));
	
	U[0] = data->north[1]*n[2] - data->north[2]*n[1];
	U[1] = data->north[2]*n[0] - data->north[0]*n[2];
	U[2] = data->north[0]*n[1] - data->north[1]*n[0];
	
	long double ul = sqrtf(pow(U[0], 2) + pow(U[1], 2)+ pow(U[2],2));

	U[0] = U[0]/ul;
	U[1] = U[1]/ul;
	U[2] = U[2]/ul;
	
	V[0] = n[1]*U[2] - n[2]*U[1];
	V[1] = n[2]*U[0] - n[0]*U[2];
	V[2] = n[0]*U[1] - n[1]*U[0];
	
	long double vl = sqrtf(pow(V[0], 2) + pow(V[1], 2)+ pow(V[2],2));

	V[0] = V[0]/vl;
	V[1] = V[1]/vl;
	V[2] = V[2]/vl;
	//printf("%Lf, %Lf, %Lf, %Lf, %Lf, %Lf\n", U[0], U[1], U[2], V[0], V[1], V[2]);
	result[0] = U;
	result[1] = V;
	
	free(n);
	return result;
}
object *newSphere(long double x, long double y, long double z, long double ratio, long double r, long double g, long double b, long double kd, long double ks, long double kn, long double ka, long double kt, long double kr, long double o1, long double o2, long double o3, long double o4, int planeCount, int*planes, int countText, objTexture *textures, textureNode *cutout, textureNode *bumpmap, long double mount)  {  

	object* sphere = (object*) calloc(1, sizeof(object));   
	
	//Inicializa posicion:
	sphere->position[0] = x;
	sphere->position[1] = y;
	sphere->position[2] = z; 
	
	//Define funciones:  
	sphere->getIntersection = getIntersectionSphere;   
	sphere->getNormalV = getNormalVSphere; 
	sphere->getObjectColor = getObjectColorSphere; 
	sphere->getTexturesColor = getTexturesColorSphere;
	sphere->getBumpmapVects = getBumpmapVectsSphere; 
	 
	//Crea datos adicionales de la esfera:
	sphere->data = (void*) createSphereData(ratio, r, g, b, 0,1,0, 1,0,0);  
	 
	//Define datos adicionales:
	sphere->kd = kd;   
	sphere->ks = ks;   
	sphere->kn = kn;   
	sphere->ka = ka; 
	sphere->kt = kt;
	sphere->o1 = o1;  
	sphere->o2 = o2;  
	sphere->o3 = o3;  
	sphere->o4 = o4;  
	sphere->kr = kr;
	sphere->mount = mount;
	
	if(cutout != NULL){
		sphere->hasCutout = 1;
		sphere->cutout = cutout;
	}
	else{
		sphere->hasCutout = 0;
		sphere->cutout = NULL;
	}
	if(bumpmap != NULL){
		sphere->hasBumpmap = 1;
		sphere->bumpmap = bumpmap;
	}
	else{
		sphere->hasBumpmap = 0;
		sphere->bumpmap = NULL;
	}
	
	sphere->planeCount = planeCount;
	sphere->planes = planes;
	
	sphere->texCount = countText;
	sphere->textList = textures;
	
	return sphere;  
}

