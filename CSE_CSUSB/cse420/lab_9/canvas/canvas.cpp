//canvas.cpp

#include "canvas.h"
#include <math.h>

//<<moveTo>>
//changes current point
void Canvas:: moveTo(float x, float y)
{
  CP.set(x, y);
}

//<<lineTo>>
//draws a line from current point to new point
void Canvas:: lineTo(float x, float y)
{
  glBegin(GL_LINES);
    glVertex2f((GLfloat) CP.getX(), (GLfloat) CP.getY());
    glVertex2f((GLfloat) x, (GLfloat) y); //draw the line
  glEnd();
  CP.set (x, y); //update current point to new point
  glFlush();
}

//<<setWindow>>
void Canvas:: setWindow (float l, float r, float b, float t)
{
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D ((GLdouble)l, (GLdouble)r, (GLdouble)b, (GLdouble)t);
  window.set (l, r, b, t);
}

//<<setViewport>>
void Canvas:: setViewport (int l, int r, int b, int t)
{
  glViewport(l, b, r-l, t-b);
  viewport.set (l, r, b, t);
}

IntRect Canvas:: getViewport(void)
{
  return viewport;
}

RealRect Canvas:: getWindow(void)
{
  return window;
}

void Canvas:: clearScreen(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
}

void Canvas:: setBackgroundColor(float r, float g, float b)
{
  glClearColor(r, g, b, 0.0);   //4th variable level of transparency, may need to change 
}

void Canvas:: setColor(float r, float g, float b)
{
  glColor3f(r, g, b);
}

void Canvas:: lineTo(Point2 p)
{
  glBegin (GL_LINES);
    glVertex2f((GLfloat) CP.getX(), (GLfloat) CP.getY());
    glVertex2f((GLfloat) p.getX(), (GLfloat) p.getY());
  glEnd();
  CP.set(p.getX(), p.getY());
  glFlush();
}

Canvas:: Canvas(int width, int height, char* windowTitle)
{
  char* argv[1];  //dummy argument list for glutinit()
  char dummyString[8];
  argv[0] = dummyString;  //hook up the pointer
  int argc = 1;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(width, height);
  glutInitWindowPosition (20,20);
  glutCreateWindow (windowTitle);
  setWindow(0, (float)width, 0, (float)height); // default world window
  setViewport (0, width, 0, height); //default viewport
  CP.set(0.0f, 0.0f); //initialize the cp to (0,0)
}

void Canvas:: moveTo(Point2 p)  //moves current point CP to point p object 
{
  float x1, y1;
  x1 = p.getX();
  y1 = p.getY();
  CP.set(x1, y1);
}

float Canvas:: getWindowAspectRatio(void)  //calculates aspect ratio of world window
{
  float width, height, aspectRatio;
  width = window.getR() - window.getL();
  height = window.getT() - window.getB();
  aspectRatio = width/height;
  return aspectRatio;
}

void Canvas:: moveRel(float dx, float dy)
{
  CP.set(CP.getX() + dx, CP.getY() + dy);
}

//for turtle graphics
void Canvas::turnTo( float angle )
{
  CD = angle;	//set current direction
}

void Canvas::turn( float angle )
{
  CD += angle;	//turn anti-clockwise 
}

//move line forward by amount dist, if isVisible nonzero, line is drawn
void Canvas::forward ( float dist, int isVisible )
{
//  const float RadPerDeg = 0.017453393;		//radians per degree
  float RadPerDeg = 3.14159265389 / 180.0;
  float x = CP.getX() + dist * cos ( RadPerDeg * CD );
  float y = CP.getY() + dist * sin ( RadPerDeg * CD );
  if ( isVisible )
    lineTo( x, y );
  else
    moveTo ( x, y );
}//forward
