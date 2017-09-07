#ifndef UTIL3D_H
#define UTIL3D_H
#include <vector>
#include <iostream>

using namespace std;

class XYZ {
  public:
    double x;
    double y;
    double z;
    XYZ ();
    XYZ ( double x0, double y0, double z0 );
    void set ( double x0, double y0, double z0 );
//    XYZ &getXYZ( XYZ &xyz );	//return by reference, this works also
    XYZ getXYZ();
    void getXYZ( double a[]  );
    void print();
};

class Vector3;		//forward declaration

class Point3: public XYZ 
{
  public:
    Point3();
    Point3( double x0, double y0, double z0 );  
    Point3( const Point3 &p);
    Vector3 operator - ( const Point3 &p );    //point - point --> vector
    Point3  operator + ( const Vector3 &v );	//point + vector --> point
};
    
class Vector3: public XYZ 
{
  public:
    Vector3();
    Vector3( double x0, double y0, double z0 );  
    Vector3( const Vector3 &v );
    Vector3 operator + ( const Vector3 &v );	//vector + vector --> vector
    Vector3 operator - ( const Vector3 &v );    //vector - vector --> vector
    Vector3 operator ^ ( const Vector3 &v );	//cross product
    double  operator * ( const Vector3 &v );	//dot product
    Point3  operator + ( const Point3 &p );	//vector + point --> point
    double magnitude();
    void  normalize();				//make it a unit vector
};
    
Vector3 operator * ( double a, const Vector3 &v );		
Vector3 operator * ( const Vector3 &v, double a );		

#endif
