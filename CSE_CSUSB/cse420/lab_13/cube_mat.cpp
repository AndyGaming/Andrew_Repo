/*
 *  cube_mat.cpp
 *  This program demonstrates examining the transformation matrix values.
 *  
 *  A wireframe cube is rendered.
 */
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

void init(void)
{
   glClearColor (1.0, 1.0, 1.0, 0.0);
   glShadeModel (GL_FLAT);
}

//print the transformation matrix
template<class T>
void print_mat ( T m[][4] )
{
  cout.precision ( 4 );
  cout << fixed;
  for ( int i = 0; i < 4; ++i ) {
    cout << "\t";
    for ( int j = 0; j < 4; ++j )
      cout <<  m[j][i] << "\t";
    cout << endl;
  }
  cout << endl;
}

void display(void)
{
   float p[4][4];
   double pd[4][4];

   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (0.0, 1.0, 0.0);	  	//green color
   glLoadIdentity ();             	// clear the matrix 
   glMatrixMode (GL_PROJECTION);
           				// viewing transformation  
//   glFrustum (-1.0, 1.0, -1.0, 1.0, 2.0, 20.0);
//   gluPerspective ( 53, 1, 2, 20 );   //tan 26.5 = 1/2.0
     glOrtho (-1.0, 1.0, -1.0, 1.0, 2.0, 20.0);
//   gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
   
   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity();
   glGetFloatv(GL_PROJECTION_MATRIX,&p[0][0]);
   cout << "Projection Matrix:" << endl;
   print_mat ( p );
   glGetDoublev(GL_MODELVIEW_MATRIX, &pd[0][0]);
   cout << "Modelview Transformation Matrix:" << endl;
   print_mat ( pd );
   glScalef (1.0, 2.0, 1.0);      	// modeling transformation 

   glGetFloatv(GL_MODELVIEW_MATRIX,&p[0][0]);
   cout << "Scale Matrix:" << endl;
   print_mat ( p );
  
   glRotatef ( 30, 1, 0, 0 );
   glRotatef ( 30, 0, 1, 0 );
   glRotatef ( 30, 0, 0, 1 );
   glGetDoublev(GL_MODELVIEW_MATRIX, &pd[0][0]);
   cout << "Scale Rotation Matrix:" << endl;
   print_mat ( pd );
   glutWireCube (1.0);
   
   glFlush ();
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
   }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
