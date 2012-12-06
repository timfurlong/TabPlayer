#include "guitar.h"

/*
 *  Draw a guitar neck
 *     at (x,y,z)
 *     neck facing (dx,dy,dz)
 *     up towards (ux,uy,uz)
 */
void guitar::neck(  double x , double y , double z,
			   double dx, double dy, double dz,
			   double ux, double uy, double uz)
// Simple:  neck( 0,0,0, 1,0,0, 0,1,0);
{

		double mat[16];
		getMatrix( dx,dy,dz,
							  ux,uy,uz,  mat);
		//  Save transformation
		glPushMatrix();
		//  Offset
		glTranslated(x-(neck_length/2),y, z);
		glMultMatrixd(mat);

		// Begin drawing  =================================

		// Neck Back *****************
		glEnable(GL_TEXTURE_2D);
		glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,tMode?GL_REPLACE:GL_MODULATE);
		glColor3ub(139,69,19);
		glBindTexture(GL_TEXTURE_2D,texture[0]);
		// Cylinder caps
		glBegin(GL_TRIANGLE_FAN);
			glNormal3f(-1,0,0);
			glVertex3f(0, 0, 0);
			for (int i=0; i<=180; i++){
				glTexCoord2f(Cos(i)+0.5,Sin(i)+0.5); glVertex3f(0, -neck_r*Cos(i), -neck_r*Sin(i));
			}
		glEnd();


		glBegin(GL_TRIANGLE_FAN);
			glNormal3f(1,0,0);
			glVertex3f(neck_length, 0, 0);
			for (int i=0; i<=180; i++){
				glTexCoord2f(Cos(i)+0.5,Sin(i)+0.5); glVertex3f(neck_length, -neck_r*Cos(i), -neck_r*Sin(i));
			}
		glEnd();
		// Back of the neck (half cylinder)
		glBegin(GL_QUAD_STRIP);
			for (float i=0; i<=180; i++){
				glNormal3f(0,-Cos(i),-Sin(i));
				glTexCoord2f(0,i/360); glVertex3f(0, -neck_r*Cos(i), -neck_r*Sin(i));
				glTexCoord2f(1,i/360); glVertex3f(neck_length, -neck_r*Cos(i), -neck_r*Sin(i));
			}
		glEnd();
		glDisable(GL_TEXTURE_2D);

		guitar::fretboard();
		guitar::frets();
		guitar::strings();

		//  Undo transformations
		glPopMatrix();
		ErrCheck("neck");
}

void guitar::fretboard()
{
	// Fretboard *****************
	//  Enable textures
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,tMode?GL_REPLACE:GL_MODULATE);
	glColor3f(1 ,1 ,1 );
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glBegin(GL_QUADS);
		// Front
		glNormal3f( 0, 0, 1);
		glTexCoord2f(0,0); glVertex3f(0,-neck_r,fretboard_thickness);
		glTexCoord2f(1,0); glVertex3f(neck_length,-neck_r,fretboard_thickness);
		glTexCoord2f(1,1); glVertex3f(neck_length,+neck_r,fretboard_thickness);
		glTexCoord2f(0,1); glVertex3f(0,+neck_r,fretboard_thickness);

		// Back
		glNormal3f( 0, 0, -1);
		glTexCoord2f(0,0); glVertex3f(0,-neck_r,0);
		glTexCoord2f(1,0); glVertex3f(neck_length,-neck_r,0);
		glTexCoord2f(1,1); glVertex3f(neck_length,+neck_r,0);
		glTexCoord2f(0,1); glVertex3f(0,+neck_r,0);

		// Right
		glNormal3f(1,0,0);
		glTexCoord2f(1,0); glVertex3f(neck_length,-neck_r,fretboard_thickness);
		glTexCoord2f(1,1); glVertex3f(neck_length,+neck_r,fretboard_thickness);
		glTexCoord2f(1,1); glVertex3f(neck_length,+neck_r,0);
		glTexCoord2f(1,0); glVertex3f(neck_length,-neck_r,0);

		// Left
		glNormal3f(-1,0,0);
		glTexCoord2f(1,0); glVertex3f(0,-neck_r,fretboard_thickness);
		glTexCoord2f(1,1); glVertex3f(0,+neck_r,fretboard_thickness);
		glTexCoord2f(1,1); glVertex3f(0,+neck_r,0);
		glTexCoord2f(1,0); glVertex3f(0,-neck_r,0);

		// Top
		glNormal3f(0,1,0);
		glTexCoord2f(1,1); glVertex3f(neck_length,+neck_r,fretboard_thickness);
		glTexCoord2f(0,1); glVertex3f(0,+neck_r,fretboard_thickness);
		glTexCoord2f(0,1); glVertex3f(0,+neck_r,0);
		glTexCoord2f(1,1); glVertex3f(neck_length,+neck_r,0);

		// Bottom
		glNormal3f(0,-1,0);
		glTexCoord2f(1,1); glVertex3f(neck_length,-neck_r,fretboard_thickness);
		glTexCoord2f(0,1); glVertex3f(0,-neck_r,fretboard_thickness);
		glTexCoord2f(0,1); glVertex3f(0,-neck_r,0);
		glTexCoord2f(1,1); glVertex3f(neck_length,-neck_r,0);

	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void guitar::frets()
{

	ambientvec[0] = 0.19225;
	ambientvec[1] = 0.19225;
	ambientvec[2] = 0.19225;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientvec);
	diffuseVec[0] = 0.50754;
	diffuseVec[1] = 0.50754;
	diffuseVec[2] = 0.50754;
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseVec);
	specularvec[0] = 0.508273;
	specularvec[1] = 0.508273;
	specularvec[2] = 0.508273;
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularvec);
	shinyvec[0] = 0.4*128;
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shinyvec);

	glColor3f(1,1,1);

	double X; // x position of fret
	double last_X;
	for (int fret=0; fret<num_frets; fret++){
		X = neck_length*fret_position[fret];
		// Draw the fret
		// Top
		glBegin(GL_TRIANGLE_FAN);
			glNormal3f(0,1,0);
			glVertex3f(X, +neck_r, fretboard_thickness);
			for (int i=0; i<=180; i++){
				// glTexCoord2f(Cos(i)+0.5,Sin(i)+0.5);
				glVertex3f(X+fret_r*Cos(i), +neck_r, fret_r*Sin(i)+fretboard_thickness);
			}
		glEnd();

		// Bottom
		glBegin(GL_TRIANGLE_FAN);
			glNormal3f(0,-1,0);
			glVertex3f(X, -neck_r, fretboard_thickness);
			for (int i=0; i<=180; i++){
				// glTexCoord2f(Cos(i)+0.5,Sin(i)+0.5);
				glVertex3f(X+fret_r*Cos(i), -neck_r, fret_r*Sin(i)+fretboard_thickness);
			}
		glEnd();

		// Rounded side
		glBegin(GL_QUAD_STRIP);
			for (float i=0; i<=180; i++){
				glNormal3f(Cos(i),0,Sin(i));
				glVertex3f(X+fret_r*Cos(i), -neck_r, fret_r*Sin(i)+fretboard_thickness);
				glVertex3f(X+fret_r*Cos(i), +neck_r, fret_r*Sin(i)+fretboard_thickness);
			}
		glEnd();

		// Draw inlay (if there is one)
		for (int i=0; i<num_frets ;i++){
			if ( fret+1 == 12){
				guitar::inlay( (X+last_X)/2 , +neck_r/2, fretboard_thickness+.001);
				guitar::inlay( (X+last_X)/2 , -neck_r/2, fretboard_thickness+.001);
			}
			else if ( fret+1 == inlay_frets[i] )
				guitar::inlay( (X+last_X)/2 , 0, fretboard_thickness+.001);
		}
		last_X = X;


	}


}

