//util3D.h

#ifndef UTIL3D_H
#define UTIL3D_H

#include <GL/glut.h>
#include <math.h>
#include <iostream>

using namespace std;

//3D Classes for Graphics
class Point3{ 
public: 
  float x,y,z;
  void set(float dx, float dy, float dz);
  void set(Point3& p); 	
  Point3(float xx, float yy, float zz);
  Point3();
  void build4tuple(float v[]);
};

class Vector3{ 
public: 
  float x,y,z;
  void set(float dx, float dy, float dz); 
  void set(Vector3& v);
  void flip();
  void setDiff(Point3& a, Point3& b);	//set to difference a - b
  void normalize();	//adjust this vector to unit length
  Vector3(float xx, float yy, float zz);
  Vector3(Vector3& v);
  Vector3(); 			//default constructor
  Vector3 cross(Vector3 b); 	//return this cross b
  float dot(Vector3 b); 	// return this dotted with b
};

class Color3 { // holds an red,green,blue 3-tuple
public:
  float red, green, blue;
  Color3();
  Color3(float r, float g, float b);
  Color3(Color3& c);
  void set(float r, float g, float b);
  void set(Color3& c);
  void add(float r, float g, float b);
  void add(Color3& src, Color3& refl);
  void add(Color3& colr);
  void build4tuple(float v[]);
};

class Light{ // for a linked list of light sources’ color and position
public:
  Point3 pos;
  Color3 color;
  Light* next;
  void setPosition(Point3 p);
  void setColor(Color3 c);
  Light();
};

#endif
