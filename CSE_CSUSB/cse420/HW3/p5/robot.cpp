//robot.cpp
#include <GL/glut.h>
#include <stdlib.h>

static int shoulder = 0, elbow = 0, fingers = 0, thumb = 0;

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glPushMatrix();				  //Save M0

   glTranslatef (-1.7, 0.0, 0.0);		  //M1 = T-1
   glRotatef ((GLfloat) shoulder, 0.0, 0.0, 1.0); //M2 = T-1Rs
   glTranslatef (1.0, 0.0, 0.0);		  //M3 = T-1RsT+1
   glPushMatrix();				  //Save M3
   glScalef (2.0, 0.4, 1.0);			  //M4 = T-1RsT+1S
   glutWireCube (1.0);				  //P' =  T-1RsT+1S P
   glPopMatrix();				  //Restore M3 = T-1RsT+1

   glTranslatef (1.0, 0.0, 0.0);		  //M5 = T-1RsT+1T+1
   glRotatef ((GLfloat) elbow, 0.0, 0.0, 1.0);	  //M6 = T-1RsT+1T+1Re
   glTranslatef (1.0, 0.0, 0.0);		  //M7 = T-1RsT+1T+1ReT+1
   glPushMatrix();				  //Save M7
   glScalef (2.0, 0.4, 1.0);			  //M8 = T-1RsT+1T+1ReT+1S
   glutWireCube (1.0);				  //P' = T-1RsT+1T+1ReT+1S P
   glPopMatrix();				  //Restore M7 

   glTranslatef (0.7, 0.0, 0.0);		  
   glRotatef ((GLfloat) thumb, 0.0, 0.0, 1.0);	  
   glTranslatef (0.7, 0.0, 0.0);
   glPushMatrix();				
   glScalef (1.0, 0.2, 0.2);			
   glutWireCube (1.0);				
   glPopMatrix();	

   glTranslatef (-0.5, 0.0, 0.2);		  
   glRotatef ((GLfloat) fingers, 0.0, 0.0, 1.0);	  
   glTranslatef (-0.5, 0.0, 0.2);
   glPushMatrix();				
   glScalef (1.0, 0.2, 0.2);			
   glutWireCube (1.0);				
   glPopMatrix();
	
   glTranslatef (0.0, 0.0, -0.2);		  
   glTranslatef (0.0, 0.0, -0.2);
   glPushMatrix();				
   glScalef (1.0, 0.2, 0.2);			
   glutWireCube (1.0);				
   glPopMatrix();

   glTranslatef (0.0, 0.0, -0.3);		  
   glTranslatef (0.0, 0.0, -0.3);
   glPushMatrix();				
   glScalef (1.0, 0.2, 0.2);			
   glutWireCube (1.0);				
   glPopMatrix();	

   glPopMatrix();				  //Restore M0
   glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef (0.0, 0.0, -5.0);
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case 'w':
         shoulder = (shoulder + 5) % 360;
         glutPostRedisplay();
         break;
      case 's':
         shoulder = (shoulder - 5) % 360;
         glutPostRedisplay();
         break;
      case 'a':
         elbow = (elbow + 5) % 360;
         glutPostRedisplay();
         break;
      case 'd':
         elbow = (elbow - 5) % 360;
         glutPostRedisplay();
         break;
      case 'f':
         thumb = (thumb + 5) % 360;
         glutPostRedisplay();
         break;
      case 'e':
         fingers = (fingers - 5) % 360;
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
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Robot Arm");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
