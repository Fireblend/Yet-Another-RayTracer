
#include "cylinder.h"

//Iluminacion ambiente
intDataNode *getIntersectionCylinder(long double *eye, long double *directionV, object *obj){
	intDataNode *newDataNode = NULL;
	intDataNode *newDataNode2 = NULL;
	CylinderData *data = (CylinderData*)obj->data;
	long double a, b, c, d, e, f, A, B, C, x0, y0, z0, xd, yd, zd, xe, ye, ze, xq, yq, zq;

	//Coordenadas del centro de la esfera.
	xq = data->ray[0];
	yq = data->ray[1];
	zq = data->ray[2];

	//Coordenadas del centro de la esfera.
	x0 = obj->position[0];
	y0 = obj->position[1];
	z0 = obj->position[2];
	
	//Vector de direccion de entrada.
	xd = directionV[0];
	yd = directionV[1];
	zd = directionV[2];
	
	//Posicion del ojo.
	xe = eye[0];
	ye = eye[1];
	ze = eye[2];
	
	a = (pow(xq,2)*xd)+(xq*yd*yq)+(xq*zd*zq)-xd;
	b = (pow(yq,2)*yd)+(xd*xq*yq)+(yq*zd*zq)-yd;
	c = (pow(zq,2)*zd)+(xd*xq*zq)+(yd*yq*zq)-zd;
	d = (xe-x0);
	e = (ye-y0);
	f = (ze-z0);
	
	//Valores de B y C para la ecuacion de la esfera (A = 1 pues el vector de direccion esta normalizado)
	A = pow(a,2)+pow(b,2)+pow(c,2);
	
	B = (2*a*d*(pow(xq,2)-1))+(2*a*xq*((yq*e)+(zq*f)))
	+(2*b*e*(pow(yq,2)-1))+(2*b*yq*((xq*d)+(zq*f)))
	+(2*c*f*(pow(zq,2)-1))+(2*c*zq*((xq*d)+(yq*e)));

	C = pow(((pow(xq,2)-1)*d),2)+pow((xq*((yq*e)+(zq*f))),2)
	+pow(((pow(yq,2)-1)*e),2)+pow((yq*((xq*d)+(zq*f))),2)
	+pow(((pow(zq,2)-1)*f),2)+pow((zq*((xq*d)+(yq*e))),2)
	+(2*d*xq*(pow(xq,2)-1)*((yq*e)+(zq*f)))
	+(2*e*yq*(pow(yq,2)-1)*((xq*d)+(zq*f)))
	+(2*f*zq*(pow(zq,2)-1)*((xq*d)+(yq*e)))-data->r2;
	
	//Calcula discriminante
	long double disc = pow(B, 2)-(4*A*C);
		
	//Si da > 0:
	if(disc > 0){
		long double vx, vy, vz, d, t;
		
		t = ((-B) - sqrt(disc))/(2*A);
		vx = (long double) xe + t*xd;
		vy = (long double) ye + t*yd;
		vz = (long double) ze + t*zd;
		
		d = (vx-obj->position[0])*data->ray[0] + (vy-obj->position[1])*data->ray[1] + (vz-obj->position[2])*data->ray[2];
		
		if ((d > data->endPoints[0]) && (d < data->endPoints[1])){
			intData *newData = (intData*) malloc(sizeof(intData));
			//Dos intersecciones, se busca la primera
			newData->t = t;
			
			//Se asignan valores a la informacion de salida.
			newData->vector = (long double*) calloc(3, sizeof(long double));
			newData->vector[0] = vx;
			newData->vector[1] = vy;
			newData->vector[2] = vz;
			newData->obj = obj;
			
			long double *uv = getUVCylinder(newData);
			newData->u = uv[0];
			newData->v = uv[1];
			free(uv);
			
			newDataNode = (intDataNode*) malloc(sizeof(intDataNode));
			newDataNode->data = newData;
			newDataNode->next = NULL;
		}
		
		t = ((-B) + sqrt(disc))/(2*A);
		vx = (long double) xe + t*xd;
		vy = (long double) ye + t*yd;
		vz = (long double) ze + t*zd;
		
		d = (vx-obj->position[0])*data->ray[0] + (vy-obj->position[1])*data->ray[1] + (vz-obj->position[2])*data->ray[2];
		
		if ((d > data->endPoints[0]) && (d < data->endPoints[1])){
			if(newDataNode){
				intData *newData2 = (intData*) malloc(sizeof(intData));		
				
				//Dos intersecciones, se busca la segunda
				newData2->t = t;
		
				newData2->vector = (long double*) calloc(3, sizeof(long double));
				//Se asignan valores a la informacion de salida.
				newData2->vector[0] = vx;
				newData2->vector[1] = vy;
				newData2->vector[2] = vz;
				newData2->obj = obj;
				long double *uv = getUVCylinder(newData2);
				newData2->u = uv[0];
				newData2->v = uv[1];
				free(uv);
				
				newDataNode2 = (intDataNode*) malloc(sizeof(intDataNode));
				newDataNode2->data = newData2;
				newDataNode2->next = NULL;
				
				newDataNode->next = newDataNode2;
				return newDataNode;
			}
			else{
				intData *newData = (intData*) malloc(sizeof(intData));
		
				//Solo la segunda
				newData->t = t;
				
				//Se asignan valores a la informacion de salida.
				newData->vector = (long double*) calloc(3, sizeof(long double));
				newData->vector[0] = vx;
				newData->vector[1] = vy;
				newData->vector[2] = vz;
				newData->obj = obj;
				
				long double *uv = getUVCylinder(newData);
				newData->u = uv[0];
				newData->v = uv[1];
				free(uv);
				
				newDataNode = (intDataNode*) malloc(sizeof(intDataNode));
				newDataNode->data = newData;
				newDataNode->next = NULL;
				
				return newDataNode;
			}
		}
	}
	
	//Si da =  0:
	else if (disc < 0.0003 && disc > -0.0003){
		intData *newData = (intData*) malloc(sizeof(intData));
		
		//Una interseccion, el rayo es tangente a la esfera
		newData->t = (long double) (-B/(2*A));
		
		newData->vector = (long double*) calloc(3, sizeof(long double));
		//Se asignan valores a la informacion de salida.
		newData->vector[0] = (long double) xe + newData->t*xd;
		newData->vector[1] = (long double) ye + newData->t*yd;
		newData->vector[2] = (long double) ze + newData->t*zd;
		newData->obj = obj;
		
		newDataNode = (intDataNode*) malloc(sizeof(intDataNode));
		newDataNode->data = newData;
		newDataNode->next = NULL;
		
		
		long double D = (newData->vector[0]-obj->position[0])*data->ray[0] + (newData->vector[1]-obj->position[1])*data->ray[1] + (newData->vector[2]-obj->position[2])*data->ray[2];
		if ((D > data->endPoints[1]) || (D < data->endPoints[0]))
			return NULL;
		
	}
	
	return newDataNode;	
}

