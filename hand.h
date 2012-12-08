#include "CSCIx229.h"
#include "Util.h"
#include "note.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <set>

using std::cerr;
using std::cout;
using std::endl;
using std::vector;
using std::set;
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

extern const double fret_position[];
extern const int num_frets;
extern const double neck_length;
extern const double neck_r;
extern const double fretboard_thickness;
extern int axes;
extern queue<note> notes;
extern double str_y[6];


const double buffHelp = 0.000000001;
// Base parameters
const double fingerTh[5] = { 75, 90, 105, 120, 10};
const double baseLen[5]  = { 0.5,  0.5, 0.5,   0.35, .25};
#define PI 3.1415927


class finger
{
public:
	finger(const double lens[3], int num)
	{
		jointLen[0] = lens[0];
		jointLen[1] = lens[1];
		jointLen[2] = lens[2];
		fingNum     = num;
	}

	vector<double> tip;
	vector<double> base;

	// Joints
	double j1[3];
	double j2[3];
	double jointLen[3];
	int fingNum;
};
extern vector< finger > fingers;

class hand
{
public:
	hand( note n )
	{
		this->drawHand( n );
	}

	void drawHand( note n );

private:
	// void finger(double th, double r_base,
	// 			const double boneLen[], note n);

	vector<int> get_finger_map(int strings[]);

	double get_fret_x(int fret_num);
	double get_finger_x(int fret_num);
	double get_finger_y(int str_num);
	double get_wrist_x( note n );
	double get_wrist_theta( note n );

	vector<double> get_finger_tip( note n, finger f, vector<double> wrist_pt );

	void draw_axes( float r, float g, float b);

	// ~hand();

	/* data */
	double theta;
};

