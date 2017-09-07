/*  cube.c
 *  This program demonstrates a single modeling transformation,
 *  glScalef() and a single viewing transformation, gluLookAt().
 *  A wireframe cube is rendered.
 */
#include <GL/glut.h>
#include <stdlib.h>
#include <cmath>

#define a .525731112119133606 
#define b .850650808352039932

void init(void) 
{
   glClearColor (1.0f, 1.0f, 1.0f, 1.0f);
   glShadeModel (GL_SMOOTH);
   glEnableClientState (GL_VERTEX_ARRAY);
   glEnableClientState (GL_COLOR_ARRAY);
}

void normalize(float v[3]) {    
   GLfloat d = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]); 
   if (d == 0.0) {
      return;
   }
   v[0] /= d; v[1] /= d; v[2] /= d; 
}

void normcrossprod(float v1[3], float v2[3], float out[3]) 
{ 
   GLint i, j; 
   GLfloat length;

   out[0] = v1[1]*v2[2] - v1[2]*v2[1]; 
   out[1] = v1[2]*v2[0] - v1[0]*v2[2]; 
   out[2] = v1[0]*v2[1] - v1[1]*v2[0]; 
   normalize(out); 
}

static GLfloat vdata[12][3] = {    
   {-a, 0.0, b}, {a, 0.0, b}, {-a, 0.0, -b}, {a, 0.0, -b},    
   {0.0, b, a}, {0.0, b, -a}, {0.0, -b, a}, {0.0, -b, -a},    
   {b, a, 0.0}, {-b, a, 0.0}, {b, -a, 0.0}, {-b, -a, 0.0} 
};

static GLuint tindices[20][3] = { 
   {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},    
   {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},    
   {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6}, 
   {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} 
};

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (0.0, 0.0, 0.0);
   glLoadIdentity ();            
   gluLookAt (-3.0, 3.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   int i, j;

   glBegin(GL_TRIANGLES);    
     for (i = 0; i < 20; i++) {    
        glColor3f(0.3, 1.0, 0.3);
        glVertex3fv(&vdata[tindices[i][0]][0]); 
        glVertex3fv(&vdata[tindices[i][1]][0]); 
        glVertex3fv(&vdata[tindices[i][2]][0]); 
     }
   glEnd();

   GLfloat d1[3], d2[3], norm[3];    
   for (j = 0; j < 3; j++) {    
      d1[j] = vdata[tindices[i][0]][j] - vdata[tindices[i][1]][j];    
      d2[j] = vdata[tindices[i][2]][j] - vdata[tindices[i][1]][j];    
   }
   normcrossprod(d1, d2, norm); 
   glNormal3fv(norm);

   glFlush();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
   glMatrixMode (GL_MODELVIEW);
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Icosahedron");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutMainLoop();
   return 0;
}
