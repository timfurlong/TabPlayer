#include "hand.h"


/*
 *  Draw the hand
 *     at (x,y,z)
 *     fingers facing (dx,dy,dz)
 *     up towards (ux,uy,uz)
 */
void hand::drawHand( int E, int A, int D, int G, int B, int e)
{
	// Base parameters
	const double fingerTh[5] = {10, 75, 90, 105, 120};
	const double baseLen[5]  = {.25,  0.5,  0.5,   0.5,   0.35};

	// Finger parameters
	const double thumbLen[2] = {0.3, 0.2};
	const double indexLen[3] = { 0.3, 0.3, 0.2};
	const double middleLen[3] = { 0.3, 0.3, 0.2};
	const double ringLen[3] = { 0.3, 0.3, 0.2};
	const double pinkyLen[3] = {0.2, 0.2, 0.1};

	int strings[6] = { E, A, D, G, B, e };
	double mat[16];
	double fingerBase[15];
	vector<int> finger_map = get_finger_map( strings );

	int hand_fret = get_wrist_fretnum(E, A, D, G, B, e);
	double theta  = get_wrist_theta(E, A, D, G, B, e);

	getMatrix(  1,0,0,
				0,1,0,  mat);
	//  Save transformation
	glPushMatrix();

	double x = get_finger_x( hand_fret );
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
			Vertex( fingerTh[i], 0, baseLen[i], 0,0,-(neck_r+buffHelp) );
			fingerBase[i*2]   = fingerTh[i];
			fingerBase[i*2+1] = baseLen[i];
		}
	glEnd();

	// Fingers
	glColor3f( 1,0,0 );

	// for (int i=0; i<6; i++){
	// 	finger(fingerBase[0], fingerBase[1], thumbLen, strings);
	// }
	for (int i=0; i<6; i++){
		finger(fingerBase[2], fingerBase[3], indexLen, strings, finger_map);
	}
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
	ErrCheck("hand");
}

void hand::finger(	double th, double r_base,
					const double boneLen[], int strings[], vector<int> map)
{
	// int numBones = sizeof(*boneLen) / sizeof(double);

	vector<double> pt;
	vector<double> pts;
	double ph = 0;
	double r = r_base;
	bool loopDone = false;
	double x, y;

	for (int i=0; i<6; i++){
		if (strings[i] == 1)
		{
			glPushMatrix();
				glBegin( GL_LINES );
					x = get_finger_x(1);
					// y = get_finger_y()
				glEnd();
			glPopMatrix();
			ErrCheck("finger");
		}
	}
}
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

/*
	Returns a vector indexed by string number, valued by which finger holds down which
	string, reguardless of which fret. That information will be paired later.
	-1 is a special value indicating that no finger is holding that string

	TODO: does not work for bar chords. Using abjad for manual finger mapping instead
*/
vector<int> hand::get_finger_map(int strings[])
{
	vector<int> finger_map;
	int numDiffFrets = 0; // Num of different frets that are held down by a finger (nonzero)
	int fingNum;
	vector<int> usedFrets;
	vector<int>::iterator it;

	for (int i=0; i<6; i++){
		if (strings[i] == 0)
			continue;
		if ( contains( usedFrets, strings[i] ) == false )
		{  // fret has not been used yet
			usedFrets.insert( usedFrets.end(), strings[i] );
		}
	}
	sort(usedFrets.begin(), usedFrets.end());
	fingNum = 1; // lowest frets used go to the index finger
	finger_map.assign(6, -1); // initialize all strings to not use a finger
	for (it=usedFrets.begin(); it!=usedFrets.end(); it++){
		for (int i=0; i<6; i++){
			if (*it == strings[i]){
				// This string is being held by this fret
				if (!contains(finger_map, fingNum))
					finger_map[i] = fingNum;
				else if (finger_map[i-1] == fingNum)
					finger_map[i] = fingNum;
				else{
					fingNum++;
					finger_map[i] = fingNum;
				}
			}
		}
		fingNum++;
	}

	// for (it=finger_map.begin(); it!=finger_map.end(); it++){
	// 	printf("%i\n", *it);
	// }
	// printf("\n");

	return finger_map;
}

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