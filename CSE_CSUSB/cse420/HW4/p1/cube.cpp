/*  cube.c
 *  This program demonstrates a single modeling transformation,
 *  glScalef() and a single viewing transformation, gluLookAt().
 *  A wireframe cube is rendered.
 */
#include <GL/glut.h>
#include <stdlib.h>

static int xangle = 0, yangle = 0, zangle = 0;

void init(void) 
{
   glClearColor (0.8, 0.8, 0.8, 0.8);
   glShadeModel (GL_SMOOTH);

   glEnableClientState (GL_VERTEX_ARRAY);
   glEnableClientState (GL_COLOR_ARRAY);
}

void display(void)
{
   static GLint vertices[] = {-1, -1, -1,   //0
                       1, -1, -1,
                       1, 1, -1,
                       -1, 1, -1,        //3
			-1, -1, 1,
			1, -1, 1,
			1, 1, 1,         //6
			-1, 1, 1};       //7
                       
   static GLfloat colors[] = {0.0, 0.0, 1.0,
                       0.0, 0.0, 0.0,
                       0.0, 1.0, 0.0,
                       0.0, 1.0, 1.0,
                       1.0, 0.0, 1.0,
                       1.0, 0.0, 0.0,
			1.0, 1.0, 0.0, 
			1.0, 1.0, 1.0
			};

   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);
   glLoadIdentity ();            
   gluLookAt (-3.0, 2.5, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   glTranslatef ( 0, 1, 0.5 );
   glRotatef ((GLfloat) xangle, 1.0, 0.0, 0.0);
   glRotatef ((GLfloat) yangle, 0.0, 1.0, 0.0);
   glRotatef ((GLfloat) zangle, 0.0, 0.0, 1.0);

   glVertexPointer (3, GL_INT, 0, vertices);
   glEnable( GL_CULL_FACE );
   glCullFace ( GL_BACK );
   glColorPointer (3, GL_FLOAT, 0, colors);

   static GLubyte allIndices[] = {4, 5, 6, 7, 1, 2, 6, 5, 
	0, 1, 5, 4, 0, 3, 2, 1, 
	0, 4, 7, 3, 2, 3, 7, 6};

   glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, allIndices);

   glFlush ();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity ();
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case 'x':
         xangle += 5;
         glutPostRedisplay();
         break;
      case 'y':
         yangle += 5;
         glutPostRedisplay();
         break;
      case 'z':
         zangle += 5;
         glutPostRedisplay();
         break;
      case 'X':
         xangle -= 5;
         glutPostRedisplay();
         break;
      case 'Y':
         yangle -= 5;
         glutPostRedisplay();
         break;
      case 'Z':
         zangle -= 5;
         glutPostRedisplay();
         break;
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Colored Cube");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
