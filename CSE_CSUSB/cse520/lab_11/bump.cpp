/*
  bump.cpp
  Sample program showing how to write GL shader programs.
  @Author: T.L. Yu, 2009
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

int anglex= 0, angley = 0, anglez = 0;          //rotation angles
int objectType = 0; 
GLuint texName;

const int iWidth = 64, iHeight = 64;
static GLubyte checkImage[iHeight][iWidth][3];
void makeCheckImage(void)
{
   int i, j, c;

   for (i = 0; i < iWidth; i++) {
      for (j = 0; j < iHeight; j++) {
         c = ((((i&0x8)==0)^((j&0x8))==0))* 100;    // 255;
         checkImage[i][j][0] = (GLubyte) c;
         checkImage[i][j][1] = (GLubyte) c;
         checkImage[i][j][2] = (GLubyte) c;
      }
   }
}

void init2DTexture()
{

  makeCheckImage();
  glGenTextures(1, &texName);
  glBindTexture(GL_TEXTURE_2D, texName);           //now we work on texName
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth,
                iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, checkImage );
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
   
printf("\nOK1\n"); 
    // Create a vertex shader object and a fragment shader object

    vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
    fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Load source code strings into shaders, compile and link

    glShaderSource(vertexShaderObject, 1, &vertex, NULL);
    glShaderSource(fragmentShaderObject, 1, &fragment, NULL);

    glCompileShader(vertexShaderObject);
    glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &vertCompiled);
printf("\nOK2\n"); 

    glCompileShader( fragmentShaderObject );
    glGetShaderiv( fragmentShaderObject, GL_COMPILE_STATUS, &fragCompiled);
printf("OK3\n"); 
if ( !vertCompiled )
   printf("vertex! \n");
if ( !fragCompiled )
   printf("fragment! \n");

    GLchar log[1000];
    GLsizei len;
    glGetShaderInfoLog(vertexShaderObject, 1000, &len, log);
    printf("Vert Shader Info Log: %s\n", log);
    glGetProgramInfoLog(programObject, 1000, &len, log);
    printf("Program Info Log: %s\n", log);

    if (!vertCompiled || !fragCompiled)
        return 0;

    // Create a program object and attach the two compiled shaders

    programObject = glCreateProgram();
    glAttachShader( programObject, vertexShaderObject);
    glAttachShader( programObject, fragmentShaderObject);
printf("\nOK4\n"); 
    // Link the program object 

    glLinkProgram(programObject);
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

    if (!linked)
        return 0;

    // Install program object as part of current state

    glUseProgram(programObject);
    printf("\nshader installed successfully!\n"); 

    // Set up initial uniform values
    glUniform3f(glGetUniformLocation(programObject, "LightPosition"), 2.0, 2.0, 4.0);
    glUniform3f(glGetUniformLocation(programObject, "SurfaceColor"), 0.5, 0.2, 1.0);
    glUniform1f(glGetUniformLocation(programObject, "BumpDensity"), 18.0); //16.0);
    glUniform1f(glGetUniformLocation(programObject, "BumpSize"), 0.04); //0.1);
    glUniform1f(glGetUniformLocation(programObject, "SpecularFactor"), 0.5);
  //  glUniform1i(glGetUniformLocation(programObject, "texHandle"), 0);
   
    printf("\nvariables initialized successfully!\n"); 
    return 1;
}


int init(void)
{
   
   const char *version;
   GLchar *VertexShaderSource, *FragmentShaderSource;
   int loadstatus = 0;

   version = (const char *) glGetString(GL_VERSION);
   if (version[0] != '2' || version[1] != '.') {
      printf("This program requires OpenGL 2.x, found %s\n", version);
      //exit(1);
   }

   glDepthFunc(GL_LESS);
   glEnable(GL_DEPTH_TEST);
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);


   readShaderSource("bump.vert", &VertexShaderSource );
   readShaderSource("bump.frag", &FragmentShaderSource );
   loadstatus = installShaders(VertexShaderSource, FragmentShaderSource);
   //init2DTexture();
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

//source code for glutSolidCube
static void
drawBox(GLfloat size, GLenum type)
{
  static GLfloat n[6][3] =
  {
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0}
  };
  static GLint faces[6][4] =
  {
    {0, 1, 2, 3},
    {3, 2, 6, 7},
    {7, 6, 5, 4},
    {4, 5, 1, 0},
    {5, 6, 2, 1},
    {7, 4, 0, 3}
  };
  GLfloat v[8][3];
  GLint i;

  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

  for (i = 5; i >= 0; i--) {
    glBegin(type);
    glNormal3fv(&n[i][0]);
     glTexCoord2f (0, 0);		//add texture coordinates before each glVertex
                                        //call, similar to lab 9
    glVertex3fv(&v[faces[i][0]][0]);
     glTexCoord2f (1, 0);
    glVertex3fv(&v[faces[i][1]][0]);
     glTexCoord2f (1, 1);
    glVertex3fv(&v[faces[i][2]][0]);
     glTexCoord2f (0, 1);
    glVertex3fv(&v[faces[i][3]][0]);
    glEnd();
  }
}

//void APIENTRY
void glutSolidCube(GLdouble size)
{
  drawBox(size, GL_QUADS);
}

void display(void)
{
   GLfloat vec[4];

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClearColor( 1.0, 1.0, 1.0, 0.0 );		//get white background color

   //glCullFace(GL_BACK);
   //glEnable(GL_CULL_FACE);
 
   glPushMatrix();
   glRotatef( anglex, 1.0, 0.0, 0.0);                   //rotate the cube along x-axis
   glRotatef( angley, 0.0, 1.0, 0.0);                   //rotate along y-axis   
   glRotatef( anglez, 0.0, 0.0, 1.0);                   //rotate along z-axis 

   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texName);   

/*   float f = 2 * 3.1415926 / 4;
   float a = 0;

   glRotatef (45, 0, 0, 1);
   glBegin (GL_POLYGON);
     glTexCoord2f (0, 0);
     glVertex2f (cos(a), sin(a));
     a += f;
     glTexCoord2f (1, 0);
     glVertex2f (cos(a), sin(a));
     a += f;
     glTexCoord2f (1, 1);
     glVertex2f (cos(a), sin(a));
     a += f;
     glTexCoord2f (0, 1);
     glVertex2f (cos(a), sin(a));
   glEnd();
*/


   glutSolidCube(1);




/*      
   GLUquadric *qobj = gluNewQuadric();
   //  glutSolidSphere(), glutSolidTorus() do NOT have texture coordinates
   gluQuadricTexture(qobj,GL_TRUE); 
   //glVertexAttrib3f( glGetAttribLocation ( programObject, "vTangent" ), 1, 1, 1 ); 
   if ( objectType == 0 )
     gluSphere(qobj,0.6,32,32);   
     //  glutSolidSphere();	glutSolidTorus() do NOT have texture coordinates
   else if ( objectType == 1 ) {
     glTranslatef( 0, 0.6, 0 );
     glRotatef( 90, 1, 0, 0 ); 
     gluCylinder(qobj, 0.5, 0.5, 1.2, 32, 32);		//top, base height
   } else if ( objectType == 2 ) 
      glutSolidTeapot(0.6f);				//has texture coordinates
   else { 
      glTexCoord2f (1, 1);
      glTexCoord2f (1, 0);
      glTexCoord2f (0, 1);
      glTexCoord2f (0, 0);
      glutSolidCube(1.0f);
   }
   gluDeleteQuadric(qobj);
*/
   
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

