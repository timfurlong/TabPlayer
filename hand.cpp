#include "hand.h"


hand::hand( ){

}

void hand::setHand( note n, note prev_n, double t )
{
	this->wrist.clear();
	this->fingBases.clear();
	this->baseVerts.clear();

	vector<double> pt;
	double x = get_wrist_x(n);
	theta = get_wrist_theta(n);

	// Give each finger instance the location of the cooresponding hand base
	for (int i=0; i<5; i++){
		pt = GetVertex(fingerTh[i], 0, baseLen[i],
							0,0,-(neck_r+buffHelp+baseH) );
		if (i==4)
			baseVerts.insert(baseVerts.begin(), pt);
		else
			this->baseVerts.push_back(pt);

		pt = getRotateCoord(theta, 0,1,0, pt);
		pt = getRotateCoord(-90, 0,0,1, pt);
		pt = getTranslateCoord(x,0,0, pt);
		fingers[i].base = pt;
		this->fingBases.push_back( pt );
	}
	vector<double> wrist_pt; wrist_pt.assign(3, 0);
	wrist_pt[0]=0; wrist_pt[1]=0; wrist_pt[2]=-(neck_r+buffHelp+baseH);
	this->baseVerts.insert(this->baseVerts.begin(), wrist_pt);
	wrist_pt = getRotateCoord(theta, 0,1,0, wrist_pt);
	wrist_pt = getRotateCoord(-90, 0,0,1, wrist_pt);
	wrist_pt = getTranslateCoord(x,0,0, wrist_pt);
	this->wrist = wrist_pt;
	vector<finger> prev_fingers = fingers;
	// Get final finger joint positions for current note
	get_finger_pts( n, fingers[0], wrist_pt );
	get_finger_pts( n, fingers[1], wrist_pt );
	get_finger_pts( n, fingers[2], wrist_pt );
	get_finger_pts( n, fingers[3], wrist_pt );
	get_finger_pts( n, fingers[4], wrist_pt );
	// Get finger joint positions for past note
	get_finger_pts( prev_n, prev_fingers[0], wrist_pt );
	get_finger_pts( prev_n, prev_fingers[1], wrist_pt );
	get_finger_pts( prev_n, prev_fingers[2], wrist_pt );
	get_finger_pts( prev_n, prev_fingers[3], wrist_pt );
	get_finger_pts( prev_n, prev_fingers[4], wrist_pt );

	// get max and min FingBaseLen
	double maxLen = 0;
	double minLen = 0;
	for (int i=0; i<5; i++){
		if (baseLen[i] > maxLen)
			maxLen = baseLen[i];
		if (baseLen[i] < minLen || minLen==0)
			minLen = baseLen[i];
	}
	this->maxBaseLen = maxLen;
	this->minBaseLen = minLen;

	// Timing parameters
	this->t_elapsed     = t;
	this->drawHand( n, prev_fingers );
}


void hand::get_finger_pts( note n, finger& f, vector<double> wrist_pt )
{
	const double fret_dist = 0.3;
	double x, y, z, th;
	vector<double> j, last_j; // Vectors for filling with joint's XYZ
	j.assign(3, 0); // Initialize blank
	if (f.fingNum == n.fingering && n.fret != 0)
	{
		double fret_diff;
		if (n.fret == 1)
			fret_diff = fret_position[0];
		else
			fret_diff = fret_position[n.fret-1]-fret_position[n.fret-2];
		// lowest n.string is 1, not 0. This is just MusicXML standard
		x = fret_position[ n.fret-1 ]- (fret_diff * fret_dist);
		x *= neck_length;
		x -= neck_length/2;
		y = str_y[ n.string - 1];
		z = fretboard_thickness + str_fret_dist;
		vectorAssignPt(f.tip, x, y, z);

		j[0] = x; // TODO: angle this towards the wrist
		j[1] = y - (f.boneLen.back()/sqrt(2));
		j[2] = (f.boneLen.back()/sqrt(2)) + z;
		f.joints.insert( f.joints.begin(), j );
		last_j = j;

		if (f.numJoints < 2)
			return;
		th = degrees( atan( (last_j[1]-f.base[1]) / (last_j[0]-f.base[0]) ) );
		j[0] = last_j[0] - f.boneLen[1] * Cos(th);
		j[1] = last_j[1] - f.boneLen[1] * Sin(th);
		j[2] = last_j[2];
		f.joints.insert( f.joints.begin(), j );


	}
	else
	{
		double dx, dy, dz; // Vector from wrist to base
		double fingLen; // Total length of finger
		vector<double> currentJoint;
		fingLen = f.boneLen[0] + f.boneLen[1] + f.boneLen[2];

		dx = f.base[0] - wrist_pt[0];
		dy = f.base[1] - wrist_pt[1];
		dz = f.base[2] - wrist_pt[2];

		x = dx * fingLen/baseLen[0] + f.base[0];
		y = dy * fingLen/baseLen[1] + f.base[1];
		z = dz * fingLen/baseLen[2] + f.base[2];
		vectorAssignPt(f.tip, x, y, z);

		currentJoint = f.base;
		vector< vector<double> > joints;
		for (int i=0; i<f.numJoints; i++){
			j[0] = dx * f.boneLen[i]/baseLen[0] + currentJoint[0];
			j[1] = dy * f.boneLen[i]/baseLen[1] + currentJoint[1];
			j[2] = dz * f.boneLen[i]/baseLen[2] + currentJoint[2];
			joints.push_back( j );
			currentJoint = j;
		}
		f.joints = joints;
	}

	f.fVerts.clear();
	f.fVerts.push_back(f.base);
	for (int i=0; i<f.joints.size(); i++){
		f.fVerts.push_back(f.joints[i]);
	}
	f.fVerts.push_back(f.tip);
}


