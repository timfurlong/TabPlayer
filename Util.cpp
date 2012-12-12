#include "Util.h"

void getMatrix(double dx, double dy, double dz,
					double ux, double uy, double uz,
					double* mat)
{
	//  Unit vector in direction
   double D0 = sqrt(dx*dx+dy*dy+dz*dz);
   double X0 = dx/D0;
   double Y0 = dy/D0;
   double Z0 = dz/D0;
   //  Unit vector in "up" direction
   double D1 = sqrt(ux*ux+uy*uy+uz*uz);
   double X1 = ux/D1;
   double Y1 = uy/D1;
   double Z1 = uz/D1;
   //  Cross product gives the third vector
   double X2 = Y0*Z1-Y1*Z0;
   double Y2 = Z0*X1-Z1*X0;
   double Z2 = X0*Y1-X1*Y0;

   //  Rotation matrix
   // double *mat = new double[16];
   // double mat[16];

   mat[0] = X0;   mat[4] = X1;   mat[ 8] = X2;   mat[12] = 0;
   mat[1] = Y0;   mat[5] = Y1;   mat[ 9] = Y2;   mat[13] = 0;
   mat[2] = Z0;   mat[6] = Z1;   mat[10] = Z2;   mat[14] = 0;
   mat[3] =  0;   mat[7] =  0;   mat[11] =  0;   mat[15] = 1;

}

/*
*  Draw vertex in polar coordinates with normal
*/
void Vertex(double th,double ph)
{
	double x = Sin(th)*Cos(ph);
	double y = Cos(th)*Cos(ph);
	double z =         Sin(ph);
	//  For a sphere at the origin, the position
	//  and normal vectors are the same
	glNormal3d(x,y,z);
	glVertex3f(x,y,z);
}

vector<double> GetVertex(double th, double ph, double r)
{
	vector<double> v;

	v.push_back( r * Sin(th)*Cos(ph) );
	v.push_back( r * Cos(th)*Cos(ph) );
	v.push_back( r *         Sin(ph) );

	return v;
}

vector<double> GetVertex(double th, double ph, double r,
								 double dx, double dy, double dz)
{
	vector<double> v;

	v.push_back( r * Sin(th)*Cos(ph) + dx );
	v.push_back( r * Cos(th)*Cos(ph) + dy );
	v.push_back( r *         Sin(ph) + dz );

	return v;
}

vector<double> getTranslateCoord(double x, double y, double z, vector<double> pt)
{
	pt[0] = pt[0] + x;
	pt[1] = pt[1] + y;
	pt[2] = pt[2] + z;
	return pt;
}
vector<double> getRotateCoord(double th, int rx, int ry, int rz,
										vector<double> pt)
{
	vector<double> np;
	if (rx == 1)
	{
		np.push_back(pt[0]);
		np.push_back(pt[1]*Cos(th)-pt[2]*Sin(th));
		np.push_back(pt[1]*Sin(th)+pt[2]*Cos(th));
	}
	else if (ry == 1)
	{
		np.push_back( pt[0]*Cos(th)+pt[2]*Sin(th) );
		np.push_back( pt[1] );
		np.push_back( -pt[0]*Sin(th)+pt[2]*Cos(th) );
	}
	else if (rz ==1)
	{
		np.push_back(pt[0]*Cos(th)-pt[1]*Sin(th));
		np.push_back(pt[0]*Sin(th)+pt[1]*Cos(th));
		np.push_back(pt[2]);
	}

	return np;
}

/*
*  Draw vertex in polar coordinates with normal, r away from the radius.
*/
void Vertex(double th,double ph, double r)
{
	double x = r * Sin(th)*Cos(ph);
	double y = r * Cos(th)*Cos(ph);
	double z = r *         Sin(ph);
	//  For a sphere at the origin, the position
	//  and normal vectors are the same
	glNormal3d(x,y,z);
	glVertex3f(x,y,z);
}

void Vertex(double th,double ph, double r,
				double dx, double dy, double dz)
{
	double x = r*Sin(th)*Cos(ph) + dx;
	double y = r*Cos(th)*Cos(ph) + dy;
	double z = r*        Sin(ph) + dz;
	//  For a sphere at the origin, the position
	//  and normal vectors are the same
	glNormal3d(x,y,z);
	glVertex3f(x,y,z);
}

double acot(double x)
{
	/*
	 * Calculate inverse cotangent value.
	 */
	return atan(1 / x);
}

vector<double> GetPolarCoord(double x, double y, double z)
{
	// th =tan-1(z/(x^2+y^2)^1/2)
	// ph = tan-1(y/x)
	// r = (x^2 + y^2 + z^2)^1/2
	vector<double> pt;

	pt.push_back( degrees( atan2(  (pow(x,2) + pow(y,2)), z) ) ); //th
	pt.push_back( degrees( atan2( x,y) ) ); // ph
	pt.push_back( pow(x, 2)+pow(y, 2)+pow(z, 2) ); // r

	return pt;
}

