/*  helix.cpp: Draws helix curve.
 *  Press 'x', 'X', 'y', 'Y', 'z', 'Z' to rotate; 'r' to reset.
 *  For CS 520, 2010
 *  @Author: T.L. Yu
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
  glOrtho(-5.0, 5.0, -5.0, 5.0, 0.1, 100 );
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

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glColor3f ( 0.1, 1.0, 0 ); 
  glPushMatrix();
  glRotatef( anglex, 1.0, 0.0, 0.0);   //rotate the object about x-axis
  glRotatef( angley, 0.0, 1.0, 0.0);   //rotate about y-axis   
  glRotatef( anglez, 0.0, 0.0, 1.0);   //rotate about z-axis
  float C[4];
  glLineWidth ( 3 );
  glBegin (GL_LINE_STRIP);
  for ( float t = 0; t <=  26; t += 0.2 ) {
    get_C ( C, t, 0.1 );
    glVertex4fv( C );
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

