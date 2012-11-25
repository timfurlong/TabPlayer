#include "shape.h"

void renderCylinder( double x1, double y1, double z1,
							double x2,double y2, double z2,
							double radius,int subdivisions,GLUquadricObj *quadric)
{
	double vx = x2-x1;
	double vy = y2-y1;
	double vz = z2-z1;

	//handle the degenerate case of z1 == z2 with an approximation
	if(vz == 0)
	   vz = .0001;

	double v = sqrt( vx*vx + vy*vy + vz*vz );
	double ax = 57.2957795*acos( vz/v );
	if ( vz < 0.0 )
	   ax = -ax;
	double rx = -vy*vz;
	double ry = vx*vz;
	glPushMatrix();

	//draw the cylinder body
	glTranslatef( x1,y1,z1 );
	glRotatef(ax, rx, ry, 0.0);
	gluQuadricOrientation(quadric,GLU_OUTSIDE);
	gluCylinder(quadric, radius, radius, v, subdivisions, 1);

	//draw the first cap
	gluQuadricOrientation(quadric,GLU_INSIDE);
	gluDisk( quadric, 0.0, radius, subdivisions, 1);
	glTranslatef( 0,0,v );

	//draw the second cap
	gluQuadricOrientation(quadric,GLU_OUTSIDE);
	gluDisk( quadric, 0.0, radius, subdivisions, 1);
	glPopMatrix();
}

void renderCylinder_convenient(  double x1, double y1, double z1,
											double x2,double y2, double z2,
											double radius,int subdivisions)
{
	//the same quadric can be re-used for drawing many cylinders
	GLUquadricObj *quadric=gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	renderCylinder(x1,y1,z1,x2,y2,z2,radius,subdivisions,quadric);
	gluDeleteQuadric(quadric);
}

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
void cube( double x,double y,double z,
           double dx,double dy,double dz,
           double th)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,tMode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[0]);
   //  Front
   glColor3f(1,0,0);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[1]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
   glEnd();
   //  Back
   glColor3f(0,0,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[1]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   glColor3f(1,1,0);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[1]);
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   glColor3f(0,1,0);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[1]);
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   glColor3f(0,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[1]);
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   glColor3f(1,0,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[1]);
   glBegin(GL_QUADS);
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
   glEnd();
   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}
/*
* Draws a 3D box.
*	Four top verticies defined in t[]
*  Four bottom verticies defined in b[]
*
*  The four verticies are ordered: top left, top right, bottom right, bottom left
*/
void box(double t[12],double b[12]){
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,tMode?GL_REPLACE:GL_MODULATE);
	glColor3f(1,1,1);
	// glBindTexture(GL_TEXTURE_2D,texture[1]);
	glBegin(GL_QUADS);
	setNormal(  t[0],t[1],t[2],
							t[3],t[4],t[5],
							t[6],t[7],t[8],  normal);
	glNormal3f(normal[0],normal[1],normal[2]);
	glTexCoord2f(0,0); glVertex3f(t[0],t[1],t[2]);
	glTexCoord2f(1,0); glVertex3f(t[3],t[4],t[5]);
	glTexCoord2f(1,1); glVertex3f(t[6],t[7],t[8]);
	glTexCoord2f(0,1); glVertex3f(t[9],t[10],t[11]);

	glBegin(GL_QUADS);
	setNormal(  b[0],b[1],b[2],
							b[3],b[4],b[5],
							b[6],b[7],b[8],normal);
	glNormal3f(normal[0],normal[1],normal[2]);
	glTexCoord2f(0,0); glVertex3f(b[0],b[1],b[2]);
	glTexCoord2f(1,0); glVertex3f(b[3],b[4],b[5]);
	glTexCoord2f(1,1); glVertex3f(b[6],b[7],b[8]);
	glTexCoord2f(0,1); glVertex3f(b[9],b[10],b[11]);
	glEnd();

	// back
	glBegin(GL_QUADS);
	setNormal(  t[0],t[1],t[2],
							b[0],b[1],b[2],
							b[3],b[4],b[5], normal);
	glNormal3f(normal[0],normal[1],normal[2]);
	glTexCoord2f(0,0); glVertex3f(t[0],t[1],t[2]);
	glTexCoord2f(1,0); glVertex3f(b[0],b[1],b[2]);
	glTexCoord2f(1,1); glVertex3f(b[3],b[4],b[5]);
	glTexCoord2f(0,1); glVertex3f(t[3],t[4],t[5]);
	glEnd();

	// front
	glBegin(GL_QUADS);
	setNormal(  t[6],t[7], t[8],
							b[6],b[7], b[8],
							b[9],b[10],b[11], normal);
	glNormal3f(normal[0],normal[1],normal[2]);
	glTexCoord2f(0,0); glVertex3f(t[6],t[7], t[8] );
	glTexCoord2f(1,0); glVertex3f(b[6],b[7], b[8] );
	glTexCoord2f(1,1); glVertex3f(b[9],b[10],b[11]);
	glTexCoord2f(0,1); glVertex3f(t[9],t[10],t[11]);
	glEnd();

	glBegin(GL_QUADS);
	setNormal(  b[9],b[10],b[11],
							t[9],t[10],t[11],
							t[0],t[1],t[2],
							normal);
	glNormal3f(normal[0],normal[1],normal[2]);
	glTexCoord2f(0,0); glVertex3f(t[0],t[1],t[2]);
	glTexCoord2f(1,0); glVertex3f(t[9],t[10],t[11]);
	glTexCoord2f(1,1); glVertex3f(b[0],b[1],b[2]);
	glEnd();
	glTexCoord2f(0,1); glVertex3f(b[9],b[10],b[11]);

	glBegin(GL_QUADS);
	setNormal(  t[3],t[4],t[5],
							t[6],t[7],t[8],
							b[6],b[7],b[8], normal);
	glNormal3f(normal[0],normal[1],normal[2]);
	glTexCoord2f(0,0); glVertex3f(t[3],t[4],t[5]);
	glTexCoord2f(1,0); glVertex3f(t[6],t[7],t[8]);
	glTexCoord2f(1,1); glVertex3f(b[6],b[7],b[8]);
	glTexCoord2f(0,1); glVertex3f(b[3],b[4],b[5]);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	ErrCheck("box");
}

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
void ball(double x,double y,double z,double r)
{
	//  Save transformation
	glPushMatrix();
	//  Offset, scale and rotate
	glTranslated(x,y,z);
	glScaled(r,r,r);
	//  White ball
	glColor3f(1,1,1);
	glutSolidSphere(1.0,16,16);
	//  Undo transofrmations
	glPopMatrix();
}

/*
 *  Draw a cone
 *    with tip at (tx,ty,tz)
 *  	and base at (bx,by,bz)
 *  	with radius r
 */
void cone( double tx, double ty, double tz,
						double bx, double by, double bz,
						double r, int subdivisions
					)
{
	glPushMatrix();
	glTranslated( tx,  ty,  tz);
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f( tx,  ty,  tz);
		double degreeIncrement = (double)360/(double)subdivisions;
		for (int i; i<360; i+=degreeIncrement){
			glVertex3f(bx+r*Cos(i), by+r*Sin(i), bz);
		}
	glEnd();
	glPopMatrix();
	ErrCheck("cone");
}