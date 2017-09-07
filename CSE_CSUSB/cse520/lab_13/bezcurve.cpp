/*  bezcurve.c
 *   *  This program uses evaluators to draw a Bezier curve.
 *    */
#include <GL/glut.h>
#include <stdlib.h>
/*                                                                                
GLfloat ctrlpoints[5][3] = {
        { -4.0, -4.0, 0.0}, { -2.0, 4.0, 0.0},
        {2.0, -4.0, 0.0}, { -2.5, 3.5, 0.0 }, {4.0, 4.0, 0.0}};*/

GLfloat ctrlpoints[4][3] = {
        { -4.0, 3.0, 0.0}, { 4.0, 2.0, 0.0},
        {-3.0, -2.0, 0.0}, {3.0, -4.0, 0.0}}; 
                                                                              
void init(void)
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
   /*   
 *       GL_MAP1_VERTEX_3 -- specifies that 3-dimensional control points are 
 *       provided and 3-D vertices should be produced
 *       0.0 -- low value of parmeter u
 *       1.0 -- high value of parmeter u
 *       3   -- number of floating-point values to advance in the data between two
 *       consecutive control points
 *       4   -- order of the spline ( = degree + 1 )
 *       			            	          */
   glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
   //glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 5, &ctrlpoints[0][0]);
   glEnable(GL_MAP1_VERTEX_3);
}
                                                                                
void display(void)
{
   int i;
   //int npoints = 5;                
   int npoints = 4;                
                                                                
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(1.0, 1.0, 1.0);
   glBegin(GL_LINE_STRIP);
      for (i = 0; i <= 30; i++)
         glEvalCoord1f((GLfloat) i/30.0);
   glEnd();
   /* The following code displays the control points as dots. */
   glPointSize(5.0);
   glColor3f(1.0, 1.0, 0.0);
   glBegin(GL_POINTS);
      for (i = 0; i < npoints; i++)
         glVertex3fv(&ctrlpoints[i][0]);
   glEnd();
   glFlush();
}
 
void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (w <= h)
      glOrtho(-5.0, 5.0, -5.0*(GLfloat)h/(GLfloat)w,
               5.0*(GLfloat)h/(GLfloat)w, -5.0, 5.0);
   else
      glOrtho(-5.0*(GLfloat)w/(GLfloat)h,
               5.0*(GLfloat)w/(GLfloat)h, -5.0, 5.0, -5.0, 5.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}
                                                                                
void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
   }
}
                                                                                
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc (keyboard);
   glutMainLoop();
   return 0;
}

