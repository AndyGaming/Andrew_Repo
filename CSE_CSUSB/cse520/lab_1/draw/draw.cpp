//draw.cpp : demo program for drawing 3 dots, two lines, ploylines, rectangles
#include <GL/glut.h>

//initialization
void init( void )
{
  glClearColor( 1.0, 1.0, 1.0, 0.0 );	//get white background color
  glColor3f( 0.0f, 0.0f, 0.0f );	//set drawing color
  glPointSize( 4.0 );			//a dot is 4x4
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();			//replace current matrix with identity matrix
  gluOrtho2D( 0.0, 500.0, 0.0, 500.0 );
}

void display( void )
{
  glClear( GL_COLOR_BUFFER_BIT );	//clear screen
  glBegin( GL_POINTS );			//draw points
    glVertex2i( 100, 50 );		//draw a point
    glVertex2i( 100, 150 );		//draw a point
    glVertex2i( 200, 200 );		//draw a point
  glEnd();
  glBegin( GL_LINES );			//draw lines
    glVertex2i( 20, 20 );		//horizontal line
    glVertex2i( 400, 20 );
    glVertex2i( 20, 10 );		//vertical line
    glVertex2i( 20, 400 );
  glEnd();
  glBegin( GL_LINE_STRIP );		//draw polyline
    glVertex2i( 200, 100 );
    glVertex2i( 300, 100 );
    glVertex2i( 450, 200 );
    glVertex2i( 200, 100 );
  glEnd();
  glColor3f( 0.6, 0.6, 0.6 );		//bright grey
  glRecti( 400, 400, 450, 480 );
  glColor3f( 1.0, 0.0, 0.0 );		//red
  glRecti( 350, 350, 380, 390 );

  glFlush();				//send all output to screen
}
//------------------------------------------------------------------------------

//draw_main.cpp: main loop of drawing program

#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

//initialization
void init(void);
//does the drawing 
void display(void);

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, depth buffer.
 */
int main(int argc, char** argv)
{
  glutInit(&argc, argv);	//initialize toolkit
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB );	//set display mode: single bufferring, RGBA model
  glutInitWindowSize(500, 500);		//set window size on screen
  glutInitWindowPosition( 100, 150 ); 	//set window position on screen
  glutCreateWindow(argv[0]);		//open screen window
  init();
  glutDisplayFunc (display);		//points to display function
  glutMainLoop();			//go into perpetual loop
  return 0;
}
