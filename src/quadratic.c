
#include "quadratic.h"


intDataNode *getIntersectionQuadratic(long double *eye, long double *directionV, object *obj){
	
	
	QuadraticData *data = (QuadraticData*)obj->data;
	long double A, B, C, D, E, F, G, H, J, K, a, b, c, xd, yd, zd, xe, ye, ze;
	
	A = data->A;
	B = data->B;
	C = data->C;
	D = data->D;
	E = data->E;
	F = data->F;
	G = data->G;
	H = data->H;
	J = data->J;
	K = data->K;
	
	//Vector de direccion de entrada.
	xd = directionV[0];
	yd = directionV[1];
	zd = directionV[2];
	
	//Posicion del ojo.
	xe = eye[0];
	ye = eye[1];
	ze = eye[2];
	
	//Valores de B y C para la ecuacion de la esfera (A = 1 pues el vector de direccion esta normalizado)
	a = (A * pow(xd, 2)) + (B * pow(yd, 2))+ (C * pow(zd, 2)) + (2*((D*xd*yd)     + (E*yd*zd)     + (F*xd*zd)));
	
	b = 2*((A*xe*xd)+(B*ye*yd)+(C*ze*zd)+(D*xe*yd)+(D*ye*xd)
	+(E*ye*zd)+(E*ze*yd)+(F*ze*xd)+(F*xe*zd)+(G*xd)+(H*yd)+(J*zd));
	
	c = (A*pow(xe, 2)) + (B*pow(ye, 2))+
			(C*pow(ze, 2)) + (2*((D*xe*ye) + (E*ye*ze) + (F*ze*xe)+
					(G*xe) + (H*ye) + (J*ze))) + K;
					
	//Calcula discriminante
	long double disc = pow(b, 2)-(4*a*c);
	long double t;
	
	//Si da > 0:
	if(disc > 0.0001){
		
		t = ((-b) - sqrt(disc))/(2*a);
		intDataNode *newDataNode = createDataNode(t, xe, ye, ze, xd, yd, zd, 1, obj);
		t = ((-b) + sqrt(disc))/(2*a);
		intDataNode *newDataNode2 = createDataNode(t, xe, ye, ze, xd, yd, zd, 1, obj);
		
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
	else if (disc < 0.0000001 && disc > -0.0000001){
		t = (-b)/(2*a);
		intDataNode *newDataNode = createDataNode(t, xe, ye, ze, xd, yd, zd, 1, obj);
		if(checkCutPlanes(newDataNode, obj)){
			return newDataNode;
		}
		else{
			return NULL;
		}
		
	}
	return NULL;
}


rgb* getTexturesColorQuadratic(struct intData *inter){
	rgb *result = (rgb*) calloc (1, sizeof(rgb));
	result->r = 1;	result->g = 1;	result->b = 1;
	
	return result;
}

long double *getNormalVQuadratic(long double *intersection,  struct object* obj){
	
	long double *normal = (long double*) calloc(3, sizeof(long double));
	QuadraticData *data = (QuadraticData*)obj->data;
	
	long double xi, yi, zi, A, B, C, D, E, F, G, H, J, K;
	
	xi = intersection[0];
	yi = intersection[1];
	zi = intersection[2];
	
	A = data->A;
	B = data->B;
	C = data->C;
	D = data->D;
	E = data->E;
	F = data->F;
	G = data->G;
	H = data->H;
	J = data->J;
	K = data->K;

	normal[0] = (A*xi)+ (D*yi)+ (F*zi) + G;
	normal[1] = (D*xi) + (B*yi) + (E*zi) + H;
	normal[2] = (F*xi) + (E*yi) + (C*zi) + J;
	
	long double l = sqrt(pow(normal[0],2)+pow(normal[1],2)+pow(normal[2],2));

	normal[0] = normal[0]/l;
	normal[1] = normal[1]/l;
	normal[2] = normal[2]/l;
	
	return normal;

}

QuadraticData *createQuadraticData(long double A,long double B,long double C,
long double D,long double E,long double F,long double G,long double H, long double J,long double K, long double r, long double g, long double b){
	
	QuadraticData *data = (QuadraticData*) calloc(1, sizeof(QuadraticData));
	
	data->A = A;
	data->B = B;
	data->C = C;
	data->D = D;
	data->E = E;
	data->F = F;
	data->G = G;
	data->H = H;
	data->J = J;
	data->K = K;
	
	rgb *ncolor = (rgb*) calloc(1, sizeof(rgb));
	ncolor->r = r;
	ncolor->g = g;
	ncolor->b = b;
	
	data->color = ncolor;
	
	return data;
}

rgb* getObjectColorQuadratic(struct object *quadratic){
	QuadraticData *data = (QuadraticData*)quadratic->data;
	
	return data->color;
}

object *newQuadratic(long double A, long double B, long double C, long double D, 
long double E, long double F, long double G, long double H, long double J, long double K, 
long double r, long double g, long double b, long double kd, long double ks, 
long double kn, long double ka, long double kt, long double kr, long double o1, 
long double o2, long double o3, long double o4, int planeCount, int *planes)  {  

	object* quadratic = (object*) calloc(1, sizeof(object));   
	
	//Define funciones:  
	quadratic->getIntersection = getIntersectionQuadratic;   
	quadratic->getNormalV = getNormalVQuadratic; 
	quadratic->getObjectColor = getObjectColorQuadratic; 
	quadratic->getTexturesColor = getTexturesColorQuadratic;
	 
	//Crea datos adicionales de la esfera:
	quadratic->data = (void*) createQuadraticData(A, B, C, D, E, F, G, H, J, K, r, g, b);  
	 
	//Define datos adicionales:
	quadratic->kd = kd;   
	quadratic->ks = ks;   
	quadratic->kn = kn;   
	quadratic->ka = ka; 
	quadratic->kt = kt;
	quadratic->kr = kr;
	quadratic->o1 = o1;  
	quadratic->o2 = o2;  
	quadratic->o3 = o3;  
	quadratic->o4 = o4;  
	
	quadratic->hasBumpmap = 0;
	quadratic->hasCutout = 0;
	
	quadratic->planeCount = planeCount;
	quadratic->planes = planes;
	
	return quadratic;  
}