long double *getUVCylinder(struct intData *inter){
	long double *result = (long double*) calloc(2, sizeof(long double));
	CylinderData *data = (CylinderData*)inter->obj->data;
	
	long double v = (inter->vector[0]-inter->obj->position[0])*data->ray[0]
			+ (inter->vector[1]-inter->obj->position[1])*data->ray[1]
			+ (inter->vector[2]-inter->obj->position[2])*data->ray[2];
			
	v = v-data->endPoints[0];
	
	result[1] = v/data->h;
	
	long double* n = getNormalVCylinder(inter->vector, inter->obj);
	
	long double u = n[0]*-1 + n[1]*0 +n[2]*0;
	u = acos(u);
	
	result[0] = u/(2*M_PI);
	
	//CALCULA G x Q
	long double *GxQ = (long double*) calloc(3, sizeof(long double));

	GxQ[0] = data->G[1]*data->ray[2] - data->G[2]*data->ray[1];
	GxQ[1] = data->G[2]*data->ray[0] - data->G[0]*data->ray[2];
	GxQ[2] = data->G[0]*data->ray[1] - data->G[1]*data->ray[0];
	
	//CALCULA EL D
	long double d = -(GxQ[0]*inter->obj->position[0]+ GxQ[1]*inter->obj->position[1] + GxQ[2]*inter->obj->position[2]);
	
	if ((GxQ[0] * inter->vector[0] + GxQ[1] * inter->vector[1] + GxQ[2] * inter->vector[2] + d) > 0)
	u = 1-u;
		
	free(n);
	free(GxQ);
	return result;
}

