/* COSC 3P98 Final Project
 * Space Animation
 * This animation is a short animation in which a spaceship leaves a planet, and is
 * confronted by an alien spaceship. Soon after the first spaceship is destroyed by
 * the alien.
 * Michael Young - 4245718
 * May 2011 
 */

#if !defined(Linux)
  #include <windows.h>           //Not Linux must be windows
#else
  #include <stdlib.h>            //use for exit(0) function
#endif

#include <stdio.h>
#include <glut.h>
#include <math.h>
#include <FreeImage.h>

#define X 0
#define Y 1
#define Z 2
#define TRUE 1
#define FALSE 0
#define PARTICLE_NUM 500
#define STAR_NUM 300

double cameraPos[3];
int paused;
/* State variables so different scenes can be drawn*/
int scene1=1;//ship leaves the planet
int scene2=2;//ship stops and alien ship comes into view
int scene3=3;//alien ship moves around and shoots the ship
int scene4=4;//finish
int currentScene;
int stepCounter;//counts the number of steps that have occured
/**********************/

//Camera Focus
typedef struct
{
	double x;
	double y;
	double z;
} position;
position cameraFocus;

//Pixel structure
typedef struct
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} pixel;

//the colour structure
typedef struct {
	int r;
	int g;
	int b;
} colour_type;

//the global structure
typedef struct {
	pixel *data;
	int w, h;
} bigShip_texture;
bigShip_texture bigShipTex;

/**************************************************************************/
/*********************************Models***********************************/
/**************************************************************************/
//particle structure
typedef struct
{
	double dir[3];
	double pos[3];
	colour_type colour;
} particle;

//Bone structure
typedef struct
{
	particle pieces[PARTICLE_NUM];
} bone;
bone explosion;

//Sphere structure
typedef struct
{
	float vertices[229][3];//vertices of the sphere
	float uv[229][2];//texture coordinates
	float normals[182][3];//normals of all the faceIndex that make up the sphere
	int normalIndex[360][3];//holds the indices for the normals
	int faceIndex[360][3];//the faceIndex that makes up the sphere. Array holds indice values for the coordinates.
} sphere;
sphere planet;

//Alien ship body structure
typedef struct
{
	float vertices[89][3];//vertices of the sphere
	float normals[62][3];//normals of all the faceIndex that make up the sphere
	int normalIndex[120][3];//holds the indices for the normals
	int faceIndex[120][3];//the faceIndex that makes up the sphere. Array holds indice values for the coordinates.
} alienShip_body;

//Alien ship cockpit structure
typedef struct
{
	float vertices[89][3];//vertices of the sphere
	float normals[62][3];//normals of all the faceIndex that make up the sphere
	int normalIndex[120][3];//holds the indices for the normals
	int faceIndex[120][3];//the faceIndex that makes up the sphere. Array holds indice values for the coordinates.
} alienShip_cockpit;

//Alien ship structure
typedef struct
{
	alienShip_cockpit cockpit;
	alienShip_body body;
} alien_ship;
alien_ship alienShip;

//Ship cockpit structure
typedef struct
{
	float vertices[89][3];//vertices of the sphere
	float normals[89][3];//normals of all the faceIndex that make up the sphere
	int normalIndex[120][3];//holds the indices for the normals
	int faceIndex[120][3];//the faceIndex that makes up the sphere. Array holds indice values for the coordinates.
} ship_cockpit;

//Ship body structure
typedef struct
{
	float vertices[229][3];//vertices of the sphere
	float uv[229][2];//texture coordinates
	float normals[229][3];//normals of all the faceIndex that make up the sphere
	int normalIndex[360][3];//holds the indices for the normals
	int faceIndex[360][3];//the faceIndex that makes up the sphere. Array holds indice values for the coordinates.
} ship_body;

//Small ship engine structure
typedef struct
{
	float vertices[109][3];//vertices of the sphere
	float normals[107][3];//normals of all the faceIndex that make up the sphere
	int normalIndex[132][3];//holds the indices for the normals
	int faceIndex[132][3];//the faceIndex that makes up the sphere. Array holds indice values for the coordinates.
} ship_engine_small;

//large ship engine structure
typedef struct
{
	/*Engine*/
	float vertices[109][3];//vertices of the sphere
	float normals[107][3];//normals of all the faceIndex that make up the sphere
	int normalIndex[132][3];//holds the indices for the normals
	int faceIndex[132][3];//the faceIndex that makes up the sphere. Array holds indice values for the coordinates.
} ship_engine_big;

//Ship structure
typedef struct
{
	ship_body body;
	ship_engine_small left_engine;
	ship_engine_small right_engine;
	ship_engine_big big_engine;
	ship_cockpit cockpit;
} ship;
ship bigShip;

//Stars structure
typedef struct
{
	particle Stars[STAR_NUM];
} stars;
stars theStars;
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/

/**************************************************************************/
/****************************Initialize************************************/
/**************************************************************************/

/*Initializes the stars*/
void starsInit()
{
	int i,j,typeCheck;
	double randDirX,randDirY,randDirZ;
	
	for(i=0;i<STAR_NUM;i++)
	{
		randDirX = (double)(rand()/(double)RAND_MAX);
		randDirY = (double)(rand()/(double)RAND_MAX);
		randDirZ = (double)(rand()/(double)RAND_MAX);

		typeCheck = (int)rand()%2;
		if(typeCheck==0)//negative direction
		{
			randDirX = -randDirX;
		}
		//else positive direction

		typeCheck = (int)rand()%2;
		if(typeCheck==0)//negative direction
		{
			randDirY = -randDirY;
		}
		//else positive direction

		typeCheck = (int)rand()%2;
		if(typeCheck==0)//negative direction
		{
			randDirZ = -randDirZ;
		}
		//else positive direction

		theStars.Stars[i].dir[X] = randDirX;
		theStars.Stars[i].dir[Y] = randDirY;
		theStars.Stars[i].dir[Z] = randDirZ;

		theStars.Stars[i].pos[X] = 0.0;
		theStars.Stars[i].pos[Y] = 0.0;
		theStars.Stars[i].pos[Z] = 0.0;
	}
	
	for(j=0;j<1000;j++)
	{
		for(i=0;i<STAR_NUM;i++)
		{
			theStars.Stars[i].pos[X] = theStars.Stars[i].pos[X] + theStars.Stars[i].dir[X];
			theStars.Stars[i].pos[Y] = theStars.Stars[i].pos[Y] + theStars.Stars[i].dir[Y];
			theStars.Stars[i].pos[Z] = theStars.Stars[i].pos[Z] + theStars.Stars[i].dir[Z];
		}
	}
}//starsInit

/*Reads in the texture picture for the big ship.
 *Default texture is bigShip_texture.tiff*/
pixel *readBigShipTexture(char *name, int *width, int *height) 
{
	FIBITMAP *image;
	int i,j,pnum;
	RGBQUAD aPixel;
	pixel *data;

	if((image = FreeImage_Load(FIF_PNG, name, 0)) == NULL) {
		return NULL;
	}      
	*width = FreeImage_GetWidth(image);
	*height = FreeImage_GetHeight(image);

	data = (pixel *)malloc((*height)*(*width)*sizeof(pixel *));
	pnum=0;
	for(i = 0 ; i < (*height) ; i++) {
		for(j = 0 ; j < (*width) ; j++) {
			FreeImage_GetPixelColor(image, j, i, &aPixel);
			data[pnum].r = (aPixel.rgbRed);
			data[pnum].g = (aPixel.rgbGreen);
			data[pnum++].b = (aPixel.rgbBlue);
		}
	}
	FreeImage_Unload(image);
	return data;
}//readBigShipTexture

