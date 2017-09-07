//gobang.cpp

#include <GL/glut.h>  
#include <stdio.h>    
#include <stdlib.h>   
#include <math.h>    
#include <string.h>

int abc = 0, 
    mouse_x, mouse_y, Win_x, Win_y, object_select,
    spin, spinboxes;

const int width = 850, height = 600;

// win: 1 = player 1 win; -1 = player 2 win; 2 = tie
// player_1 or player_2; -1 = black piece, 1 = white piece
int player_1, player_2, win, start_game;

// 0  | 1  | 2  | 3  | 4  | 5  | 6 
// 7  | 8  | 9  | 10 | 11 | 12 | 13
// 14 | 15 | 16 | 17 | 18 | 19 | 20
// 21 | 22 | 23 | 24 | 25 | 26 | 27
// 28 | 29 | 30 | 31 | 32 | 33 | 34
// 35 | 36 | 37 | 38 | 39 | 40 | 41
// 42 | 43 | 44 | 45 | 46 | 47 | 48 
/*static int box[51][5] = {{0,1,2,3,4}, {1,2,3,4,5}, {2,3,4,5,6}, {7,8,9,10,11},    
                       {8,9,10,11,12}, {9,10,11,12,13}, {14,15,16,17,18}, 
                       {15,16,17,18,19}, {16,17,18,19,20},{21,22,23,24,25},
                       {22,23,24,25,26},{23,24,25,26,27},{28,29,30,31,32},
                       {29,30,31,32,33},{30,31,32,33,34},{35,36,37,38,39},
                       {36,37,38,39,40},{37,38,39,40,41},{42,43,44,45,46},
                       {43,44,45,46,47},{44,45,46,47,48},{0,7,14,21,28},
                       {7,14,21,28,35},{14,21,28,35,42},{1,8,15,22,29},
                       {8,15,22,29,36},{15,22,29,36,43},{2,9,16,23,30},
                       {9,16,23,30,37},{16,23,30,37,44},{3,10,17,24,31},
                       {10,17,24,31,38},{17,24,31,38,45},{4,11,18,25,32},
                       {11,18,25,32,39},{18,25,32,39,46},{5,12,19,26,33},
                       {12,19,26,33,40},{19,26,33,40,47},{6,13,20,27,34}, 
                       {13,20,27,34,41},{20,27,34,41,48},{2,10,18,26,34},
                       {1,9,17,25,33},{9,17,25,33,41},{0,8,16,24,32},
                       {8,16,24,32,40},{16,24,32,40,48},{7,15,23,31,39},
                       {15,23,31,39,47},{14,22,30,38,46}};
*/

// 0  | 1  | 2  | 3  | 4
// 5  | 6  | 7  | 8  | 9
// 10 | 11 | 12 | 13 | 14
// 15 | 16 | 17 | 18 | 19
// 20 | 21 | 22 | 23 | 24
// there are 12 possible ways to win the game
// each way consists 5 pieces to line up
static int box[12][5] = {{0,1,2,3,4}, {5,6,7,8,9}, {10,11,12,13,14}, {15,16,17,18,19},
                        {20,21,22,23,24}, {0,5,10,15,20}, {1,6,11,16,21}, {2,7,12,17,22}, 
                        {3,8,13,18,23}, {4,9,14,19,24}, {0,6,12,18,24}, {4,8,12,16,20}};

// 1 = black, -1 = white, 0 = open space
int box_map[25];
//int box_map[49];

// location for each box, 25 boxes i total
int object_map[25][2] = {{-10,10}, {-5,10}, {0,10}, {5,10}, {10,10},
                         {-10,5}, {-5,5}, {0,5}, {5,5}, {10,5},
                         {-10,0}, {-5,0}, {0,0}, {5,0}, {10,0}, 
                         {-10,-5}, {-5,-5}, {0,-5}, {5,-5}, {10,-5},
                         {-10,-10}, {-5,-10}, {0,-10}, {5,-10}, {10,-10}};  
