#ifndef shape
#define shape

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "CSCIx229.h"
#include "Util.h"

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
extern double		normal[3];
extern unsigned int texture[];
extern int tMode;
extern int ntex;

void renderCylinder( double x1, double y1, double z1,
							double x2, double y2, double z2,
							double radius,int subdivisions,GLUquadricObj *quadric);

void renderCylinder_convenient(double x1, double y1, double z1,
								 double x2, double y2, double z2,
								 double radius,int subdivisions);

void cube( double x, double y, double z,
           double dx,double dy,double dz,
           double th);

void box(double t[12],double b[12]);

void ball(double x,double y,double z,double r);

void ball(double x,double y,double z,
			double r, double g, double b,
			double radius);

void cone( double tx, double ty, double tz,
						double bx, double by, double bz,
						double r, int subdivisions
					);

#endif