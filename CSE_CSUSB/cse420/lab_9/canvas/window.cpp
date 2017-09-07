//window.cpp : defines world window and view port

#include "window.h"
void setWindow( float left, float right, float bottom, float top )
{
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluOrtho2D( left, right, bottom, top );
}

void setViewport( int left, int right, int bottom, int top )
{
  glViewport( left, bottom, right - left, top - bottom );
}
