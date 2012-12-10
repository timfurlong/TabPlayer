#include "CSCIx229.h"
#include "Util.h"
#include "note.h"
#include "shape.h"

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
extern const double str_fret_dist;

const double buffHelp = 0.000000001;
// Base parameters
const double fingerTh[5] = {74.97, 87.12, 98.83, 115.30, 52.33};
const double baseLen[5]  = {1.17, 1.11, 1.02, 0.97, 0.98};
const double fingRadius = 0.03;

#define PI 3.1415927

class finger
{
public:
	finger(const double lens[], int numBones, int fingNum)
	{
		this->fingNum   = fingNum;
		this->numBones  = numBones;
		this->numJoints = numBones - 1;
		for (int i=0; i<numBones; i++){
			boneLen.push_back( lens[i] );
		}
	}
	vector<double> tip;
	vector<double> base;

	// Joints
	vector< vector<double> > joints;
	int numJoints;

	vector<double> boneLen;
	int numBones;
	int fingNum;
};
extern vector< finger > fingers;

class hand
{
public:
	hand( note );

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

	void get_finger_pts( note n, finger& f, vector<double> wrist_pt );

	void draw_axes( float r, float g, float b);

	// ~hand();

	/* data */
	double theta;
	vector<double> wrist;
	vector< vector<double> > fingBases;
};

