/********************************************************************************************

	Matching Pokemon game
	Programmer: 
		Yazhuo Liu

    A matching pokemon game where player clicks on the correct tiles to match the same pictures.

********************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <iostream>

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "imageio.h"

using namespace std;

GLuint programObject = 0;
GLuint vertexShaderObject = 0;
GLuint fragmentShaderObject = 0;
static GLint window = 0;

int right_lower= 0, right_upper = 0, left_lower = 0, left_upper = 0; 
int win, start_game, count;
int objectType = 0; 
GLuint texName1, texName2, texNameB, texNameW, texNameS;
GLuint timeParam;

vector<int> v;

int iWidth = 64, iHeight = 64;

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
  GLubyte *texImage_1 = makeTexImage( "pic1.png" );
  glGenTextures(1, &texName1);
  glBindTexture(GL_TEXTURE_2D, texName1);           //now we work on texName
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, 
                  GL_RGBA, GL_UNSIGNED_BYTE, texImage_1);

  GLubyte *texImage_2 = makeTexImage( "pic2.png" );
  glGenTextures(1, &texName2);
  glBindTexture(GL_TEXTURE_2D, texName2);           //now we work on texName
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, 
                  GL_RGBA, GL_UNSIGNED_BYTE, texImage_2);

  GLubyte *texImage_back = makeTexImage( "back.png" );
  glGenTextures(1, &texNameB);
  glBindTexture(GL_TEXTURE_2D, texNameB);           //now we work on texName
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, 
                  GL_RGBA, GL_UNSIGNED_BYTE, texImage_back);

  GLubyte *texImage_win = makeTexImage( "win.png" );
  glGenTextures(1, &texNameW);
  glBindTexture(GL_TEXTURE_2D, texNameW);           //now we work on texName
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, 
                  GL_RGBA, GL_UNSIGNED_BYTE, texImage_win);

  GLubyte *texImage_suck = makeTexImage( "suck.png" );
  glGenTextures(1, &texNameS);
  glBindTexture(GL_TEXTURE_2D, texNameS);           //now we work on texName
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, 
                  GL_RGBA, GL_UNSIGNED_BYTE, texImage_suck);

  delete texImage_1;
  delete texImage_2;
  delete texImage_back;
  delete texImage_win;
  delete texImage_suck;
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

void print( float x, float y, char *st)
{
   int l,i;

   l = strlen( st ); 
   glRasterPos2f( x, y );  //the location of the text
   for(i = 0; i < l; i++)  
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]); 
}

int check_win(vector<int> & v)
{
   // if all the cards have been fliped over, the player wins the game
   if (left_lower != 0 && left_upper != 0 && right_lower != 0 && right_upper != 0) 
     return 1;

   // if it's the first time the player flips over a card, the game continues
   if (v.size() < 2) return 0;
   else {
     // if the two cards are of the same texture, game continues
     if (v[0] == v[1]) {
       while (v.size() > 0) v.pop_back();
       return 0;
     }
     // if the two cards are not of the same texture, player loses
     if (v[0] != v[1]) {
       while (v.size() > 0) v.pop_back();
       return 2;
     }
   }
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

   win = start_game = count = 0;

   readShaderSource("pokemon.vert", &VertexShaderSource );
   readShaderSource("pokemon.frag", &FragmentShaderSource );
   loadstatus = installShaders(VertexShaderSource, FragmentShaderSource);

   timeParam = glGetUniformLocation ( programObject, "time" ); 

   init2DTexture();
   return loadstatus;
}

void Timer(int t)
{
    if (count == 0)  // only works when the game first starts
      right_lower = right_upper = left_lower = left_upper = 0;
    glutPostRedisplay();
    glutTimerFunc(2000, Timer, 0);
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
   glutDestroyWindow(window);
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
    case 'd':
      if (start_game == 1) {
        if (right_lower == 0)
          right_lower++;
        if (win == 0) {
          v.push_back(1);
          win = check_win(v);
          if (win != 0) start_game = 0;	// the game stops when the player wins or loses
        }
        count++;
      }
      break;
    case 'w':
      if (start_game == 1) {
        if (right_upper == 0)
          right_upper++;
        if (win == 0) {
          v.push_back(2);
          win = check_win(v);
          if (win != 0) start_game = 0;
        }
        count++;
      }
      break;
    case 'q':
      if (start_game == 1) {
        if (left_upper == 0)
          left_upper++;
        if (win == 0) {
          v.push_back(1);
          win = check_win(v);
          if (win != 0) start_game = 0;
        }
        count++;
      }
      break; 
    case 's':
      if (start_game == 1) {
        if (left_lower == 0)
          left_lower++;
        if (win == 0) {
          v.push_back(2);
          win = check_win(v);
          if (win != 0) start_game = 0;
        }
        count++;
      }
      break;
    case 'r':
      if (start_game == 0) 
        // resets the game, everything goes to its initial state
        right_lower = right_upper = left_lower = left_upper = start_game = win = count = 0;
      break;
    case 'p':
      if (start_game == 0 && win == 0) {
        start_game = 1; 
        right_lower = right_upper = left_lower = left_upper = 1;
      }
     break;
   }
   glutPostRedisplay();
}

void display(void)
{
   GLfloat vec[4];

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClearColor( 0.439216, 0.858824, 0.576471, 0.0 );		//background color

   glActiveTexture(GL_TEXTURE0);

   print(-0.42, 1.22, "MATCHING POKEMON");
   print(-1.1, 1.13, "Use q, w, s, d, to flip each card");
   print(-1.1, 1.04, "p = start game   r = reset game   Esc = exit game");

   if (start_game == 1) print(-0.34, -1.3, "GAME STARTED!");

   if (win == 1) { 
     //print(-0.2, -0.13, "YOU WIN!");
     glPushMatrix();
     glBindTexture(GL_TEXTURE_2D, texNameW);  
     glBegin ( GL_QUADS ); 
       glTexCoord2f (0,0);
       glVertex3f (-0.4,-0.2,0);
       glTexCoord2f (1,0);
       glVertex3f (0.4,-0.2,0);
       glTexCoord2f (1,1);
       glVertex3f (0.4,0,0);
       glTexCoord2f (0,1);
       glVertex3f (-0.4,0,0);
     glEnd();  
     glPopMatrix();
   }
   if (win == 2) {
     //print(-0.2, -0.13, "YOU SUCK!");
     glPushMatrix();
     glBindTexture(GL_TEXTURE_2D, texNameS);  
     glBegin ( GL_QUADS ); 
       glTexCoord2f (0,0);
       glVertex3f (-0.35,-0.55,0);
       glTexCoord2f (1,0);
       glVertex3f (0.35,-0.55,0);
       glTexCoord2f (1,1);
       glVertex3f (0.35,0.35,0);
       glTexCoord2f (0,1);
       glVertex3f (-0.35,0.35,0);
     glEnd();  
     glPopMatrix();
   }

   // right upper card
   glPushMatrix();
   // if the card has not been selected, display the back texture,
   // otherwise display the picture
   if (right_upper == 0) 
     glBindTexture(GL_TEXTURE_2D, texNameB);  
   else glBindTexture(GL_TEXTURE_2D, texName1); 
   glBegin ( GL_QUADS ); 
     glTexCoord2f (0,0);
     glVertex3f (0.1,0,0);
     glTexCoord2f (1,0);
     glVertex3f (1.1,0,0);
     glTexCoord2f (1,1);
     glVertex3f (1.1,1,0);
     glTexCoord2f (0,1);
     glVertex3f (0.1,1,0);
   glEnd();
   glPopMatrix();

   // right lower card
   glPushMatrix();
   if (right_lower == 0)
     glBindTexture(GL_TEXTURE_2D, texNameB);  
   else glBindTexture(GL_TEXTURE_2D, texName2);   
   glBegin ( GL_QUADS ); 
     glTexCoord2f (0,1);
     glVertex3f (0.1,-0.2,0);
     glTexCoord2f (0,0);
     glVertex3f (0.1,-1.2,0);
     glTexCoord2f (1,0);
     glVertex3f (1.1,-1.2,0);
     glTexCoord2f (1,1);
     glVertex3f (1.1,-0.2,0);
   glEnd();
   glPopMatrix();

   // left upper card
   glPushMatrix();
   if (left_upper == 0)
     glBindTexture(GL_TEXTURE_2D, texNameB);  
   else glBindTexture(GL_TEXTURE_2D, texName2);   
   glBegin ( GL_QUADS ); 
     glTexCoord2f (1,0);
     glVertex3f (-0.1,0,0);
     glTexCoord2f (1,1);
     glVertex3f (-0.1,1,0);
     glTexCoord2f (0,1);
     glVertex3f (-1.1,1,0);
     glTexCoord2f (0,0);
     glVertex3f (-1.1,0,0);
   glEnd();
   glPopMatrix();

   // left lower card
   glPushMatrix();
   if (left_lower == 0)
     glBindTexture(GL_TEXTURE_2D, texNameB);  
   else glBindTexture(GL_TEXTURE_2D, texName1);   
   glBegin ( GL_QUADS ); 
     glTexCoord2f (1,1);
     glVertex3f (-0.1,-0.2,0);
     glTexCoord2f (0,1);
     glVertex3f (-1.1,-0.2,0);
     glTexCoord2f (0,0);
     glVertex3f (-1.1,-1.2,0);
     glTexCoord2f (1,0);
     glVertex3f (-0.1,-1.2,0);
   glEnd();
   glPopMatrix();

   glutSwapBuffers();
   glFlush();
}


int main(int argc, char *argv[])
{
   int success = 0;

   glutInit(&argc, argv);
   glutInitWindowPosition(50, 0);
   glutInitWindowSize(800, 800);
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   window = glutCreateWindow("MATCHING POKEMON");
   glutReshapeFunc(Reshape);
   glutKeyboardFunc(Key);
   glutDisplayFunc(display);
   glutIdleFunc(Idle);
   glutTimerFunc(2000, Timer, 0);
   
   // Initialize the "OpenGL Extension Wrangler" library
    glewInit();

   success = init();
   if ( success )
     glutMainLoop();
   return 0;
}

