#include "CSCIx229.h"
#include "Util.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
// #include <tr1/array>

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

const double buffHelp = 0.000000001;

class hand
{
public:
	hand( int E, int A, int D, int G, int B, int e )
	{
		this->drawHand( E, A, D, G, B, e);
	}

	void drawHand( int E, int A, int D, int G, int B, int e);

private:
	void finger(double th, double r_base,
				const double boneLen[], int strings[], vector<int> map);

	vector<int> get_finger_map(int strings[]);

	double get_fret_x(int fret_num);
	double get_finger_x(int fret_num);
	double get_finger_y(int str_num);
	int get_wrist_fretnum( int E, int A, int D, int G, int B, int e);
	double get_wrist_theta( int E, int A, int D, int G, int B, int e);

	void draw_axes( float r, float g, float b);

	// ~hand();

	/* data */
};


