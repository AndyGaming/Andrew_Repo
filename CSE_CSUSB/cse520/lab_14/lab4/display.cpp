//display.cpp
#include "Mesh.h"
#include <SDL/SDL.h>
#include <GL/glut.h>
#include <stdlib.h>

char  DATA_FILE[100];

int anglex= 0, angley = 0, anglez = 0;          //rotation angles
int window;
Mesh msh;
int lighton = 0;

void init(void)
{
  glClearColor(1.0, 1.0, 1.0, 1.0);
  if ( !msh.readData ( DATA_FILE ) ) {
    cout << "Error opening file " << DATA_FILE << endl;
    return;
  } 

     //The light components that are reflected
  GLfloat mat_specular[] = { 1, 0.8, 0.4, 1.0 };
  GLfloat mat_ambient[] = { 0.8, 0.8, .2, 1.0 };
  GLfloat mat_diffuse[] = { 1, 0.8, 0.4, 1.0 };
  GLfloat mat_shininess[] = { 500.0 };
  GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
  GLfloat light[] = { 1, 1, 1 };
  GLfloat lmodel_ambient[] = { 1, 1, 1, 1.0 };
  GLfloat light1[] = {1, 0, 0 };
  GLfloat light_position1[] = { -1.0, -1.0, -1.0, 0.0 };
  glClearColor (1.0, 1.0, 1.0, 0.0);
  glShadeModel (GL_SMOOTH);

  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_ambient);

  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  glLightfv(GL_LIGHT0, GL_DIFFUSE, light );
  glLightfv(GL_LIGHT0, GL_AMBIENT, light );
  glLightfv(GL_LIGHT0, GL_SPECULAR, light );


  if ( lighton ) {    //turn lightling on 
     glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);
   }

   glEnable(GL_DEPTH_TEST);

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glOrtho(-5.0, 5.0, -5.0, 5.0, 0.1, 100 );
  glMatrixMode(GL_MODELVIEW); // position and aim the camera
  glLoadIdentity();
  gluLookAt( 5, 5, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
  glColor3f( 0, 0, 0 );
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  glRotatef( anglex, 1.0, 0.0, 0.0);   //rotate the object about x-axis
  glRotatef( angley, 0.0, 1.0, 0.0);   //rotate about y-axis   
  glRotatef( anglez, 0.0, 0.0, 1.0);   //rotate about z-axis

  msh.renderMesh();
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
  if ( argc < 2 ) {
    cout << "\nUsage: " << argv[0] << " data file name [0/1] (e.g. " << argv[0] << " data.txt 1)" << endl;
    return 1;
  }
  strcpy ( DATA_FILE, argv[1] );
  if ( argc > 2 )
    lighton = atoi ( argv[2] );
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

