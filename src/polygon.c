
#include "polygon.h"

//Iluminacion ambiente
//long double ia = 0.3;
//long double threshold = 0.0000001;

ptsNode *squashThoseVertexes(ptsNode* vertexes, int count, int unused){
	ptsNode *newPoints = (ptsNode*) calloc(count, sizeof(ptsNode));
	int i;
	if(unused == 0){
		for(i = 0; i < count; i++){
			ptsNode newNode;
			newNode.x = vertexes[i].y;
			newNode.y = vertexes[i].z;
			newPoints[i] = newNode;
		}
	}
	else if(unused == 1){
		for(i = 0; i < count; i++){
			ptsNode newNode;
			newNode.x = vertexes[i].x;
			newNode.y = vertexes[i].z;
			newPoints[i] = newNode;
		}
	}
	else{
		for(i = 0; i < count; i++){
			ptsNode newNode;
			newNode.x = vertexes[i].x;
			newNode.y = vertexes[i].y;
			newPoints[i] = newNode;
		}
	}
	
	return newPoints;
}

int intersects(long double *intersection, object* obj){
	
	PolygonData* data = (PolygonData*) obj->data;
	ptsNode *squashedVertexes = data->squashed;
	
	int i, counter;
	counter = 0;
	
	long double x, y, m, b;
	
	if(data->unusedAxis == 0){
		x = intersection[1] ; y = intersection[2];
	}
	
	else if(data->unusedAxis == 1){
		x = intersection[0] ; y = intersection[2];
	}
	
	else{
		x = intersection[0] ; y = intersection[1];
	}
	
	for(i = 0; i < data->vertexCount-1; i++){
		long double x1, y1, x2, y2;
		
		x1 = (long double)squashedVertexes[i].x-x;
		y1 = (long double)squashedVertexes[i].y-y;
		x2 = (long double)squashedVertexes[i+1].x-x;
		y2 = (long double)squashedVertexes[i+1].y-y;
		
		if((x1 > 0.0001) && (x2 > 0.0001) && ((y1 < -0.0001 && y2 > 0.0001)||(y2 < -0.0001 && y1 > 0.0001))){
			counter++;
		}
		
		else if(((x1 < -0.0001 && x2 > 0.0001)||(x2 < -0.0001 && x1 > 0.0001))&&((y1 < -0.0001 && y2 > 0.0001)||(y2 < -0.0001 && y1 > 0.0001))){
			if(!(x2-x1 > -0.0001 && x2-x1 < 0.0001)){
				m = (long double)(y2-y1)/(x2-x1);
				
				if(!(m> -0.0001 && m < 0.0001)){
					b = (long double)y1-(m*x1);
					long double result = -b/m;
					if(result > 0.0001)
					
					counter++;
				}
				
			}
		}
	}
	
	long double x1, y1, x2, y2;
		
	x1 = (long double)squashedVertexes[data->vertexCount-1].x-x;
	y1 = (long double)squashedVertexes[data->vertexCount-1].y-y;
	x2 = (long double)squashedVertexes[0].x-x;
	y2 = (long double)squashedVertexes[0].y-y;
		
	if((x1 > 0.0001) && (x2 > 0.0001) && ((y1 < -0.0001 && y2 > 0.0001)||(y2 < -0.0001 && y1 > 0.0001))){
		counter++;
	}
		
	else if(((x1 < -0.0001 && x2 > 0.0001)||(x2 < -0.0001 && x1 > 0.0001))&&((y1 < -0.0001 && y2 > 0.0001)||(y2 < -0.0001 && y1 > 0.0001))){
		if(!(x2-x1 > -0.0001 && x2-x1 < 0.0001)){
			m = (long double)(y2-y1)/(x2-x1);
			
			if(!(m> -0.0001 && m < 0.0001)){
				b = (long double)y1-(m*x1);
				long double result = -b/m;
				if(result > 0.0001)
				
				counter++;
			}
				
		}
	}
	
	if(counter%2 == 1)
		return 1;
		
	else return 0;
}

intDataNode *getIntersectionPolygon(long double *eye, long double *directionV, object *obj){
	
	
	PolygonData* data = (PolygonData*)obj->data;
	if(data->vertexes[0].z == 9500){
		return NULL;
	}
	
	long double *intVector = NULL;
	long double denom, t;
	
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

	//Se verifica que este dentro del poligono, si no es nulo
	if(intersects(intVector, obj)){ 
		intData *newData = (intData*) malloc(sizeof(intData));
		//Happiness!
		newData->t = t;
		newData->vector = intVector;
		newData->obj = obj;
		
		long double *uv = getUVPolygon(newData);
		newData->u = uv[0];
		newData->v = uv[1];
		intDataNode *newDataNode = (intDataNode*) malloc(sizeof(intDataNode));
		newDataNode->data = newData;
		newDataNode->next = NULL;
		free(uv);
		return newDataNode;
	}
	else{
		free(intVector);
		return NULL;
	}	
}

