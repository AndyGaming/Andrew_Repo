//rectangle.cpp
#include "rectangle.h"

//<<Support Classes for Canvas>>>

//constructor 1
Point2::Point2()
{
  x = y = 0.0f;
}

//constructor 2
Point2::Point2(float xx, float yy) 
{
  x=xx; y=yy;
} 

void Point2::set(float xx, float yy) 
{
  x=xx; y=yy;
}

float Point2::getX() 
{
  return x;
}

float Point2::getY() 
{
  return y;
}

void Point2::draw(void)
{
  glBegin(GL_POINTS); //draw this point
    glVertex2f((GLfloat)x, (GLfloat)y);
  glEnd();
}

//Viewport Rectangle class
//aligned rectangle with integer coordinates, used for viewport

//constructors
IntRect::IntRect() 
{
  l = 0; r=100; b=0; t=100;
} 

IntRect::IntRect(int left, int right, int bottom, int top)
{
  l = left; r=right; b=bottom; t=top;
}

void IntRect::set(int left, int right, int bottom, int top)
{
  l=left; r=right; b=bottom; t=top;
}

int IntRect::getL(void)	//left boundary
{
  return l;
}

int IntRect::getR(void)	//right boundary
{
  return r;
}

int IntRect::getT(void)	//top
{
  return t;
}

int IntRect::getB(void)	//bottom
{
  return b;
}

//world window rectangle class
//simlar to IntRect but w/ floating points & used for world window

//constructors
RealRect::RealRect() 
{
  l = 0; r=100; b=0; t=100;
}
 
RealRect::RealRect(float left, float right, float bottom, float top)
{
  l = left; r=right; b=bottom; t=top;
}

void RealRect::set(float left, float right, float bottom, float top)
{
  l=left; r=right; b=bottom; t=top;
}

float RealRect::getL(void)	//left boundary
{
  return l;
}

float RealRect::getR(void)	//right
{
  return r;
}

float RealRect::getT(void)
{
  return t;
}

float RealRect::getB(void)
{
  return b;
}

void RealRect::draw(void)
{
  glRectf( l, b, r, t);
}

