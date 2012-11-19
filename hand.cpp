#include "hand.h"

void hand::finger(double x, double y, double z,
						double ph,
					const double boneLen[], const double boneAngle[])
{
	glPushMatrix();
	glColor3f(1,0,0);
	int numBones = sizeof boneLen/sizeof(double);
	glRotated(ph, 0,0,1);



	glBegin(GL_LINES);
		double pt[3] = {x,y,z};
		for (int i=0; i<numBones; i++){
			glVertex3dv( pt );
			pt[0] += boneLen[i]*Sin(boneAngle[i])*Cos(ph);
			pt[1] += boneLen[i]*Cos(boneAngle[i])*Cos(ph);
			pt[2] += boneLen[i]*                  Sin(ph);
			glVertex3dv( pt );
		}
	glEnd();
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
		const double fingerPh[5] = {10, 75, 90, 105, 120};
		const double baseLen[5]  = {.7,  1,  1,   1,   0.7};

		// Finger parameters
		const double thumbLen[2] = {0.3, 0.2};
		const double thumbAngle[2] = {0, 20};

		const double indexLen[3] = { 0.3, 0.3, 0.2};
		const double indexAngle[3] = {0, 0, 0};

		const double middleLen[3] = { 0.3, 0.3, 0.2};
		const double middleAngle[3] = {0, 0, 0};

		const double ringLen[3] = { 0.3, 0.3, 0.2};
		const double ringAngle[3] = {0, 0, 0};

		const double pinkyLen[3] = {0.2, 0.2, 0.1};
		const double pinkyAngle[3] = {0, 0, 0};

		double mat[16];
		double pt[3];
		double fingerBase[15];

		getMatrix(  dx,dy,dz,
						ux,uy,uz,  mat);
		//  Save transformation
		glPushMatrix();
		//  Offset
		glTranslated( x ,y ,z );
		glScaled( scaleFactor, scaleFactor, scaleFactor);
		glMultMatrixd(mat);

		// Begin drawing  =================================
		glColor3f( 0, 0, 1 );
		glBegin(GL_LINES);
			for (int i=0; i<5; i++){
				pt[0] = -Cos(fingerPh[i]) * baseLen[i];
				pt[1] = Sin(fingerPh[i]) * baseLen[i];
				pt[2] = 0;
				glVertex3d(0,0,0);
				glVertex3dv(pt);
				fingerBase[i*3]   = pt[0];
				fingerBase[i*3+1] = pt[1];
				fingerBase[i*3+2] = pt[2];
			}
		glEnd();

		this->finger(fingerBase[0],fingerBase[1],fingerBase[2],
							fingerPh[0], thumbLen,thumbAngle);
		this->finger(fingerBase[3],fingerBase[4],fingerBase[5],
							fingerPh[1], indexLen,indexAngle);
		this->finger(fingerBase[6],fingerBase[7],fingerBase[8],
							fingerPh[2], middleLen,middleAngle);
		this->finger(fingerBase[9],fingerBase[10],fingerBase[11],
							fingerPh[3], ringLen,ringAngle);
		this->finger(fingerBase[12],fingerBase[13],fingerBase[14],
							fingerPh[4], pinkyLen,pinkyAngle);
		//  Undo transformations
		glPopMatrix();
		ErrCheck("hand");
}





/*
 *  Draw the hand base
 *     at (x,y,z)
 *     neck facing (dx,dy,dz)
 *     up towards (ux,uy,uz)
 */
void hand::base(  double x , double y , double z,
					   double dx, double dy, double dz,
					   double ux, double uy, double uz)
{
		const double scaleFactor     = 0.5;
		const double fingerAngle [5] = {10, 75, 90, 105, 120};
		const double baseLen[5]      = {0.7, 1, 1, 1, 1};

		double mat[16];
		getMatrix(  dx,dy,dz,
						ux,uy,uz,  mat);
		//  Save transformation
		glPushMatrix();
		//  Offset
		glTranslated( x ,y ,z );
		glScaled( scaleFactor, scaleFactor, scaleFactor);
		glMultMatrixd(mat);

		// Begin drawing  =================================

		glBegin(GL_LINES);
			for (int i=0; i<5; i++){
				glVertex3d(0,0,0);
				glVertex3d( -baseLen[i]*Cos(fingerAngle[i]),
								 baseLen[i]*Sin(fingerAngle[i]), 0 );
			}
		glEnd();
		//  Undo transformations
		glPopMatrix();
		ErrCheck("hand base");
}