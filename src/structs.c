#include "structs.h"

//Macro que invierte los bytes de un int, para leer el header del archivo .avs
#define SWAP(x) ( ((x) << 24) | \
         (((x) << 8) & 0x00ff0000) | \
         (((x) >> 8) & 0x0000ff00) | \
         ((x) >> 24) )
#define FIX(x) (*(unsigned *)&(x) = \
         SWAP(*(unsigned *)&(x)))
		 
//Agrega elementos a la lista. Toma una referencia a un nodo ("previo"), que era el head de la lista, y agrega un nuevo nodo
//newNode con newData como dato numerico. Devuelve la nueva lista (Una referencia a newNode).
node *add(node **previous, object *newData)
{
	node *newNode = malloc(sizeof(node));
	//Verificacion de errores
	if (newNode == NULL)
		return NULL;
	
 	//La referencia de newNode se asigna a previous
	newNode->next = *previous;
	
	//Previous es ahora el nuevo nodo.
	*previous = newNode;
	
	//El numero de newNode es ahora newData
	newNode->data = newData;
 
	return *previous;
}

intDataNode *addIntersection(intDataNode **previous, intData *newData)
{
	long double t = newData->t;
	
	intDataNode *newNode = malloc(sizeof(intDataNode));
	//Verificacion de errores
	if (newNode == NULL)
		return NULL;
		
	intDataNode *current = *previous;
	
	
	if(!current){
		newNode->data = newData;
		//La referencia de newNode se asigna a previous
		newNode->next = *previous;
		*previous = newNode;
		return *previous;
	}
	
	if(current->data->t > t){
		newNode->data = newData;
		//La referencia de newNode se asigna a previous
		newNode->next = *previous;
		*previous = newNode;
		return *previous;	
	}
	
	intDataNode *currentNext = current->next;
	
	while(currentNext){
		if(currentNext->data->t > t){
			newNode->data = newData;
			newNode->next = currentNext;
			current->next = newNode;
			return *previous;
		} 
		current = currentNext;
		currentNext = current->next;
	}

	newNode->data = newData;
	newNode->next = NULL;
	current->next = newNode;
	return *previous;
}

textureNode *addTexture(textureNode **previous, rgb *newData, int width, int height)
{
	textureNode *newNode = malloc(sizeof(textureNode));
	//Verificacion de errores
	if (newNode == NULL)
		return NULL;
	
 	//La referencia de newNode se asigna a previous
	newNode->next = *previous;
	
	//Previous es ahora el nuevo nodo.
	*previous = newNode;
	
	//El numero de newNode es ahora newData
	newNode->data = newData;
	newNode->width = width;
	newNode->height = height;
 
	return *previous;
}

lightNode *addLight(lightNode **previous, long double x, long double y, long double z, long double r, long double g, long double b, long double i, long double c1, long double c2, long double c3)
{
	long double *newData = (long double*)calloc(4, sizeof(long double));

	lightNode *newNode = malloc(sizeof(lightNode));
	
	//Verificacion de errores
	if (newNode == NULL)
	return NULL;
	
	newData[0] = x;
	newData[1] = y;
	newData[2] = z;
	newData[3] = i;
	
	newNode->next = *previous;
	*previous = newNode;
	
	newNode->data = newData;
	newNode->c1 = c1;
	newNode->c2 = c2;
	newNode->c3 = c3;
	newNode->b = b;
	newNode->g = g;
	newNode->r = r;
	return *previous;
}

objTexture *addObjTexture(objTexture **previous, int index, int mode, int type, long double leftAdjust, long double rightAdjust, long double upperAdjust, long double bottomAdjust, long double tancho, long double talto)
{
	objTexture *newObjTexture = (objTexture*)calloc(4, sizeof(objTexture));
	
	//Verificacion de errores
	if (newObjTexture == NULL)
	return NULL;
	
	newObjTexture->next = *previous;
	*previous = newObjTexture;
	
	newObjTexture->index = index;
	newObjTexture->mode = mode;
	newObjTexture->type = type;
	
	newObjTexture->leftAdjust = leftAdjust;
	newObjTexture->rightAdjust = rightAdjust;
	newObjTexture->upperAdjust = upperAdjust;
	newObjTexture->bottomAdjust = bottomAdjust;
	newObjTexture->tancho = tancho;
	newObjTexture->talto = talto;
	
	return *previous;
}

