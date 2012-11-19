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
const double neck_length         = 5;
const double neck_r              = .3;
const double fretboard_thickness = 0.06;
const double fret_r              = 0.02;
const double fret_position[]     = {
	0.978378, 0.955683, 0.931640, 0.906167, 0.879180,
	0.850588, 0.820295, 0.788202, 0.754199, 0.718177,
	0.680010, 0.639575, 0.596735, 0.551346, 0.503261,
	0.452316, 0.398341, 0.341155, 0.280570, 0.216383,
	0.148380, 0.076331
	};
const int inlay_frets[]   = { 3,  5,  7,  9, 12,
										15, 17, 19, 21 };
const double inlay_r      = 0.05;
const double tiny_inlay_r = 0.02;

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

