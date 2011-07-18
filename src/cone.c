
#include "cone.h"

long double *getUVCone(struct intData *inter){
	
	ConeData *data = (ConeData*)inter->obj->data;
	
	long double *result = (long double*) calloc(2, sizeof(long double));
	long double *normal = getNormalVCone(inter->vector, inter->obj);
	long double u, v, h, d, xm, ym, zm;
	
	d = (inter->vector[0]-inter->obj->position[0])*data->ray[0] + (inter->vector[1]-inter->obj->position[1])*data->ray[1] + (inter->vector[2]-inter->obj->position[2])*data->ray[2];
	d = d-data->endPoints[0];
	v = d/(data->endPoints[1]-data->endPoints[0]);

	xm = inter->obj->position[0] - d*data->ray[0]; 
	ym = inter->obj->position[1] - d*data->ray[1];
	zm = inter->obj->position[2] - d*data->ray[2];
	
	xm = inter->vector[0]-xm;
	ym = inter->vector[1]-ym;
	zm = inter->vector[2]-zm;
		
	h = sqrt(pow(xm,2)+pow(ym,2)+pow(zm,2));
	
	xm = xm/h;
	ym = ym/h;
	zm = zm/h;
	
	u = xm*data->green[0] + ym*data->green[1] + zm*data->green[2];
	
	u = acosf(u)/(2*M_PI);
	
	if(data->ray[0]*inter->vector[0] + data->ray[1]*inter->vector[1] + data->ray[2]*inter->vector[2] + d > 0) u = 1-u;
	
	result[0] = u;
	result[1] = v;
	
	free(normal);
	
	return result;
}

intDataNode *getIntersectionCone(long double *eye, long double *directionV, object *obj){
	
	intDataNode *newDataNode = NULL;
	intDataNode *newDataNode2 = NULL;
	ConeData *data = (ConeData*)obj->data;
	long double a, b, c, d, e, f, g, h, A, B, C, x0, y0, z0, xd, yd, zd, xe, ye, ze, xq, yq, zq;

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
    g = (xd*xq)+(yd*yq)+(zd*zq);
    h = pow((data->prop[1]/data->prop[0]),2);
    
    //Valores de A, B y C para la ecuacion
    
    A = pow(a,2)+pow(b,2)+pow(c,2)-(h*pow(g,2));

    B = (2*a*d*(pow(xq,2)-1))+(2*a*xq*((yq*e)+(zq*f)))
    +(2*b*e*(pow(yq,2)-1))+(2*b*yq*((xq*d)+(zq*f)))
    +(2*c*f*(pow(zq,2)-1))+(2*c*zq*((xq*d)+(yq*e)))
    -(h*2*g*d*xq)-(h*2*g*e*yq)-(h*2*g*f*zq);

    C = pow(((pow(xq,2)-1)*d),2)+pow((xq*((yq*e)+(zq*f))),2)
    +pow(((pow(yq,2)-1)*e),2)+pow((yq*((xq*d)+(zq*f))),2)
    +pow(((pow(zq,2)-1)*f),2)+pow((zq*((xq*d)+(yq*e))),2)
    +(2*d*xq*(pow(xq,2)-1)*((yq*e)+(zq*f)))
    +(2*e*yq*(pow(yq,2)-1)*((xq*d)+(zq*f)))
    +(2*f*zq*(pow(zq,2)-1)*((xq*d)+(yq*e)))
    -(h*pow((xq*d),2))-(h*pow((yq*e),2))-(h*pow((zq*f),2))
    -(h*2*d*e*xq*yq)-(h*2*d*f*xq*zq)-(h*2*e*f*yq*zq);
	
	//Calcula discriminante
	long double disc = pow(B, 2)-(4*A*C);
	long double *uv;
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
			uv = getUVCone(newData);
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
				uv = getUVCone(newData2);
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
				uv = getUVCone(newData);
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
		uv = getUVCone(newData);
		newData->u = uv[0];
		newData->v = uv[1];
		free(uv);
		newDataNode = (intDataNode*) malloc(sizeof(intDataNode));
		newDataNode->data = newData;
		newDataNode->next = NULL;
		
		
		long double D = (newData->vector[0]-obj->position[0])*data->ray[0] + (newData->vector[1]-obj->position[1])*data->ray[1] + (newData->vector[2]-obj->position[2])*data->ray[2];
		if ((D > data->endPoints[1]) || (D < data->endPoints[0]))
			return NULL;
		
	}
	
	return newDataNode;	
}


