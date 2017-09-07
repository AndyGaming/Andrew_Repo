#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

#define POINTER 1
#define INTERLEAVED 2

#define DRAWARRAY 1
#define ARRAYELEMENT  2
#define DRAWELEMENTS 3

//int setupMethod = POINTER;
int derefMethod = DRAWARRAY;

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glEnable (GL_CULL_FACE);
   if (derefMethod == DRAWARRAY) { 
      glCullFace ( GL_BACK );
      glColor3f( 1.0, 0.0, 0.0 );
      glFrontFace( GL_CCW );
      glPolygonMode( GL_FRONT, GL_FILL );
      glBegin( GL_POLYGON );
         glVertex2i( 100, 100 );
         glVertex2i( 300, 100 );
         glVertex2i( 300, 300 );
         glVertex2i( 100, 300 );
      glEnd(); 
   }
   else if (derefMethod == ARRAYELEMENT) {
      glCullFace (GL_FRONT);
      glColor3f( 0.0, 1.0, 0.0 );
      glFrontFace( GL_CCW );
      glPolygonMode( GL_BACK, GL_FILL );
      glBegin( GL_POLYGON );
         glVertex2i( 100, 100 );
         glVertex2i( 100, 300 );
         glVertex2i( 300, 300 );
         glVertex2i( 300, 100 );
      glEnd();
   }
   glFlush ();
}

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
//   setupPointers ();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluOrtho2D (0.0, (GLdouble) w, 0.0, (GLdouble) h);
}

void mouse (int button, int state, int x, int y)
{
   switch (button) {
      case GLUT_LEFT_BUTTON:
         if (state == GLUT_DOWN) {
            if (derefMethod == DRAWARRAY) 
               derefMethod = ARRAYELEMENT;
            else if (derefMethod == ARRAYELEMENT) 
               derefMethod = DRAWELEMENTS;
            glutPostRedisplay();
         }
         break;
      default:
         break;
   }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (400, 400); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   glutMainLoop();
   return 0;
}

//#endif
