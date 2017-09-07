/*
  morph.cpp
  Sample program showing how to write GL shader programs.
  Shader sources are in files "particle.vert" and "tests.frag".
  @Author: T.L. Yu, 2009
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

using namespace std;

/*
   Global handles for the currently active program object, with its two shader objects
*/
GLuint programObject = 0;
GLuint vertexShaderObject = 0;
GLuint fragmentShaderObject = 0;
static GLint win = 0;

GLuint tParam;		//parameters for sending to vertex shader
float tValue;
GLuint Vertex2Param;

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

    *shader = (GLchar *) malloc( shaderSize + 1);
    
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

    //check log
    GLchar log[1000];
    GLsizei len;
    glGetShaderInfoLog(vertexShaderObject, 1000, &len, log);
    printf("Vert Shader Info Log: %s\n", log);
    glGetProgramInfoLog(programObject, 1000, &len, log);
    printf("Program Info Log: %s\n", log);

    return 1;
}



int init(void)
{
   
   const char *version;
   GLchar *VertexShaderSource, *FragmentShaderSource;
   int loadstatus = 0;

   version = (const char *) glGetString(GL_VERSION);
   if (version[0] < '2' || version[1] != '.') {
      printf("This program requires OpenGL >= 2.x, found %s\n", version);
      exit(1);
   }
   readShaderSource( (char *) "morph.vert", &VertexShaderSource );
   readShaderSource((char *) "tests.frag", &FragmentShaderSource );
   loadstatus = installShaders(VertexShaderSource, FragmentShaderSource);
 
   printf("loadstatus=%d\n", loadstatus ); 
   tParam = glGetUniformLocation ( programObject, "t" ); 
   Vertex2Param = glGetAttribLocation ( programObject, "Vertex2" ); 
   return loadstatus;
}

static void Reshape(int width, int height)
{
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 25.0);
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
   float t = glutGet ( GLUT_ELAPSED_TIME );
   while ( t > 3000 ) t -= 3000;
   glUniform1f( tParam, t );
   glutPostRedisplay();
}


static void Key(unsigned char key, int x, int y)
{
   switch(key) {
   case 27:
      CleanUp();
      exit(0);
      break;
   case 't':
      if ( tValue < 1 )
        tValue += 0.05;
      glUniform1f( tParam, tValue );
      printf("tValue = %f\n", tValue );
      break;
   case 'T':
      if ( tValue >= 0.05 )
        tValue -= 0.05;
      glUniform1f( tParam, tValue );
      break;
   }
   glutPostRedisplay();
}

//arbitrarily create two figures for demo use
void makeFigures( float A[][3], float B[][3] )
{
  A[0][0] = 1;   A[0][1] = 1;     A[1][0] = 4;     A[1][1] = 4;
  A[2][0] = 6;   A[2][1] = 0;     A[3][0] = 6;     A[3][1] = -6;
  A[4][0] = 4;   A[4][1] = -6;    A[5][0] = 4;     A[5][1] = -4;
  A[6][0] = 2;   A[6][1] = -4;    A[7][0] = 2;     A[7][1] = -6;
  A[8][0] = 0;   A[8][1] = -6;
  B[0][0] = 0;   B[0][1] = 0;     B[1][0] = 3;     B[1][1] = 0;
  B[2][0] = 6;   B[2][1] = 0;     B[3][0] = 6;     B[3][1] = -2;
  B[4][0] = 4;   B[4][1] = -2;    B[5][0] = 4;     B[5][1] = -6;
  B[6][0] = 2;   B[6][1] = -6;    B[7][0] = 2;     B[7][1] = -2;
  B[8][0] = 0;   B[8][1] = -2;
}

void display(void)
{
   GLfloat vec[4];

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClearColor( 1.0, 1.0, 1.0, 0.0 );	//get white background color
   glColor3f ( 1, 0, 0 );  		//red, this will have no effect if shader is loaded
   glPolygonMode( GL_FRONT, GL_LINE );
   glPolygonMode( GL_BACK, GL_LINE );

   int N = 9;
   GLfloat A[N][3], B[N][3];
   makeFigures( A, B );
   glUniform1f ( tParam, tValue );
   glBegin ( GL_POLYGON );
   for ( int i = 0; i < N; ++i ) {
     glVertexAttrib3fv ( Vertex2Param,  &B[i][0] );	//send second object vertex to vertex shader 
     glVertex2fv ( A[i] );				//vertex of first object
   }
   glEnd();
   glutSwapBuffers();
   glFlush();

}


int main(int argc, char *argv[])
{
   int success = 0;

   glutInit(&argc, argv);
   glutInitWindowPosition( 0, 0);
   glutInitWindowSize(500, 500);
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