//redraws the image
void redraw(void) {
   glClear(GL_COLOR_BUFFER_BIT);
}//redraw

/*Initialize the lighting.*/
void lightInit() {
   GLfloat ambient[] = {0.8, 0.8, 0.8, 1.0};
   GLfloat diffuse[] = {0.8, 0.8, 0.8, 1.0};
   GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
   GLfloat position[] = {200.0, 250.0, 100.0, 0.0};
   //GLfloat position[] = {-66.0, 29.0, -63.0, 0.0};
   GLfloat lmodel_ambient[] = {1.0, 1.0, 0.0, 1.0};
   GLfloat local_view[] = {0.0};

   glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
   glLightfv(GL_LIGHT0, GL_POSITION, position);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
   glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

   glEnable(GL_LIGHTING);   /* turns on lighting */
   glEnable(GL_LIGHT0);     /* turns on light 0  */
   glEnable(GL_NORMALIZE);
}//lightInit

/*initializes the texture*/
void textureInit()
{
	/*Texture**********************************/
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);	// Use texture number 0

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, 203, 203, 0, GL_RGBA, GL_UNSIGNED_BYTE,bigShipTex.data);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 4, 203, 203, GL_RGBA, GL_UNSIGNED_BYTE, bigShipTex.data);

	glEnable(GL_TEXTURE_2D);

	free(bigShipTex.data);
	/*******************************************/

}//textureInit

/*Initializes the explosion animation.*/
void explosionInit()
{
	int i,typeCheck,colourIndex,rIndex,gIndex,bIndex;
	double randDirX,randDirY,randDirZ;
	int randomColours[][3]={{255,255,51},{255,255,0},{255,204,51},{255,204,0},{255,152,0},{255,0,0},{255,102,0}};//array of different colours that a particle can have
	rIndex = 0;
	gIndex = 1;
	bIndex = 2;
	
	for(i=0;i<PARTICLE_NUM;i++)
	{
		randDirX = (double)(rand()/(double)RAND_MAX);
		randDirY = (double)(rand()/(double)RAND_MAX);
		randDirZ = (double)(rand()/(double)RAND_MAX);
		colourIndex = rand()%7;//0-6
		explosion.pieces[i].colour.r = randomColours[colourIndex][rIndex];
		explosion.pieces[i].colour.g = randomColours[colourIndex][gIndex];
		explosion.pieces[i].colour.b = randomColours[colourIndex][bIndex];
		typeCheck = (int)rand()%2;
		if(typeCheck==0)//negative direction
		{
			randDirX = -randDirX;
		}
		//else positive direction

		typeCheck = (int)rand()%2;
		if(typeCheck==0)//negative direction
		{
			randDirY = -randDirY;
		}
		//else positive direction

		typeCheck = (int)rand()%2;
		if(typeCheck==0)//negative direction
		{
			randDirZ = -randDirZ;
		}
		//else positive direction

		explosion.pieces[i].dir[X] = randDirX;
		explosion.pieces[i].dir[Y] = randDirY;
		explosion.pieces[i].dir[Z] = randDirZ;

		explosion.pieces[i].pos[X] = 100.0;
		explosion.pieces[i].pos[Y] = 130.0;
		explosion.pieces[i].pos[Z] = 40.0;
	}
}//explosionInit

/*Initializes the alien ship*/
void alienShipInit()
{
	FILE *dataIn;
	char str[15];
	char name[20];
	int i;
	int verticeCount;
	int verticeNormCount;
	int fps;
	int frameCount;
	int meshCount;
	int triangleCount;
	
	dataIn = fopen("alien_ship.txt","r");

	if(dataIn == NULL)
	{
		printf("Error opening file.");
		exit(1);
	}
	else
	{	
		/*get data*/

		/*FPS********************/
		fscanf(dataIn,"%s",str);
		fscanf(dataIn,"%u",&fps);
		printf("%s %u\n",str,fps);
		/************************/

		/*Frame Count************/
		fscanf(dataIn,"%s",str);
		fscanf(dataIn,"%u",&frameCount);
		printf("%s %u\n",str,frameCount);
		/************************/

		/*Mesh Count************/
		fscanf(dataIn,"%s",str);
		fscanf(dataIn,"%u",&meshCount);
		printf("%s %u\n",str,frameCount);
		/************************/

		/*Alien Ship Cockpit************/
		/****************************/
		fscanf(dataIn,"%s",name);
		fscanf(dataIn,"%d");//flag
		fscanf(dataIn,"%d");//flag
		printf("%s \n",name);
		/************************/
		
		/**Vertice Count***************
		 **Flag, X, Y, Z, S, T, BoneID*/
		fscanf(dataIn,"%u",&verticeCount);
		printf("Vertice Count: %u\n",verticeCount);
		/*******************************/
		
		/*Get each vertice of sphere***/
		for(i=0;i<verticeCount;i++)
		{
			fscanf(dataIn,"%d");//flag
			fscanf(dataIn,"%f",&alienShip.cockpit.vertices[i][X]);
			fscanf(dataIn,"%f",&alienShip.cockpit.vertices[i][Y]);
			fscanf(dataIn,"%f",&alienShip.cockpit.vertices[i][Z]);
			fscanf(dataIn,"%f");//s
			fscanf(dataIn,"%f");//t
			fscanf(dataIn,"%d");//BoneID
		}
		/******************************/

		/*Get the number of vertice normals****/
		fscanf(dataIn,"%u",&verticeNormCount);
		printf("Vertice norm count %u\n",verticeNormCount);
		/*******************************/

		/*Get normals of each vertice and assign to test sphere normals*/
		for(i=0;i<verticeNormCount;i++)
		{
			fscanf(dataIn,"%f",&alienShip.cockpit.normals[i][X]);
			fscanf(dataIn,"%f",&alienShip.cockpit.normals[i][Y]);
			fscanf(dataIn,"%f",&alienShip.cockpit.normals[i][Z]);
		}
		/******************************/

		/*Get the number of faceIndex that make up the sphere****/
		fscanf(dataIn,"%u",&triangleCount);
		printf("Triangle count %u\n",triangleCount);
		/*******************************/

		/*Get indices of the triangle faces*
		 ** Flags, vertex_index1, v_index2, v_index3, normal_index1, n_index2, n_index3, smoothing group.*/
		for(i=0;i<triangleCount;i++)
		{
			fscanf(dataIn,"%d");//flag
			fscanf(dataIn,"%u",&alienShip.cockpit.faceIndex[i][X]);//v_index1
			fscanf(dataIn,"%u",&alienShip.cockpit.faceIndex[i][Y]);//v_index2
			fscanf(dataIn,"%u",&alienShip.cockpit.faceIndex[i][Z]);//v_index3
			fscanf(dataIn,"%u",&alienShip.cockpit.normalIndex[i][X]);//n_index1
			fscanf(dataIn,"%u",&alienShip.cockpit.normalIndex[i][Y]);//n_index2
			fscanf(dataIn,"%u",&alienShip.cockpit.normalIndex[i][Z]);//n_index3
			fscanf(dataIn,"%d");//smoothing group
		}
		/******************************/

		/*Alien Ship Body************/
		/****************************/
		fscanf(dataIn,"%s",name);
		fscanf(dataIn,"%d");//flag
		fscanf(dataIn,"%d");//flag
		printf("%s \n",name);
		/************************/
		
		/**Vertice Count***************
		 **Flag, X, Y, Z, S, T, BoneID*/
		fscanf(dataIn,"%u",&verticeCount);
		printf("Vertice Count: %u\n",verticeCount);
		/*******************************/
		
		/*Get each vertice of sphere***/
		for(i=0;i<verticeCount;i++)
		{
			fscanf(dataIn,"%d");//flag
			fscanf(dataIn,"%f",&alienShip.body.vertices[i][X]);
			fscanf(dataIn,"%f",&alienShip.body.vertices[i][Y]);
			fscanf(dataIn,"%f",&alienShip.body.vertices[i][Z]);
			fscanf(dataIn,"%f");//s
			fscanf(dataIn,"%f");//t
			fscanf(dataIn,"%d");//BoneID
		}
		/******************************/

		/*Get the number of vertice normals****/
		fscanf(dataIn,"%u",&verticeNormCount);
		printf("Vertice norm count %u\n",verticeNormCount);
		/*******************************/

		/*Get normals of each vertice and assign to test sphere normals*/
		for(i=0;i<verticeNormCount;i++)
		{
			fscanf(dataIn,"%f",&alienShip.body.normals[i][X]);
			fscanf(dataIn,"%f",&alienShip.body.normals[i][Y]);
			fscanf(dataIn,"%f",&alienShip.body.normals[i][Z]);
		}
		/******************************/

		/*Get the number of faceIndex that make up the sphere****/
		fscanf(dataIn,"%u",&triangleCount);
		printf("Triangle count %u\n",triangleCount);
		/*******************************/

		/*Get indices of the triangle faces*
		 ** Flags, vertex_index1, v_index2, v_index3, normal_index1, n_index2, n_index3, smoothing group.*/
		for(i=0;i<triangleCount;i++)
		{
			fscanf(dataIn,"%d");//flag
			fscanf(dataIn,"%u",&alienShip.body.faceIndex[i][X]);//v_index1
			fscanf(dataIn,"%u",&alienShip.body.faceIndex[i][Y]);//v_index2
			fscanf(dataIn,"%u",&alienShip.body.faceIndex[i][Z]);//v_index3
			fscanf(dataIn,"%u",&alienShip.body.normalIndex[i][X]);//n_index1
			fscanf(dataIn,"%u",&alienShip.body.normalIndex[i][Y]);//n_index2
			fscanf(dataIn,"%u",&alienShip.body.normalIndex[i][Z]);//n_index3
			fscanf(dataIn,"%d");//smoothing group
		}
		/******************************/

		fclose (dataIn);
	}
}//alienShipInit

