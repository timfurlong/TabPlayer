/*
 *  Visual Tab Player
 *  Author: Tim Furlong
 *  CSCI 4229 Final Project
 */

#include "guitar.h"
#include "CSCIx229.h"
#include "Util.h"
#include "shape.h"
#include "hand.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <cstdlib> // for exit function
#include <math.h>

#include <fstream>
#include <iostream>
#include <vector>
using std::cerr;
using std::cout;
using std::endl;
using namespace std;

//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int axes   = 0;     //  Display axes
int pMode  = 1;     //  Projection mode
int move   = 1;     //  Move light
int th     = 0;     //  Azimuth of view angle
int ph     = 0;     //  Elevation of view angle
int fov    = 55;    //  Field of view (for perspective)
int light  = 1;     //  Lighting
double asp = 1;     //  Aspect ratio
double dim = 3.0;   //  Size of world


// Light values
int one           =   1;  // Unit value
int dist          =   5;  		 // Light distance
int inc           =  10;  // Ball increment
int smooth        =   0;  // Smooth/Flat shading
int local         =   0;  // Local Viewer Model
int emission      =   0;  // Emission intensity (%)
int ambient       =  50;  // Ambient intensity (%)
int diffuse       = 100;  // Diffuse intensity (%)
int specular      =   0;  // Specular intensity (%)
int shininess     =   0;  // Shininess (power of two)
int zh            =  90;  // Light azimuth
float ylight      =   0;  // Elevation of light
int moveLightVert =   0;
float Position[4];
double		normal[3];		// An Array To Store The Normal Data
float shinyvec[1];    // Shininess (value)
float ambientvec[3];
float diffuseVec[3];
float specularvec[3];

// Texture values
unsigned int texture[5];  //  Texture names
int tMode=0;
int ntex=0;    //  Texture
int rep=1;        //  Repitition

#define PI 3.1415927
#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))


