#include <GL/glut.h>
#include <stdlib.h>

void drawtriangle(float *v1, float *v2, float *v3) 
{ 
   glBegin(GL_TRIANGLES); 
      glVertex3fv(v1);    
      glVertex3fv(v2);    
      glVertex3fv(v3);        
   glEnd(); 
}

void subdivide(float *v1, float *v2, float *v3, long depth)
{
   GLfloat v12[3], v23[3], v31[3];
   GLint i;

   if (depth == 0) {
      drawtriangle(v1, v2, v3);
      return;
   }
   for (i = 0; i < 3; i++) {
      v12[i] = ( v1[i]+v2[i] ) / 2.0;
      v23[i] = ( v2[i]+v3[i] ) / 2.0;
      v31[i] = ( v3[i]+v1[i] ) / 2.0;
   }

   subdivide(v1, v12, v31, depth-1);
   subdivide(v2, v23, v12, depth-1);
   subdivide(v3, v31, v23, depth-1);
   subdivide(v12, v23, v31, depth-1);
}

void display() 
{
  glClear(GL_COLOR_BUFFER_BIT);
  GLfloat v1[3], v2[3], v3[3];
  for (int i = 0; i < 3; i++) {
    v1[i] = 30.0;
    v2[i] = 50.0;
    v3[i] = 100.0;
  }
//  subdivide (v1, v2, v3, 2);
  drawtriangle(v1, v2, v3);
  glFlush();
}

void init(void) 
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(-2.0, 2.0, -2.0, 2.0);
   glMatrixMode(GL_MODELVIEW);
   glClearColor (1.0, 1.0, 1.0, 1.0);
   glColor3f(0.0, 0.0, 0.0);
}

int main(int argc, char** argv) 
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(600, 600);
  glutCreateWindow("Subdivide Triangle");
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}
