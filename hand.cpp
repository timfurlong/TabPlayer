#include "hand.h"

void hand::finger(double th, double r_base,
						const double boneLen[],
						const double boneAngle[])
{
	// int numBones = sizeof(boneLen) / sizeof(double);
	vector<double> pt;
	vector<double> pts;
	double ph = 0;
	double r = r_base;

	glColor3f(1,0,0);

	glPushMatrix();
		pt = GetVertex(th, ph, r);
		for (int i=0; i<3; i++)
		{
			glTranslated( pt[0], pt[1], pt[2] );
			ph += boneAngle[i];
			pt = GetVertex(th, ph, boneLen[i]);
			glBegin(GL_LINES);
				glVertex3d( 0,0,0 );
				glVertex3d( pt[0], pt[1], pt[2] );
			glEnd();
		}
	glPopMatrix();

	ErrCheck("finger");
}

/*
 *  Draw the hand
 *     at (x,y,z)
 *     neck facing (dx,dy,dz)
 *     up towards (ux,uy,uz)
 */
void hand::drawHand(  double x , double y , double z,
						   double dx, double dy, double dz,
						   double ux, double uy, double uz)
{
		const double scaleFactor     = 1;

		// Base parameters
		const double fingerTh[5] = {10, 75, 90, 105, 120};
		const double baseLen[5]  = {.5,  1,  1,   1,   0.7};

		// Finger parameters
		const double thumbLen[3] = {0.3, 0.2, 0};
		const double thumbAngle[3] = {20, 20, 0};

		const double indexLen[3] = { 0.3, 0.3, 0.2};
		const double indexAngle[3] = {0, 0, 0};

		const double middleLen[3] = { 0.3, 0.3, 0.2};
		const double middleAngle[3] = {0, 0, 0};

		const double ringLen[3] = { 0.3, 0.3, 0.2};
		const double ringAngle[3] = {0, 0, 0};

		const double pinkyLen[3] = {0.2, 0.2, 0.1};
		const double pinkyAngle[3] = {0, 0, 0};

		double mat[16];
		double fingerBase[10];

		getMatrix(  dx,dy,dz,
						ux,uy,uz,  mat);
		//  Save transformation
		glPushMatrix();
		//  Offset
		glTranslated( x ,y ,z );
		glScaled( scaleFactor, scaleFactor, scaleFactor);
		glMultMatrixd(mat);

		// Begin drawing  =================================

		// Hand Base
		glColor3f( 0, 0, 1 );
		glBegin(GL_LINES);
			for (int i=0; i<5; i++){
				glVertex3d(0,0,0);
				Vertex( fingerTh[i], 0, baseLen[i] );
				fingerBase[i*2]   = fingerTh[i];
				fingerBase[i*2+1] = baseLen[i];
			}
		glEnd();

		// Fingers
		finger(fingerBase[0], fingerBase[1], thumbLen, thumbAngle);
		finger(fingerBase[2], fingerBase[3], indexLen, indexAngle);
		finger(fingerBase[4], fingerBase[5], middleLen, middleAngle);
		finger(fingerBase[6], fingerBase[7], ringLen, ringAngle);
		finger(fingerBase[8], fingerBase[9], pinkyLen, pinkyAngle);

		//  Undo transformations
		glPopMatrix();
		ErrCheck("hand");
}