void stageFloor(double y)
{
	const double len         = 20;
	const double carpet_r = 3;
	const float floor_l   = 0.3;
	const float carpet_l  = 0.7;
	const double l_prec = 0.1;

	//  Set specular color to white
	float white[] = {1,1,1,1};
	float Emission[]  = {0.0,0.0,0.01*emission,1.0};
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);

	//  Enable textures
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,tMode?GL_REPLACE:GL_MODULATE);
	glColor3f(floor_l ,floor_l ,floor_l );
	glBindTexture(GL_TEXTURE_2D,texture[4]);
	// glBegin(GL_QUADS);
	// 	glNormal3f(0,1,0);
	// 	glTexCoord2f(0,0); glVertex3d(-len,y,-len);
	// 	glTexCoord2f(0,1); glVertex3d(-len,y,+len);
	// 	glTexCoord2f(1,1); glVertex3d(+len,y,+len);
	// 	glTexCoord2f(1,0); glVertex3d(+len,y,-len);
	// glEnd();
	glBegin(GL_QUADS);
		glNormal3f(0,1,0);
		glColor3f(1,1,1);
		for (double z=-len; z<len; z+=l_prec){
			for (double x=-len; x<len; x+=l_prec){
				glTexCoord2f(x/len,z/len); glVertex3d(x,y,z);
				glTexCoord2f(x/len,(z+l_prec)/len); glVertex3d(x,y,z+l_prec);
				glTexCoord2f((x+l_prec)/len,(z+l_prec)/len); glVertex3d(x+l_prec,y,z+l_prec);
				glTexCoord2f((x+l_prec)/len,z/len); glVertex3d(x+l_prec,y,z);
			}
		}
	glEnd();

	glColor3f(carpet_l, carpet_l, carpet_l);
	glBindTexture(GL_TEXTURE_2D,texture[3]);
	glBegin( GL_POLYGON );
		glNormal3f(0,1,0);
		for (int i = 0; i<=360 ; i++){
			glTexCoord2f(Sin(i)/2+.5,Cos(i)/2+.5);
			glVertex3d(carpet_r*Sin(i),y+0.001,carpet_r*Cos(i));
		}
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
	//  Erase the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	//  Undo previous transformations
	glLoadIdentity();
	setEye();

	//  Flat or smooth shading
	glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

	//  Light switch
	if (light)
	{
		//  Translate intensity to color vectors
		float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
		float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
		float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
		//  Light position

		// double Position[] = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
		if (moveLightVert == 1)
			{
				Position[0] = ylight;
				Position[1] = dist*Cos(zh);
				Position[2] = dist*Sin(zh);
				Position[3] = 1.0;
			}
		else
			{
				Position[0] = dist*Cos(zh);
				Position[1] = ylight;
				Position[2] = dist*Sin(zh);
				Position[3] = 1.0;
			}

		//  Draw light position as ball (still no lighting here)
		glColor3f(1,1,1);
		ball(Position[0],Position[1],Position[2] , 0.1);
		//  OpenGL should normalize normal vectors
		glEnable(GL_NORMALIZE);
		//  Enable lighting
		glEnable(GL_LIGHTING);
		//  Location of viewer for specular calculations
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
		//  glColor sets ambient and diffuse color materials
		glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
		//  Enable light 0®
		glEnable(GL_LIGHT0);
		//  Set ambient, diffuse, specular components and position of light 0
		glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
		glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
		glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
		glLightfv(GL_LIGHT0,GL_POSITION,Position);
	}
	else
	  // glDisable(GL_LIGHTING);
	{
		//  Translate intensity to color vectors
		float Ambient[]       = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
		float Diffuse[]       = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
		float Specular[]      = {0.01*specular,0.01*specular,0.01*specular,1.0};
		float spotPosition[3] = {0,3,3};
		spotPosition[0] = 0;
		spotPosition[1] = 3;
		spotPosition[2] = 3;
		float spotDirection[4];
		for (int i=0; i<3; i++)
			spotDirection[i] = -spotPosition[i];
		spotDirection[3] = 0;

		//  Draw light position as ball (still no lighting here)
		glColor3f(1,1,1);
		ball(spotPosition[0],spotPosition[1],spotPosition[2] , 0.1);
		//  OpenGL should normalize normal vectors
		glEnable(GL_NORMALIZE);
		//  Enable lighting
		glEnable(GL_LIGHTING);
		//  Location of viewer for specular calculations
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
		//  glColor sets ambient and diffuse color materials
		glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
		//  Enable light 0®
		glEnable(GL_LIGHT0);
		//  Set ambient, diffuse, specular components and position of light 0
		glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
		glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
		glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);

		glLightfv(GL_LIGHT0, GL_POSITION, spotPosition );
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirection );
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 10); // angle is 0 to 180
		glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 20); // exponent is 0 to 128
		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.5);

		// glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 2.0);
		// glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1.0);
		// glLightModelf( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
		// glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Ambient);
		// glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirection);


	}

	//    ||* * * * * * * * * ********************************
	//    || * * * * * * * *  ********************************
	//    ||* * * * * * * * * ********************************
	//    || * * * * * * * *  ********************************
	//    ||* * * * * * * * * ********************************
	//    || * * * * * * * *  ********************************
	//    ||**************************************************
	//    ||**************************************************
	//    ||**************************************************
	//    ||**************************************************
	//    ||**************************************************
	//    ||**************************************************
	//    ||**************************************************
	//    ||
	//    ||
	//    ||

	// guitar( 0,0,0, 1,0,0, 0,1,0);
	// stageFloor(-2);
	hand(0,0,0, 1,0,0, 0,1,0 );
	// shape::cube(0,0,0 , 0.5,0.5,0.5 , 0);
	//  Draw axes
	glDisable(GL_LIGHTING);
	drawAxes();
	//  Five pixels from the lower left corner of the window
	glWindowPos2i(5,5);
	//  Print the text string
	Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s Light=%s, ",
	  th,ph,dim,fov,pMode?"Perpective":"Orthogonal",light?"On":"Off");

	if (light)
	{
		glWindowPos2i(5,45);
		Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",dist,ylight);
		glWindowPos2i(5,25);
		Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",shinyvec[0]);
	}

	// Check for errors
	ErrCheck("display");
	//  Render the scene
	glFlush();
	//  Make the rendered scene visible
	glutSwapBuffers();
}


