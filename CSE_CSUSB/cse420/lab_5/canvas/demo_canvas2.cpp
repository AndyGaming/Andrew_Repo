//demo_canvas.cpp
#include "canvas.h"

//Canvas cvs(640, 480, "try out Canvas"); //global canvas object
Canvas cvs(1600, 1200, "A big Canvas"); //global canvas object



void polygon(int n, float cx, float cy, float radius, float rotAngle)
{
  //assumes global Canvas object, cvs
  if( n < 3 ) return ; //bad number of sides
  double angle = rotAngle * 3.14159265 / 180; //initial angle
  double angleInc = 2 * 3.14159265 / n;  //angle increment
  cvs.moveTo(radius * cos(angle) + cx, radius * sin(angle) + cy);
  for(int k=0; k < n; k++) //repeat n times
  {
    angle += angleInc;
    cvs.lineTo(radius * cos(angle) + cx, radius * sin(angle) + cy);
  }
} //polygon

void drawCircle(Point2 center, float radius)
{
  glColor3f(1.0, 0.0, 0.0);     //red 
  glLineWidth( 6.0 );       

  const int numVerts = 50;  //many-sided polygon to approximate circle.
  polygon(numVerts, center.getX(), center.getY(), radius, 0);
  glPointSize ( 3 );
  center.draw();

  glFlush();
}//drawCircle


void drawArc(Point2 center, float radius, float startAngle, float sweep)
{ // startAngle and sweep are in degrees 
  glColor3f(0.0, 0.0, 1.0);     //blue
  const int n = 30; // number of intermediate segments in arc 
  float angle = startAngle * 3.14159265 / 180; // initial angle in radians
  float angleInc = sweep * 3.14159265 /(180 * n); // angle increment
  float cx = center.getX(), cy = center.getY();
  cvs.moveTo(cx + radius * cos(angle), cy + radius * sin(angle));
  for(int k = 1; k < n; k++, angle += angleInc)
    cvs.lineTo(cx + radius * cos(angle), cy + radius * sin(angle));
}

void display(void)
{
  int n = 5;
  float radius = 5.0;
  //display at upper right hand quadrant
  float cx = 4.0, cy = 4.0;     
  cvs.clearScreen();
  //draw a polygon
  polygon( n, cx, cy, radius, 0.0);
  //center at lower left quadrant
  Point2 circle_center( -4.0, -4.5 );   
  drawCircle( circle_center, radius );
  //center at lower right quadrant
  Point2 arc_center( 4.0, -4.5 );       
  //starts at 30 deg, sweep through 120 degrees
  drawArc( arc_center, radius, 30, 120 ); 
}

void display0()
{
  cvs.clearScreen();	//clear screen
  cvs.setColor ( 1.0, 0.0, 0.0 );
  cvs.moveTo( 0, 0.0 ); //draw line
  cvs.lineTo( 5, 5.0 );
  cvs.setColor ( 0.0, 1.0, 0.0 );
  

  glFlush();
}
