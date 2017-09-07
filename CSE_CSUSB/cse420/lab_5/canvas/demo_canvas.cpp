//demo_canvas.cpp
#include "canvas.h"

Canvas cvs(500, 400, "CSE 420 Canvas"); //global canvas object

void display()
{
  cvs.clearScreen();	//clear screen
/*
  cvs.setColor ( 1.0, 0.0, 0.0 );
  cvs.moveTo( 0, 0.0 ); //draw line
  cvs.lineTo( 5, 5.0 );
  cvs.setColor ( 0.0, 1.0, 0.0 );
  RealRect box( -1.0, 1.0, -1.0, 2.0 );	//construct a box
  box.draw();		//draw box
*/

  cvs.setColor (0,0,0);
  cvs.moveTo( -5.0, 0 );
  for (int i = 0; i < 5; i++) {
    cvs.forward(10, 1);
    cvs.turn (144); 
  }

  glFlush();
}