void idle()
{
	//  Elapsed time in seconds
	double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
	zh = fmod(90*t,360.0);
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
	//  Right arrow key - increase angle by 8 degrees
	if (key == GLUT_KEY_RIGHT)
		th += 8;
	//  Left arrow key - decrease angle by 8 degrees
	else if (key == GLUT_KEY_LEFT)
		th -= 8;
	//  Up arrow key - increase elevation by 8 degrees
	else if (key == GLUT_KEY_UP)
		ph += 8;
	//  Down arrow key - decrease elevation by 8 degrees
	else if (key == GLUT_KEY_DOWN)
		ph -= 8;
	//  PageUp key - increase dim
	else if (key == GLUT_KEY_PAGE_UP)
		dim += 0.1;
	//  PageDown key - decrease dim
	else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
		dim -= 0.1;
	//  Smooth color model
	else if (key == GLUT_KEY_F1)
		smooth = 1-smooth;
	else if (key == GLUT_KEY_F2)
		dist = (dist==1) ? 5 : 1;
	//  Toggle ball increment
	else if (key == GLUT_KEY_F8)
		inc = (inc==10)?3:10;
	//  Keep angles to +/-360 degrees
	th %= 360;
	ph %= 360;
	//  Update projection
	Project(pMode?fov:0,asp,dim);
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
	//  Exit on ESC
		if (ch == 27)
			exit(0);
		//  Reset view angle
		else if (ch == '0')
			th = ph = 0;
		//  Toggle axes
		else if (ch == 'x' || ch == 'X')
			axes = 1-axes;
		//  Toggle lighting mode
		else if (ch == 'l' || ch == 'L')
			light = 1-light;
		//  Switch projection mode
		else if (ch == 'p' || ch == 'P')
			pMode = 1-pMode;
		//  Toggle texture mode
		else if (ch == 't' || ch == 'T')
		   tMode = 1-tMode;
		//  Move light
		else if (ch == '<')
			zh += 1;
		else if (ch == '>')
			zh -= 1;
		//  Change field of view angle
		else if (ch == '-' && ch>1)
			fov--;
		else if (ch == '+' && ch<179)
			fov++;
		//  Light elevation
		else if (ch=='[')
			ylight -= 0.1;
		else if (ch==']')
			ylight += 0.1;
		//  Ambient level
		else if (ch=='a' && ambient>0)
			ambient -= 5;
		else if (ch=='A' && ambient<100)
			ambient += 5;
		//  Diffuse level
		else if (ch=='d' && diffuse>0)
			diffuse -= 5;
		else if (ch=='D' && diffuse<100)
			diffuse += 5;
		//  Specular level
		else if (ch=='s' && specular>0)
			specular -= 5;
		else if (ch=='S' && specular<100)
			specular += 5;
		//  Emission level
		else if (ch=='e' && emission>0)
			emission -= 5;
		else if (ch=='E' && emission<100)
			emission += 5;
		//  Shininess level
		else if (ch=='n' && shininess>-1)
			shininess -= 1;
		else if (ch=='N' && shininess<7)
			shininess += 1;
		else if (ch=='v' || ch=='V')
			moveLightVert = 1-moveLightVert;

		//  Translate shininess power to value (-1 => 0)
	   shinyvec[0] = shininess<0 ? 0 : pow(2.0,shininess);
		//  Reproject
		Project(pMode?fov:0,asp,dim);
		//  Animate if requested
		glutIdleFunc(move?idle:NULL);
		//  Tell GLUT it is necessary to redisplay the scene
		glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
	//  Ratio of the width to the height of the window
	asp = (height>0) ? (double)width/height : 1;
	//  Set the viewport to the entire window
	glViewport(0,0, width,height);
	//  Set projection
	Project(pMode?fov:0,asp,dim);
}

bool Init(){
	//  Load textures
	texture[0] = LoadTexBMP("Data/Mahogany.bmp");
	texture[1] = LoadTexBMP("Data/rosewood2.bmp");
	texture[2] = LoadTexBMP("Data/pearlyInlay.bmp");
	texture[3] = LoadTexBMP("Data/carpet.bmp");
	texture[4] = LoadTexBMP("Data/woodFlooring.bmp");
	return true;
}
/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
	//  Initialize GLUT and process user parameters
	glutInit(&argc,argv);
	//  Request double buffered, true color window with Z buffering at 600x600
	glutInitWindowSize(600,600);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow("Visual Tab Player");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);
	if (!Init())									// Initialize Our Newly Created GL Window
	{
		exit(0);
	}
	//  Pass control to GLUT so it can interact with the user
	ErrCheck("init");
	glutMainLoop();
	return 0;
}



