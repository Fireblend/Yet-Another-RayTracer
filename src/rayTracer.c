/*
||=================================================||
|| 	Instituto Tecnologico de Costa Rica	   ||
|| Ingeniera en Computacion - Computer Graphics    ||
|| 		       Proyecto 2 		   ||
||                                                 ||
|| Sergio Morales - 2007 30434                     ||
||=================================================||

*/

#include "rayTracer.h"

//Macro que invierte los bytes de un int, para leer el header del archivo .avs
#define SWAP(x) ( ((x) << 24) | \
         (((x) << 8) & 0x00ff0000) | \
         (((x) >> 8) & 0x0000ff00) | \
         ((x) >> 24) )
#define FIX(x) (*(unsigned *)&(x) = \
         SWAP(*(unsigned *)&(x)))
		 
int hRes = 1440;
int vRes = 900;

float *framebuffer;

/*Main :)*/
int main(int argc, char** argv) {
	
	loadConfig();
	loadLights();
	loadScene();	
	
	framebuffer = (float*) malloc(3 * hRes * vRes * sizeof(float));
	
	printf("\nRay-Tracer (C) 2009 Laura Vasquez & Sergio Morales\n");
	
 	onWithTheShow(eye, window);
	return 1;
}

void set_pixel(float *buffer, int x, int y, float r, float g, float b)
{
	buffer[(y * hRes + x) * 3 + 0] = r;
	buffer[(y * hRes + x) * 3 + 1] = g;
	buffer[(y * hRes + x) * 3 + 2] = b;
}

void onWithTheShow(long double *eye, int *window){
	int i, j;
	long double tracesI, tracesJ;
	long double L, r, g, b;
	
	//Para cada pixel....
	printf("\nRendering!\n");
	for(i = 0; i < hRes; i++){
		if(i%(hRes/20) == 0)
		printf("...%d%% \n", (i*5/(hRes/20)));
		for (j = 0; j < vRes; j++){
			r = 0.0;
			g = 0.0;
			b = 0.0;
			for(tracesI = 0.5; tracesI < 1; tracesI += 0.5){
				for(tracesJ = 0.5; tracesJ < 1; tracesJ += 0.5){
					
					long double *ptWindow= (long double*)calloc(3, sizeof(long double));
					long double *vDirection= (long double*)calloc(3, sizeof(long double));
					
					//Obtiene Xw, Yw, Zw
					ptWindow[0] = window[0] + (((long double) i + tracesI) * (window[1] - window[0])) / hRes; 
					ptWindow[1] = window[2] + (((long double) j + tracesJ) * (window[3] - window[2])) / vRes; 
					ptWindow[2] = 0.0;
					
					//Obtiene vector de direccion
					vDirection[0] = ptWindow[0] - eye[0];
					vDirection[1] = ptWindow[1] - eye[1];
					vDirection[2] = ptWindow[2] - eye[2];
					
					//Obtiene norma vector de direccion
					L = sqrt(pow(vDirection[0], 2.0) + pow(vDirection[1], 2.0) + pow(vDirection[2], 2.0));
					
					//Normaliza vector de direccion
					vDirection[0] = vDirection[0]/L;
					vDirection[1] = vDirection[1]/L;
					vDirection[2] = vDirection[2]/L;
					
					//printf("hello from onWithTheShow\n");
					//Obtiene color para el punto (i, j)
					rgb *color = getColor(eye, vDirection, 0, 0);
					//printf("hello from onWithTheShow\n");
					if(color == NULL){
						r +=bgColor.r;
						g +=bgColor.g;
						b +=bgColor.b;
					}
					else{
						r += color->r;
						g += color->g;
						b += color->b;
					}
					free(color);
					free(ptWindow);
					free(vDirection);
				}
			}
			set_pixel(framebuffer, i, j, r, g, b);
		}
	}//Cierra para cada pixel.
	
	writeToFile();
	printf("...Done! :)\n\n");
}

rgb *getColor(long double *eye, long double *vDirection, int rDepth, int tDepth){
	
	//Objeto de la primera interseccion.
	intDataNode *inter = firstIntersection(eye, vDirection);
	rgb *foundColor = NULL;
	//Si encuentra una interseccion, obtiene el color en esa interseccion.
	if(inter) 
		foundColor = getObjectColor(eye, vDirection, inter->data, lightList, rDepth, tDepth);
	
	intDataNode *tmp;
		
	while(inter){
		tmp = inter;
		inter = inter->next;
			
		tmp->data->obj = NULL;
		free(tmp->data->vector);
		free(tmp->data);
		free(tmp);
	}			
	
	free(inter);
	return foundColor;
}