long double *getNormalVCylinder(long double *intersection,  struct object* obj){
	long double *normal = (long double*) calloc(3, sizeof(long double));
	long double *m = (long double*) calloc(3, sizeof(long double));
	
	CylinderData *data = (CylinderData*)obj->data;
	long double xi, yi, zi, x0, y0, z0, xq, yq, zq;
	xi = intersection[0];
	yi = intersection[1];
	zi = intersection[2];
	
	x0 = obj->position[0];
	y0 = obj->position[1];
	z0 = obj->position[2];
	
	xq = data->ray[0];
	yq = data->ray[1];
	zq = data->ray[2];
	
	long double D = ((xi - x0) * xq) + ((yi - y0) * yq) + ((zi - z0) * zq);
	
	m[0] = (x0+(D*xq));
	m[1] = (y0+(D*yq));
	m[2] = (z0+(D*zq));
	
	normal[0] = (xi - m[0]);
	normal[1] = (yi - m[1]);
	normal[2] = (zi - m[2]);
	
	long double l = sqrt(pow(normal[0], 2) + pow(normal[1], 2) + pow(normal[2], 2));
	
	normal[0] = normal[0]/l;
	normal[1] = normal[1]/l;
	normal[2] = normal[2]/l;
	
	free(m);
	return normal;
}