/*
 *  Draw the hand
 *     at (x,y,z)
 *     fingers facing (dx,dy,dz)
 *     up towards (ux,uy,uz)
 */
void hand::drawHand( note n, vector<finger> prev_fingers )
{

	vector<double> pt, j, next_pt;
	vector<finger>::iterator f_it;
	vector< vector<double> >::iterator j_it;
	finger f = fingers[0]; //dummy assignment to avoid using a constructor
	// Begin drawing  =================================

	// Hand Base
	glColor3f( 0, 0, 1 );
	// glBegin(GL_LINES);
	// 	for (int i=0; i<5; i++){
	// 		glVertex3d(this->wrist[0],this->wrist[1],this->wrist[2]);
	// 		glVertex3d(this->fingBases[i][0],this->fingBases[i][1],this->fingBases[i][2]);
	// 	}
	// glEnd();

	// DRAW WRIST (aka base)
	glPushMatrix();
		vector<double> wrist_pt; wrist_pt.assign(3, 0);
		wrist_pt[0]=0; wrist_pt[1]=0; wrist_pt[2]=-(neck_r+buffHelp+baseH);
		glTranslated(this->wrist[0],0,0);
		glRotated(-90, 0,0,1);
		glRotated(theta, 0,1,0);
		draw_axes(1,1,1);
		glColor3ub(hRGB[0],hRGB[1],hRGB[2]);

		glTranslated(0,0,baseH);
		glBegin( GL_POLYGON );
			glVertex3d(wrist_pt[0],wrist_pt[1],wrist_pt[2]);
			Vertex(  fingerTh[4]-thHelp, 0, baseLen[4],
						0,0,-(neck_r+buffHelp+baseH) );
			for (int i=0; i<4; i++){
				Vertex(  fingerTh[i], 0, baseLen[i],
							0,0,-(neck_r+buffHelp+baseH) );
			}
		glEnd();
		glTranslated(0,0,-2*baseH);
		glBegin( GL_POLYGON );
			glVertex3d(wrist_pt[0],wrist_pt[1],wrist_pt[2]);
			Vertex(  fingerTh[4]-thHelp, 0, baseLen[4],
						0,0,-(neck_r+buffHelp+baseH) );
			for (int i=0; i<4; i++){
				Vertex(  fingerTh[i], 0, baseLen[i],
							0,0,-(neck_r+buffHelp+baseH) );
			}
		glEnd();

		glBegin( GL_QUAD_STRIP );
			for (int i=0; i<baseVerts.size(); i++){
				pt = baseVerts[i];
				glVertex3d( pt[0],pt[1],pt[2]);
				glVertex3d( pt[0],pt[1],pt[2]+2*baseH);
			}
			pt = baseVerts[0];
			glVertex3d( pt[0],pt[1],pt[2]);
			glVertex3d( pt[0],pt[1],pt[2]+2*baseH);
		glEnd();
	glPopMatrix();

	// DRAW FINGERS
	glColor3ub( hRGB[0], hRGB[1], hRGB[2] );
	for (int i=0; i<fingers.size(); i++){
		f = fingers[i];
		for (int j=0; j<f.fVerts.size()-1; j++){
			pt      = f.fVerts[j];
			next_pt = f.fVerts[j+1];
			renderCylinder_convenient( pt[0],pt[1],pt[2],
												next_pt[0],next_pt[1],next_pt[2],
												fingRadius, fingSubDiv);
		}
	}
	// 	f = fingers[ii];
	// 	j = f.joints.front();
	// 	renderCylinder_convenient( f.base[0],f.base[1],f.base[2],
	// 										j[0],j[1],j[2],
	// 										fingRadius, fingSubDiv);
	// 	for (int jj=1; jj<f.joints.size(); jj++){
	// 		j = f.joints[jj];
	// 		last_j = f.joints[jj-1];
	// 		renderCylinder_convenient( last_j[0], last_j[1], last_j[2],
	// 											j[0], j[1], j[2],
	// 											fingRadius, fingSubDiv);
	// 	}
	// 	renderCylinder_convenient( j[0], j[1], j[2],
	// 										f.tip[0], f.tip[1], f.tip[2],
	// 										fingRadius, fingSubDiv);
	// }

	// DRAW JOINTS
	glColor3f( 0,1,0 );
	glPointSize(5);
	// for(f_it = fingers.begin(); f_it != fingers.end(); f_it++){
	for (int ii=0; ii<fingers.size(); ii++){
		// f = *f_it;
		fingers[ii];
		// for (j_it=f.joints.begin(); j_it<f.joints.end(); j_it++){
		for (int jj=0; jj<f.joints.size(); jj++){
			// j = *j_it;
			j = f.joints[jj];
			ball(j[0],j[1],j[2],
					jRGB[0],jRGB[1],jRGB[2],
					jointRadius);
		}
		ball(f.tip[0],f.tip[1],f.tip[2],
				hRGB[0],hRGB[1],hRGB[2],
				jointRadius);
	}
	ErrCheck("drawHand");
}

double hand::get_fret_x(int fret_num)
{
	if (fret_num == 0)
		return -neck_length/2;
	// return (fret_position[num_frets - fret_num] * neck_length)
	// 				- (neck_length/2);
	return (fret_position[fret_num-1] * neck_length)
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
	x -= Cos(fingerTh[n.fingering-1]) * baseLen[n.fingering-1];
	return x;
}

double hand::get_wrist_theta( note n )
{
	const double max_theta = -30;
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