intDataNode *firstIntersection(long double *eye, long double *vDirection){
	//Objeto a retornar.
	intDataNode *data = NULL;
	
	//Nodo para recorrer la lista
	node *currentNode = NULL;
	
	//Objeto de cada nodo
	object *currentObject = NULL;
	
	currentNode = objectList;

	//Si quedan nodos de objetos en la lista...
	while(currentNode) {
		//currentObject se asigna al objeto actual y la informacion de la interseccion se calcula.
		currentObject = currentNode->data;
		
		//Interseccion de cada objeto
		intDataNode *intersectionD = currentObject->getIntersection(eye, vDirection, currentObject);
		intDataNode *tmp = intersectionD;
		
		//Si hay interseccion...
		if(intersectionD) {
			while(intersectionD){
				if(intersectionD->data->t > 0.0001  && !checkCutPlanes(intersectionD, currentObject) && exists(intersectionD, currentObject)){
					addIntersection(&data, intersectionD->data);
					intersectionD = intersectionD->next;
				}
				else{
					tmp = intersectionD;
					intersectionD = intersectionD->next;
					
					tmp->data->obj = NULL;
					free(tmp->data->vector);
					free(tmp->data);
					tmp->next = NULL;
					free(tmp);
				}
			}
		}
		//Se sigue con el siguiente nodo.
		currentNode = currentNode->next;
		currentObject = NULL;
	}
	if(currentObject){
		free(currentObject);
		free(currentNode);
	}
	
	return data;
}

void writeToFile(){
	int i,j;
	FILE *fptr;
	int width,height; 
   
	if ((fptr = fopen("results/result.avs","w")) != NULL) {
	
		width = hRes;
		height = vRes;
		
		width = FIX(width);
		height = FIX(height);
	   
		fwrite(&width,sizeof(unsigned int),1,fptr);
		fwrite(&height,sizeof(unsigned int),1,fptr);
	
		height = hRes;
		width = vRes;
		
		for (i=width;i>0;i--) {
			for (j=0;j<height;j++) {
				fputc(255, fptr);										//alpha
				fputc(framebuffer[(i * hRes + j) * 3 + 0]*255, fptr);	//R
				fputc(framebuffer[(i * hRes + j) * 3 + 1]*255, fptr);	//G
				fputc(framebuffer[(i * hRes + j) * 3 + 2]*255, fptr);	//B
			}
		}
	}
	else printf("Error escribiendo a archivo AVS! The sky is falling!\n");
}