rgb *getTexturesColorCylinder(struct intData *inter){
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

CylinderData *createCylinderData(long double ratio, long double r, long double g, long double b, long double endPoint1, long double endPoint2, long double Rx, long double Ry, long double Rz){
	CylinderData *data = (CylinderData*) calloc(1, sizeof(CylinderData));
	
	data->r = ratio;
	data->r2 = pow(ratio, 2);
	
	data->endPoints = (long double*) calloc(2, sizeof(long double));
	data->endPoints[0] = endPoint1;
	data->endPoints[1] = endPoint2;
	data->h = endPoint2-endPoint1;
	
	long double *base = (long double*) calloc(2, sizeof(long double));
	
	if ((Rx <= Ry) && (Rx <= Rz)){
		base[0] = 1;
		base[1] = 0;
		base[2] = 0;
	}

	else if ((Ry <= Rx) && (Ry <= Rz)){
		base[0] = 0;
		base[1] = 1;
		base[2] = 0;
	}

	else if ((Rz <= Rx) && (Rz <= Ry)){
		base[0] = 0;
		base[1] = 0;
		base[2] = 1;
	}
	
	data->G = (long double*) calloc(3, sizeof(long double));
	data->G[0] = Ry*base[2] - Rz*base[1];
	data->G[1] = Rz*base[0] - Rx*base[2];
	data->G[2] = Rx*base[1] - Ry*base[0];
	
	data->ray = (long double*) calloc(3, sizeof(long double));
	data->ray[0] = Rx;
	data->ray[1] = Ry;
	data->ray[2] = Rz;
	
	//printf("%Lf, %Lf, %Lf\n", data->ray[0], data->ray[1], data->ray[2]);
	
	rgb *ncolor = (rgb*) calloc(1, sizeof(rgb));
	ncolor->r = r;
	ncolor->g = g;
	ncolor->b = b;
	
	data->color = ncolor;
	
	return data;
}

rgb* getObjectColorCylinder(struct object *sphere){
	CylinderData *data = (CylinderData*)sphere->data;
	
	return data->color;
}

long double ** getBumpmapVectsCylinder(struct intData *inter, struct object *sphere){
	long double **result = (long double**)calloc(2, sizeof(long double*));
	CylinderData *data = (CylinderData*)sphere->data;
	
	long double *n = getNormalVCylinder(inter->vector, sphere);
	long double *U = (long double*)calloc(3, sizeof(long double));
	long double *V = (long double*)calloc(3, sizeof(long double));
	
	V[0] = data->ray[0];
	V[1] = data->ray[1];
	V[2] = data->ray[2];
	
	long double vl = sqrtf(pow(V[0], 2) + pow(V[1], 2)+ pow(V[2],2));

	V[0] = V[0]/vl;
	V[1] = V[1]/vl;
	V[2] = V[2]/vl;
	
	U[0] = V[1]*n[2] - V[2]*n[1];
	U[1] = V[2]*n[0] - V[0]*n[2];
	U[2] = V[0]*n[1] - V[1]*n[0];
	
	long double ul = sqrtf(pow(U[0], 2) + pow(U[1], 2)+ pow(U[2],2));

	U[0] = U[0]/ul;
	U[1] = U[1]/ul;
	U[2] = U[2]/ul;
		
	result[0] = U;
	result[1] = V;
	free(n);
	return result;
}

object *newCylinder(long double x, long double y, long double z, long double r, long double g, long double b, long double kd, long double ks, long double kn, long double ka,long double kt,long double kr,long double o1,long double o2,long double o3,long double o4,long double ratio, long double endPoint1, long double endPoint2, long double Rx, long double Ry, long double Rz, int texCount, objTexture *textures,int planeCount, int*planes, textureNode *cutout, textureNode *bumpmap, long double mount)  {  

	object* cylinder = (object*) calloc(1, sizeof(object));   
	
	//Inicializa posicion:
	cylinder->position[0] = x;
	cylinder->position[1] = y;
	cylinder->position[2] = z; 
	
	//Define funciones:  
	cylinder->getIntersection = getIntersectionCylinder;   
	cylinder->getNormalV = getNormalVCylinder;
	cylinder->getObjectColor = getObjectColorCylinder; 
	cylinder->getTexturesColor = getTexturesColorCylinder;  
	cylinder->getBumpmapVects = getBumpmapVectsCylinder; 
	cylinder->texCount = texCount;
	cylinder->textList = textures; 
	 
	cylinder->data = (void*) createCylinderData(ratio, r, g, b, endPoint1, endPoint2, Rx, Ry,Rz);  
	
	//Define datos adicionales:
	cylinder->kd = kd;   
	cylinder->ks = ks;   
	cylinder->kn = kn;   
	cylinder->ka = ka; 
	cylinder->kt = kt;
	cylinder->o1 = o1; 
	cylinder->o2 = o2; 
	cylinder->o3 = o3; 
	cylinder->o4 = o4;  
	cylinder->kr = kr;
	cylinder->mount = mount;
	
	if(cutout != NULL){
		cylinder->hasCutout = 1;
		cylinder->cutout = cutout;
	}
	else{
		cylinder->hasCutout = 0;
		cylinder->cutout = NULL;
	}
	if(bumpmap != NULL){
		cylinder->hasBumpmap = 1;
		cylinder->bumpmap = bumpmap;
	}
	else{
		cylinder->hasBumpmap = 0;
		cylinder->bumpmap = NULL;
	}
	
	cylinder->planeCount = planeCount;
	cylinder->planes = planes;
	
	return cylinder;  
}