/*
int object_map[49][2] = {{-15,15},{-10,15},{-5,15},{0,15},{5,15},{10,15},{15,15},
                         {-15,10},{-10,10},{-5,10},{0,10},{5,10},{10,10},{15,10},
                         {-15,5},{-10,5},{-5,5},{0,5},{5,5},{10,5},{15,5},
                         {-15,0},{-10,0},{-5,0},{0,0},{5,0},{10,0},{15,0},
                         {-15,-5},{-10,-5},{-5,-5},{0,-5},{5,-5},{10,-5},{15,-5},
                         {-15,-10},{-10,-10},{-5,-10},{0,-10},{5,-10},{10,-10},{15,-10},
                         {-15,-15},{-10,-15},{-5,-15},{0,-15},{5,-15},{10,-15},{15,-15}};
*/
GLUquadricObj *Cylinder;

void init_game(void)
{
   for (int i = 0; i < 25; i++)
      box_map[i] = 0;

   win = 0;
   start_game = 1;
}

int check_move( void )
{
   int i, t = 0;

//Check for five in a row
   for( i = 0; i < 12; i++) {
   	 t = box_map[box[i][0]] + box_map[box[i][1]] + box_map[box[i][2]]
                     + box_map[box[i][3]] + box_map[box[i][4]];

	 if ( (t == 5) || ( t == -5) ) {
          spinboxes = i;
          return( 1 );
         }
   }

   t = 0;

// check for tie
   for( i = 0; i < 12; i++) {
   	 t = t + abs(box_map[box[i][0]]) + abs( box_map[box[i][1]]) 
          + abs( box_map[box[i][2]]) + abs(box_map[box[i][3]]) + abs(box_map[box[i][4]]);
   }

   if ( t == 60 ) return( 2 );

   return( 0 );
}

void print( int x, int y, char *st)
{
   int l,i;

   l = strlen( st ); 
   glRasterPos2i( x, y ); 
   for(i = 0; i < l; i++)  
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]); 
}

static void TimeEvent(int te)
{
   spin++;  
   if (spin > 360) spin = 180; 
   glutPostRedisplay();  
   glutTimerFunc( 8, TimeEvent, 1);  
}

GLfloat ambient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat diffuse[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat position[]= { 5.0f, 25.0f, 5.0f, 1.0f };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };

void init(void)
{
   glClearColor (0.7,0.45,0.25,0.0);  
   glShadeModel (GL_SMOOTH);  
   glEnable(GL_DEPTH_TEST);   

   glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
   glLightfv(GL_LIGHT0, GL_POSITION, position);
   glLightfv(GL_LIGHT0, GL_SPECULAR, mat_specular);
   glEnable(GL_LIGHTING);  
   glEnable(GL_LIGHT0);    

   start_game = 0;
   win = 0;

   Cylinder = gluNewQuadric();
   gluQuadricDrawStyle( Cylinder, GLU_FILL );
   gluQuadricNormals( Cylinder, GLU_SMOOTH );
   gluQuadricOrientation( Cylinder, GLU_OUTSIDE );
}

void Draw_black(int x, int y, int z, int a)
{
   glPushMatrix();
   glTranslatef(x, y, z);
   glRotatef(a, 0, 1, 0);
   glColor3f(0, 0, 0);
   //glutSolidSphere(1, 100, 100);
   glutSolidTorus(0.2, 0.8, 8, 36);
   glPopMatrix();
}

