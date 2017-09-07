//demo_canvas.cpp
#include "canvas.h"

Canvas cvs(640, 480, "try out Canvas"); //global canvas object

void display()
{
  cvs.clearScreen();	//clear screen
  cvs.setColor ( 1.0, 0.0, 0.0 );
  cvs.moveTo( 0, 0.0 ); //draw line
  cvs.lineTo( 5, 5.0 );
  cvs.setColor ( 0.0, 1.0, 0.0 );
  RealRect box( 8.0, 10.0, -1.0, 2.0 );	//construct a box
  box.draw();		//draw box
  glLineWidth ( 10 );
  cvs.moveTo ( -9, -9 );
  cvs.lineTo ( -8, -8 );
  cvs.setColor( 0, 0, 1 );
  cvs.turnTo ( 0 );
  
  cvs.forward ( 2, 1 );
  cvs.turn ( 45 );
  cvs.forward ( 2, 1 );
  cvs.turn ( 45 );
  cvs.forward ( 2, 1 );
  glLineWidth ( 1 );
  cvs.turn ( 45 );
  cvs.forward ( 2, 1 );
  cvs.turn ( 45 );
  cvs.forward ( 2, 1 );


  
  
  
  glFlush();
}
