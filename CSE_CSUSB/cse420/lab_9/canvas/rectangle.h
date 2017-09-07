//rectangle.h
#ifndef RECTANGLE_H
#define RECTANGLE_H

#include<GL/glut.h>

//<<Support Classes for Canvas>>>
class Point2   //single point w/ floating point coordinates
{
public:
  Point2();	//contructor 1
  Point2(float xx, float yy);	//constructor 2
  void set(float xx, float yy);
  float getX();
  float getY();
  void draw(void);
  float x, y;
};

//Viewport Rectangle class
//aligned rectangle with integer coordinates, used for viewport
class IntRect  
{
public:
  IntRect();	//constructors
  IntRect(int left, int right, int bottom, int top);
  void set(int left, int right, int bottom, int top);
  int getL(void);	//left boundary
  int getR(void);	//right boundary
  int getT(void);	//top
  int getB(void);	//bottom

private:
  int l, r, b, t;
};

//world window rectangle class
//simlar to IntRect but w/ floating points & used for world window
class RealRect   
{
public:
  RealRect(); //constructors
  RealRect(float left, float right, float bottom, float top);
  void set(float left, float right, float bottom, float top);
  float getL(void);	//left boundary
  float getR(void);	//right
  float getT(void);
  float getB(void);
  void draw(void); 	//draw this rectangle using OpenGL
private:
  float l, r, b, t;
};

#endif

