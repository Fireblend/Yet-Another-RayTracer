#include "loader.h"

void loadConfig()
{
	FILE *file;
	char line[50];
	char *temp;
	char *id;			//Identificador del ojo, ventana y fondo
	long double r;	//Red
	long double g;	//Green
	long double b;	//Blue
	

	file = fopen ("data/config.txt", "rt");
	
	while(fgets(line, 50, file) != NULL)
	{
		temp = strtok(line, ",");
		if(strcmp(temp,"\n") == 0 || strncmp(temp,"#",1) == 0)
		continue;
		//sscanf(temp, "%s", id);	
		
		id=(char*)temp;
	
		//Ojo
		if(strcmp(temp,"Eye")==0){
			//x
			temp = strtok(NULL, ",");
			sscanf (temp, "%Lg", &eye[0]);	
			//y
			temp = strtok(NULL, ",");
			sscanf (temp, "%Lg", &eye[1]);	
			temp = strtok(NULL, ",");
			//z
			sscanf (temp, "%Lg", &eye[2]);
		}
		//Ventana
		else if(strcmp(temp,"Window")==0){
			//xMin
			temp = strtok(NULL, ",");
			sscanf (temp, "%d", &window[0]);	
			//xMax
			temp = strtok(NULL, ",");
			sscanf (temp, "%d", &window[1]);	
			//yMin
			temp = strtok(NULL, ",");
			sscanf (temp, "%d", &window[2]);
			//yMax
			temp = strtok(NULL, ",");
			sscanf (temp, "%d", &window[3]);
		}
		//Color de fondo
		else if(strcmp(temp,"Background")==0){
			//Red
			temp = strtok(NULL, ",");
			sscanf (temp, "%Lg", &r);	
			//Greem
			temp = strtok(NULL, ",");
			sscanf (temp, "%Lg", &g);	
			//Blue
			temp = strtok(NULL, ",");
			sscanf (temp, "%Lg", &b);
			
			bgColor.r = r;
			bgColor.g = g;
			bgColor.b = b;
		}
	}
	
	/*
		printf( "eye:%Lg ", eye[0]);
		printf( "eye:%Lg ", eye[1]);
		printf( "eye:%Lg ", eye[2]);
		printf( "w1:%d ", window[0]);
		printf( "w2:%d ", window[1]);
		printf( "w3:%d ", window[2]);
		printf( "w4:%d ", window[3]);
		printf( "fondoR:%Lg ", r);
		printf( "fondoG:%Lg ", g);
		printf( "FondoB:%Lg\n", b);
		printf( "Config Ready! :D\n");
	
*/
	}
	
void loadLights()
{
	FILE *file;
	char line[60];
	char *temp;
	long double x;
	long double y;
	long double z;
	long double r;	//Red
	long double g;	//Green
	long double b;	//Blue
	long double i;	//Intensity
	long double c1;
	long double c2;
	long double c3;
	
	file = fopen ("data/lights.txt", "rt");
	
	while(fgets(line, 60, file) != NULL)
	{
		//x
		temp = strtok(line, ",");
		if(strcmp(temp,"\n") == 0 || strncmp(temp,"#",1) == 0)
		continue;
		sscanf (temp, "%Lg", &x);
		//y	
		temp = strtok(NULL, ",");
		sscanf (temp, "%Lg", &y);	
		temp = strtok(NULL, ",");
		//z
		sscanf (temp, "%Lg", &z);	
		temp = strtok(NULL, ",");
		//r
		sscanf (temp, "%Lg", &r);	
		temp = strtok(NULL, ",");
		//g
		sscanf (temp, "%Lg", &g);
		temp = strtok(NULL, ",");
		//b
		sscanf (temp, "%Lg", &b);
		temp = strtok(NULL, ",");
		//intensity
		sscanf (temp, "%Lg", &i);
		temp = strtok(NULL, ",");
		//c1
		sscanf (temp, "%Lg", &c1);
		temp = strtok(NULL, ",");
		//c2
		sscanf (temp, "%Lg", &c2);
		temp = strtok(NULL, ",");
		//c3
		sscanf (temp, "%Lg", &c3);
		
	
		//Agrega la luz a la escena
		//printf("%Lf, %Lf, %Lf\n", r, g, b);
		addLight(&lightList, x, y,z, r, g, b, i,c1,c2,c3);
			
	}
}