/*Initializes the sphere structure that represents the planet 
 *so it can be drawn. A lot of the information in the MilkShape 
 *3D ASCII output is unnecassary for my purposes. In order to 
 *traverse through the file, the numbers have to be read, but they 
 *aren't stored anywhere.*/
void planetInit()
{
	FILE *dataIn;
	char str[15];
	char name[20];
	int i;
	int verticeCount;
	int verticeNormCount;
	int fps;
	int frameCount;
	int meshCount;
	int triangleCount;
	
	dataIn = fopen("planet.txt","r");

	if(dataIn == NULL)
	{
		printf("Error opening file.");
		exit(1);
	}
	else
	{	
		/*get data*/

		/*FPS********************/
		fscanf(dataIn,"%s",str);
		fscanf(dataIn,"%u",&fps);
		printf("%s %u\n",str,fps);
		/************************/

		/*Frame Count************/
		fscanf(dataIn,"%s",str);
		fscanf(dataIn,"%u",&frameCount);
		printf("%s %u\n",str,frameCount);
		/************************/

		/*Mesh Count************/
		fscanf(dataIn,"%s",str);
		fscanf(dataIn,"%u",&meshCount);
		printf("%s %u\n",str,frameCount);
		/************************/

		/*Name************/
		fscanf(dataIn,"%s",name);
		fscanf(dataIn,"%d");//flag
		fscanf(dataIn,"%d");//flag
		printf("%s \n",name);
		/************************/
		
		/**Vertice Count***************
		 **Flag, X, Y, Z, S, T, BoneID*/
		fscanf(dataIn,"%u",&verticeCount);
		printf("Vertice Count: %u\n",verticeCount);
		/*******************************/
		
		/*Get each vertice of sphere***/
		for(i=0;i<verticeCount;i++)
		{
			fscanf(dataIn,"%d");//flag
			fscanf(dataIn,"%f",&planet.vertices[i][X]);
			fscanf(dataIn,"%f",&planet.vertices[i][Y]);
			fscanf(dataIn,"%f",&planet.vertices[i][Z]);
			fscanf(dataIn,"%f",&planet.uv[i][X]);
			fscanf(dataIn,"%f",&planet.uv[i][Y]);
			fscanf(dataIn,"%d");//BoneID
		}
		/******************************/

		/*Get the number of vertice normals****/
		fscanf(dataIn,"%u",&verticeNormCount);
		printf("Vertice norm count %u\n",verticeNormCount);
		/*******************************/

		/*Get normals of each vertice and assign to test sphere normals*/
		for(i=0;i<verticeNormCount;i++)
		{
			fscanf(dataIn,"%f",&planet.normals[i][X]);
			fscanf(dataIn,"%f",&planet.normals[i][Y]);
			fscanf(dataIn,"%f",&planet.normals[i][Z]);
		}
		/******************************/

		/*Get the number of faceIndex that make up the sphere****/
		fscanf(dataIn,"%u",&triangleCount);
		printf("Triangle count %u\n",triangleCount);
		/*******************************/

		/*Get indices of the triangle faces*
		 ** Flags, vertex_index1, v_index2, v_index3, normal_index1, n_index2, n_index3, smoothing group.*/
		for(i=0;i<triangleCount;i++)
		{
			fscanf(dataIn,"%d");//flag
			fscanf(dataIn,"%u",&planet.faceIndex[i][X]);//v_index1
			fscanf(dataIn,"%u",&planet.faceIndex[i][Y]);//v_index2
			fscanf(dataIn,"%u",&planet.faceIndex[i][Z]);//v_index3
			fscanf(dataIn,"%u",&planet.normalIndex[i][X]);//n_index1
			fscanf(dataIn,"%u",&planet.normalIndex[i][Y]);//n_index2
			fscanf(dataIn,"%u",&planet.normalIndex[i][Z]);//n_index3
			fscanf(dataIn,"%d");//smoothing group
		}
		/******************************/

		fclose (dataIn);
	}
}//planetInit

/*Initializes the ship structure, the same way the sphere for the
 *planet was initialized, with a different source file. */
