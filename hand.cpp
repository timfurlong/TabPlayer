#include "hand.h"


/*
 *  Draw the hand
 *     at (x,y,z)
 *     fingers facing (dx,dy,dz)
 *     up towards (ux,uy,uz)
 */
void hand::drawHand( note n )
{

	double mat[16];
	double fingerBase[15];
	vector<double> pt;

	theta = get_wrist_theta(n);
	double x     = get_wrist_x(n);
	getMatrix(  1,0,0,
					0,1,0,  mat);
	//  Save transformation
	glPushMatrix();

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
	// 	finger(fingerBase[2], fingerBase[3], indexLen, n);
	// }

	//  Undo transformations
	glPopMatrix();

	// Give each finger instance the location of the cooresponding hand base
	for (int i=0; i<5; i++){
		pt = GetVertex(fingerTh[i], 0, baseLen[i],
							0,0,-(neck_r+buffHelp) );
		pt = getRotateCoord(theta, 0,1,0, pt);
		pt = getRotateCoord(-90, 0,0,1, pt);
		pt = getTranslateCoord(x,0,0, pt);
		fingers[i].base = pt;
	}
	vector<double> wrist_pt; wrist_pt.assign(3, 0);
	wrist_pt[0]=0; wrist_pt[1]=0; wrist_pt[2]=-(neck_r+buffHelp);
	wrist_pt = getRotateCoord(theta, 0,1,0, wrist_pt);
	wrist_pt = getRotateCoord(-90, 0,0,1, wrist_pt);
	wrist_pt = getTranslateCoord(x,0,0, wrist_pt);
	fingers[0].tip = get_finger_tip( n, fingers[0], wrist_pt);
	fingers[1].tip = get_finger_tip( n, fingers[1], wrist_pt);
	fingers[2].tip = get_finger_tip( n, fingers[2], wrist_pt);
	fingers[3].tip = get_finger_tip( n, fingers[3], wrist_pt);
	fingers[4].tip = get_finger_tip( n, fingers[4], wrist_pt);
	glColor3f(1,0,0);
	glBegin(GL_LINES);
		finger f = fingers[0];
		glVertex3d(f.base[0],f.base[1],f.base[2]);
		glVertex3d(f.tip[0],f.tip[1],f.tip[2]);
		f = fingers[1];
		glVertex3d(f.base[0],f.base[1],f.base[2]);
		glVertex3d(f.tip[0],f.tip[1],f.tip[2]);
		f = fingers[2];
		glVertex3d(f.base[0],f.base[1],f.base[2]);
		glVertex3d(f.tip[0],f.tip[1],f.tip[2]);
		f = fingers[3];
		glVertex3d(f.base[0],f.base[1],f.base[2]);
		glVertex3d(f.tip[0],f.tip[1],f.tip[2]);
		f = fingers[4];
		glVertex3d(f.base[0],f.base[1],f.base[2]);
		glVertex3d(f.tip[0],f.tip[1],f.tip[2]);
	glEnd();
	// Get the cooresponding place to put the tip of each finger

	// for (int i=0; i<6; i++){
	// 	finger(fingerBase[0], fingerBase[1], thumbLen, strings);
	// }
	ErrCheck("hand");
}


vector<double> hand::get_finger_tip( note n, finger f, vector<double> wrist_pt )
{
	const double fret_dist = 0.3;
	double x, y;
	vector<double> xyz;
	if (f.fingNum == n.fingering)
	{
		// lowest n.string is 1, not 0. This is just MusicXML standard
		y = str_y[ n.string - 1];

		double fret_diff = fret_position[num_frets-n.fret]-fret_position[num_frets-n.fret+1];
		x = fret_position[ num_frets - n.fret ]- (fret_diff * fret_dist);
		x *= neck_length;
		x -= neck_length/2;

		xyz.push_back(x);
		xyz.push_back(y);
		xyz.push_back( fretboard_thickness );
	}
	else
	{
		double dx, dy, dz; // Vector from wrist to base
		double fingLen; // Total length of finger
		fingLen = f.jointLen[0] + f.jointLen[1] + f.jointLen[2];

		dx = f.base[0] - wrist_pt[0];
		dy = f.base[1] - wrist_pt[1];
		dz = f.base[2] - wrist_pt[2];
		dx *= fingLen/baseLen[0];
		dy *= fingLen/baseLen[1];
		dz *= fingLen/baseLen[2];
		dx += f.base[0];
		dy += f.base[1];
		dz += f.base[2];
		xyz.push_back( dx );
		xyz.push_back( dy );
		xyz.push_back( dz );
	}
	// glPointSize(20);
	// glBegin(GL_POINTS);
	// 	glVertex3d(wrist_pt[0],wrist_pt[1],wrist_pt[2]);
	// glEnd();
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

double hand::get_wrist_x( note n )
{
	double x = get_fret_x( n.fret );
	x -= Cos(fingerTh[n.fingering]) * baseLen[n.fingering];
	return x;
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