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
	// Get final finger joint positions for current note
	prev_fingers = fingers;
	get_finger_pts( n, fingers[0], wrist_pt );
	get_finger_pts( n, fingers[1], wrist_pt );
	get_finger_pts( n, fingers[2], wrist_pt );
	get_finger_pts( n, fingers[3], wrist_pt );
	get_finger_pts( n, fingers[4], wrist_pt );

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
	vector<double> j, last_j, pt; // Vectors for filling with joint's XYZ
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

	f.relFVerts.clear();
	for (int i=0; i<f.fVerts.size(); i++){
		pt = f.fVerts[i];
		// Get coordinates relative to the wrist
		pt = getTranslateCoord(-(this->wrist[0]),0,0,  pt);
		pt = getRotateCoord(90, 0,0,1,  pt);
		pt = getRotateCoord(-theta, 0,1,0,  pt);
		f.relFVerts.push_back(pt);
	}

	glPointSize(20);
	glColor3f(1,0,0);
	glBegin(GL_POINTS);
	for (int i=0; i<f.fVerts.size(); i++){
		pt = f.fVerts[i];
		glVertex3d(pt[0],pt[1],pt[2]);
	}

	Vertex(1, 1, 1);
	glEnd();

	glPointSize(22);
	glBegin(GL_POINTS);
	glColor3f(0,0,1);
	// printf("%f, %f, %f\n",pt[0],pt[1],pt[2] );
	pt = GetVertex(1,1,1);
	pt = GetPolarCoord(pt[0],pt[1],pt[2]);
	printf("%f, %f, %f\n\n",pt[0],pt[1],pt[2] );
	Vertex(pt[0],pt[1],pt[2]);

	glEnd();

	glPushMatrix();
		glColor3f(0,1,0);
		glTranslated(this->wrist[0],0,0);
		glRotated(-90, 0,0,1);
		glRotated(theta, 0,1,0);
		glBegin(GL_POINTS);
		for (int i=0; i<f.relFVerts.size(); i++){
			pt = f.relFVerts[i];
			glVertex3d(pt[0],pt[1],pt[2]);
		}
		glEnd();
	glPopMatrix();
}


/*
 *  Draw the hand
 *     at (x,y,z)
 *     fingers facing (dx,dy,dz)
 *     up towards (ux,uy,uz)
 */
void hand::drawHand( note n, vector<finger>& prev_fingers )
{

	vector<double> pt, j, next_pt, last_pt;
	vector<finger>::iterator f_it;
	vector< vector<double> >::iterator j_it;
	finger f = fingers[0]; //dummy assignment to avoid using a constructor

	// int t = ((int)(t_elapsed*speedScale)%n.duration)/n.duration;
	// Get current position of all fingers at this time
	// vector<finger> currentFingers;

	finger cur_f = fingers[0];
	pt.assign(3,0);
	// printf("%s\n", "before");
	for (int i=0; i<fingers.size(); i++){
		f  = fingers[i];
		vector< vector<double> > prev_verts = prev_fingers[i].fVerts;
		cur_f = f;
		for (int j=0; j<f.fVerts.size(); j++){
			next_pt = f.fVerts[j];
			printf("%f\n", prev_verts[i][j]);
			// last_pt = prev_verts[j];
	// 		last_pt = prev_verts[j];
	// 		// printf("%f\n", prev_f.fVerts[0][0]);

	// 		// pt[0] = next_pt[0]*t + (1-t)*last_pt[0];
	// 		// pt[1] = next_pt[1]*t + (1-t)*last_pt[1];
	// 		// pt[2] = next_pt[2]*t + (1-t)*last_pt[2];
	// 		// cur_f.fVerts[j] = pt;
	// 		pt.assign(3,0);
		}
	// 	// currentFingers.push_back(cur_f);
	}

	printf("%s\n\n", "after");
	// Begin drawing  =================================

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

	// DRAW JOINTS
	for (int ii=0; ii<fingers.size(); ii++){
		fingers[ii];
		for (int jj=0; jj<f.joints.size(); jj++){
			j = f.joints[jj];
			ball(j[0],j[1],j[2],
					jRGB[0],jRGB[1],jRGB[2],
					jointRadius);
		}
		ball(f.tip[0],f.tip[1],f.tip[2],
				hRGB[0],hRGB[1],hRGB[2],
				jointRadius);
	}

	// currentFingers.clear();
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

void hand::drawLineHandBase( )
{
	// Hand Base
	glColor3f( 0, 0, 1 );
	glBegin(GL_LINES);
		for (int i=0; i<5; i++){
			glVertex3d(this->wrist[0],this->wrist[1],this->wrist[2]);
			glVertex3d(this->fingBases[i][0],this->fingBases[i][1],this->fingBases[i][2]);
		}
	glEnd();
}