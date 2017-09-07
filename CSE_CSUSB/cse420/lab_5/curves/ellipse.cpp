#include "canvas.h"

Canvas cvs ( 800, 800, "Turtle" );

void ellipse()
{
  cvs.setWindow ( -200, 200, -200, 200 );
  double W = 80, H = 120, t, x, y;
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
 
void display(void)
{
  cvs.clearScreen();
 
  cvs.moveTo(0.0, 0.0); //starts at center
  cvs.turnTo ( 0.0 );   //points horizontally
  glLineWidth ( 6 );
  ellipse();
}
  
