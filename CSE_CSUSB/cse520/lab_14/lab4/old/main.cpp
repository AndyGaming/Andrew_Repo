//main.cpp
//main loop for graphics
#include <GL/glut.h>
#include <stdlib.h>

void display( void );

void keyboard ( unsigned char key, int x, int y )
{
  switch ( key ) {
  case 27:
    exit ( 0 );
  }
}


int main( int argc, char *argv[] )
{
  glutInit( &argc, argv );
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH );
  glutInitWindowSize( 500, 500 );
  glutInitWindowPosition( 100, 100 );
  glutCreateWindow("Mesh ");
  glutDisplayFunc(display);
  glutKeyboardFunc( keyboard );
  glClearColor( 1.0f, 1.0f, 1.0f, 0.0f ); //white background
  glViewport ( 0, 0, 500, 500 );

  glutMainLoop();

  return 0;
}

