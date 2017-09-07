#include "canvas.h"

Canvas cvs ( 800, 800, "Turtle" );

void parabola()
{
  cvs.setWindow ( -200, 200, -200, 200 );
  double a = 8, t, x, y;
  
  t = -4;
  y = a * t * t;
  x = 2 * a * t;
  cvs.moveTo ( x, y );

  for ( t = -4; t <= 4; t += 0.1 ) {
    y = a * t * t;
    x = 2 * a * t;
    cvs.lineTo ( x, y );
  }
}

void ellipse()
{
  cvs.setWindow ( -200, 200, -200, 200 );
  cvs.setColor ( 1, 0, 1 );
  double W = 150, H = 70, t, x, y;
  double pi = 3.14159265389;

  t = 0;
  x = W * cos ( t );
  y = H * sin ( t );
  cvs.moveTo ( x, y );

  for ( t = 0; t <= 2.1 * pi; t += 0.1 ) {
    x = W * cos ( t );
    y = H * sin ( t );
    cvs.lineTo ( x, y );
  }
}

void spiral()
{
  cvs.setWindow ( -100, 100, -100, 100 );
  cvs.setColor ( 0, 1, 1 );
  double K = 0.02, a = 0.25, x, y, f, t;
  const double pi = 3.14159265389;
  t = 0;
  f = K * exp ( a * t );
  x = f * cos ( t );
  y = f * sin ( t );
  cvs.moveTo ( x, y );
  for ( t = 0; t <= 12 * pi; t += 0.1 ) {
    f = K * exp ( a * t );
    x = f * cos ( t );
    y = f * sin ( t );
    cvs.lineTo ( x, y );
  }
}

void hyperbola()
{
  cvs.setWindow ( -200, 200, -200, 200 );
  cvs.setColor ( 0, 0, 1 );
  double x, y, t, a = 50, b = 50;
  const double pi = 3.14159265389;
  t = -5; 
  x = a * (1 / cos (t));
  y = b * (sin (t) / cos (t));
  cvs.moveTo (x, y);

  for (t = -5; t < 2 * pi ; t += 0.1) {
    x = a * (1 / cos (t));
    y = b * (sin (t) / cos (t));
    cvs.lineTo (x, y);
  }
}

void display(void)
{
  cvs.clearScreen();
  cvs.moveTo(0.0, 0.0); //starts at center
  cvs.turnTo ( 0.0 );   //points horizontally
  glLineWidth ( 6 );
//  parabola();
//  ellipse();
//  spiral();
  hyperbola();
}
  