long double *getNormalVCone(long double *intersection,  struct object* obj){
	
	long double *normal = (long double*) calloc(3, sizeof(long double));
	ConeData *data = (ConeData*)obj->data;
	
	long double xi, yi, zi, x0, y0, z0, xq, yq, zq, xl, yl, zl, l, cosLQ, H;
	xi = intersection[0];
	yi = intersection[1];
	zi = intersection[2];
	
	x0 = obj->position[0];
	y0 = obj->position[1];
	z0 = obj->position[2];
	
	xq = data->ray[0];
	yq = data->ray[1];
	zq = data->ray[2];
	
	xl = xi-x0;
	yl = yi-y0;
	zl = zi-z0;
	
	l = sqrt(pow(xl,2)+pow(yl,2)+pow(zl,2));
	
	xl = xl/l;
	yl = yl/l;
	zl = zl/l;
	
	cosLQ = (xq*xl+yq*yl+zq*zl);
	
	H = l/cosLQ;
	
	normal[0] = xi-(x0+H*xq);
	normal[1] = yi-(y0+H*yq);
	normal[2] = zi-(z0+H*zq);
	
	l = sqrt(pow(normal[0],2)+pow(normal[1],2)+pow(normal[2],2));
	
	normal[0] = normal[0]/l;
	normal[1] = normal[1]/l;
	normal[2] = normal[2]/l;
	
	return normal;
}

ConeData *createConeData(long double r, long double g, long double b, long double endPoint1, long double endPoint2, long double Rx, long double Ry, long double Rz, long double prop1, long double prop2, long double greenx, long double greeny, long double greenz){
	ConeData *data = (ConeData*) calloc(1, sizeof(ConeData));
	
	data->endPoints = (long double*) calloc(2, sizeof(long double));
	data->endPoints[0] = endPoint1;
	data->endPoints[1] = endPoint2;

	data->prop = (long double*) calloc(2, sizeof(long double));
	data->prop[0] = prop1;
	data->prop[1] = prop2;	

	data->ray = (long double*) calloc(3, sizeof(long double));
	data->ray[0] = Rx;
	data->ray[1] = Ry;
	data->ray[2] = Rz;
	
	data->green = (long double*) calloc(3, sizeof(long double));
	data->green[0] = greenx;
	data->green[1] = greeny;
	data->green[2] = greenz;
	
	rgb *ncolor = (rgb*) calloc(1, sizeof(rgb));
	ncolor->r = r;
	ncolor->g = g;
	ncolor->b = b;
	
	data->color = ncolor;
	
	return data;
}

rgb* getObjectColorCone(struct object *sphere){
	ConeData *data = (ConeData*)sphere->data;
	
	return data->color;
}


rgb* getTexturesColorCone(struct intData *inter){
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
				//printf("%Lf, %Lf\n",inter->u,inter->v);
				
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

long double ** getBumpmapVectsCone(struct intData *inter, struct object *sphere){
	long double **result = (long double**)calloc(2, sizeof(long double*));
	ConeData *data = (ConeData*)sphere->data;
	long double *n = getNormalVCone(inter->vector, sphere);
	long double *U = (long double*)calloc(3, sizeof(long double));
	long double *V = (long double*)calloc(3, sizeof(long double));
	
	U[0] = data->ray[1]*n[2] - data->ray[2]*n[1];
	U[1] = data->ray[2]*n[0] - data->ray[0]*n[2];
	U[2] = data->ray[0]*n[1] - data->ray[1]*n[0];
	
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
	
	result[0] = U;
	result[1] = V;
	
	free(n);
	return result;
}

object *newCone(long double x, long double y, long double z, long double r, long double g, long double b, long double kd, long double ks, long double kn, long double ka,long double kt,long double kr,long double o1,long double o2,long double o3,long double o4,long double endPoint1, long double endPoint2, long double Rx, long double Ry, long double Rz, long double greenx, long double greeny, long double greenz, long double prop1, long double prop2, int texCount, objTexture * textures, int planeCount, int*planes, textureNode *cutout, textureNode *bumpmap, long double mount)  { 

	object* cone = (object*) calloc(1, sizeof(object));   
	
	//Inicializa posicion:
	cone->position[0] = x;
	cone->position[1] = y;
	cone->position[2] = z; 
	
	//Define funciones:  
	cone->getIntersection = getIntersectionCone; 
	cone->getNormalV = getNormalVCone;  
	cone->getObjectColor = getObjectColorCone; 
	cone->getTexturesColor = getTexturesColorCone;
	cone->getBumpmapVects = getBumpmapVectsCone; 
	 
	//Crea datos adicionales de la esfera:
	cone->data = (void*) createConeData(r, g, b, endPoint1, endPoint2, Rx, Ry,Rz, prop1, prop2, greenx, greeny, greenz);  
	 
	//Define datos adicionales:
	cone->kd = kd;   
	cone->ks = ks;   
	cone->kn = kn;   
	cone->ka = ka; 
	cone->kt = kt;
	cone->kr = kr;
	cone->mount = mount;
	
	cone->o1 = o1; 
	cone->o2 = o2; 
	cone->o3 = o3; 
	cone->o4 = o4;  
	if(cutout != NULL){
		cone->hasCutout = 1;
		cone->cutout = cutout;
	}
	else{
		cone->hasCutout = 0;
		cone->cutout = NULL;
	}
	if(bumpmap != NULL){
		cone->hasBumpmap = 1;
		cone->bumpmap = bumpmap;
	}
	else{
		cone->hasBumpmap = 0;
		cone->bumpmap = NULL;
	}
	
	cone->planeCount = planeCount;
	cone->planes = planes;
	
	cone->texCount = texCount;
	cone->textList = textures;
	
	return cone;  
}

