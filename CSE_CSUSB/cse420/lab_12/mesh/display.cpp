//display.cpp
#include "mesh.h"
#define DATA_FILE	"data.txt"

void display(void)
{
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glOrtho(-2.0, 2.0, -2.0, 2.0, 0.1, 100 );
  glMatrixMode(GL_MODELVIEW); // position and aim the camera
  glLoadIdentity();
  gluLookAt(2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  glClear(GL_COLOR_BUFFER_BIT);
  Mesh msh;
  msh.readFile( DATA_FILE );  
  msh.drawMesh();
  glFlush();
}