void addToTexture(char file[50]){
	FILE *fptr;
	int height, width, pixH, pixW;
	double red, green, blue;
	
	//Intenta abrir el archivo
	if ((fptr = fopen(file, "r")) != NULL){
		int alpha;
					
		//Determina width y height
		fread(&width,sizeof(int),1,fptr);
		width = FIX(width);
							
		fread(&height,sizeof(int),1,fptr);
		height = FIX(height);
			
		rgb *texture = (rgb*) calloc(width * height, sizeof(rgb));
		
		
		for (pixH=0;pixH<height;pixH++) {
			for (pixW=0;pixW<width;pixW++) {
				//Lee el pixel actual
				//Alpha se ignora
				alpha = fgetc(fptr);	
				red =(float)getc(fptr)/255;
				green =(float)fgetc(fptr)/255;
				blue =(float)fgetc(fptr)/255;
				
				texture[pixH * width + pixW].r = red;
				texture[pixH * width + pixW].g = green;
				texture[pixH * width + pixW].b = blue;
			}
		}
		//printf("%i, %i\n", width, height);
		addTexture(&textureList, texture, width, height); 
	}
	else{
		//printf("No se encontro el archivo de imagen\n");
	}		
}

textureNode * grabTexture(char file[50]){
	FILE *fptr;
	int height, width, pixH, pixW;
	double red, green, blue;
	textureNode *newText = NULL;
	//Intenta abrir el archivo
	if ((fptr = fopen(file, "r")) != NULL){
		int alpha;
					
		//Determina width y height
		fread(&width,sizeof(int),1,fptr);
		width = FIX(width);
							
		fread(&height,sizeof(int),1,fptr);
		height = FIX(height);
		
		rgb *texture = (rgb*) calloc(width * height, sizeof(rgb));
		
		for (pixH=0;pixH<height;pixH++) {
			for (pixW=0;pixW<width;pixW++) {
				//Lee el pixel actual
				//Alpha se ignora
				alpha = fgetc(fptr);	
				red =(float)fgetc(fptr)/255;
				green =(float)fgetc(fptr)/255;
				blue =(float)fgetc(fptr)/255;
				
				texture[pixH * width + pixW].r = red;
				texture[pixH * width + pixW].g = green;
				texture[pixH * width + pixW].b = blue;
			}
		}
		
		newText = (textureNode*)calloc(1, sizeof(textureNode));
		newText->width = width;
		newText->height = height;
		newText->data = texture;
	}
	else{
		//printf("No se encontro el archivo de imagen\n");
	}	
	return newText;	
}


textureNode *get(int count, textureNode *txt){
	textureNode *hey = txt;
	
	while(count != 0){
		hey = hey->next;
		count--;
	}
	
	return hey;
} 

cutPlane *addCutPlane(cutPlane **previous, ptsNode *points)
{
	long double d;
	
	long double *vect1 = (long double*) calloc(3, sizeof(long double));
	long double *vect2 = (long double*) calloc(3, sizeof(long double));
	long double *norm = (long double*) calloc(3, sizeof(long double));
	
	//Obtener Normal:
	vect1[0] = points[1].x - points[0].x;
	vect1[1] = points[1].y - points[0].y;
	vect1[2] = points[1].z - points[0].z;
	
	vect2[0] = points[2].x - points[0].x;
	vect2[1] = points[2].y - points[0].y;
	vect2[2] = points[2].z - points[0].z;
	
	norm[0] = (vect1[1] * vect2[2]) - (vect2[1] * vect1[2]);
	norm[1] = (vect2[0] * vect1[2]) - (vect1[0] * vect2[2]);
	norm[2] = (vect1[0] * vect2[1]) - (vect2[0] * vect1[1]);
	
	//Se despeja D:
	d = -(norm[0]*points[0].x + norm[1]*points[0].y + norm[2]*points[0].z);
	
	//Se obtiene A, B, C y D a partir de la normal:
	long double l = sqrt(pow(norm[0],2) + pow(norm[1],2) + pow(norm[2],2));
	norm[0] = norm[0]/l;
	norm[1] = norm[1]/l;
	norm[2] = norm[2]/l;
	d = d/l;
	
	cutPlane *newNode = malloc(sizeof(cutPlane));
	//Verificacion de errores
	if (newNode == NULL)
		return NULL;
	
 	//La referencia de newNode se asigna a previous
	newNode->next = *previous;
	
	//Previous es ahora el nuevo nodo.
	*previous = newNode;
	
	newNode->a = norm[0];
	newNode->b = norm[1];
	newNode->c = norm[2];
	newNode->d = d;
 
	free(vect1);
	free(vect2);
	free(norm);
	
	//free(points);
	//printf("%Lf, %Lf, %Lf, %Lf\n", newNode->a, newNode->b, newNode->c, newNode->d);
	return *previous;
}

cutPlane *getCutPlane(int count, cutPlane *planes){
	cutPlane *current = planes;
	while(count != 0){
		count--;
		current = current->next;
	}
	
	return current;
} 
