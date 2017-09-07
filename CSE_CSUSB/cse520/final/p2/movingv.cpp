/*
  movingv.cpp
  Sample program showing how to write GL shader programs.
  Shader sources are in files "tests.vert" and "tests.frag".
  @Author: T.L. Yu, 2008
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <cmath>

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>


using namespace std;

/*
   Global handles for the currently active program object, with its two shader objects
*/
GLuint programObject = 0;
GLuint vertexShaderObject = 0;
GLuint fragmentShaderObject = 0;
static GLint win = 0;

GLuint timeParam;
int anglex= 0, angley = 0, anglez = 0;          //rotation angles

int readShaderSource(char *fileName, GLchar **shader )
{
    // Allocate memory to hold the source of our shaders.
    FILE *fp;
    int count, pos, shaderSize;
     
    fp = fopen( fileName, "r");
    if ( !fp )
        return 0;

    pos = (int) ftell ( fp );
    fseek ( fp, 0, SEEK_END );			//move to end
    shaderSize = ( int ) ftell ( fp ) - pos;	//calculates file size	
    fseek ( fp, 0, SEEK_SET ); 			//rewind to beginning
     
    if ( shaderSize <= 0 ){
        printf("Shader %s empty\n", fileName);
        return 0;
    }

    shaderSize += 1;
    *shader = (GLchar *) malloc( shaderSize);
    
    // Read the source code
   
    count = (int) fread(*shader, 1, shaderSize, fp);
    (*shader)[count] = '\0';

    if (ferror(fp))
        count = 0;

    fclose(fp);
    
    return 1;
}


//  public 
int installShaders(const GLchar *vertex, const GLchar *fragment)
{
    GLint  vertCompiled, fragCompiled;  // status values
    GLint  linked;
    
    // Create a vertex shader object and a fragment shader object

    vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
    fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Load source code strings into shaders, compile and link

    glShaderSource(vertexShaderObject, 1, &vertex, NULL);
    glShaderSource(fragmentShaderObject, 1, &fragment, NULL);

    glCompileShader(vertexShaderObject);
    glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &vertCompiled);

    glCompileShader( fragmentShaderObject );
    glGetShaderiv( fragmentShaderObject, GL_COMPILE_STATUS, &fragCompiled);

    if (!vertCompiled || !fragCompiled)
        return 0;

    // Create a program object and attach the two compiled shaders

    programObject = glCreateProgram();
    glAttachShader( programObject, vertexShaderObject);
    glAttachShader( programObject, fragmentShaderObject);

    // Link the program object 

    glLinkProgram(programObject);
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

    if (!linked)
        return 0;

    // Install program object as part of current state

    glUseProgram(programObject);
/*
    GLchar names[][20] = { "CoolColor", "HotColor", "tempRange" };
    GLint loc[10];
    for (int i = 0; i < 3; i++) {
       loc[i] = glGetUniformLocation( programObject, names[i]);
       if (loc[i] == -1) 
         printf("No such uniform named %s\n", names[i]);
    }

    glUniform3f(loc[0], 0.0, 0.0, 1);
    glUniform3f(loc[1], 1.0, 0.0, 0.0);
    //glUniform1f(loc[2], 0.0);
    glUniform1f(loc[2], 1.0);
*/

    return 1;
}


int init(void)
{
   
   const char *version;
   GLchar *VertexShaderSource, *FragmentShaderSource;
   int loadstatus = 0;

   version = (const char *) glGetString(GL_VERSION);
   printf("\nversion=%s\n", version );
   if (version[0] < '2' || version[1] != '.') {
      printf("This program requires OpenGL >= 2.x, found %s\n", version);
      exit(1);
   }
   readShaderSource("movingv.vert", &VertexShaderSource );
   readShaderSource("tests.frag", &FragmentShaderSource );
   loadstatus = installShaders(VertexShaderSource, FragmentShaderSource);
  
   timeParam = glGetUniformLocation ( programObject, "time" ); 
   return loadstatus;
}

static void Reshape(int width, int height)
{
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-1.0, 1.0, -1.0, 1.0, 5.0, 25.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(0.0f, 0.0f, -15.0f);
}

void CleanUp(void)
{
   glDeleteShader(vertexShaderObject);
   glDeleteShader(fragmentShaderObject);
   glDeleteProgram(programObject);
   glutDestroyWindow(win);
}

static void Idle(void)
{
   glUniform1f( timeParam, glutGet ( GLUT_ELAPSED_TIME ) );
   glutPostRedisplay();
}


