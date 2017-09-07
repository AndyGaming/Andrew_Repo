//util3D.cpp
#include "util3D.h"

//3D Classes for Graphics

//@@@@@@@@@@@@@@@@@@ Point3 class @@@@@@@@@@@@@@@@

void Point3::set(float dx, float dy, float dz)
{x = dx; y = dy; z = dz;}
	
void Point3::set(Point3& p)
{x = p.x; y = p.y; z = p.z;} 	

Point3::Point3(float xx, float yy, float zz)
{x = xx; y = yy; z = zz;}

Point3::Point3()
{x = y = z = 0;}
	
void Point3::build4tuple(float v[])
{// load 4-tuple with this color: v[3] = 1 for homogeneous
  v[0] = x; v[1] = y; v[2] = z; v[3] = 1.0f;
}	

//@@@@@@@@@@@@@@@@@@ Vector3 class @@@@@@@@@@@@@@@@
void Vector3::set(float dx, float dy, float dz)
{ x = dx; y = dy; z = dz;} 

void Vector3::set(Vector3& v)
{ x = v.x; y = v.y; z = v.z;}
	
void Vector3::flip()
{x = -x; y = -y; z = -z;} // reverse this vector

void Vector3::setDiff(Point3& a, Point3& b)//set to difference a - b
{ x = a.x - b.x; y = a.y - b.y; z = a.z - b.z;}

void Vector3::normalize()//adjust this vector to unit length
{		
  double sizeSq = x * x + y * y + z * z;
  if(sizeSq < 0.0000001)
  {
    cerr << "\nnormalize() sees vector (0,0,0)!";
    return; // does nothing to zero vectors;
  }
  float scaleFactor = 1.0/(float)sqrt(sizeSq);
  x *= scaleFactor; y *= scaleFactor; z *= scaleFactor;
} //normalize

Vector3::Vector3(float xx, float yy, float zz)
{x = xx; y = yy; z = zz;}

Vector3::Vector3(Vector3& v)
{x = v.x; y = v.y; z = v.z;}
	
Vector3::Vector3()
{x = y = z = 0;} //default constructor

Vector3 Vector3::cross(Vector3 b) //return this cross b
{
  Vector3 c(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x);
  return c;
}

float Vector3::dot(Vector3 b) // return this dotted with b
{return x * b.x + y * b.y + z * b.z;}


// @@@@@@@@@@@@@@@@@@@@@ Color3 class @@@@@@@@@@@@@@@@
Color3::Color3()
{red = green = blue = 0;}

Color3::Color3(float r, float g, float b)
{red = r; green = g; blue = b;}

Color3::Color3(Color3& c)
{red = c.red; green = c.green; blue = c.blue;}

void Color3::set(float r, float g, float b)
{red = r; green = g; blue = b;}

void Color3::set(Color3& c) 
{red = c.red; green = c.green; blue = c.blue;}

void Color3::add(float r, float g, float b)
{red += r; green += g; blue += b;}

void Color3::add(Color3& src, Color3& refl)
{ // add the product of source color and reflection coefficient
  red   += src.red   * refl.red;
  green += src.green * refl.green; 
  blue  += src.blue  * refl.blue;
}

void Color3::add(Color3& colr)
{ // add colr to this color
  red += colr.red ; green += colr.green; blue += colr.blue;}

void Color3::build4tuple(float v[])
{// load 4-tuple with this color: v[3] = 1 for homogeneous
  v[0] = red; v[1] = green; v[2] = blue; v[3] = 1.0f;
}

//@@@@@@@@@@@@@@@@@@@@ light class @@@@@@@@@@@@@@@@@@@
// for a linked list of light sources’ color and position

void Light::setPosition(Point3 p)
{pos.set(p);}
	
void Light::setColor(Color3 c)
{color.set(c);}
	
Light::Light()
{next = NULL;}