void bigShipInit()
{
	FILE *dataIn;
	char str[15];
	char name[20];
	int i;
	int verticeCount;
	int verticeNormCount;
	int fps;
	int frameCount;
	int meshCount;
	int triangleCount;
	
	dataIn = fopen("big_ship.txt","r");

	if(dataIn == NULL)
	{
		printf("Error opening file.");
		exit(1);
	}
	else
	{	
		/*get data*/

		/*FPS********************/
		fscanf(dataIn,"%s",str);
		fscanf(dataIn,"%u",&fps);
		printf("%s %u\n",str,fps);
		/************************/

		/*Frame Count************/
		fscanf(dataIn,"%s",str);
		fscanf(dataIn,"%u",&frameCount);
		printf("%s %u\n",str,frameCount);
		/************************/

		/*Mesh Count************/
		fscanf(dataIn,"%s",str);
		fscanf(dataIn,"%u",&meshCount);
		printf("%s %u\n",str,frameCount);
		/************************/

		/*Name************/
		fscanf(dataIn,"%s",name);
		fscanf(dataIn,"%d");//flag
		fscanf(dataIn,"%d");//flag
		printf("%s \n",name);
		/************************/
		
		/**Vertice Count***************
		 **Flag, X, Y, Z, S, T, BoneID*/
		fscanf(dataIn,"%u",&verticeCount);
		printf("Vertice Count: %u\n",verticeCount);
		/*******************************/
		
		/*Get each vertice of sphere***/
		for(i=0;i<verticeCount;i++)
		{
			fscanf(dataIn,"%d");//flag
			fscanf(dataIn,"%f",&bigShip.body.vertices[i][X]);
			fscanf(dataIn,"%f",&bigShip.body.vertices[i][Y]);
			fscanf(dataIn,"%f",&bigShip.body.vertices[i][Z]);
			fscanf(dataIn,"%f",&bigShip.body.uv[i][X]);//s
			fscanf(dataIn,"%f",&bigShip.body.uv[i][Y]);//t
			fscanf(dataIn,"%d");//BoneID
		}
		/******************************/

		/*Get the number of vertice normals****/
		fscanf(dataIn,"%u",&verticeNormCount);
		printf("Vertice norm count %u\n",verticeNormCount);
		/*******************************/

		/*Get normals of each vertice and assign to test sphere normals*/
		for(i=0;i<verticeNormCount;i++)
		{
			fscanf(dataIn,"%f",&bigShip.body.normals[i][X]);
			fscanf(dataIn,"%f",&bigShip.body.normals[i][Y]);
			fscanf(dataIn,"%f",&bigShip.body.normals[i][Z]);
		}
		/******************************/

		/*Get the number of faceIndex that make up the sphere****/
		fscanf(dataIn,"%u",&triangleCount);
		printf("Triangle count %u\n",triangleCount);
		/*******************************/

		/*Get indices of the triangle faces*
		 ** Flags, vertex_index1, v_index2, v_index3, normal_index1, n_index2, n_index3, smoothing group.*/
		for(i=0;i<triangleCount;i++)
		{
			fscanf(dataIn,"%d");//flag
			fscanf(dataIn,"%u",&bigShip.body.faceIndex[i][X]);//v_index1
			fscanf(dataIn,"%u",&bigShip.body.faceIndex[i][Y]);//v_index2
			fscanf(dataIn,"%u",&bigShip.body.faceIndex[i][Z]);//v_index3
			fscanf(dataIn,"%u",&bigShip.body.normalIndex[i][X]);//n_index1
			fscanf(dataIn,"%u",&bigShip.body.normalIndex[i][Y]);//n_index2
			fscanf(dataIn,"%u",&bigShip.body.normalIndex[i][Z]);//n_index3
			fscanf(dataIn,"%d");//smoothing group
		}
		/******************************/

		/*ship_cockpit************/
		fscanf(dataIn,"%s",name);
		fscanf(dataIn,"%d");//flag
		fscanf(dataIn,"%d");//flag
		printf("%s \n",name);
		/************************/

		/**Vertice Count***************
		 **Flag, X, Y, Z, S, T, BoneID*/
		fscanf(dataIn,"%u",&verticeCount);
		printf("Vertice Count: %u\n",verticeCount);
		/*******************************/
		
		/*Get each vertice of sphere***/
		for(i=0;i<verticeCount;i++)
		{
			fscanf(dataIn,"%d");//flag
			fscanf(dataIn,"%f",&bigShip.cockpit.vertices[i][X]);
			fscanf(dataIn,"%f",&bigShip.cockpit.vertices[i][Y]);
			fscanf(dataIn,"%f",&bigShip.cockpit.vertices[i][Z]);
			fscanf(dataIn,"%f");//s
			fscanf(dataIn,"%f");//t
			fscanf(dataIn,"%d");//BoneID
		}
		/******************************/

		/*Get the number of vertice normals****/
		fscanf(dataIn,"%u",&verticeNormCount);
		printf("Vertice norm count %u\n",verticeNormCount);
		/*******************************/

		/*Get normals of each vertice and assign to test sphere normals*/
		for(i=0;i<verticeNormCount;i++)
		{
			fscanf(dataIn,"%f",&bigShip.cockpit.normals[i][X]);
			fscanf(dataIn,"%f",&bigShip.cockpit.normals[i][Y]);
			fscanf(dataIn,"%f",&bigShip.cockpit.normals[i][Z]);
		}
		/******************************/

		/*Get the number of faceIndex that make up the sphere****/
		fscanf(dataIn,"%u",&triangleCount);
		printf("Triangle count %u\n",triangleCount);
		/*******************************/

		/*Get indices of the triangle faces*
		 ** Flags, vertex_index1, v_index2, v_index3, normal_index1, n_index2, n_index3, smoothing group.*/
		for(i=0;i<triangleCount;i++)
		{
			fscanf(dataIn,"%d");//flag
			fscanf(dataIn,"%u",&bigShip.cockpit.faceIndex[i][X]);//v_index1
			fscanf(dataIn,"%u",&bigShip.cockpit.faceIndex[i][Y]);//v_index2
			fscanf(dataIn,"%u",&bigShip.cockpit.faceIndex[i][Z]);//v_index3
			fscanf(dataIn,"%u",&bigShip.cockpit.normalIndex[i][X]);//n_index1
			fscanf(dataIn,"%u",&bigShip.cockpit.normalIndex[i][Y]);//n_index2
			fscanf(dataIn,"%u",&bigShip.cockpit.normalIndex[i][Z]);//n_index3
			fscanf(dataIn,"%d");//smoothing group
		}
		/******************************/

		/*ship_engine_big************/
		/******************************/
		fscanf(dataIn,"%s",name);
		fscanf(dataIn,"%d");//flag
		fscanf(dataIn,"%d");//flag
		printf("%s \n",name);
		/************************/

		/**Vertice Count***************
		 **Flag, X, Y, Z, S, T, BoneID*/
		fscanf(dataIn,"%u",&verticeCount);
		printf("Vertice Count: %u\n",verticeCount);
		/*******************************/
		
		/*Get each vertice of sphere***/
		for(i=0;i<verticeCount;i++)
		{
			fscanf(dataIn,"%d");//flag
			fscanf(dataIn,"%f",&bigShip.big_engine.vertices[i][X]);
			fscanf(dataIn,"%f",&bigShip.big_engine.vertices[i][Y]);
			fscanf(dataIn,"%f",&bigShip.big_engine.vertices[i][Z]);
			fscanf(dataIn,"%f");//s
			fscanf(dataIn,"%f");//t
			fscanf(dataIn,"%d");//BoneID
		}
		/******************************/

		/*Get the number of vertice normals****/
		fscanf(dataIn,"%u",&verticeNormCount);
		printf("Vertice norm count %u\n",verticeNormCount);
		/*******************************/

		/*Get normals of each vertice and assign to test sphere normals*/
		for(i=0;i<verticeNormCount;i++)
		{
			fscanf(dataIn,"%f",&bigShip.big_engine.normals[i][X]);
			fscanf(dataIn,"%f",&bigShip.big_engine.normals[i][Y]);
			fscanf(dataIn,"%f",&bigShip.big_engine.normals[i][Z]);
		}
		/******************************/

		/*Get the number of faceIndex that make up the sphere****/
		fscanf(dataIn,"%u",&triangleCount);
		printf("Triangle count %u\n",triangleCount);
		/*******************************/

		/*Get indices of the triangle faces*
		 ** Flags, vertex_index1, v_index2, v_index3, normal_index1, n_index2, n_index3, smoothing group.*/
		for(i=0;i<triangleCount;i++)
		{
			fscanf(dataIn,"%d");//flag
			fscanf(dataIn,"%u",&bigShip.big_engine.faceIndex[i][X]);//v_index1
			fscanf(dataIn,"%u",&bigShip.big_engine.faceIndex[i][Y]);//v_index2
			fscanf(dataIn,"%u",&bigShip.big_engine.faceIndex[i][Z]);//v_index3
			fscanf(dataIn,"%u",&bigShip.big_engine.normalIndex[i][X]);//n_index1
			fscanf(dataIn,"%u",&bigShip.big_engine.normalIndex[i][Y]);//n_index2
			fscanf(dataIn,"%u",&bigShip.big_engine.normalIndex[i][Z]);//n_index3
			fscanf(dataIn,"%d");//smoothing group
		}
		/******************************/

		/*ship_engine_right************/
		/******************************/
		fscanf(dataIn,"%s",name);
		fscanf(dataIn,"%d");//flag
		fscanf(dataIn,"%d");//flag
		printf("%s \n",name);
		/************************/

		/**Vertice Count***************
		 **Flag, X, Y, Z, S, T, BoneID*/
		fscanf(dataIn,"%u",&verticeCount);
		printf("Vertice Count: %u\n",verticeCount);
		/*******************************/
		
		/*Get each vertice of sphere***/
		for(i=0;i<verticeCount;i++)
		{
			fscanf(dataIn,"%d");//flag
			fscanf(dataIn,"%f",&bigShip.right_engine.vertices[i][X]);
			fscanf(dataIn,"%f",&bigShip.right_engine.vertices[i][Y]);
			fscanf(dataIn,"%f",&bigShip.right_engine.vertices[i][Z]);
			fscanf(dataIn,"%f");//s
			fscanf(dataIn,"%f");//t
			fscanf(dataIn,"%d");//BoneID
		}
		/******************************/

		/*Get the number of vertice normals****/
		fscanf(dataIn,"%u",&verticeNormCount);
		printf("Vertice norm count %u\n",verticeNormCount);
		/*******************************/

		/*Get normals of each vertice and assign to test sphere normals*/
		for(i=0;i<verticeNormCount;i++)
		{
			fscanf(dataIn,"%f",&bigShip.right_engine.normals[i][X]);
			fscanf(dataIn,"%f",&bigShip.right_engine.normals[i][Y]);
			fscanf(dataIn,"%f",&bigShip.right_engine.normals[i][Z]);
		}
		/******************************/

		/*Get the number of faceIndex that make up the sphere****/
		fscanf(dataIn,"%u",&triangleCount);
		printf("Triangle count %u\n",triangleCount);
		/*******************************/

		/*Get indices of the triangle faces*
		 ** Flags, vertex_index1, v_index2, v_index3, normal_index1, n_index2, n_index3, smoothing group.*/
		for(i=0;i<triangleCount;i++)
		{
			fscanf(dataIn,"%d");//flag
			fscanf(dataIn,"%u",&bigShip.right_engine.faceIndex[i][X]);//v_index1
			fscanf(dataIn,"%u",&bigShip.right_engine.faceIndex[i][Y]);//v_index2
			fscanf(dataIn,"%u",&bigShip.right_engine.faceIndex[i][Z]);//v_index3
			fscanf(dataIn,"%u",&bigShip.right_engine.normalIndex[i][X]);//n_index1
			fscanf(dataIn,"%u",&bigShip.right_engine.normalIndex[i][Y]);//n_index2
			fscanf(dataIn,"%u",&bigShip.right_engine.normalIndex[i][Z]);//n_index3
			fscanf(dataIn,"%d");//smoothing group
		}
		/******************************/

		/*ship_engine_left************/
		/******************************/
		fscanf(dataIn,"%s",name);
		fscanf(dataIn,"%d");//flag
		fscanf(dataIn,"%d");//flag
		printf("%s \n",name);
		/************************/

		/**Vertice Count***************
		 **Flag, X, Y, Z, S, T, BoneID*/
		fscanf(dataIn,"%u",&verticeCount);
		printf("Vertice Count: %u\n",verticeCount);
		/*******************************/
		
		/*Get each vertice of sphere***/
		for(i=0;i<verticeCount;i++)
		{
			fscanf(dataIn,"%d");//flag
			fscanf(dataIn,"%f",&bigShip.left_engine.vertices[i][X]);
			fscanf(dataIn,"%f",&bigShip.left_engine.vertices[i][Y]);
			fscanf(dataIn,"%f",&bigShip.left_engine.vertices[i][Z]);
			fscanf(dataIn,"%f");//s
			fscanf(dataIn,"%f");//t
			fscanf(dataIn,"%d");//BoneID
		}
		/******************************/

		/*Get the number of vertice normals****/
		fscanf(dataIn,"%u",&verticeNormCount);
		printf("Vertice norm count %u\n",verticeNormCount);
		/*******************************/

		/*Get normals of each vertice and assign to test sphere normals*/
		for(i=0;i<verticeNormCount;i++)
		{
			fscanf(dataIn,"%f",&bigShip.left_engine.normals[i][X]);
			fscanf(dataIn,"%f",&bigShip.left_engine.normals[i][Y]);
			fscanf(dataIn,"%f",&bigShip.left_engine.normals[i][Z]);
		}
		/******************************/

		/*Get the number of faceIndex that make up the sphere****/
		fscanf(dataIn,"%u",&triangleCount);
		printf("Triangle count %u\n",triangleCount);
		/*******************************/

		/*Get indices of the triangle faces*
		 ** Flags, vertex_index1, v_index2, v_index3, normal_index1, n_index2, n_index3, smoothing group.*/
		for(i=0;i<triangleCount;i++)
		{
			fscanf(dataIn,"%d");//flag
			fscanf(dataIn,"%u",&bigShip.left_engine.faceIndex[i][X]);//v_index1
			fscanf(dataIn,"%u",&bigShip.left_engine.faceIndex[i][Y]);//v_index2
			fscanf(dataIn,"%u",&bigShip.left_engine.faceIndex[i][Z]);//v_index3
			fscanf(dataIn,"%u",&bigShip.left_engine.normalIndex[i][X]);//n_index1
			fscanf(dataIn,"%u",&bigShip.left_engine.normalIndex[i][Y]);//n_index2
			fscanf(dataIn,"%u",&bigShip.left_engine.normalIndex[i][Z]);//n_index3
			fscanf(dataIn,"%d");//smoothing group
		}
		/******************************/

		fclose (dataIn);
	}
}//bigShipInit