void loadScene(){

	FILE *file;
	char line[100];
	char *temp=(char*)calloc(50, sizeof(char));		
	char *id=(char*)calloc(50, sizeof(char));	
	long double x;
	long double y;
	long double z;
	long double gX,gY,gZ;
	long double r;		//Red
	long double g;		//Green
	long double b;		//Blue
	long double kd;		//coeficiente de reflexion difusa [ENTRE 0 Y 1]
	long double ks;		//coeficiente de reflexion especular [ENTRE 0 Y 1]
	long double kn;		//area de la mancha blanca [NUMERO ENTERO >= 1]
	long double ka;		//coeficiente de absorcion de luz [ENTRE 0 Y 1]
	long double kr;		//coeficiente de reflexion [ENTRE 0 Y 1]
	long double kt;
	long double ratio;	//Radio
	long double xRayo;	//Rayo x
	long double yRayo;	//Rayo y
	long double zRayo;	//Rayo z
	long double lim1;	//Limite 1
	long double lim2;	//Limite 2
	long double prop1;	//Proporcion 1
	long double prop2;	//Proporcion 2
	long double xp;		//x Poligono
	long double yp;		//y Poligono
	long double zp;		//Z Poligono
	long double numPoints;			//Numero vertices poligono
	long double o1,o2,o3,o4;
	long double mount;
	int numPlane,numTexture;
	char bumpM[50],cutOutM[50],planes[50];
	
	//long double height;	//Altura
	object *sphere,*cylinder,*cone,*quadratic,*polygon,*disc;

	file = fopen ("data/scene.txt", "rt");
	
	while(fgets(line, 100, file) != NULL )
	{	
		if(strcmp(line,"\n") == 0 || strncmp(line,"/",1) == 0)
		continue;
		
		if(strncmp(line,"$",1) == 0)
		{	
			strcpy(id,line);
			//printf("\n");
			
			//Color
			fgets(line, 50, file);
			sscanf (line, "\tColor = %Lg,%Lg,%Lg", &r,&g,&b);
			//printf("Color:%Lg,%Lg,%Lg\n",r,g,b);
			
			//Os
			fgets(line, 50, file);
			sscanf (line, "\tOs = %Lg,%Lg,%Lg,%Lg", &o1,&o2,&o3,&o4);
			//printf("Os:%Lg,%Lg,%Lg,%Lg\n",o1,o2,o3,o4);
			
			//Kd
			fgets(line, 50, file);
			sscanf (line, "\tKd = %Lg", &kd);
			//printf("Kd:%Lg\n",kd);
			
			//Ks
			fgets(line, 50, file);
			sscanf (line, "\tKs = %Lg", &ks);
			//printf("Ks:%Lg\n",ks);
			
			//Kn
			fgets(line, 50, file);
			sscanf (line, "\tKn = %Lg", &kn);
			//printf("Kn:%Lg\n",kn);
			
			//Ka
			fgets(line, 50, file);
			sscanf (line, "\tKa = %Lg", &ka);
			//printf("Ka:%Lg\n",ka);
			
			//Kt
			fgets(line, 50, file);
			sscanf (line, "\tKt = %Lg", &kt);
			//printf("Kt:%Lg\n",kt);
			
			//Kr
			fgets(line, 50, file);
			sscanf (line, "\tKr = %Lg", &kr);
			//printf("Kr:%Lg\n",kr);
			
			//Bump Mapping
			fgets(line, 50, file);
			sscanf (line, "\tBumpM = %s",  bumpM);
			//printf("BumpM:%s\n",bumpM);
			
			//Valor "montana"
			fgets(line,50,file);
			sscanf(line, "\tMount = %Lg", &mount);
			//printf("Mount:%Lg\n",mount);
			
			//Calado
			fgets(line, 50, file);
			sscanf (line, "\tCutOutM = %s", cutOutM);
			//printf("CutOutM:%s\n",cutOutM);
			
			//Texturas
			fgets(line, 50, file);
			sscanf (line, "\tTextures# = %i", &numTexture);
			//printf("textures#:%i\n",numTexture);
			
			int i;
			long double* texturesA=(long double*)calloc(10, sizeof(long double));	
			objTexture *textures = (objTexture*) calloc(5, sizeof(objTexture));
			
			
			
			for(i = 0; i<numTexture;i++)
			{
				fgets(line, 50, file);
				
				sscanf (line, "\t[%Lg,%Lg,%Lg,%Lg,%Lg,%Lg,%Lg,%Lg,%Lg]", &texturesA[0],&texturesA[1],&texturesA[2],&texturesA[3],&texturesA[4],
				&texturesA[5],&texturesA[6],&texturesA[7],&texturesA[8]);
				addObjTexture(&textures,texturesA[0],texturesA[1],texturesA[2],texturesA[3],texturesA[4],
				texturesA[5],texturesA[6],texturesA[7],texturesA[8]);
				//printf("textures:%Lg,%Lg,%Lg,%Lg,%Lg,%Lg,%Lg,%Lg,%Lg\n",texturesA[0],texturesA[1],texturesA[2],texturesA[3],texturesA[4],texturesA[5],texturesA[6],
				//texturesA[7],texturesA[8]);
			}
			
			//Planos de corte
			fgets(line, 50, file);
			sscanf (line, "\tPlanes# = %d", &numPlane);
			//printf("Planos#:%d\n",numPlane);
			fgets(line, 50, file);
			//Cambiar Variable!!
			sscanf(line,"\tPlanes = %s", planes);
						
			int* objPlanes=(int*)calloc(numPlane, sizeof(int));
			temp = strtok(planes, ",");
			
			
			for(i = 0; i<numPlane;i++)
			{
				sscanf (temp, "%d", &objPlanes[i]);
				//printf("Planos:%d\n",objPlanes[i]);
				temp = strtok(NULL, ",");
				
			}

	
 

				if(strcmp(id,"$Sphere\n") == 0){
				
				//Radio
				fgets(line, 50, file);
				sscanf (line, "\tRatio = %Lg", &ratio);
				//printf("Radio:%Lg\n",ratio);
				
				//Xyz
				fgets(line, 50, file);
				sscanf (line, "\tXyz = %Lg,%Lg,%Lg", &x,&y,&z);
				//printf("Xyz:%Lg,%Lg,%Lg\n",x,y,z);
				//sphere = newSphere(x,y,z, ratio, r,g,b, 0.9,1,5,0.5,0.6 ,1,0,0,0, 0, objPlanes);
				//textureNode *cutout, textureNode *bumpmap
				sphere = newSphere(x,y,z,ratio,r,g,b,kd,ks,kn,ka,kt,kr,o1,o2,o3,o4,numPlane,objPlanes,numTexture,textures,grabTexture(cutOutM),grabTexture(bumpM),mount);   
				add(&objectList, sphere);
	
				}
				else if (strcmp(id,"$Cylinder\n") == 0){
					
					//Radio
					fgets(line, 50, file);
					sscanf (line, "\tRatio = %Lg", &ratio);
					//printf("Radio:%Lg\n", ratio);
					
					//Limites
					fgets(line, 50, file);
					sscanf (line, "\tLimites = %Lg,%Lg", &lim1,&lim2);
					//printf("Limites:%Lg,%Lg\n", lim1,lim2);
					
					//Rayo x y z
					fgets(line, 50, file);
					sscanf (line, "\tRayo = %Lg,%Lg,%Lg", &xRayo,&yRayo,&zRayo);
					//printf("Rayos:%Lg,%Lg,%Lg\n", xRayo,yRayo,zRayo);
					
					//Coordenada x, y z
					fgets(line, 50, file);
					sscanf (line, "\tXyz = %Lg,%Lg,%Lg", &x,&y,&z);
					//printf("Xyz:%Lg,%Lg,%Lg\n",x,y,z);

					//cylinder = newCylinder(x,y,z, r,g,b, 0.9,1,4,0.5, 0,      0,0,0,1,  300,lim1,lim2, xRayo,yRayo,zRayo,  1,textures); 
					 
					cylinder = newCylinder(x, y, z, r, g, b, kd, ks, kn, ka, kt,kr, o1, o2, o3, o4, ratio, lim1, lim2, xRayo, yRayo, 
					zRayo, numTexture, textures, numPlane,objPlanes,grabTexture(cutOutM),grabTexture(bumpM), mount);
					add(&objectList, cylinder);
				}else if(strcmp(id,"$Cone\n")==0)
				{
					//Limites
					fgets(line, 50, file);
					sscanf (line, "\tLimites = %Lg,%Lg", &lim1,&lim2);
					//printf("Limites:%Lg,%Lg\n", lim1,lim2);
					
					//Rayo
					fgets(line, 50, file);
					sscanf (line, "\tRayo = %Lg,%Lg,%Lg", &xRayo,&yRayo,&zRayo);
					//printf("Rayos:%Lg,%Lg,%Lg\n", xRayo,yRayo,zRayo);
					
					//Proporcion
					fgets(line, 50, file);
					sscanf (line, "\tProp = %Lg,%Lg",&prop1,&prop2);
					//printf("Prop:%Lg,%Lg\n", prop1,prop2);
					
					//Coordenada x, y z
					fgets(line, 50, file);
					sscanf (line, "\tXyz = %Lg,%Lg,%Lg", &x,&y,&z);
					//printf("Xyz:%Lg,%Lg,%Lg\n",x,y,z);
					
					//Greenwich
					fgets(line, 50, file);
					sscanf (line, "\tGxyz = %Lg,%Lg,%Lg", &gX,&gY,&gZ);
					//printf("Gxyz:%Lg,%Lg,%Lg\n",x,y,z);
					

					cone = newCone(x, y, z, r, g, b, kd, ks, kn, ka,kt,kr,o1,o2,o3,o4,lim1, lim2, xRayo, yRayo, zRayo,
					gX, gY, gZ, prop1, prop2, numTexture, textures, numPlane,objPlanes,grabTexture(cutOutM),grabTexture(bumpM), mount);
					add(&objectList, cone);
				}else if(strcmp(id,"$Quadratic\n")==0){
					
					long double* quad=(long double*)calloc(10, sizeof(long double));
					
					//A
					fgets(line, 50, file);
					sscanf (line, "\tA = %Lg,", &quad[0]);
					//printf("A:%Lg\n", quad[0]);
					
					//B
					fgets(line, 50, file);
					sscanf (line, "\tB = %Lg,", &quad[1]);
					//printf("B:%Lg\n", quad[1]);
					
					//C
					fgets(line, 50, file);
					sscanf (line, "\tC = %Lg,", &quad[2]);
					//printf("C:%Lg\n", quad[2]);
					
					//D
					fgets(line, 50, file);
					sscanf (line, "\tD = %Lg,", &quad[3]);
				//	printf("D:%Lg\n", quad[3]);
					
					//E
					fgets(line, 50, file);
					sscanf (line, "\tE = %Lg,", &quad[4]);
					//printf("E:%Lg\n", quad[4]);
					
					//F
					fgets(line, 50, file);
					sscanf (line, "\tF = %Lg,", &quad[5]);
					//printf("F:%Lg\n", quad[5]);
					
					//G
					fgets(line, 50, file);
					sscanf (line, "\tG = %Lg,", &quad[6]);
					//printf("G:%Lg\n", quad[6]);
					
					//H
					fgets(line, 50, file);
					sscanf (line, "\tH = %Lg,", &quad[7]);
					//printf("H:%Lg\n", quad[7]);
					
					//J
					fgets(line, 50, file);
					sscanf (line, "\tJ = %Lg,", &quad[8]);
					//printf("J:%Lg\n", quad[8]);
					
					//K
					fgets(line, 50, file);
					sscanf (line, "\tK = %Lg,", &quad[9]);
					//printf("K:%Lg\n", quad[9]);
					
					quadratic  = newQuadratic(quad[0],quad[1],quad[2],quad[3],quad[4],quad[5],quad[6],quad[7],quad[8],quad[9],
					 r, g, b, kd, ks, kn, ka, kt,kr, o1, o2, o3, o4, (int)numPlane,objPlanes);
					 add(&objectList, quadratic);
					 
					}
				else if(strcmp(id,"$Polygon\n")==0)
				{
					fgets(line, 50, file);
					sscanf (line, "\tVertices# = %Lg", &numPoints);
					//printf("Vertices:%s",line);
					
					int i;
					int pointsPerLine =0;
					ptsNode* points = (ptsNode*) calloc(numPoints,sizeof(ptsNode));
					fgets(line, 100, file);
					temp = strtok(line, ",");
			
			
			for(i=0;i<numPoints;i++)
			{
				sscanf (temp, "[%Lg_%Lg_%Lg]", &xp,&yp,&zp);
				ptsNode newNode;
				//printf("Points:%Lg,%Lg,%Lg\n",xp,yp,zp);
				newNode.x = xp;
				newNode.y = yp;
				newNode.z = zp;
				points[i] = newNode;
				
				if(pointsPerLine==1){
					fgets(line, 100, file);
					pointsPerLine = 0;
					temp = strtok(line, ",");
				}
				
				else{
					pointsPerLine++;
					temp = strtok(NULL, ",");
				}
				
			}
			
			long double xT,yT,zT,xR,yR,zR,dx,dy,dz;
			
			//printf("Line:%s\n",line);	
			sscanf (line, "\tT = %Lg-%Lg-%Lg", &xT,&yT,&zT);
			//printf("Traslado:%Lg,%Lg,%Lg\n",xT,yT,zT);
					
			fgets(line, 50, file);
			//printf("Line:%s\n",line);	
			sscanf (line, "\tR = %Lg-%Lg-%Lg-%Lg-%Lg-%Lg", &xR,&yR,&zR,&dx,&dy,&dz);
			//printf("Rotacion:%Lg,%Lg,%Lg,%Lg,%Lg,%Lg\n",xR,yR,zR,dx,dy,dz);
			
			polygon = newPolygon(r, g, b, kd, ks, kn, ka, kt,kr, o1, o2, o3, o4, numPoints, points, numTexture, textures, numPlane,objPlanes,grabTexture(cutOutM),grabTexture(bumpM), mount);
			add(&objectList, polygon);
			translatePolygon(polygon->data, xT,yT,zT);
			rotatePolygon(polygon->data, xR,yR,zR,dx,dy,dz);
			}else if(strcmp(id,"$Disc\n")==0)
			{
				long double innerRatio, qX,qY,qZ;
				
				//Radio
				fgets(line, 50, file);
				sscanf (line, "\tRatio = %Lg", &ratio);
				//printf("Radio:%Lg\n", ratio);
				
				fgets(line, 50, file);
				sscanf (line, "\tInnerRatio = %Lg", &innerRatio);
				//printf("Inner Radio:%Lg\n", innerRatio);
				
				//Coordenada x, y z
				fgets(line, 50, file);
				sscanf (line, "\tXyz = %Lg,%Lg,%Lg", &x,&y,&z);
				//printf("Xyz:%Lg,%Lg,%Lg\n",x,y,z);
				
				//QxQyQz
				fgets(line, 50, file);
				sscanf (line, "\tQxyz = %Lg,%Lg,%Lg", &qX,&qY,&qZ);
				//printf("Qxyz:%Lg,%Lg,%Lg\n",qX,qY,qZ);
				
				disc = newDisc(x,y,z,r,g,b,kd,ks,kn,ka,kt,kr,o1,o2,o3,o4,ratio,innerRatio,qX,qY,qZ,numTexture,textures,numPlane,objPlanes);
				add(&objectList, disc);
				}
	}

			else if(strncmp(line,"#",1) == 0)
			{
				strcpy(id,line);
				if(strcmp(id,"#Cutplane\n") == 0){
					
					long double* planoC=(long double*)calloc(10, sizeof(long double));
					
					fgets(line, 50, file);
					sscanf (line, "%Lg,%Lg,%Lg", &planoC[0],&planoC[1],&planoC[2]);
					fgets(line, 50, file);
					sscanf (line, "%Lg,%Lg,%Lg",&planoC[3],&planoC[4],&planoC[5]);
					fgets(line, 50, file);
					sscanf (line, "%Lg,%Lg,%Lg", &planoC[6],&planoC[7],&planoC[8]);
										
					ptsNode* qPtsNode1 = (ptsNode*) calloc(3,sizeof(ptsNode));
					
					ptsNode newNode11;
					newNode11.x = planoC[0];
					newNode11.y = planoC[1];
					newNode11.z = planoC[2];
					qPtsNode1[0] = newNode11;

					ptsNode newNode12;
					newNode12.x = planoC[3];
					newNode12.y = planoC[4];
					newNode12.z = planoC[5];
					qPtsNode1[1] = newNode12;
			
					ptsNode newNode13;
					newNode13.x = planoC[6];
					newNode13.y = planoC[7];
					newNode13.z = planoC[8];
					qPtsNode1[2] = newNode13;
					
					//printf("Nodos:%Lg,%Lg,%Lg,%Lg,%Lg,%Lg,%Lg,%Lg,%Lg\n",planoC[0],planoC[1],planoC[2],planoC[3],planoC[4],planoC[5],planoC[6],planoC[7],planoC[8]);
					
					addCutPlane(&cutPlaneList, qPtsNode1);

			}else if(strcmp(id,"#Texture\n") == 0)
			{
				fgets(line, 50, file);
				sscanf (line, "%s", temp);
				//printf("Adding texture: %s\n", temp);
				addToTexture(temp);
				}
			}
}
}