static void Key(unsigned char key, int x, int y)
{
   switch(key) {
   case 27:
      CleanUp();
      exit(0);
      break;
     case 'x':
      anglex = ( anglex + 3 ) % 360;
      break;
    case 'X':
      anglex = ( anglex - 3 ) % 360;
      break;
    case 'y':
      angley = ( angley + 3 ) % 360;
      break;
    case 'Y':
      angley = ( angley - 3 ) % 360;
      break;
    case 'z':
      anglez = ( anglez + 3 ) % 360;
      break;
    case 'Z':
      anglez = ( anglez - 3 ) % 360;
      break;
    case 'r':
      anglex = angley = anglez = 0;
      break;
   }
   glutPostRedisplay();
}

void display(void)
{
   GLfloat vec[4];

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClearColor( 1.0, 1.0, 1.0, 0.0 );	//get white background color

   //float angle = glGetAttribLocation(programObject, "angle");
   //int loc = glGetAttribLocation(programObject, "VertexTemp" );

   //glRotatef(angle, 1, 1, 1);

   //glutWireTetrahedron();
   //glutSolidTetrahedron();

   int loc = glGetAttribLocation(programObject, "temp" );

   glPushMatrix();
   glRotatef( anglex, 1.0, 0.0, 0.0);                   //rotate the cube along x-axis
   glRotatef( angley, 0.0, 1.0, 0.0);                   //rotate along y-axis   
   glRotatef( anglez, 0.0, 0.0, 1.0);                   //rotate along z-axis 


   float a = 2 * 3.14159265 / 5;
   float a1 = 0;

   glBegin ( GL_POLYGON );
     glVertexAttrib3f(loc,1,0,0);
     glVertex2f (cos (a1 ), sin (a1) );
     a1 += a;
     glVertexAttrib3f(loc,0,1,0);
     glVertex2f (cos (a1 ), sin (a1) );
     a1 += a;
     glVertexAttrib3f(loc,1,1,0);
     glVertex2f (cos (a1 ), sin (a1) );
     a1 += a;
     glVertexAttrib3f(loc,1,0,1);
     glVertex2f (cos (a1 ), sin (a1) );
     a1 += a;
     glVertexAttrib3f(loc,0,0,1);
     glVertex2f (cos (a1 ), sin (a1) );
   glEnd();




   glPopMatrix();






/*
   glPushMatrix();

   //glRotatef(angle, 1, 1, 1);

   glBegin(GL_TRIANGLES);
     glVertexAttrib1f( loc, 1.0 );	//set initial color to red
     glVertex3f( 0,0,0 );
     glVertex3f( 0,1,0 );
     glVertex3f( 1,0,0 );
   glEnd();

   glBegin(GL_TRIANGLES);
     glVertexAttrib1f( loc, 0.0 );	//set initial color to blue
     glVertex3f( 1,0,0 );
     glVertex3f( 0,0,1 );
     glVertex3f( 0,0,0 );
   glEnd();

   glBegin(GL_TRIANGLES);
     glVertexAttrib1f( loc, 0.7 );	//set initial color to red
     glVertex3f( 0,0,0 );
     glVertex3f( 0,1,0 );
     glVertex3f( 0,0,1 );
   glEnd();

   glBegin(GL_TRIANGLES);
     glVertexAttrib1f( loc, 0.3 );	//set initial color to red
     glVertex3f( 0,1,0 );
     glVertex3f( 1,0,0 );
     glVertex3f( 0,0,1 );
   glEnd();

   glPopMatrix();
*/

/*
   glPointSize ( 8 );

   glBegin ( GL_TRIANGLES );
     glVertex2f ( 0, 0 );
     glVertex2f ( 0, 1 );
     glVertex2f ( 1, 1 );
   glEnd();
*/
   glutSwapBuffers();
   glFlush();
}


int main(int argc, char *argv[])
{
   int success = 0;

   glutInit(&argc, argv);
   glutInitWindowPosition( 0, 0);
   glutInitWindowSize(400, 400);
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   win = glutCreateWindow(argv[0]);
   glutReshapeFunc(Reshape);
   glutKeyboardFunc(Key);
   glutDisplayFunc(display);
   glutIdleFunc(Idle);

   // Initialize the "OpenGL Extension Wrangler" library
    glewInit();

   success = init();
   if ( success )
     glutMainLoop();
   return 0;
}

