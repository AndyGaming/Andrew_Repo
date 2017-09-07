/*
  simpletex.cpp
  Sample program showing how to write GL shader programs.
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
#include "imageio.h"

using namespace std;

/*
   Global handles for the currently active program object, with its two shader objects
*/
GLuint programObject = 0;
GLuint vertexShaderObject = 0;
GLuint fragmentShaderObject = 0;
static GLint win = 0;

int anglex= 0, angley = 0, anglez = 0;          //rotation angles
int objectType = 0; 
GLuint texName;

int iWidth = 64, iHeight = 64;
/*
static GLubyte checkImage[iHeight][iWidth][3];
void makeCheckImage(void)
{
   int i, j, c;

   for (i = 0; i < iWidth; i++) {
      for (j = 0; j < iHeight; j++) {
         c = ((((i&0x8)==0)^((j&0x8))==0))*255;
         checkImage[i][j][0] = (GLubyte) c;
         checkImage[i][j][1] = (GLubyte) c;
         checkImage[i][j][2] = (GLubyte) c;
      }
   }
}
*/
GLubyte* makeTexImage( char *loadfile )
{
   int i, j, c, width, height;
   GLubyte *texImage;

   texImage = loadImageRGBA( (char *) loadfile, &width, &height);
   iWidth = width;
   iHeight = height;

   return texImage;
}

void init2DTexture()
{

 // makeCheckImage();
  char filename[100];
  printf("Enter png file name: ");
  scanf ("%s", filename );
  //GLubyte *texImage = makeTexImage( "front.png" );
  GLubyte *texImage = makeTexImage( filename );
  if ( !texImage ) {
       printf("\nError reading %s \n", filename );
   exit(1); 
  }

  glGenTextures(1, &texName);
  glBindTexture(GL_TEXTURE_2D, texName);           //now we work on texName
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth,
//                iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, checkImage );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, 
                  GL_RGBA, GL_UNSIGNED_BYTE, texImage);
  delete texImage;

}

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

    // Set up initial uniform values
    glUniform3f(glGetUniformLocation(programObject, "LightPosition"), 2.0, 2.0, 4.0);
    glUniform1i(glGetUniformLocation(programObject, "texHandle"), 0);
    
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

   glDepthFunc(GL_LESS);
   glEnable(GL_DEPTH_TEST);
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);


   readShaderSource("simpletex.vert", &VertexShaderSource );
   readShaderSource("simpletex.frag", &FragmentShaderSource );
   loadstatus = installShaders(VertexShaderSource, FragmentShaderSource);
   init2DTexture();
   return loadstatus;
}

static void Reshape(int w, int h)
{
    float vp = 0.8f;
    float aspect = (float) w / (float) h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // glOrtho(-1.0, 1.0, -1.0, 1.0, -10.0, 10.0);
    glFrustum(-vp, vp, -vp / aspect, vp / aspect, 3, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
}
    
void CleanUp(void)
{
   glDeleteShader(vertexShaderObject);
   glDeleteShader(fragmentShaderObject);
   glDeleteProgram(programObject);
   glutDestroyWindow(win);
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
    case 't':
      objectType = (objectType + 1 ) % 4;
      break;     
   }
   glutPostRedisplay();
}

void display(void)
{
   GLfloat vec[4];

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClearColor( 1.0, 1.0, 1.0, 0.0 );		//get white background color
 
   glPushMatrix();
   glRotatef( anglex, 1.0, 0.0, 0.0);                   //rotate the cube along x-axis
   glRotatef( angley, 0.0, 1.0, 0.0);                   //rotate along y-axis   
   glRotatef( anglez, 0.0, 0.0, 1.0);                   //rotate along z-axis 

   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texName);   
      
   GLUquadric *qobj = gluNewQuadric();
   //  glutSolidSphere(), glutSolidTorus() do NOT have texture coordinates
   gluQuadricTexture(qobj,GL_TRUE); 
   if ( objectType == 0 )
     gluSphere(qobj,0.6,32,32);
   else if ( objectType == 1 ) {
     glTranslatef( 0, 0.6, 0 );
     glRotatef( 90, 1, 0, 0 ); 
     gluCylinder(qobj, 0.5, 0.5, 1.2, 32, 32);		//top, base height
   } else if ( objectType == 2 )
     glutSolidTeapot(0.6f);				//has texture coordinates
   else {
     gluDisk( qobj, 0, 1, 8, 10);
   }
   gluDeleteQuadric(qobj);
   
   glPopMatrix();
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
   
   // Initialize the "OpenGL Extension Wrangler" library
    glewInit();

   success = init();
   if ( success )
     glutMainLoop();
   return 0;
}