void guitar::inlay(double x, double y, double z)
{
	//  Enable textures
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,tMode?GL_REPLACE:GL_MODULATE);
	glColor3f(1 ,1 ,1 );
	glBindTexture(GL_TEXTURE_2D,texture[2]);

	glBegin(GL_TRIANGLE_FAN);
		for (int i=0; i<=360; i++){
			glTexCoord2f(0.5*Sin(i)+.5,0.5*Cos(i)+.5);
			glVertex3f(x+inlay_r*Sin(i),y+inlay_r*Cos(i),z);
		}
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void guitar::strings()
{
	const double str_fret_dist      = 0.05;
	const double scale_factor       = 0.4;
	const double str_spacing_factor = 1.1;
	const int rep                   = 200;

	double z = fretboard_thickness + fret_r + str_fret_dist;
	double y;
	double r;

	glColor3f(1,1,1);
	for (int i=0; i<6; i++){
		y = neck_r * (2*(double(i)+1))/7 - neck_r;
		y *= str_spacing_factor;
		r = scale_factor * str_width[i];

		// Left
		glBegin(GL_TRIANGLE_FAN);
			glNormal3f(-1,0,0);
			glVertex3f(0, y, z);
			for (int j=0; j<=360; j+=2){
				glTexCoord2f(Cos(j)+0.5,Sin(j)+0.5);
				glVertex3f(0, y+r*Cos(j) , r*Sin(j)+z);
			}
		glEnd();

		// Right
		glBegin(GL_TRIANGLE_FAN);
			glNormal3f(1,0,0);
			glVertex3f(neck_length, y, z);
			for (int j=0; j<=360; j+=2){
				glTexCoord2f(Cos(j)+0.5,Sin(j)+0.5);
				glVertex3f(neck_length, y+r*Cos(j) , r*Sin(j)+z);
			}
		glEnd();

		// Rounded side (only one w/ textures)
		glEnable(GL_TEXTURE_2D);
		glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,tMode?GL_REPLACE:GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D,texture[5]);
		glBegin(GL_QUAD_STRIP);
			for (int j=0; j<=360; j++){
				glNormal3f(Cos(j),0,Sin(j));
				glTexCoord2f(0, (float(j)/360)*rep);
				glVertex3f(0,
							y+r*Cos(j), r*Sin(j)+z);
				glTexCoord2f(rep, (float(j)/360)*rep);
				glVertex3f(neck_length,
							y+r*Cos(j), r*Sin(j)+z);
			}
		glEnd();
		glDisable(GL_TEXTURE_2D);
		// renderCylinder_convenient( 0,y,z,
		// 						   1,y,z,
		// 						   scale_factor * str_width[i],  15);
	}
	ErrCheck("strings");
}