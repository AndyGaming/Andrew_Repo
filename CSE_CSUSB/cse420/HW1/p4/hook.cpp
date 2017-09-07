/*
  draw_demo.cpp
  Demonstrate the use of turtle grapics using the surface class developed
  by T.L. Yu

  @Author: T.L. Yu, April 15, 2006

*/
#include <SDL/SDL.h>
#include <stdlib.h>
#include <stdio.h>

#include "draw.h"
#include "surface.h"

//draw a hook
void draw_hook ( Surface &surf, int L )
{
  surf.forward ( L, 1 );
  surf.turn( 90 );
  surf.forward ( L/5, 1 );
  surf.turn ( 90 );
  surf.forward ( L/3, 1 );
}

//draw a star pattern
void draw_star( Surface &surf, int L )
{
  for ( int i = 0; i < 5; ++i ) {
    surf.forward( L, 1 );
    surf.turn( 144 );
  }
}

//draw an n-sided regular polygon
void draw_polygon ( Surface &surf, int n,  int radius, float rotAngle )
{
  if ( n < 3 ) return;				//bad number of sides
  int cx = surf.getCP().x;
  int cy = surf.getCP().y;
  double angle = rotAngle * 3.14159265 / 180;	//initial angle
  double angleInc = 2 * 3.14159265 / n;		//angle increment
  surf.moveTo ( ( int) (radius * cos( angle ) + cx), 
		 ( int ) ( radius * sin ( angle ) + cy ) );
  for ( int k = 0; k < n; k++ ) {		//repeat n times
    angle += angleInc;
    surf.lineTo ( ( int) (radius * cos( angle ) + cx), 
		 ( int ) ( radius * sin ( angle ) + cy ) );
  }
} //draw_polygon

//draw an arc
void draw_arc ( Surface &surf, int n,  int radius, float rotAngle, float sweep )
{
  if ( n < 3 ) return;				//bad number of sides
  int cx = surf.getCP().x;
  int cy = surf.getCP().y;
  double angle = rotAngle * 3.14159265 / 180;	//initial angle
  double angleInc = sweep * 3.14159265 / (180 * n);	//angle increment
  surf.moveTo ( ( int) (radius * cos( angle ) + cx), 
		 ( int ) ( radius * sin ( angle ) + cy ) );
  for ( int k = 0; k < n; k++ ) {		//repeat n times
    angle += angleInc;
    surf.lineTo ( ( int) (radius * cos( angle ) + cx), 
		 ( int ) ( radius * sin ( angle ) + cy ) );
  }
}

//draw no.3
void draw_no_3 (Surface &surf, int L)
{
  surf.turn( 330 );
  surf.forward( L/3*4, 1 );
  surf.turn( 60 );
  surf.forward( L, 1 );
  surf.turn( 60 );
  surf.forward( L, 1 );
  surf.turn( 60 );
  surf.forward( L/6*5, 1 );
  surf.turn( 60 );
  surf.forward( L/6*5, 1 );
  surf.turn( 60 );
  surf.forward( L/3*2, 1 );
  surf.turn( 60 );
  surf.forward( L/3*2, 1 );
  surf.turn( 60 );
  surf.forward( L/2, 1 );
  surf.turn( 60 );
  surf.forward( L/3, 1 );
  surf.turn( 60 );
  surf.forward( L/4, 1 );
  surf.turn( 60 );
}

void draw_3(Surface & surf, int L)
{
  draw_no_3( surf, L );
  surf.turn( 30 );
  draw_no_3( surf, L );
  surf.turn( 30 );
  draw_no_3( surf, L );
}

//draw turtle 1
void draw_turtle1( Surface & surf, int L)
{
  surf.forward( L * 0.8, 1 );
  surf.turn( 60 );
  surf.forward( L, 1 );
  surf.turn( 260 );
  surf.forward( L, 1 );
  surf.turn( 135 );
  surf.forward( L * 1.5, 1 );
  surf.turn( 345 );
  surf.forward( L * 3, 1 );
  surf.turn( 270 );
  surf.forward( L * 2, 1 );
  surf.turn( 125 );
  surf.forward( L * 1.5, 1 );
  surf.turn( 70 );
  surf.forward( L * 2.5, 1 );
  surf.turn( 290 );
  surf.forward( L * 2, 1 );
  surf.turn( 130 );
  surf.forward( L * 2, 1 );
  surf.turn( 290 );
  surf.forward( L * 2.5, 1 );
  surf.turn( 70 );
  surf.forward( L * 1.5, 1 );
  surf.turn( 125 );
  surf.forward( L * 2, 1 );
  surf.turn( 270 );
  surf.forward( L * 3, 1 );
  surf.turn( 345 );
  surf.forward( L * 1.5, 1 );
  surf.turn( 135 );
  surf.forward( L, 1 );
  surf.turn( 260 );
  surf.forward( L, 1 );
  surf.turn( 60 );
}