void Draw_white(int x, int y, int z, int a)
{
   glPushMatrix();
   glTranslatef(x, y, z);
   glRotatef(a, 0, 1, 0);
   glColor3f(1, 1, 1);
   //glutSolidSphere(1, 100, 100);
   glutSolidTorus(0.2, 0.8, 8, 36);
   glPopMatrix();
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
   glColor3f(0.0,1.0,0.0);
   glMatrixMode (GL_PROJECTION);  
   glLoadIdentity();  
   glOrtho(-9.0, 9.0, -9.0, 9.0, 0.0, 30.0); 
   glMatrixMode(GL_MODELVIEW);  
   glLoadIdentity(); 

   glDisable(GL_COLOR_MATERIAL);
   glDisable(GL_LIGHTING);

   glColor3f(0.0, 1.0, 1.0);

   if (win == 1) print( -1, 1, "Player 1 win");
   if (win == -1) print( -1, 1, "Player 2 win");
   if (win == 2) print( 0, 1, "Tie");

   glColor3f( 0.0, 0.0, 1.0);
   print(-1, 8, "GOBANG");
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(70, 1, 1, 30);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glEnable(GL_LIGHTING);
   glEnable(GL_COLOR_MATERIAL);

   gluLookAt( 0, 0, 20, 0, 0, 0, 0, 1, 0);

   int ix, iy, i, j;

// Draw Grid
   for( ix = 0; ix < 6; ix++) {
      glPushMatrix();
      glColor3f(1,1,1);
      glBegin(GL_LINES);
	glVertex2i(-10, -10 + ix * 5);
	glVertex2i(10, -10 + ix * 5);
      glEnd();
      glPopMatrix();
   }
   for( iy = 0; iy < 6; iy++ ) {
      glPushMatrix();
      glColor3f(1,1,1);
      glBegin(GL_LINES);
	glVertex2i(-10 + iy * 5, 10);
	glVertex2i(-10 + iy * 5, -10);
      glEnd();
      glPopMatrix();
   }

   glColorMaterial(GL_FRONT, GL_AMBIENT);
   glColor4f(0.5, 0.5, 0.5, 1.0);
   glColorMaterial(GL_FRONT, GL_EMISSION);
   glColor4f(0.0, 0.0, 0.0, 1.0 );
   glColorMaterial(GL_FRONT, GL_SPECULAR);
   glColor4f(0.1, 0.1, 0.1, 1.0);
   glColorMaterial(GL_FRONT, GL_DIFFUSE);
   glColor4f(0.5, 0.5, 0.5, 1.0);

   glColor3f( 0.0, 0.0, 0.0); 

// Draw object in boxes
   for( i = 0; i < 25; i++) {
      j = 0;
      if (abs( win ) == 1 ) {
         if ( (i == box[spinboxes][0]) 
               || (i == box[spinboxes][1]) || (i == box[spinboxes][2])
               || (i == box[spinboxes][3]) || (i == box[spinboxes][4])) {
            j = spin;
         } else j = 0;
      }
      if(box_map[i] == 1) Draw_white( object_map[i][0], object_map[i][1], -1, j);

      if(box_map[i] == -1) Draw_black( object_map[i][0], object_map[i][1], -1, j);
   }

   glutSwapBuffers();
}

void reshape (int w, int h)
{
   Win_x = w;
   Win_y = h;
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key)
   {
      case 27:
         exit(0); 
         break;
      default:
         break;
   }
}

void mouse(int button, int state, int x, int y)
{
   mouse_x =  (20 * (float) ((float)x/(float)Win_x))/4;
   mouse_y =  (20 * (float) ((float)y/(float)Win_y))/4;

// click square
   object_select = mouse_x + mouse_y * 5;

   if ( start_game == 0) {
      if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
         player_1 = 1;
         player_2 = -1;
         init_game();
         return;
      }
   }

   if ( start_game == 1) {
      if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) 
         if (win == 0) 
            if (box_map[ object_select ] == 0) {
               box_map[ object_select ] = player_1;
	       win = check_move();
	       if (win == 1) {
	          start_game = 0;
                  return;
               }
            }

      if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN)) 
         if (win == 0) 
            if (box_map[ object_select ] == 0) {
               box_map[ object_select ] = player_2;
	       win = check_move();
	          if (win == 1) {
                     win = -1;
                     start_game = 0;
                     return;
                  }
            } 
   }

   if ( win == 2 ) start_game = 0;
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (width, height);
   glutInitWindowPosition (10, 10);
   glutCreateWindow ("Gobang");
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   glutTimerFunc( 50, TimeEvent, 1);
   glutMainLoop();
   return 0;
}