rgb *getObjectColor(long double *eye, long double *directionV, struct intData* inter, struct lightNode *lightList, int rDepth, int tDepth){
	
	//Valor de retorno
	rgb *localColor = (rgb*) calloc(1, sizeof(rgb));
	rgb *reflexColor= (rgb*) calloc(1, sizeof(rgb));
	rgb *transparencyColor = (rgb*) calloc(1, sizeof(rgb));
	rgb *textureColor = (rgb*) calloc(1, sizeof(rgb));
	
	reflexColor->r = 0;
	reflexColor->g = 0;
	reflexColor->b = 0;
		
	transparencyColor->r = 0;
	transparencyColor->g = 0;
	transparencyColor->b = 0;
		
	textureColor->r = 0;
	textureColor->g = 0;
	textureColor->b = 0;
	
	long double fatt = 0.0; //Atenuacion de la luz
	long double sr = 0.0; 	//Reflexion especular
	
	long double ir = 0.0; 	//Intensidad
	long double ig = 0.0; 	//Intensidad
	long double ib = 0.0; 	//Intensidad
	long double ia = 0.3; 	
	
	long double cosNL;
	
	long double *V = (long double*)calloc(3, sizeof(long double));
	long double *n = inter->obj->getNormalV(inter->vector, inter->obj);
	
	long double cosND = (long double)n[0]*directionV[0]+n[1]*directionV[1]+n[2]*directionV[2];
	
	if(cosND >= 0)
	{
		n[0] = -n[0];
		n[1] = -n[1];
		n[2] = -n[2];
	}
	
	//printf("%i\n", inter->obj->hasBumpmap);
	if(inter->obj->hasBumpmap){
		//printf("hello\n");
		long double result, mountain;
		mountain = inter->obj->mount;
		int column, row;
		column = inter->u * inter->obj->bumpmap->width;
		row = inter->v * inter->obj->bumpmap->height;
		
		//Modified distance		
		result = inter->obj->bumpmap->data[(row * inter->obj->bumpmap->width + column)].r;
		
		inter->vector[0] = inter->vector[0] + n[0]*(result*mountain);
		inter->vector[1] = inter->vector[1] + n[1]*(result*mountain);
		inter->vector[2] = inter->vector[2] + n[2]*(result*mountain);
		
		free(n);
		n = inter->obj->getNormalV(inter->vector, inter->obj);
		
		cosND = (long double)n[0]*directionV[0]+n[1]*directionV[1]+n[2]*directionV[2];
		if(cosND >= 0)
		{
			n[0] = -n[0];
			n[1] = -n[1];
			n[2] = -n[2];
		}
		
		//corregir normal
		long double k1, k2;
		
		
		if(column == 0){
			column = inter->obj->bumpmap->width;
			k1 = inter->obj->bumpmap->data[(row * inter->obj->bumpmap->width + column)].r*mountain;
			column = 1;
			k1 = -((inter->obj->bumpmap->data[(row * inter->obj->bumpmap->width + column)].r*mountain)-k1)/2; 
		}
		else if(column == inter->obj->bumpmap->width){
			column--;
			column = 0;
			k1 = inter->obj->bumpmap->data[(row * inter->obj->bumpmap->width + column)].r*mountain;
			column = inter->obj->bumpmap->width-2;
			k1 = -((inter->obj->bumpmap->data[(row * inter->obj->bumpmap->width + column)].r*mountain)-k1)/2; 
		}
		else{
			column = column - 1;
			k1 = inter->obj->bumpmap->data[(row * inter->obj->bumpmap->width + column)].r*mountain;
			column = column + 2;
			k1 = -((inter->obj->bumpmap->data[(row * inter->obj->bumpmap->width + column)].r*mountain)-k1)/2; 
		}
		
		
		if(row == 0){
			row = inter->obj->bumpmap->height-1;
			k2 = inter->obj->bumpmap->data[(row * inter->obj->bumpmap->width + column)].r*mountain;
			row = 1;
			k2 = -((inter->obj->bumpmap->data[(row * inter->obj->bumpmap->width + column)].r*mountain)-k2)/2;  
		}
		else if(row == inter->obj->bumpmap->height-1){
			row--;
			row = inter->obj->bumpmap->height-2;
			k2 = inter->obj->bumpmap->data[(row * inter->obj->bumpmap->width + column)].r*mountain;
			row = 0;
			k2 = -((inter->obj->bumpmap->data[(row * inter->obj->bumpmap->width + column)].r*mountain)-k2)/2; 
		}
		else{
			row = row - 1;
			k2 = inter->obj->bumpmap->data[(row * inter->obj->bumpmap->width + column)].r*mountain;
			row = row + 2;
			k2 = -((inter->obj->bumpmap->data[(row * inter->obj->bumpmap->width + column)].r*mountain)-k2)/2; 
		}
		
		long double **UV = inter->obj->getBumpmapVects(inter, inter->obj);
		long double *Ub = UV[0];
		long double *Vb = UV[1];
		
		n[0] = n[0] + Ub[0]*-k1 + Vb[0]*k2;
		n[1] = n[1] + Ub[1]*-k1 + Vb[1]*k2;
		n[2] = n[2] + Ub[2]*-k1 + Vb[2]*k2;
		
		long double l = sqrt(pow(n[0],2)+pow(n[1],2)+pow(n[2],2));
	
		n[0] = n[0]/l;
		n[1] = n[1]/l;
		n[2] = n[2]/l;
	
		free(Ub);
		free(Vb);
		free(UV);
	}
	
	
	long double *R = (long double*)calloc(3, sizeof(long double));
	long double *p = (long double*)calloc(3, sizeof(long double));
	
	
	//Recorre la lista de luces.
	while(lightList){
		
		//Vector hacia la luz
		p[0] = (long double)lightList->data[0] - inter->vector[0];
		p[1] = (long double)lightList->data[1] - inter->vector[1];
		p[2] = (long double)lightList->data[2] - inter->vector[2];
		
		//Normalizar...
		long double l = sqrt(pow(p[0],2) + pow(p[1],2) + pow(p[2],2));
		
		p[0] = p[0]/l;
		p[1] = p[1]/l;
		p[2] = p[2]/l;
		
		//Se calcula la luz :)
		cosNL = (long double)n[0]*p[0]+n[1]*p[1]+n[2]*p[2];
				
		//Machetazo
		if(cosNL > 1.0 ) cosNL = 1.0;
		
		if((cosNL < 0.0)&&(inter->obj->kt > 0.0)){
			
			cosNL = (long double)(-n[0]*p[0])+(-n[1]*p[1])+(-n[2]*p[2]);
			
			if(cosNL > 1.0 ) cosNL = 1.0;
					
			R[0] = -p[0];
			R[1] = -p[1];
			R[2] = -p[2];
		}
		
		else{
			if(cosNL < 0.0 ) cosNL = 0.0;
					
			R[0] = 2*(cosNL * n[0]) - p[0];
			R[1] = 2*(cosNL * n[1]) - p[1];
			R[2] = 2*(cosNL * n[2]) - p[2];
		}		
				
		V[0] = -directionV[0];
		V[1] = -directionV[1];
		V[2] = -directionV[2];
			
		//Calculo de acumulados.
		sr += (long double)pow(V[0]*R[0]+V[1]*R[1]+V[2]*R[2], inter->obj->kn) * inter->obj->ks * lightList->data[3];
					
		//Otro machetazo
		if(sr > 1.0 ) sr = 1.0;
		if(sr < 0.0 ) sr = 0.0; 
			
		fatt = 1/(lightList->c1 + lightList->c2 * l + lightList->c3 * pow(l, 2));
			
		ir += (long double)cosNL * inter->obj->kd * lightList->data[3] * fatt;
		ig += (long double)cosNL * inter->obj->kd * lightList->data[3] * fatt;
		ib += (long double)cosNL * inter->obj->kd * lightList->data[3] * fatt;
			
		//Obstaculo para la sombra
		intDataNode *obstacleNode = firstIntersection(inter->vector, p);
		
		intData *obstacle = NULL;
		
		intDataNode *tmp = obstacleNode;
		
		while (obstacleNode){
			obstacle = obstacleNode->data;
			
			//Si el obstaculo se encuentra mas alla de la distancia
			if(obstacle && obstacle->t > l){ 
				obstacle = NULL;
			}
			
			//Si hay obstaculo...
			if(obstacle){
				ir = ir*obstacle->obj->kt*obstacle->obj->getObjectColor(obstacle->obj)->r;
				ig = ig*obstacle->obj->kt*obstacle->obj->getObjectColor(obstacle->obj)->g;
				ib = ib*obstacle->obj->kt*obstacle->obj->getObjectColor(obstacle->obj)->b;
			}
			tmp = obstacleNode;
			
			tmp->data->obj = NULL;
			free(tmp->data->vector);
			free(tmp->data);
			tmp->next = NULL;
			free(tmp);
			
			obstacleNode = obstacleNode->next;
		}
		
		free(obstacleNode);
		
		ir *= lightList->r;
		ig *= lightList->g;
		ib *= lightList->b;
		
		//Seguimos con la siguiente luz.
		lightList = lightList->next;
	}
	free(lightList);
	
	ir += ia * inter->obj->kd;
	ig += ia * inter->obj->kd;
	ib += ia * inter->obj->kd;
	
	if(ir > 1.0) ir = 1.0;
	if(ig > 1.0) ig = 1.0;
	if(ib > 1.0) ib = 1.0;
	
	//printf("gettingColor\n");
	rgb *objColor = inter->obj->getObjectColor(inter->obj);
	
	//printf("Color: %Lf, %Lf, %Lf\n", objColor->r, objColor->g, objColor->b);
	
	localColor->r = ir * objColor->r;
	localColor->g = ig * objColor->g;
	localColor->b = ib * objColor->b;
	
	
	//reflejo.
	if(inter->obj->o2 > 0 && rDepth < 5){
		long double *reflexVect= (long double*)calloc(3, sizeof(long double));	
		long double cosNV = (long double)n[0]*directionV[0]+n[1]*directionV[1]+n[2]*directionV[2];

		reflexVect[0] = directionV[0] - 2*(cosNV * n[0]);
		reflexVect[1] = directionV[1] - 2*(cosNV * n[1]);
		reflexVect[2] = directionV[2] - 2*(cosNV * n[2]);

		reflexColor =  getColor(inter->vector, reflexVect, rDepth+1, tDepth);
		
		if(reflexColor == NULL){
			 reflexColor = (rgb*) calloc(1, sizeof(rgb));
			 reflexColor->r = bgColor.r;
			 reflexColor->g = bgColor.g;
			 reflexColor->b = bgColor.b;
		}
		free(reflexVect);
	}
	
	//intento de refraccion
	if(inter->obj->o3 > 0 && tDepth < 5){
		//printf("%d\n",tDepth);
		long double nr = inter->obj->kr;
		
		long double *refractedVect = (long double*)calloc(3, sizeof(long double));	
		long double cosNV = (long double)n[0]*V[0]+n[1]*V[1]+n[2]*V[2];
		
		long double cos_ot = (nr * cosNV) - sqrt(1-(nr*nr*(1 - (cosNV * cosNV))));
		
		refractedVect[0] = (cos_ot*n[0]) - (nr*V[0]);
		refractedVect[1] = (cos_ot*n[1]) - (nr*V[1]);
		refractedVect[2] = (cos_ot*n[2]) - (nr*V[2]);
		
		transparencyColor = getColor(inter->vector, refractedVect, rDepth, tDepth+1);
		
		if(transparencyColor == NULL){
			 transparencyColor = (rgb*) calloc(1, sizeof(rgb));
			 transparencyColor->r = bgColor.r;
			 transparencyColor->g = bgColor.g;
			 transparencyColor->b = bgColor.b;
		}
		free(refractedVect);
	}		
	
	//texturas;
	if(inter->obj->o4 > 0){
		textureColor = inter->obj->getTexturesColor(inter);
	}
	
	localColor->r = localColor->r*inter->obj->o1 + reflexColor->r * inter->obj->o2 + transparencyColor->r * inter->obj->o3 + textureColor->r * inter->obj->o4;
	localColor->g = localColor->g*inter->obj->o1 + reflexColor->g * inter->obj->o2 + transparencyColor->g * inter->obj->o3 + textureColor->g * inter->obj->o4;
	localColor->b = localColor->b*inter->obj->o1 + reflexColor->b * inter->obj->o2 + transparencyColor->b * inter->obj->o3 + textureColor->b * inter->obj->o4;
	
	if(sr > 1.0 ) sr = 1.0;
	localColor->r = localColor->r+sr*(1.0 -localColor->r);
	localColor->g = localColor->g+sr*(1.0 -localColor->g);
	localColor->b = localColor->b+sr*(1.0 -localColor->b);	
	
	if(localColor->r > 1.0) localColor->r = 1.0;
	if(localColor->g > 1.0) localColor->g = 1.0;
	if(localColor->b > 1.0) localColor->b = 1.0;
	
	free(V);
	free(R);
	free(n);
	free(p);
	free(reflexColor);
	free(transparencyColor);
	free(textureColor);
	
	return localColor;
}