long double *getUVPolygon(struct intData *inter){
	long double *result = (long double*) calloc(2, sizeof(long double));
	PolygonData *data = (PolygonData*)inter->obj->data;
	long double *d = (long double*) calloc(3, sizeof(long double));
	
	d[0] = inter->vector[0]-data->square[0].x;
	d[1] = inter->vector[1]-data->square[0].y;
	d[2] = inter->vector[2]-data->square[0].z;
	
	long double cosDU = (long double)d[0]*data->U[0]+d[1]*data->U[1]+d[2]*data->U[2];
				
	result[0] = cosDU/data->w;
				
	long double cosDV = (long double)d[0]*data->V[0]+d[1]*data->V[1]+d[2]*data->V[2];
			
	result[1] = 1-cosDV/data->h;
	
	free(d);
	return result;
}

rgb *getTexturesColorPolygon(struct intData *inter){
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

long double *getNormalVPolygon(long double *intersection, struct object * obj){
	long double *res = (long double*)calloc(3, sizeof(long double));
	PolygonData *data = (PolygonData*)obj->data;
	res[0] = data->a;
	res[1] = data->b;
	res[2] = data->c;
	return res;
}

ptsNode *getSquare(ptsNode* points, int count, long double*norm, long double d){
	ptsNode *result = (ptsNode*)calloc(3, sizeof(PolygonData));
	long double xMin, yMin, xMax, yMax;
	int i;
	
	xMin = points[0].x;
	xMax = points[0].x;
	yMin = points[0].y;
	yMax = points[0].y;
	
	//obtiene xMin, yMin, xMax, yMax;
	for(i = 1; i < count; i++){
		if(points[i].x < xMin){
			xMin = points[i].x;
		}
		if(points[i].x > xMax){
			xMax = points[i].x;	
		}
		if(points[i].y < yMin){
			yMin = points[i].y;
		}
		if(points[i].y > yMax){
			yMax = points[i].y;
		}
	}
	
	//Origen
	result[0].x = xMin;
	result[0].y = yMin;
	result[0].z = 0;
	
	//Esquina superior izquierda
	result[1].x = xMin;
	result[1].y = yMax;
	result[1].z = 0;
	
	//Esquina inferior derecha
	result[2].x = xMax;
	result[2].y = yMin;
	result[2].z = 0;
	
	//printf("xMin:%Lf,yMin:%Lf,\nxMin:%Lf,yMax:%Lf,\nxMax:%Lf,yMin:%Lf\n\n", xMin,yMin, xMin,yMax, xMax,yMin); 
	
	return result;
}



void refreshPolygonData(PolygonData *data){
	//Calcula A, B, C, D, y DiscardedAxis
	
	long double *vect1 = (long double*) calloc(3, sizeof(long double));
	long double *vect2 = (long double*) calloc(3, sizeof(long double));
	long double *norm = (long double*) calloc(3, sizeof(long double));
	
	//Obtener Normal:
	vect1[0] = data->vertexes[1].x - data->vertexes[0].x;
	vect1[1] = data->vertexes[1].y - data->vertexes[0].y;
	vect1[2] = data->vertexes[1].z - data->vertexes[0].z;
	
	vect2[0] = data->vertexes[2].x - data->vertexes[0].x;
	vect2[1] = data->vertexes[2].y - data->vertexes[0].y;
	vect2[2] = data->vertexes[2].z - data->vertexes[0].z;
	
	norm[0] = (vect1[1] * vect2[2]) - (vect2[1] * vect1[2]);
	norm[1] = (vect2[0] * vect1[2]) - (vect1[0] * vect2[2]);
	norm[2] = (vect1[0] * vect2[1]) - (vect2[0] * vect1[1]);
	
	//Se despeja D:
	long double d = -(norm[0]*data->vertexes[0].x + norm[1]*data->vertexes[0].y + norm[2]*data->vertexes[0].z);
	
	//Se obtiene A, B, C y D a partir de la normal:
	long double l = sqrt(pow(norm[0],2) + pow(norm[1],2) + pow(norm[2],2));
	norm[0] = norm[0]/l;
	norm[1] = norm[1]/l;
	norm[2] = norm[2]/l;
	d = d/l;
	
	//Asignacion de valores resultado
	data->a = norm[0];
	data->b = norm[1];
	data->c = norm[2];
	data->d = d;
	
	//Se deduce que eje se debe descartar
	if(fabsl(norm[0]) > fabsl(norm[1]) && fabsl(norm[0]) > fabsl(norm[2])){
		data->unusedAxis = 0;	
	}
	else if(fabsl(norm[1]) > fabsl(norm[0]) && fabsl(norm[1]) > fabsl(norm[2])){
		data->unusedAxis = 1;
	}
	else
		data->unusedAxis = 2;
	
	//Vertices y el contador de vertices
	data->squashed = squashThoseVertexes(data->vertexes, data->vertexCount, data->unusedAxis);
	
	//w
	data->w = sqrt(pow(data->square[1].x-data->square[0].x,2)+pow(data->square[1].y-data->square[0].y,2)+pow(data->square[1].z-data->square[0].z,2));
	
	data->U = (long double*) calloc(3, sizeof(long double));
	data->U[0] = ((data->square[1].x-data->square[0].x)/data->w);
	data->U[1] = ((data->square[1].y-data->square[0].y)/data->w);
	data->U[2] = ((data->square[1].z-data->square[0].z)/data->w);
	
	//h
	data->h = sqrt(pow(data->square[2].x-data->square[0].x,2)+pow(data->square[2].y-data->square[0].y,2)+pow(data->square[2].z-data->square[0].z,2));
	
	data->V = (long double*) calloc(3, sizeof(long double));
	data->V[0] = ((data->square[2].x-data->square[0].x)/data->h);
	data->V[1] = ((data->square[2].y-data->square[0].y)/data->h);
	data->V[2] = ((data->square[2].z-data->square[0].z)/data->h);
	
	
	free(vect1);
	free(vect2);
	free(norm);
}


void translatePolygon(PolygonData *data, long double x, long double y, long double z){
	if(x == 0 && y == 0 && z == 0){
		//printf("did nothing\n");
		 return;
	 }
	int count = data->vertexCount;
	int i;
	//cambiar el cuadrado
	
	data->square[0].x += x;
	data->square[0].y += y;
	data->square[0].z += z;
	
	data->square[1].x += x;
	data->square[1].y += y;
	data->square[1].z += z;
	
	data->square[2].x += x;
	data->square[2].y += y;
	data->square[2].z += z;
	
	//cambiar los vertices
	for(i = 0; i < count; i++){
		data->vertexes[i].x += x;
		data->vertexes[i].y += y;
		data->vertexes[i].z += z;
	}

	//actualizar el resto
	refreshPolygonData(data);
}

void rotatePolygon(PolygonData *data, long double mx, long double my, long double mz, long double dx,long double dy,long double dz){
	
	if(mx == 0 && my == 0 && mz == 0 && dx == 0 && dy == 0 && dz == 0) return;
	int count = data->vertexCount;
	int i;
	
	long double sin_x,cos_x,sin_y,cos_y,sin_z,cos_z;
	
	sin_x = sinf(mx);
	cos_x = cosf(mx);
	sin_y = sinf(my);
	cos_y = cosf(my);
	sin_z = sinf(mz);
	cos_z = cosf(mz);
	
	//cambiar el cuadrado
	data->square[0].x = cos_z*cos_y*data->square[0].x+((dx*cos_z+(-dy-data->square[0].y)*sin_z)*cos_y+((dz+data->square[0].z)*sin_y+dx));
	data->square[0].y = (sin_x*sin_y*cos_z+cos_x*sin_z)*data->square[0].x+((-sin_x*dz-data->square[0].z*sin_x)*cos_y+((sin_x*dx*sin_y+(cos_x*dy+data->square[0].y*cos_x))*cos_z+(((-sin_x*dy-data->square[0].y*sin_x)*sin_y+cos_x*dx)*sin_z+dy)));
	data->square[0].z = (-cos_x*sin_y*cos_z+sin_x*sin_z)*data->square[0].x+((cos_x*dz+data->square[0].z*cos_x)*cos_y+((-cos_x*dx*sin_y+(sin_x*dy+data->square[0].y*sin_x))*cos_z+(((cos_x*dy+data->square[0].y*cos_x)*sin_y+sin_x*dx)*sin_z+dz)));
	
	data->square[1].x = cos_z*cos_y*data->square[1].x+((dx*cos_z+(-dy-data->square[1].y)*sin_z)*cos_y+((dz+data->square[1].z)*sin_y+dx));
	data->square[1].y = (sin_x*sin_y*cos_z+cos_x*sin_z)*data->square[1].x+((-sin_x*dz-data->square[1].z*sin_x)*cos_y+((sin_x*dx*sin_y+(cos_x*dy+data->square[1].y*cos_x))*cos_z+(((-sin_x*dy-data->square[1].y*sin_x)*sin_y+cos_x*dx)*sin_z+dy)));
	data->square[1].z = (-cos_x*sin_y*cos_z+sin_x*sin_z)*data->square[1].x+((cos_x*dz+data->square[1].z*cos_x)*cos_y+((-cos_x*dx*sin_y+(sin_x*dy+data->square[1].y*sin_x))*cos_z+(((cos_x*dy+data->square[1].y*cos_x)*sin_y+sin_x*dx)*sin_z+dz)));
	
	data->square[2].x = cos_z*cos_y*data->square[2].x+((dx*cos_z+(-dy-data->square[2].y)*sin_z)*cos_y+((dz+data->square[2].z)*sin_y+dx));
	data->square[2].y = (sin_x*sin_y*cos_z+cos_x*sin_z)*data->square[2].x+((-sin_x*dz-data->square[2].z*sin_x)*cos_y+((sin_x*dx*sin_y+(cos_x*dy+data->square[2].y*cos_x))*cos_z+(((-sin_x*dy-data->square[2].y*sin_x)*sin_y+cos_x*dx)*sin_z+dy)));
	data->square[2].z = (-cos_x*sin_y*cos_z+sin_x*sin_z)*data->square[2].x+((cos_x*dz+data->square[2].z*cos_x)*cos_y+((-cos_x*dx*sin_y+(sin_x*dy+data->square[2].y*sin_x))*cos_z+(((cos_x*dy+data->square[2].y*cos_x)*sin_y+sin_x*dx)*sin_z+dz)));
	
	
	//cambiar los vertices
	for(i = 0; i < count; i++){
		data->vertexes[i].x = cos_z*cos_y*data->vertexes[i].x+((dx*cos_z+(-dy-data->vertexes[i].y)*sin_z)*cos_y+((dz+data->vertexes[i].z)*sin_y+dx));
		data->vertexes[i].y = (sin_x*sin_y*cos_z+cos_x*sin_z)*data->vertexes[i].x+((-sin_x*dz-data->vertexes[i].z*sin_x)*cos_y+((sin_x*dx*sin_y+(cos_x*dy+data->vertexes[i].y*cos_x))*cos_z+(((-sin_x*dy-data->vertexes[i].y*sin_x)*sin_y+cos_x*dx)*sin_z+dy)));
		data->vertexes[i].z = (-cos_x*sin_y*cos_z+sin_x*sin_z)*data->vertexes[i].x+((cos_x*dz+data->vertexes[i].z*cos_x)*cos_y+((-cos_x*dx*sin_y+(sin_x*dy+data->vertexes[i].y*sin_x))*cos_z+(((cos_x*dy+data->vertexes[i].y*cos_x)*sin_y+sin_x*dx)*sin_z+dz)));
	}

	//actualizar el resto
	refreshPolygonData(data);
}

PolygonData *createPolygonData(ptsNode* points, long double count, long double r, long double g, long double b){
	PolygonData *data = (PolygonData*) calloc(1, sizeof(PolygonData));

	//Color
	rgb *ncolor = (rgb*) calloc(1, sizeof(rgb));
	ncolor->r = r;
	ncolor->g = g;
	ncolor->b = b;
	
	data->color = ncolor;
	
	//Calcula A, B, C, D, y DiscardedAxis
	
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
	long double d = -(norm[0]*points[0].x + norm[1]*points[0].y + norm[2]*points[0].z);
	
	//Se obtiene A, B, C y D a partir de la normal:
	long double l = sqrt(pow(norm[0],2) + pow(norm[1],2) + pow(norm[2],2));
	norm[0] = norm[0]/l;
	norm[1] = norm[1]/l;
	norm[2] = norm[2]/l;
	d = d/l;

	//printf("%Lf, %Lf, %Lf, %Lf\n", norm[0], norm[1], norm[2], d);
	
	ptsNode *squared = (ptsNode*)calloc(3, sizeof(ptsNode));
	squared[0].x = points[0].x;
	squared[0].y = points[0].y;
	squared[0].z = points[0].z;
	
	squared[1].x = points[1].x;
	squared[1].y = points[1].y;
	squared[1].z = points[1].z;
	
	squared[2].x = points[3].x;
	squared[2].y = points[3].y;
	squared[2].z = points[3].z;
	
	//U, V, h, w
	data->square = squared;
	
	//w
	data->w = sqrt(pow(squared[1].x-squared[0].x,2)+pow(squared[1].y-squared[0].y,2)+pow(squared[1].z-squared[0].z,2));
	
	data->U = (long double*) calloc(3, sizeof(long double));
	data->U[0] = ((squared[1].x-squared[0].x)/data->w);
	data->U[1] = ((squared[1].y-squared[0].y)/data->w);
	data->U[2] = ((squared[1].z-squared[0].z)/data->w);
	
	//h
	data->h = sqrt(pow(squared[2].x-squared[0].x,2)+pow(squared[2].y-squared[0].y,2)+pow(squared[2].z-squared[0].z,2));
	
	data->V = (long double*) calloc(3, sizeof(long double));
	data->V[0] = ((squared[2].x-squared[0].x)/data->h);
	data->V[1] = ((squared[2].y-squared[0].y)/data->h);
	data->V[2] = ((squared[2].z-squared[0].z)/data->h);
	
	//Asignacion de valores resultado
	data->a = norm[0];
	data->b = norm[1];
	data->c = norm[2];
	data->d = d;
	
	//Se deduce que eje se debe descartar
	if(fabsl(norm[0]) > fabsl(norm[1]) && fabsl(norm[0]) > fabsl(norm[2])){
		data->unusedAxis = 0;	
	}
	else if(fabsl(norm[1]) > fabsl(norm[0]) && fabsl(norm[1]) > fabsl(norm[2])){
		data->unusedAxis = 1;
	}
	else
		data->unusedAxis = 2;
	
	//Vertices y el contador de vertices
	data->vertexes = points;
	data->vertexCount = count;
	data->squashed = squashThoseVertexes(points, count, data->unusedAxis);
	
	free(vect1);
	free(vect2);
	free(norm);
	
	return data;
}

rgb* getObjectColorPolygon(struct object *sphere){
	PolygonData *data = (PolygonData*)sphere->data;
	
	return data->color;
}

long double ** getBumpmapVectsPolygon(struct intData *inter, struct object *sphere){
	long double **result = (long double**)calloc(2, sizeof(long double*));
	PolygonData *data = (PolygonData*)sphere->data;
	
	long double *U = (long double*)calloc(3, sizeof(long double));
	long double *V = (long double*)calloc(3, sizeof(long double));
	
	U[0] = data->U[0];
	U[1] = data->U[1];
	U[2] = data->U[2];
	
	V[0] = data->V[0];
	V[1] = data->V[1];
	V[2] = data->V[2];
	
	result[0] = U;
	result[1] = V;
	
	return result;
}


object *newPolygon(long double r, long double g, long double b, long double kd, long double ks, long double kn, long double ka, long double kt, long double kr, long double o1,  long double o2,  long double o3,  long double o4,  long double ptsCount, ptsNode* points, int texCount, objTexture* textures,int planeCount, int*planes, textureNode *cutout, textureNode *bumpmap, long double mount){

	object* polygon = (object*) calloc(1, sizeof(object));   
	
	//Define funciones:  
	polygon->getIntersection = getIntersectionPolygon;    
	polygon->getNormalV = getNormalVPolygon;  
	polygon->getObjectColor = getObjectColorPolygon; 
	polygon->getTexturesColor = getTexturesColorPolygon; 
	polygon->getBumpmapVects = getBumpmapVectsPolygon; 
	 
	//Crea datos adicionales de la esfera:
	polygon->data = (void*) createPolygonData(points, ptsCount, r, g, b);  
	 
	//Define datos adicionales:
	polygon->kd = kd;   
	polygon->ks = ks;   
	polygon->kn = kn;   
	polygon->ka = ka; 
	polygon->kt = kt;
	polygon->kr = kr;
	polygon->mount = mount;
	
	polygon->o1 = o1;
	polygon->o2 = o2;
	polygon->o3 = o3;
	polygon->o4 = o4;
	
	polygon->textList = textures;
	polygon->texCount = texCount;
	
	if(cutout != NULL){
		polygon->hasCutout = 1;
		polygon->cutout = cutout;
	}
	else{
		polygon->hasCutout = 0;
		polygon->cutout = NULL;
	}
	
	if(bumpmap != NULL){
		polygon->hasBumpmap = 1;
		polygon->bumpmap = bumpmap;
	}
	else{
		polygon->hasBumpmap = 0;
		polygon->bumpmap = NULL;
	}
	
	polygon->planeCount = planeCount;
	polygon->planes = planes;
	
	return polygon;  
}

