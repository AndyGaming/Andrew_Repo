//cubes.cpp

#include <GL/glut.h>
#include <stdlib.h>

GLfloat diffuseMaterial[4] = { 1.0, 1.0, 1.0, 0.0 };

void init(void) 
{
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 0.0 };
   GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
   GLfloat lmodel_ambient[] = { 1.0, 0.0, 0.0, 0.0 };

   glClearColor (1.0, 1.0, 1.0, 1.0);
   glShadeModel (GL_SMOOTH);
   //glEnable(GL_DEPTH_TEST);
/*
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
*/
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

   static GLfloat colors_1[] = {1.0, 0.0, 0.0,
                       1.0, 0.0, 0.0,
                       1.0, 0.0, 0.0,
                       1.0, 0.0, 0.0,
                       1.0, 0.0, 0.0,
                       1.0, 0.0, 0.0,
			1.0, 0.0, 0.0, 
			1.0, 0.0, 0.0
			};
                       
   static GLfloat colors_2[] = {0.0, 0.0, 1.0,
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
   gluLookAt (-3.0, 3.0, 7.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   glTranslatef ( -2, 1, 0 );
   glRotatef (15, 0, 1, 0);

   glVertexPointer (3, GL_INT, 0, vertices);
   glEnable( GL_CULL_FACE );
   glCullFace ( GL_BACK );
   glColorPointer (3, GL_FLOAT, 0, colors_1);

   static GLubyte allIndices[] = {4, 5, 6, 7, 1, 2, 6, 5, 
	0, 1, 5, 4, 0, 3, 2, 1, 
	0, 4, 7, 3, 2, 3, 7, 6};

   glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, allIndices);

   glColorPointer (3, GL_FLOAT, 0, colors_1);
   glRotatef ( 20, 1, 1, 1 ); 
  // glRotatef ( 10, 0, 1, 0 );
   glTranslatef ( 5, 0, 2 );

   glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, allIndices);

   glFlush ();
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
      default:
         break;
   }
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

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Cubes");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
