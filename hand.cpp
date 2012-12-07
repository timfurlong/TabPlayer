#include "hand.h"


/*
 *  Draw the hand
 *     at (x,y,z)
 *     fingers facing (dx,dy,dz)
 *     up towards (ux,uy,uz)
 */
void hand::drawHand( note n )
{
	// Base parameters
	const double fingerTh[5] = {10, 75, 90, 105, 120};
	const double baseLen[5]  = {.25,  0.5,  0.5,   0.5,   0.35};

	double mat[16];
	double fingerBase[15];

	// int hand_fret = get_wrist_fretnum(E, A, D, G, B, e);
	double theta  = get_wrist_theta(n);

	getMatrix(  1,0,0,
					0,1,0,  mat);
	//  Save transformation
	glPushMatrix();

	vector<double> tip = get_finger_tip( n );
	glBegin(GL_LINES);
		glVertex3d(tip[0],tip[1],tip[2]);

	glEnd();

	double x = get_finger_x( n.fret );
	//  Offset
	glTranslated( x, 0, 0 );
	glRotated(-90, 0,0,1);
	glRotated(theta, 0,1,0);
	glMultMatrixd(mat);
	draw_axes(0,1,0);
	// Begin drawing  =================================

	// Hand Base
	glColor3f( 0, 0, 1 );
	glBegin(GL_LINES);
		for (int i=0; i<5; i++){
			glVertex3d(0,0,-(neck_r+buffHelp));
			Vertex( fingerTh[i], 0, baseLen[i],
						0,0,-(neck_r+buffHelp) );
			fingerBase[i*2]   = fingerTh[i];
			fingerBase[i*2+1] = baseLen[i];
		}
	glEnd();

	// Fingers
	glColor3f( 1,0,0 );
	// for (int i=0; i<6; i++){
	// 	finger(fingerBase[0], fingerBase[1], thumbLen, strings);
	// }
	// for (int i=0; i<6; i++){
	// 	finger(fingerBase[2], fingerBase[3], indexLen, n);
	// }
	// for (int i=0; i<6; i++){
	// 	finger(fingerBase[4], fingerBase[5], middleLen, strings);
	// }
	// for (int i=0; i<6; i++){
	// 	finger(fingerBase[6], fingerBase[7], ringLen, strings);
	// }
	// for (int i=0; i<6; i++){
	// 	finger(fingerBase[8], fingerBase[9], pinkyLen, strings);
	// }

	//  Undo transformations
	glPopMatrix();
	// for (int i=0; i<6; i++){
	// 	finger(fingerBase[0], fingerBase[1], thumbLen, strings);
	// }
	ErrCheck("hand");
}

vector<double> hand::get_finger_tip( note n )
{
	const double fret_dist = 0.3;
	double x, y;
	vector<double> xyz;
	// lowest n.string is 1, not 0. This is just MusicXML standard
	y = str_y[ n.string - 1];

	double fret_diff = fret_position[num_frets-n.fret]-fret_position[num_frets-n.fret+1];
	x = fret_position[ num_frets - n.fret ]- (fret_diff * fret_dist);
	x *= neck_length;
	x -= neck_length/2;

	xyz.push_back(x);
	xyz.push_back(y);
	xyz.push_back( fretboard_thickness );
	return xyz;
}
// void hand::finger(double th, double r_base,
// 				const double boneLen[], note n)
// {
// 	// int numBones = sizeof(*boneLen) / sizeof(double);

// 	vector<double> pt;
// 	vector<double> pts;
// 	double ph = 0;
// 	double r = r_base;
// 	double x, y;

// 	for (int i=0; i<6; i++){
// 		if (strings[i] == 1)
// 		{
// 			glPushMatrix();
// 				glBegin( GL_LINES );
// 					x = get_finger_x(1);
// 					// y = get_finger_y()
// 				glEnd();
// 			glPopMatrix();
// 			ErrCheck("finger");
// 		}
// 	}
// }
// glPushMatrix();
// 	pt = GetVertex(th, ph, r,
// 						0,0,-neck_r);
// 	for (int i=0; i<3; i++)
// 	{
// 		glTranslated( pt[0], pt[1], pt[2] );
// 		ph += boneAngle[i];
// 		pt = GetVertex(th, ph, boneLen[i]);
// 		glBegin(GL_LINES);
// 			glVertex3d( 0,0,0 );
// 			glVertex3d( pt[0], pt[1], pt[2] );
// 		glEnd();
// 	}
// glPopMatrix();

double hand::get_fret_x(int fret_num)
{
	if (fret_num == 0)
		return -neck_length/2;
	return (fret_position[num_frets - fret_num] * neck_length)
					- (neck_length/2);

}

double hand::get_finger_y(int str_num)
{
	// NOTE: str_num must be passed in from 0-5, where 0 is E and 5 is e.
	return neck_r - ((2/5)*neck_r)*str_num;
}

double hand::get_finger_x( int fret_num )
{
	double finger_x;
	if (fret_num == 0)
		finger_x = 0;
	if (fret_num == 1)
		finger_x = ((fret_position[num_frets-1]/2) * neck_length);

	finger_x = (get_fret_x(fret_num)+get_fret_x(fret_num-1)) / 2;

	return finger_x;
}

int hand::get_wrist_fretnum( int E, int A, int D, int G, int B, int e)
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

double hand::get_wrist_theta( note n )
{
	const double max_theta = -40;
	if (n.string < 4)
		return 0;
	return (max_theta - (max_theta/3)*(6-n.string));
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