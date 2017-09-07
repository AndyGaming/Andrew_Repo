//main.cpp
//main loop for canvas graphics
#include "canvas.h"

void display( void );

int main(void)
{
  extern Canvas cvs;
  cvs.setWindow(-10.0, 10.0, -10.0, 10.0);
  cvs.setViewport(0, 500, 0, 500); 
  cvs.setBackgroundColor(1.0, 1.0, 1.0);
  cvs.setColor(0.0, 0.0, 0.0);
	
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutDisplayFunc(display);
  glutMainLoop();

  return 0;
}

