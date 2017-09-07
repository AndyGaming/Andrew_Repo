#include "canvas.h"

Canvas cvs ( 800, 800, "Turtle" );

//L is length of short side
void draw_hook( float L )
{
  cvs.forward( 3*L, 1 );
  cvs.turn( 90 );
  cvs.forward( L, 1 );
  cvs.turn( 90 );
  cvs.forward( L, 1 );
  cvs.turn( 90 );
  
  //the following is not supposed to be here
 /*
  cvs.moveTo ( 2, 0 );
  glLineWidth ( 6 );
  for ( int i = 0; i < 8; i++ ){
    cvs.forward ( 4 * L, 1 );
    cvs.turn ( 20 );  
  }
*/
}
 
void display(void)
{
  cvs.clearScreen();
 
  cvs.moveTo(0.0, 0.0); //starts at center
  cvs.turnTo ( 0.0 );   //points horizontally
  draw_hook ( 0.5 );
}
  
