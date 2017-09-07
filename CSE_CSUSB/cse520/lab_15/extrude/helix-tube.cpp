/* helix-tube.cpp : Draws square along a helix curve using Frenet frames.
 *
 * For CS 520, 2010.
 * @Author: T.L. Yu
 */

#include <SDL/SDL.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "LinearR4.h"

using namespace std;

int anglex= 0, angley = 0, anglez = 0;          //rotation angles
int window;

void init(void)
{
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glEnable(GL_DEPTH_TEST);
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glOrtho(-3.0, 3.0, -3.0, 3.0, 0.1, 100 );
  glMatrixMode(GL_MODELVIEW); // position and aim the camera
  glLoadIdentity();
  gluLookAt( 0, 0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

//helix curve
void get_C ( float C[4], float t, float b )
{
  C[0] = cos ( t );
  C[1] = sin ( t );
  C[2] = b * t;
  C[3] = 1;
}


//Matrix for transforming to Frenet frame
void setM( LinearMapR4 &M, float t, float b )
{
  float c = 1.0 / sqrt ( 1 + b*b );
  M.SetColumn1( -cos(t), -sin(t), 0, 0 );		//Normal    N(t)
  M.SetColumn2( sin(t)*b*c, -cos(t)*b*c, c, 0 );	//Binormal  B(t)
  M.SetColumn3( -sin(t)*c, cos(t) *c, b * c, 0);	//Tangent   T(t)
  M.SetColumn4( cos(t), sin(t), b*t, 1 );  		//The curve C(t)
}

void print_M ( LinearMapR4 &M )
{
  cout << "(" << M.m11 << ",\t" << M.m12 << ",\t" << M.m13 << ",\t" <<  M.m14 << ")" << endl;
  cout << "(" << M.m21 << ",\t" << M.m22 << ",\t" << M.m23 << ",\t" <<  M.m24 << ")" << endl;
  cout << "(" << M.m31 << ",\t" << M.m32 << ",\t" << M.m33 << ",\t" <<  M.m34 << ")" << endl;
  cout << "(" << M.m41 << ",\t" << M.m42 << ",\t" << M.m43 << ",\t" <<  M.m44 << ")" << endl;
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  const float b = 0.1;		//constant of Helix curve
  double H = 6.0;
  LinearMapR4 M;		//Transformation matrix

  
  //4 vertices of a quad
  //float p[4][3]= { {-0.2,-0.2,0}, {0.2,-0.2,0}, {0.2,0.2, 0},{-0.2,0.2,0} };
  //homogeneous coordinates of the four vertices of a quad
  VectorR4 points[4];		//define four points
  points[0] =  VectorR4 (-0.1, -0.1, 0, 1 );	//x, y, z, w
  points[1] =  VectorR4 ( 0.1, -0.1, 0, 1 );	//x, y, z, w
  points[2] =  VectorR4 ( 0.1,  0.1, 0, 1 );	//x, y, z, w
  points[3] =  VectorR4 ( -0.1, 0.1, 0, 1 );	//x, y, z, w

  glColor3f ( 0.1, 1.0, 0 ); 
  glPushMatrix();
  glRotatef( anglex, 1.0, 0.0, 0.0);   //rotate the object about x-axis
  glRotatef( angley, 0.0, 1.0, 0.0);   //rotate about y-axis   
  glRotatef( anglez, 0.0, 0.0, 1.0);   //rotate about z-axis
  float C[4];
  glLineWidth ( 3 );
  glPolygonMode( GL_FRONT, GL_LINE );
  glPolygonMode( GL_BACK, GL_LINE );
  
  //The curve
  glBegin(GL_LINE_STRIP );
  for ( float t = 0; t <=  26; t += 0.2 ) {
    get_C ( C, t, b );
     glVertex4fv( C );
  }
  glColor3f ( 1.0, 0.1, 0 ); 
  glEnd();

  float p3[3];		//3-D point, (x, y, z)
  glBegin( GL_QUADS );
  for ( float t = 0; t <=  26; t += 0.2 ) {
    VectorR4 p_1;	//transformed point	
    setM ( M, t, b );
    //  print_M ( M );
    for ( int i = 0; i < 4; ++i ){
      p_1 = M * points[i];	//transform the point
      p_1.Dump( p3 );		//put (x, y, z) in p3[]
      glVertex3fv( p3 );
    }
  }
  glEnd();
  glPopMatrix();
  glFlush();
}

void keyboard ( unsigned char key, int x, int y )
{
  switch ( key ) {
  case 27:
    glutDestroyWindow(window);
    exit ( 0 );
   case 'x':
      anglex = ( anglex + 3 ) % 360;
      break;
    case 'X':
      anglex = ( anglex - 3 ) % 360;
      break;
    case 'y':
      angley = ( angley + 3 ) % 360;
      break;
    case 'Y':
      angley = ( angley - 3 ) % 360;
      break;
    case 'z':
      anglez = ( anglez + 3 ) % 360;
      break;
    case 'Z':
      anglez = ( anglez - 3 ) % 360;
      break;
    case 'r':                                   //reset
      anglez = angley = anglex = 0;
      break;
  }
  glutPostRedisplay();
}


int main( int argc, char *argv[] )
{
  glutInit( &argc, argv );
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH );
  glutInitWindowSize( 500, 500 );
  glutInitWindowPosition( 100, 100 );
  window = glutCreateWindow("Mesh ");
  glutDisplayFunc(display);
  glutKeyboardFunc( keyboard );
  glClearColor( 1.0f, 1.0f, 1.0f, 0.0f ); //white background
  glViewport ( 0, 0, 500, 500 );
  init ();

  glutMainLoop();

  return 0;
}

