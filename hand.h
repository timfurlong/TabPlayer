#include "CSCIx229.h"
#include "Util.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::vector;
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

class hand
{
public:
	hand(   double x , double y , double z,
		     double dx, double dy, double dz,
		     double ux, double uy, double uz)
		{
			this->drawHand( x,y,z,  dx,dy,dz,  ux,uy,uz );
		}

	void drawHand( double x , double y , double z,
				 double dx, double dy, double dz,
				 double ux, double uy, double uz);

	void finger(double th, double r_base,
					const double boneLen[],
					const double boneAngle[]);

	// ~hand();

	/* data */
};


