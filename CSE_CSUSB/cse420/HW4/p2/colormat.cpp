/*
 *  colormat.cpp
 *  After initialization, the program will be in
 *  ColorMaterial mode.  Interaction:  pressing the 
 *  mouse buttons will change the diffuse reflection values.
 */
#include <GL/glut.h>
#include <stdlib.h>  

void init(void) 
{
   GLfloat light_position_0[] = { 1.0, 1.0, 1.0, 0.0 };
   GLfloat light_position_1[] = { -1.0, -1.0, -1.0, 0.0 };
   GLfloat light_position_2[] = { 0.0, -1.0, 0.0, 0.0 };
   GLfloat ambientLight[] = { 0.2, 0.2, 1.0, 0.0 };
   GLfloat diffuseLight[] = { 0.2, 1.0, 0.2, 0.0 };
   GLfloat specularLight[] = { 1.0, 1.0, 1.0, 0.0 };
   GLfloat emission[] = { 0.3, 0.2, 0.2, 0.0 };

   glClearColor (0.9, 0.9, 0.9, 0.9);
   glShadeModel (GL_SMOOTH);
   glEnable(GL_DEPTH_TEST);

   glMaterialf(GL_FRONT, GL_SHININESS, 25.0);

   glLightfv(GL_LIGHT0, GL_POSITION, light_position_0);
   glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
   glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
   glLightfv(GL_LIGHT0, GL_EMISSION, emission);

   glLightfv(GL_LIGHT1, GL_POSITION, light_position_1);
   glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
   glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
   glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight);
   glLightfv(GL_LIGHT1, GL_EMISSION, emission);

   glLightfv(GL_LIGHT2, GL_POSITION, light_position_2);
   glLightfv(GL_LIGHT2, GL_AMBIENT, ambientLight);
   glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLight);
   glLightfv(GL_LIGHT2, GL_SPECULAR, specularLight);
   glLightfv(GL_LIGHT2, GL_EMISSION, emission);
   glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 3.0);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_LIGHT1);
   glEnable(GL_LIGHT2);

   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
   //glColor3f (0.5, 1.0, 2.5);
   glColorMaterial(GL_FRONT, GL_SPECULAR);
   //glColor3f (0.0, 2.5, 2.5);
   glColorMaterial(GL_FRONT, GL_EMISSION);
   //glColor3f (0.5, 0.5, 0.5);
   glEnable(GL_COLOR_MATERIAL);

}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glPushMatrix();

   glPushMatrix();
   glTranslatef (0.0, 1.0, 0.0);
   glColor3f (0.0, 0.0, 1.0);
   glutSolidSphere(0.4, 30, 16);
//   glDisable(GL_LIGHT1);
   glPopMatrix();

   glPushMatrix();
   glTranslatef (-1.0, -0.3, 0.0);
   glColor3f (0.0, 1.0, 0.0);
   glutSolidSphere(0.4, 30, 16);
//   glEnable(GL_LIGHT1);
//   glDisable(GL_LIGHT0);
   glPopMatrix();

   glPushMatrix();
   glTranslatef (1.0, -0.3, 0.0);
   glColor3f (1.0, 0.0, 0.0);
   glutSolidSphere(0.4, 30, 16);
//   glEnable(GL_LIGHT0);
//   glDisable(GL_LIGHT2);
   glPopMatrix();

   glPopMatrix();
   glFlush ();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   if (w <= h)
      glOrtho (-1.5, 1.5, -1.5*(GLfloat)h/(GLfloat)w,
         1.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
   else
      glOrtho (-1.5*(GLfloat)w/(GLfloat)h,
         1.5*(GLfloat)w/(GLfloat)h, -1.5, 1.5, -10.0, 10.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Spheres");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
//   glutMouseFunc(mouse);
//   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