int checkCutPlanes(intDataNode* dNode, struct object *obj){
	long double x, y, z;
	int n;
	int *planes;
	
	x = dNode->data->vector[0];
	y = dNode->data->vector[1];
	z = dNode->data->vector[2];
	
	n = obj->planeCount;
	planes = obj->planes;
	
	while(n != 0){
		
		cutPlane *plane = getCutPlane(planes[n-1], cutPlaneList);
		
		if(plane->a*x + plane->b*y + plane->c*z + plane->d < 0)
			return 1;
			
		n--;
	}
	
	return 0;
}


int exists(intDataNode* dNode, struct object *obj){
	long double result;
	int row, column;
	if(!obj->hasCutout)return 1;
	
	column = dNode->data->u * obj->cutout->width;
	row = dNode->data->v * obj->cutout->height;
				
	result = obj->cutout->data[(row * obj->cutout->width + column)].r;
	
	if(result < 0.5)
	return 1;
	
	else
	return 0;
}


intDataNode *createDataNode(long double t, long double xe, long double ye, long double ze, long double xd, long double yd, long double zd, int inOut, struct object *obj){
	
	intDataNode *newNode = (intDataNode*) malloc(sizeof(intDataNode));
	intData *newData = (intData*) malloc(sizeof(intData));
	
	//Una interseccion, el rayo es tangente a la esfera
	newData->t = t;
		
	newData->vector = (long double*) calloc(3, sizeof(long double));
	
	//Se asignan valores a la informacion de salida.
	newData->vector[0] = (long double) xe + t*xd;
	newData->vector[1] = (long double) ye + t*yd;
	newData->vector[2] = (long double) ze + t*zd;
	
	newData->obj = obj;
	
	newNode->data = newData;
	newNode->next = NULL;
	
	return newNode;
}
