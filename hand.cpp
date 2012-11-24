#include "hand.h"


/*
 *  Draw the hand
 *     at (x,y,z)
 *     fingers facing (dx,dy,dz)
 *     up towards (ux,uy,uz)
 */
void hand::drawHand( int E, int A, int D, int G, int B, int e)
{
	const double scaleFactor     = 1;

	// Base parameters
	const double fingerTh[5] = {10, 75, 90, 105, 120};
	const double baseLen[5]  = {.25,  0.5,  0.5,   0.5,   0.35};

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
	double fingerBase[15];

	int hand_fret = get_hand_fret(E, A, D, G, B, e);
	double theta  = get_wrist_theta(E, A, D, G, B, e);

	getMatrix(  1,0,0,
					0,1,0,  mat);
	//  Save transformation
	glPushMatrix();

	double x = get_finger_pos( hand_fret );
	//  Offset
	glTranslated( x, 0, 0 );
	glRotated(-90, 0,0,1);
	glRotated(theta, 0,1,0);
	glScaled( scaleFactor, scaleFactor, scaleFactor);
	glMultMatrixd(mat);
	draw_axes(0,1,0);
	// Begin drawing  =================================

	// Hand Base
	glColor3f( 0, 0, 1 );
	glBegin(GL_LINES);
		for (int i=0; i<5; i++){
			glVertex3d(0,0,-(neck_r+buffHelp));
			Vertex( fingerTh[i], 0, baseLen[i], 0,0,-(neck_r+buffHelp) );
			fingerBase[i*2]   = fingerTh[i];
			fingerBase[i*2+1] = baseLen[i];
		}
	glEnd();

	// Fingers
	glColor3f( 1,0,0 );
	finger(fingerBase[0], fingerBase[1], thumbLen, thumbAngle);
	finger(fingerBase[2], fingerBase[3], indexLen, indexAngle);
	finger(fingerBase[4], fingerBase[5], middleLen, middleAngle);
	finger(fingerBase[6], fingerBase[7], ringLen, ringAngle);
	finger(fingerBase[8], fingerBase[9], pinkyLen, pinkyAngle);

	//  Undo transformations
	glPopMatrix();
	ErrCheck("hand");
}

void hand::finger(double th, double r_base,
						const double boneLen[],
						const double boneAngle[])
{
	// int numBones = sizeof(boneLen) / sizeof(double);
	// printf("numBones = %i\n", numBones);
	// CHANGE
	vector<double> pt;
	vector<double> pts;
	double ph = 0;
	double r = r_base;

	glPushMatrix();
		pt = GetVertex(th, ph, r,
							0,0,-neck_r);
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


double hand::get_fret_pos(int fret_num)
{
	if (fret_num == 0)
		return -neck_length/2;
	return (fret_position[num_frets - fret_num] * neck_length)
					- (neck_length/2);

}

double hand::get_finger_pos( int fret_num )
{
	double finger_pos;
	if (fret_num == 0)
		finger_pos = 0;
	if (fret_num == 1)
		finger_pos = ((fret_position[num_frets-1]/2) * neck_length);

	finger_pos = (get_fret_pos(fret_num)+get_fret_pos(fret_num-1)) / 2;

	return finger_pos;
}

int hand::get_hand_fret( int E, int A, int D, int G, int B, int e)
{
	int strings[6] = {E, A, D, G, B, e};
	int sum        = 0;
	int num_nonzeros  = 0;

	for (int i=0; i<6; i++){
		sum += strings[i];
		if (strings[i] != 0)
			num_nonzeros++;
	}

	if (num_nonzeros == 0)
		return 0;

	return floor( sum/num_nonzeros );
}

double hand::get_wrist_theta( int E, int A, int D, int G, int B, int e)
{
	const double max_theta = -40;
	int strings[6] = {e, B, G, D, A, E};
	double highest_str = 0;

	for (int str_num=1; str_num <= 6; str_num++)
	{
		if (strings[str_num-1] > 0)
		{
			if (str_num > highest_str)
				highest_str = str_num;
		}
	}
	if (highest_str <= 3)
		return 0;
	return ( (highest_str-3)/3 ) * max_theta;
}


void hand::draw_axes( float r, float g, float b)
{
	const double len=1;  //  Length of axes
	//  Draw axes - no lighting from here on
	glColor3f(r,g,b);



	if (axes)
	{
		glDisable(GL_LIGHTING);

		glBegin(GL_LINES);
			glVertex3f(0.0,0.0,0.0);
			glVertex3f(len,0.0,0.0);
			glVertex3f(0.0,0.0,0.0);
			glVertex3f(0.0,len,0.0);
			glVertex3f(0.0,0.0,0.0);
			glVertex3f(0.0,0.0,len);
		glEnd();
		//  Label axes
		glColor3f(1,1,1);
		glRasterPos3d(len,0.0,0.0);
		Print("X");
		glRasterPos3d(0.0,len,0.0);
		Print("Y");
		glRasterPos3d(0.0,0.0,len);
		Print("Z");
	}

	glEnable(GL_LIGHTING);
}