//draw rosette with N-sided polygon
void rosette (Surface &surf,  int N, int radius )
{
  if ( N < 3 ) return;
  Point pt[N+1];

  int cx = surf.getCP().x;
  int cy = surf.getCP().y;
  double angle = 0;				//initial angle
  double angleInc = 2 * 3.14159265 / N;		//angle increment
  pt[0] = Point ( ( int) (radius * cos( angle ) + cx), 
		 ( int ) ( radius * sin ( angle ) + cy ) );
  for ( int k = 1; k < N; k++ ) {		//repeat n times
    angle += angleInc;
    pt[k] = Point ( ( int) (radius * cos( angle ) + cx), 
		 ( int ) ( radius * sin ( angle ) + cy ) );
  }
  for ( int i = 0; i < N - 1; i++ ) {
    for ( int j = i + 1; j < N; j++ ) {
    	surf.moveTo ( pt[i] );	//connect all vertices
    	surf.lineTo ( pt[j] );
    }
  }
} //rosette
 
int main()
{
 
#ifndef ARM
    const int VWIDTH = 640;
    const int VHEIGHT = 480;
#else
    const int VWIDTH = 320;
    const int VHEIGHT = 240;
#endif

  const Point center ( VWIDTH/2, VHEIGHT/2 );	//center of screen
  Surface surf( VWIDTH, VHEIGHT, (char *) "Draw_demo" );

  surf.clearScreen();			//clear screen
  surf.updateSurface();
//  SDL_Delay ( 1000 );			//dealy one second, just for demo

  surf.setBackgroundColor ( 0xe0, 0xe0, 0xe0 );	//set background to grey
/*
  //draw no.3
  surf.setColor (0xff, 0, 0 );
  surf.moveTo (center.x - 90, center.y);
  surf.turnTo (0);
  draw_3 (surf, 100);

  //draw turtle 1
  surf.setColor (0, 0, 0xff);
  surf.moveTo (center.x, center.y + 120);
  surf.turnTo (0);
  draw_turtle1 (surf, 30);

  //draw a hook
  surf.setColor ( 0xff, 0, 0 );		//using red color
  surf.moveTo ( center );		//move to center of screen
  surf.turnTo ( 0 );			//points horizontally
  draw_hook ( surf, 60 );

  //draw a star
  surf.setColor ( 0, 0xff, 0 );		//using green color
  surf.moveTo ( center.x + 90, center.y  );
  surf.turnTo ( 0 );			//points horizontally
  draw_star ( surf, 50 );

  //draw an hexagon
  surf.setColor ( 0, 0, 0xff );		//using blue color
  surf.moveTo ( center.x, center.y - 90  );
  draw_polygon ( surf, 6, 80, 0 );	

  //draw a circle
  surf.setColor (0, 0, 0);
  surf.moveTo (center.x - 120, center.y + 90);
  draw_polygon (surf, 100, 80, 0); 

  //draw an arc
  surf.setColor (0, 0, 0);
  surf.moveTo (center.x + 100, center.y);
  draw_arc ( surf, 100, 220, 260, 250 );

  //draw a big square
  surf.setColor (0, 0xff, 0);
  surf.moveTo (center.x - 130, center.y);
  draw_polygon (surf, 4, 130, 45);

  //draw a small square
  surf.setColor (0, 0, 0);
  surf.moveTo (center.x - 160, center.y - 80);
  draw_polygon (surf, 4, 70, 45);
*/
  //draw an 8-sided rosette
  surf.setColor ( 0, 0, 0 );		//using black color
  surf.moveTo ( center.x + 120, center.y - 20 );
  rosette ( surf, 30, 200 );

  surf.updateSurface();
  SDL_Delay ( 5000 );		//display 10 seconds before exit
  return 1;
}
