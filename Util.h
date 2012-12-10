#ifndef Util
#define Util

#include "CSCIx229.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <set>
using std::vector;
using std::set;

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
#define degrees(th) (180/3.1415926)*(th)

extern int axes;
extern int pMode;
extern int th;
extern int ph;
extern double dim;

template <typename T, size_t N>
inline
size_t SizeOfArray( const T(&)[ N ] )
{
	return N;
}

template <typename T, size_t N>
inline
size_t SizeOfArray( T(&)[ N ] )
{
	return N;
}

void getMatrix(double dx, double dy, double dz,
								double ux, double uy, double uz,
								double* mat);

void Vertex(double th,double ph);
void Vertex(double th,double ph, double r);
void Vertex(double th,double ph, double r,
				double dx, double dy, double dz);

void setEye();

void ReduceToUnit(double vector[3]); // Reduces A Normal Vector (3 Coordinates)

void setNormal( double v0x, double v0y, double v0z,
					 double v1x, double v1y, double v1z,
					 double v2x, double v2y, double v2z,
					 double normal[3]);

void drawAxes();

vector<double> GetVertex(double th, double ph, double r);
vector<double> GetVertex(double th, double ph, double r,
								 double dx, double dy, double dz);

vector<double> getTranslateCoord(double x, double y, double z, vector<double> pt);
vector<double> getRotateCoord(double th, int rx, int ry, int rz,
										vector<double> pt);

bool contains(int num, int array[], int arraySize);
bool contains( vector<int> v, int num);

int lenD( int array[] );
#endif