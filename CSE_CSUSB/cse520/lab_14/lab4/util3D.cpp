#include "util3D.h"
#include <math.h>

XYZ::XYZ()
{
  x = y = z = 0;
}

XYZ::XYZ( double x0, double y0, double z0 )
{
  x = x0;  y = y0;  z = z0;
}

void XYZ::set( double x0, double y0, double z0 )
{
  x = x0;  y = y0;  z = z0;
}

/*
XYZ &XYZ::getXYZ( XYZ &xyz )
{
  xyz.set ( x, y, z );
  
  return xyz;
}
*/

XYZ XYZ::getXYZ()
{
  XYZ a( x, y, z );
  
  return a;
}

void XYZ::getXYZ( double a[] )
{
  a[0] = x;
  a[1] = y;
  a[2] = z;
}

void XYZ::print()
{
  cout << "(" << x <<", " << y << ", " << z << ")" << endl;
}


Point3::Point3():XYZ()
{
}

Point3::Point3( double x0, double y0, double z0 ): XYZ( x0, y0, z0 )
{
}

Point3::Point3( const Point3 &p )
{
  x = p.x;
  y = p.y;
  z = p.z;
}

Vector3 Point3::operator - ( const Point3 &p )
{
  Vector3 v1;

  v1.x = x - p.x;
  v1.y = y - p.y;
  v1.z = z - p.z;

  return v1;
}

Point3 Point3::operator + ( const Vector3 &v )
{
  Point3 p1;

  p1.x = x + v.x;
  p1.y = y + v.y;
  p1.z = z + v.z;

  return p1;
}


//Vector3.cpp : Vector3 class functions and external functions operating on Vector3

Vector3::Vector3():XYZ()
{
}

Vector3::Vector3( double x0, double y0, double z0 ): XYZ( x0, y0, z0 )
{
}

Vector3::Vector3( const Vector3 &v )
{
  x = v.x;
  y = v.y;
  z = v.z;
}

Vector3 Vector3::operator + ( const Vector3 &v )
{
  Vector3 v1;
  v1.x = x + v.x;
  v1.y = y + v.y;
  v1.z = z + v.z;

  return v1;
}

Vector3 Vector3::operator - ( const Vector3 &v )
{
  Vector3 v1;
  v1.x = x - v.x;
  v1.y = y - v.y;
  v1.z = z - v.z;

  return v1;
}

//cross product
Vector3 Vector3::operator ^ ( const Vector3 &v )
{
  Vector3 v1;
  v1.x = y * v.z - z * v.y;
  v1.y = z * v.x - x * v.z;
  v1.z = x * v.y - y * v.x;

  return v1; 
}

//dot product
double Vector3::operator * ( const Vector3 &v )
{
  double d;
  d = x * v.x + y * v.y + z * v.z;

  return d;
}

//vector + point --> point
Point3 Vector3::operator + ( const Point3 &p )
{
  Point3 p1;

  p1.x = x + p.x;
  p1.y = y + p.y;
  p1.z = z + p.z;

  return p1;
}

double Vector3::magnitude()
{
  return sqrt(x * x + y * y + z * z );
}

void Vector3::normalize()
{
  double d = x*x + y*y + z*z;

  if ( d > 0 ) {
    d = sqrt ( d );
    x /= d;
    y /= d;
    z /= d;
  }
}

//------------------- external functions ---------------

//scalar times vector
Vector3 operator * ( double a, const Vector3 &v )
{
  Vector3 v1;
  v1.x = a * v.x;
  v1.y = a * v.y;
  v1.z = a * v.z;

  return v1;
}

Vector3 operator * ( const Vector3 &v, double a )
{
  return a * v;
}
