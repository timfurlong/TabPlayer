#include "CSCIx229.h"
#include "Util.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <fstream>
#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
using namespace std;

#ifdef USEGLEW
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

extern int axes;
extern int pMode;
extern int th;
extern int ph;
extern double dim;
extern int emission  ;
extern float shinyvec[1];
extern float ambientvec[3];
extern float diffuseVec[3];
extern float specularvec[3];
extern double		normal[3];
extern unsigned int texture[];
extern int tMode;
extern int ntex;

// Guitar measurement values
extern const double neck_length;
extern const double neck_r;
extern const double fretboard_thickness;
extern const double fret_r;
extern const double fret_position[];
extern const int num_frets;
extern const int inlay_frets[];
extern const double inlay_r;
extern const double tiny_inlay_r;

class guitar
{
public:
	/*
	 *  Draw a guitar
	 *     at (x,y,z)
	 *     neck facing (dx,dy,dz)
	 *     up towards (ux,uy,uz)
	 */
	guitar(  double x , double y , double z,
		     double dx, double dy, double dz,
		     double ux, double uy, double uz)
	{
		this->neck( x,y,z,  dx,dy,dz,  ux,uy,uz );
	}
	// ~guitar();


	void neck(  double x , double y , double z,
				double dx, double dy, double dz,
				double ux, double uy, double uz);

	void fretboard();
	void frets();
	void inlay(double x, double y, double z);
};