/*Displays the list of commands for the user to interact
 *with the animation*/
void showMenu() {
   printf("Welcome to the Space Animation.\n");
   printf("q = quit\n");
}//showMenu


/**************************************************************************/
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*****************************Draw Methods*********************************/
/**************************************************************************/
/*Draws the stars in the background*/
void drawStars()
{
	int i;
	GLfloat mat_ambient[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess[] = { 100.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glColor3f(1.0,1.0,1.0);
	glPointSize(3);

	for(i=0;i<STAR_NUM;i++)
	{
		glBegin(GL_POINTS);
			glVertex3d(theStars.Stars[i].pos[X],theStars.Stars[i].pos[Y],theStars.Stars[i].pos[Z]);
		glEnd();
	}
	glFlush();
}//drawStars

/*Draws the cockpit of the big ship*/
void drawBigShipCockpit()
{
	int i,j;
	/*Cockpit*/
	GLfloat mat_ambient[] = {0.2, 0.2, 1.0, 1.0};
	GLfloat mat_diffuse[] = {0.7, 0.7, 1.0, 1.0};
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess[] = { 100.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	for (i=0; i < 120; i++) 
	{	 
		glBegin(GL_POLYGON);
			for (j=0; j < 3; j++)
			{
			   glNormal3fv(bigShip.cockpit.normals[bigShip.cockpit.normalIndex[i][j]]);
			   glVertex3fv(bigShip.cockpit.vertices[bigShip.cockpit.faceIndex[i][j]]);
			}
		glEnd();
    }
	/****/
}//drawBigShipCockpit

/*Draws the body of the big ship*/
void drawBigShipBody()
{
	int i,j;
	/*Body**/
	GLfloat mat_ambient[] = {0.0, 0.0, 0.8, 1.0};
	GLfloat mat_diffuse[] = {0.0, 1.0, 0.8, 1.0};
	GLfloat mat_specular[] = {0.4, 0.4, 0.4, 1.0};
	GLfloat mat_shininess[] = { 90.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glColor3f(1.0,1.0,1.0);
	glPointSize(3);

	//glBindTexture(GL_TEXTURE_2D, 0);
	for (i=0; i < 360; i++) 
	{	 
		glBegin(GL_POLYGON);
			for (j=0; j < 3; j++)
			{
			   	//glTexCoord2iv(bigShip.body.uv[bigShip.body.faceIndex[i][j]]);
				glNormal3fv(bigShip.body.normals[bigShip.body.normalIndex[i][j]]);
			   glVertex3fv(bigShip.body.vertices[bigShip.body.faceIndex[i][j]]);
			}
		glEnd();
    }
	/****/
	glFlush();
}//drawBigShipBody

/*Draws the small engines of the ship*/
void drawBigShipSmallEngines()
{
	int i,j;
	/*Small Ship Engines**/
	GLfloat mat_ambient[] = {0.1, 0.1, 0.1, 1.0};
	GLfloat mat_diffuse[] = {0.3, 0.3, 0.3, 1.0};
	GLfloat mat_specular[] = {0.9, 0.9, 0.9, 1.0};
	GLfloat mat_shininess[] = { 90.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glColor3f(1.0,1.0,1.0);
	glPointSize(3);

	for (i=0; i < 132; i++) 
	{	 
		glBegin(GL_POLYGON);
			for (j=0; j < 3; j++)
			{
			   glNormal3fv(bigShip.right_engine.normals[bigShip.right_engine.normalIndex[i][j]]);
			   glVertex3fv(bigShip.right_engine.vertices[bigShip.right_engine.faceIndex[i][j]]);
			}
		glEnd();

		glBegin(GL_POLYGON);
			for (j=0; j < 3; j++)
			{
			   glNormal3fv(bigShip.left_engine.normals[bigShip.left_engine.normalIndex[i][j]]);
			   glVertex3fv(bigShip.left_engine.vertices[bigShip.left_engine.faceIndex[i][j]]);
			}
		glEnd();
    }
	/****/
	glFlush();
}//drawBigShipEngine

/*Draws the big engine of the big ship.*/
void drawBigShipEngine()
{
	int i,j,k;
	/*Big Engine**/
	GLfloat mat_ambient[] = {0.1, 0.1, 0.1, 1.0};
	GLfloat mat_diffuse[] = {0.3, 0.3, 0.3, 1.0};
	GLfloat mat_specular[] = {0.9, 0.9, 0.9, 1.0};
	GLfloat mat_shininess[] = { 90.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glColor3f(1.0,1.0,1.0);
	glPointSize(3);
	k=0;
	for (i=0; i < 132; i++) 
	{	 
		glBegin(GL_POLYGON);
			for (j=0; j < 3; j++)
			{
			   glNormal3fv(bigShip.big_engine.normals[bigShip.big_engine.normalIndex[i][j]]);
			   glVertex3fv(bigShip.big_engine.vertices[bigShip.big_engine.faceIndex[i][j]]);
			}
		glEnd();
    }
	/****/
	glFlush();
}//drawBigShipEngine

/*Draws the big ship so it appears to be moving away from the planet*/
void drawBigShip()
{
	drawBigShipCockpit();
	drawBigShipBody();
	drawBigShipSmallEngines();
	drawBigShipEngine();
}//drawBigShip

/*Draws the body of the alien ship.*/
void drawAlienShipBody()
{
	int i,j,k;
	GLfloat mat_ambient[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat mat_diffuse[] = {0.1, 0.1, 0.1, 1.0};
	GLfloat mat_specular[] = {0.7, 0.7, 0.7, 1.0};
	GLfloat mat_shininess[] = { 100.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glColor3f(1.0,1.0,1.0);
	glPointSize(3);
	k=0;

	for (i=0; i < 120; i++) 
	{	 
		glBegin(GL_POLYGON);
			for (j=0; j < 3; j++)
			{
			   glNormal3fv(alienShip.body.normals[alienShip.body.normalIndex[i][j]]);
			   glVertex3fv(alienShip.body.vertices[alienShip.body.faceIndex[i][j]]);
			}
		glEnd();
		k++;
    }
	glFlush();
}//drawAlienShipBody

/*Draws the cockpit of the alien ship.*/
void drawAlienShipCockpit()
{
	int i,j,k;
	GLfloat mat_ambient[] = {0.2, 0.9, 0.2, 1.0};
	GLfloat mat_diffuse[] = {0.3, 0.9, 0.3, 1.0};
	GLfloat mat_specular[] = {0.7, 0.7, 0.7, 1.0};
	GLfloat mat_shininess[] = { 100.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glColor3f(1.0,1.0,1.0);
	glPointSize(3);
	k=0;

	for (i=0; i < 120; i++) 
	{	 
		glBegin(GL_POLYGON);
			for (j=0; j < 3; j++)
			{
			   glNormal3fv(alienShip.cockpit.normals[alienShip.cockpit.normalIndex[i][j]]);
			   glVertex3fv(alienShip.cockpit.vertices[alienShip.cockpit.faceIndex[i][j]]);
			}
		glEnd();
		k++;
    }
	glFlush();
}//drawAlienShipCockpit

/*Draws the alien ship.*/
void drawAlienShip()
{
	drawAlienShipCockpit();
	drawAlienShipBody();
}//drawAlienShip

/*Draws the planet that is viewed in the background*/
void drawPlanet()
{
	int i,j,k;
	/*Lighting*********************************/
	GLfloat mat_ambient[] = {0.3, 0.3, 0.3, 1.0};
	GLfloat mat_diffuse[] = {0.5, 0.8, 0.0, 1.0};
	GLfloat mat_specular[] = {0.3, 0.3, 0.3, 1.0};
	GLfloat mat_shininess[] = { 50.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	/******************************************/

	glColor3f(1.0,1.0,1.0);
	glPointSize(3);
	k=0;
	glBindTexture(GL_TEXTURE_2D, 0);
	for (i=0; i < 360; i++) 
	{	 
		glBegin(GL_POLYGON);
			for (j=0; j < 3; j++)
			{
				glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
				glTexCoord2iv(planet.uv[planet.faceIndex[i][j]]);
			   glNormal3fv(planet.normals[planet.normalIndex[i][j]]);
			   glVertex3fv(planet.vertices[planet.faceIndex[i][j]]);
			}
		glEnd();
		k++;
    }
	glFlush();
}//drawPlanet

/*The ship fires a laser at the ship and the ship explodes.*/
void shootLaser()
{
	/*Lighting*********************************/
	GLfloat mat_ambient[] = {1.0, 0.0, 0.0, 1.0};
	GLfloat mat_diffuse[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess[] = { 100.0 };

	glEnable (GL_LIGHTING  );
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	/******************************************/
	glColor3f(1.0,0.0,0.0);
	glLineWidth(9);
		glBegin(GL_LINES);
			glVertex3f(10.0f,20.0f,40.0f);
			glVertex3f(100.0f,130.0f,40.0f);
		glEnd();
	glFlush();
}//shootLaser

/*Draws each particle of the explosion on the screen*/
void drawExplosion()
{
	int i;

	/*Lighting*********************************/
	GLfloat mat_ambient[] = {1.0, 1.0, 0.0, 1.0};
	GLfloat mat_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_specular[] = {0.1, 0.1, 0.1, 1.0};
	GLfloat mat_shininess[] = { 100.0 };

	glEnable (GL_LIGHTING  );
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	/******************************************/

	for(i=0;i<PARTICLE_NUM;i++)
	{
		glColor3i(explosion.pieces[i].colour.r,explosion.pieces[i].colour.g,explosion.pieces[i].colour.b);
		glBegin(GL_POINTS);
			glVertex3f(explosion.pieces[i].pos[X],explosion.pieces[i].pos[Y],explosion.pieces[i].pos[Z]);
		glEnd();
	}
	glFlush();
}//drawExplosion

/**************************************************************************/
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/****************************Updaters and Functionality********************/
/**************************************************************************/

/*Updates each particles position in the explosion*/
void updateExplosion()
{
	int i;

	for(i=0;i<PARTICLE_NUM;i++)
	{
		explosion.pieces[i].pos[X] = explosion.pieces[i].pos[X] + explosion.pieces[i].dir[X]*0.1;
		explosion.pieces[i].pos[Y] = explosion.pieces[i].pos[Y] + explosion.pieces[i].dir[Y]*0.1;
		explosion.pieces[i].pos[Z] = explosion.pieces[i].pos[Z] + explosion.pieces[i].dir[Z]*0.1;
	}
}//updateExplosion

/*Updates the position of all the vertices of the ship by stepAmount.
 *The smaller stepAmount, the slower the movement.*/
void updateBigShipPos(float stepAmount)
{
	int i;
	/*Body*/
	for(i=0;i<229;i++)
	{
		bigShip.body.vertices[i][X] = bigShip.body.vertices[i][X]+stepAmount;
		bigShip.body.vertices[i][Y] = bigShip.body.vertices[i][Y]+stepAmount;
		bigShip.body.vertices[i][Z] = bigShip.body.vertices[i][Z]+stepAmount;
	}

	/*Cockpit*/
	for(i=0;i<89;i++)
	{
		bigShip.cockpit.vertices[i][X] = bigShip.cockpit.vertices[i][X]+stepAmount;
		bigShip.cockpit.vertices[i][Y] = bigShip.cockpit.vertices[i][Y]+stepAmount;
		bigShip.cockpit.vertices[i][Z] = bigShip.cockpit.vertices[i][Z]+stepAmount;
	}

	/*Big Engine*/
	for(i=0;i<109;i++)
	{
		bigShip.big_engine.vertices[i][X] = bigShip.big_engine.vertices[i][X]+stepAmount;
		bigShip.big_engine.vertices[i][Y] = bigShip.big_engine.vertices[i][Y]+stepAmount;
		bigShip.big_engine.vertices[i][Z] = bigShip.big_engine.vertices[i][Z]+stepAmount;
	}

	/*Small Engines*/
	for(i=0;i<109;i++)
	{
		bigShip.left_engine.vertices[i][X] = bigShip.left_engine.vertices[i][X]+stepAmount;
		bigShip.left_engine.vertices[i][Y] = bigShip.left_engine.vertices[i][Y]+stepAmount;
		bigShip.left_engine.vertices[i][Z] = bigShip.left_engine.vertices[i][Z]+stepAmount;

		bigShip.right_engine.vertices[i][X] = bigShip.right_engine.vertices[i][X]+stepAmount;
		bigShip.right_engine.vertices[i][Y] = bigShip.right_engine.vertices[i][Y]+stepAmount;
		bigShip.right_engine.vertices[i][Z] = bigShip.right_engine.vertices[i][Z]+stepAmount;
	}
}//updateBigShipPos


/*Adjusts the direction the camera faces. The camera should
 *always be focused on the ship.*/
void adjustCameraFocus()
{
	float step;
	step = 0.06;
	cameraPos[X] = cameraPos[X] + step;
    cameraPos[Y] = cameraPos[Y] + step;
	cameraPos[Z] = cameraPos[Z] + step;
}//adjustCameraFocus

/* This is what is renderd in the first scene. The first scene consists
 * of the ship leaving the earth for a certain distance.*/
void renderScene(void)
{
	float step,stepAmount;
	int i;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glEnable(GL_LIGHTING);

  gluPerspective(30.0, 1.0, 10.0, -100.0);
  gluLookAt(cameraPos[X], cameraPos[Y], cameraPos[Z]+10, cameraFocus.x, cameraFocus.y, cameraFocus.z, 0.0, 1.0, 0.0);
	step = 0.01;
	drawStars();

	switch(currentScene)
	{
	case 1://scene 1
		drawBigShip();
		updateBigShipPos(step);//move ship away from planet
		stepCounter++;
		adjustCameraFocus();
		if(stepCounter ==2700)
		{
			currentScene = scene2;//next scene when ships reaches position
			stepCounter = 0;
		}
		break;
	
	case 2://scene 2
		drawBigShip();
		stepAmount = 0.1;
		stepCounter++;
		if(stepCounter < 400)
		{
			/*Body*/
			for(i=0;i<89;i++)
			{
				alienShip.body.vertices[i][Y] = alienShip.body.vertices[i][Y]+stepAmount;
				alienShip.body.vertices[i][Z] = alienShip.body.vertices[i][Z]+stepAmount;
			}
			
			/*Cockpit*/
			for(i=0;i<89;i++)
			{
				alienShip.cockpit.vertices[i][Y] = alienShip.cockpit.vertices[i][Y]+stepAmount;
				alienShip.cockpit.vertices[i][Z] = alienShip.cockpit.vertices[i][Z]+stepAmount;
			}
		}
		/*Turn around the planet*/
		if(stepCounter >=400 && stepCounter < 900)
		{
			/*Body*/
			for(i=0;i<89;i++)
			{
				alienShip.body.vertices[i][X] = alienShip.body.vertices[i][X]+stepAmount*0.3;
				alienShip.body.vertices[i][Y] = alienShip.body.vertices[i][Y]+stepAmount;
				alienShip.body.vertices[i][Z] = alienShip.body.vertices[i][Z]+stepAmount;
			}

			/*Cockpit*/
			for(i=0;i<89;i++)
			{
				alienShip.cockpit.vertices[i][X] = alienShip.cockpit.vertices[i][X]+stepAmount*0.3;
				alienShip.cockpit.vertices[i][Y] = alienShip.cockpit.vertices[i][Y]+stepAmount;
				alienShip.cockpit.vertices[i][Z] = alienShip.cockpit.vertices[i][Z]+stepAmount;
			}
		}

		if(stepCounter >=900 && stepCounter < 1700)
		{
			
			/*Body*/
			for(i=0;i<89;i++)
			{
				alienShip.body.vertices[i][X] = alienShip.body.vertices[i][X]+stepAmount*0.3;
				alienShip.body.vertices[i][Y] = alienShip.body.vertices[i][Y]+stepAmount;
				alienShip.body.vertices[i][Z] = alienShip.body.vertices[i][Z]+stepAmount;
			}

			/*Cockpit*/
			for(i=0;i<89;i++)
			{
				alienShip.cockpit.vertices[i][X] = alienShip.cockpit.vertices[i][X]+stepAmount*0.3;
				alienShip.cockpit.vertices[i][Y] = alienShip.cockpit.vertices[i][Y]+stepAmount;
				alienShip.cockpit.vertices[i][Z] = alienShip.cockpit.vertices[i][Z]+stepAmount;
			}
		}

		/*Approach ship_1*/
		if(stepCounter >=1700 && stepCounter < 3000)
		{
			
			/*Body*/
			for(i=0;i<89;i++)
			{
				alienShip.body.vertices[i][X] = alienShip.body.vertices[i][X]+stepAmount;
				alienShip.body.vertices[i][Z] = alienShip.body.vertices[i][Z]+stepAmount;
			}

			/*Cockpit*/
			for(i=0;i<89;i++)
			{
				alienShip.cockpit.vertices[i][X] = alienShip.cockpit.vertices[i][X]+stepAmount;
				alienShip.cockpit.vertices[i][Z] = alienShip.cockpit.vertices[i][Z]+stepAmount;
			}
		}

		/*Approach ship_3*/
		if(stepCounter >=3000 && stepCounter < 3500)
		{
			
			/*Body*/
			for(i=0;i<89;i++)
			{
				alienShip.body.vertices[i][X] = alienShip.body.vertices[i][X]+stepAmount;
				alienShip.body.vertices[i][Y] = alienShip.body.vertices[i][Y]+stepAmount*0.3;
			}

			/*Cockpit*/
			for(i=0;i<89;i++)
			{
				alienShip.cockpit.vertices[i][X] = alienShip.cockpit.vertices[i][X]+stepAmount;
				alienShip.cockpit.vertices[i][Y] = alienShip.cockpit.vertices[i][Y]+stepAmount*0.3;
			}
		}

		/*Circle ship_1*/
		if(stepCounter >=3500 && stepCounter < 4000)
		{
			
			/*Body*/
			for(i=0;i<89;i++)
			{
				alienShip.body.vertices[i][X] = alienShip.body.vertices[i][X]+stepAmount;
				alienShip.body.vertices[i][Z] = alienShip.body.vertices[i][Z]-stepAmount;
			}

			/*Cockpit*/
			for(i=0;i<89;i++)
			{
				alienShip.cockpit.vertices[i][X] = alienShip.cockpit.vertices[i][X]+stepAmount;
				alienShip.cockpit.vertices[i][Z] = alienShip.cockpit.vertices[i][Z]-stepAmount;
			}
		}

		/*Circle ship_2*/
		if(stepCounter >=4000 && stepCounter < 4800)
		{
			
			/*Body*/
			for(i=0;i<89;i++)
			{
				alienShip.body.vertices[i][Y] = alienShip.body.vertices[i][Y]+stepAmount;
			}

			/*Cockpit*/
			for(i=0;i<89;i++)
			{
				alienShip.cockpit.vertices[i][Y] = alienShip.cockpit.vertices[i][Y]+stepAmount;
			}
		}

		/*Circle ship_3*/
		if(stepCounter >=4800 && stepCounter < 5500)
		{
			
			/*Body*/
			for(i=0;i<89;i++)
			{
				alienShip.body.vertices[i][X] = alienShip.body.vertices[i][X]-stepAmount;
				alienShip.body.vertices[i][Z] = alienShip.body.vertices[i][Z]+stepAmount;
			}

			/*Cockpit*/
			for(i=0;i<89;i++)
			{
				alienShip.cockpit.vertices[i][X] = alienShip.cockpit.vertices[i][X]-stepAmount;
				alienShip.cockpit.vertices[i][Z] = alienShip.cockpit.vertices[i][Z]+stepAmount;
			}
		}

		/*Circle ship_4*/
		if(stepCounter >=5500 && stepCounter < 5900)
		{
			
			/*Body*/
			for(i=0;i<89;i++)
			{
				alienShip.body.vertices[i][Y] = alienShip.body.vertices[i][Y]-stepAmount;
			}

			/*Cockpit*/
			for(i=0;i<89;i++)
			{
				alienShip.cockpit.vertices[i][Y] = alienShip.cockpit.vertices[i][Y]-stepAmount;
			}
		}

		if(stepCounter >=5900 && stepCounter < 6700)
		{
			shootLaser();
		}

		if(stepCounter > 6700)
		{
			stepCounter=0;
			currentScene = scene3;
		}
		break;
	case 3://scene 3
		stepCounter++;
		drawExplosion();
		updateExplosion();
		if(stepCounter > 4000)
		{
			currentScene = scene4;
		}
		break;

	case 4://scene 4
		//do nothing
		break;
	}
	drawPlanet();
	drawAlienShip();

	glFlush();
	glutSwapBuffers();
}//renderFrame
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/

/**************************************************************************/
/**************************User Input Handling*****************************/
/**************************************************************************/
/* Gets the users input as key and does the necessary 
 * action. */
void keyboard(unsigned char key, int x, int y) {

   switch (key){

      case 0x1B:
      case 'q':
      case 'Q':
	   exit(0);
	 break;
   }
}//keyboard

void mouse(int btn, int state, int x, int y) {

     /*
     printf("%3d, %3d, %f %f %f\n", btn, state, 
		global.angle[X],global.angle[Y],global.angle[Z]); */
}//mouse
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/

main(int argc, char **argv) {

  showMenu();
  planetInit();
  bigShipInit();
  alienShipInit();
  explosionInit();
  starsInit();
  paused = FALSE;
  bigShipTex.data = readBigShipTexture("big_ship_texture.png",&bigShipTex.w,&bigShipTex.h);
  printf("height: %i\n",bigShipTex.h);
  printf("width: %i\n",bigShipTex.w);
  cameraPos[X] = 40.0;
  cameraPos[Y] = 350.0;
  cameraPos[Z] = 300.0;
  cameraFocus.x = -40.0;
  cameraFocus.y = 0.0;
  cameraFocus.z = -40.0;
  currentScene = scene1;
  stepCounter = 0;

  glutInit(&argc, argv);
  glutInitWindowSize(800, 800);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("Space Animation");

  glutMouseFunc(mouse);
  glutKeyboardFunc(keyboard);
  glutDisplayFunc(redraw);
  glutIdleFunc(renderScene); 
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //glOrtho(-100.0, 100.0, -100.0, 100.0, -100.0, 100.0);

  gluPerspective(30.0, 1.0, 10.0, -100.0);
  gluLookAt(cameraPos[X], cameraPos[Y], cameraPos[Z], cameraFocus.x, cameraFocus.y, cameraFocus.z, 0.0, 1.0, 0.0);

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glEnable(GL_DEPTH_TEST);
  
  lightInit();
  textureInit();

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  glFrontFace(GL_CCW); 
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

  glutMainLoop();
}//main