//canvas.h
//header file for canvas.cpp
// this includes code from Figs. 3.25 3.26 3.27 3.28 
// and page 101

#ifndef CANVAS_H
#define CANVAS_H

#include<GL/glut.h>
#include<math.h>
#include<iostream>
#include "rectangle.h" 	//<<Support Classes for Canvas>>>

using namespace std;

class Canvas
{
public:
  Canvas(int width, int height, char* windowTitle); //constructor
  void setWindow(float l, float r, float b, float t);
  void setViewport(int l, int r, int b, int t);
  IntRect getViewport(void); //divulge the viewport data
  RealRect getWindow(void); // divulge the window data
  float getWindowAspectRatio(void);
  void clearScreen();
  void setBackgroundColor(float r, float g, float b);
  void setColor(float r, float g, float b);
  void lineTo(float x, float y);
  void lineTo(Point2 p);
  void moveTo(float x, float y);
  void moveTo(Point2 p);
  void moveRel(float dx, float dy);
  //for turtle graphics
  void turnTo( float angle );
  void turn( float angle );
  void forward ( float dist, int isVisible );
private:
  Point2 CP;         	//current position in the world
  IntRect viewport;  	//the current viewport
  RealRect window;   	//the current window
  float CD;		//current direction, for turtle graphics
} ;

#endif
