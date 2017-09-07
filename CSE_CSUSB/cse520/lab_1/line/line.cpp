#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define   screenHeight  500

bool can_draw = true;

void init( void )
{
  glClearColor( 1.0, 1.0, 1.0, 0.0 );	//get white background color
  glColor3f( 0.0f, 0.0f, 0.0f );	//set drawing color
  glPointSize( 4.0 );			//a dot is 4x4
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluOrtho2D( 0.0, 500.0, 0.0, 500.0 );
} //init

void display()
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glFlush();
}

void drawDot( int x, int y )
{
  glBegin( GL_POINTS );			
    glVertex2i( x, y );		//draw a point
  glEnd(); 
} //drawDot

void drawLine ( int x1, int y1, int x2, int y2 )
{
  glBegin( GL_LINES );
    glVertex2i ( x1, y1 );
    glVertex2i ( x2, y2 );
  glEnd();
}

void myMouse( int button, int state, int x, int y )
{
  static int count = 0;
  static int x1 = 0, y1 = 0, x2 = 0, y2 = 0;

  if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {
    drawDot( x, screenHeight - y );
    if (can_draw) {
      if ( count++ % 2 == 0 ) {
         x1 = x;
         y1 = screenHeight - y;
      } else {
         x2 = x;
         y2 = screenHeight - y;
         drawLine ( x1, y1, x2, y2 );
      }
    }
  }
  glFlush();				//send all output to screen
}

void myKeyboard ( unsigned char key, int mouseX, int mouseY )
{
  //GLint x = mouseX;
  //GLint y = screenHeight - mouseY;
  switch( key )
  {
    case 'o':
 	if (can_draw) can_draw = false;
        else if (!can_draw) can_draw = true;
	break;
    case 27:
	exit (0);
        break;
    default :
	break;
  }  
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);	//initialize toolkit
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB );	//set display mode
  glutInitWindowSize(500, 500);		//set window size on screen
  glutInitWindowPosition( 100, 150 ); 	//set window position on screen
  glutCreateWindow("Draw Line");		//open screen widow
  init();
  glutMouseFunc( myMouse );
  glutKeyboardFunc( myKeyboard );
  glutDisplayFunc( display );
  glutMainLoop();			//go into perpetual loop
  return 0;
}
