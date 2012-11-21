#ifndef Util
#define Util

#include "CSCIx229.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <vector>
using std::vector;

#ifdef USEGLEW
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

extern int axes;
extern int pMode;
extern int th;
extern int ph;
extern double dim;

void getMatrix(double dx, double dy, double dz,
								double ux, double uy, double uz,
								double* mat);

void Vertex(double th,double ph);

void Vertex(double th,double ph, double r);

void setEye();

void ReduceToUnit(double vector[3]); // Reduces A Normal Vector (3 Coordinates)

void setNormal(  double v0x, double v0y, double v0z,
								double v1x, double v1y, double v1z,
								double v2x, double v2y, double v2z,
								double normal[3]);

void drawAxes();

double* GetVertex(double th, double ph, double r,
						double arr[]);

vector<double> GetVertex(double th, double ph, double r,
						vector<double> arr[]);

vector<double> GetVertex(double th, double ph, double r);

bool contains(int num, int* const array);

int lenD( int array[] );
#endif