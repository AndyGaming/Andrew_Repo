#include <SDL/SDL.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "LinearR4.h"

using namespace std;


//int anglex= 0, angley = 0, anglez = 0;          //rotation angles
//int window;
//Mesh msh;
/*
void init(void)
{
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glEnable(GL_DEPTH_TEST);
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glOrtho(-5.0, 5.0, -5.0, 5.0, 0.1, 100 );
  glMatrixMode(GL_MODELVIEW); // position and aim the camera
  glLoadIdentity();
  gluLookAt( 5, 5, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
}
*/
void print ( LinearMapR4 &M )
{
  cout << "(" << M.m11 << ",\t" << M.m12 << ",\t" << M.m13 << ",\t" <<  M.m14 << ")" << endl;
  cout << "(" << M.m21 << ",\t" << M.m22 << ",\t" << M.m23 << ",\t" <<  M.m24 << ")" << endl;
  cout << "(" << M.m31 << ",\t" << M.m32 << ",\t" << M.m33 << ",\t" <<  M.m34 << ")" << endl;
  cout << "(" << M.m41 << ",\t" << M.m42 << ",\t" << M.m43 << ",\t" <<  M.m44 << ")" << endl;
}

void display(void)
{
  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //glClearColor( 1, 1, 1, 1 );

  LinearMapR4 M;
  M.SetColumn1 ( 1, 1, 1, 1 );
  M.SetColumn2 ( 0, 0.33, 0.66, 1 );
  M.SetColumn3 ( 0, 0.11, 0.44, 1 );
  M.SetColumn4 ( 0, 0.04, 0.30, 1 );

  cout << endl << "Matrix A: \n";
  print(M);
  cout << endl << "Inverse A: \n";

  LinearMapR4 N;
  N = M.Inverse();

  print(N);
  cout << endl << "A * Inverse A: \n";

  M *= N;

  print(M);
  cout << endl;


/*
  double H = 6.0;
  LinearMapR4 M;
  M.SetColumn1 ( 0.7, 0, 0, 0 );
  M.SetColumn2 ( 0, 0.7, 0, 0 );
  M.SetColumn3 ( 0, 0, 1, H );
  M.SetColumn4 ( 0, 0, 0, 1 );

  VectorR4 r4;
  VectorR4 v ( 0, 0, 0, 1 );
  r4 = M * v;
//  cout << "(" << v.x << "," << v.y << "," << v.z << "," << v.w << ")" << endl;
//  cout << "(" << r4.x << "," << r4.y << "," << r4.z << "," << r4.w << ")" << endl;

  VectorR4 pi[3];
  pi[0] =  VectorR4 ( 2, 0, 0, 1 );
  pi[1] =  VectorR4 ( 2, 2, 0, 1 );
  pi[2] =  VectorR4 ( 0, 2, 0, 1 );
  float p[3];

  glColor3f ( 0.5, 0.5, 0 ); 
  glPushMatrix();
  glRotatef( anglex, 1.0, 0.0, 0.0);   //rotate the object about x-axis
  glRotatef( angley, 0.0, 1.0, 0.0);   //rotate about y-axis   
  glRotatef( anglez, 0.0, 0.0, 1.0);   //rotate about z-axis
  //glBegin (GL_POLYGON);
  glBegin (GL_TRIANGLES);
   for ( int i = 0; i < 3; ++i ) {
     pi[i].Dump ( p );
     glVertex3fv( p );
   }
   glColor3f (1, 0, 0 );
   for ( int i = 0; i < 3; ++i ) {
     r4 = M * pi[i];
     r4.Dump( p );
     glVertex3fv( p );
   }
  glEnd();
  glBegin( GL_QUADS );
  int j = 0;
  for ( int i = 0; i < 3; ++i ) {
    j = (i+1)%3;
     pi[i].Dump ( p );
     glVertex3fv( p );
     pi[j].Dump ( p );
     glVertex3fv( p );
     r4 = M * pi[i];
     r4.Dump( p );
     glVertex3fv( p );
     r4 = M * pi[j];
     r4.Dump( p );
     glVertex3fv( p );
  }
  glEnd();
  //msh.drawMesh();
  glPopMatrix();
*/

  //glFlush();
}
/*
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
*/

int main( int argc, char *argv[] )
{
  display();
/*
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
*/
  return 0;
}