void setEye(){
	//  Perspective - set eye position
	if (pMode)
	{
		double Ex = -2*dim*Sin(th)*Cos(ph);
		double Ey = +2*dim        *Sin(ph);
		double Ez = +2*dim*Cos(th)*Cos(ph);
		gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
	}

	//  Orthogonal - set world orientation
	else
	{
		//  Set view angle
		glRotatef(ph,1,0,0);
		glRotatef(th,0,1,0);
	}
}

void ReduceToUnit(double vector[3]) // Reduces A Normal Vector (3 Coordinates)
	{                                   // To A Unit Normal Vector With A Length Of One.
		 double length;                           // Holds Unit Length
		 // Calculates The Length Of The Vector
		 length = (double)sqrt((vector[0]*vector[0]) + (vector[1]*vector[1]) + (vector[2]*vector[2]));

		 if(length == 0.0f)                      // Prevents Divide By 0 Error By Providing
			  length = 1.0f;                      // An Acceptable Value For Vectors To Close To 0.

		 vector[0] /= length;                        // Dividing Each Element By
		 vector[1] /= length;                        // The Length Results In A
		 vector[2] /= length;                        // Unit Normal Vector.
	}

	/*
	*  Computes and sets the normal of two instersecting verticies.
	*
	*  v2          Inputs:
	*  ^					connectPt:  intersection point between the two
	*	|									vectors
	*	| 					v1, v2:     Verticies defining the two vectors
	*	|------->v1						with respect to the connecting point.
	*
	*/

void setNormal(  double v0x, double v0y, double v0z,
									double v1x, double v1y, double v1z,
									double v2x, double v2y, double v2z,
									double normal[3])
	{
		double v1[3],v2[3];       // Vector 1 (x,y,z) & Vector 2 (x,y,z)
		static const int x = 0;          // Define X Coord
		static const int y = 1;          // Define Y Coord
		static const int z = 2;          // Define Z Coord

		// Finds The Vector Between 2 Points By Subtracting
		// The x,y,z Coordinates From One Point To Another.

		// Calculate The Vector From Point 1 To Point 0
		v1[x] = v0x - v1x;                  // Vector 1.x=Vertex[0].x-Vertex[1].x
		v1[y] = v0y - v1y;                  // Vector 1.y=Vertex[0].y-Vertex[1].y
		v1[z] = v0z - v1z;                  // Vector 1.z=Vertex[0].y-Vertex[1].z
		// Calculate The Vector From Point 2 To Point 1
		v2[x] = v1x - v2x;                  // Vector 2.x=Vertex[0].x-Vertex[1].x
		v2[y] = v1y - v2y;                  // Vector 2.y=Vertex[0].y-Vertex[1].y
		v2[z] = v1z - v2z;                  // Vector 2.z=Vertex[0].z-Vertex[1].z
		// Compute The Cross Product To Give Us A Surface Normal
		normal[x] = v1[y]*v2[z] - v1[z]*v2[y];             // Cross Product For Y - Z
		normal[y] = v1[z]*v2[x] - v1[x]*v2[z];             // Cross Product For X - Z
		normal[z] = -(v1[x]*v2[y] - v1[y]*v2[x]);             // Cross Product For X - Y

		ReduceToUnit(normal);                      // Normalize The Vectors
	}

void drawAxes()
{
	const double len=2;  //  Length of axes
	//  Draw axes - no lighting from here on
	glColor3f(1,1,1);

	if (axes)
	{
		glBegin(GL_LINES);
			glVertex3f(0.0,0.0,0.0);
			glVertex3f(len,0.0,0.0);
			glVertex3f(0.0,0.0,0.0);
			glVertex3f(0.0,len,0.0);
			glVertex3f(0.0,0.0,0.0);
			glVertex3f(0.0,0.0,len);
		glEnd();
		//  Label axes
		glRasterPos3d(len,0.0,0.0);
		Print("X");
		glRasterPos3d(0.0,len,0.0);
		Print("Y");
		glRasterPos3d(0.0,0.0,len);
		Print("Z");
	}
}

void vectorAssignPt(vector<double>& v, double x, double y, double z)
{
	if (v.size() != 3){
		v.clear();
		v.push_back( x );
		v.push_back( y );
		v.push_back( z );
	}
	else{
		v[0]=x; v[1]=y; v[2]=z;
	}
}

bool contains( int num, int array[], int arraySize)
{
	for (int i=0;i< arraySize;i++){
		if ( array[i] == num )
			return true;
	}
	return false;
}

bool contains( vector<int> v, int num )
{
	vector<int>::iterator it;

	for (it=v.begin(); it!=v.end(); it++)
	{
		if (*it == num)
			return true;
	}
	return false;
}
// int lenD( int array[] )
// {
// 	return sizeof array/sizeof(double);
// }