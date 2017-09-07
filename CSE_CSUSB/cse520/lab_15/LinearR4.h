/*
 *
 * RayTrace Software Package, release 3.1.  December 20, 2006.
 *
 * Mathematics Subpackage (VrMath)
 *
 * Author: Samuel R. Buss
 *
 * Software accompanying the book
 *		3D Computer Graphics: A Mathematical Introduction with OpenGL,
 *		by S. Buss, Cambridge University Press, 2003.
 *
 * Software is "as-is" and carries no warranty.  It may be used without
 *   restriction, but if you modify it, please change the filenames to
 *   prevent confusion between different versions.  Please acknowledge
 *   all use of the software in any publications or products based on it.
 *
 * Bug reports: Sam Buss, sbuss@ucsd.edu.
 * Web page: http://math.ucsd.edu/~sbuss/MathCG
 *
 */

//
// Linear Algebra Classes over R4
//
//
// A. Vector and Position classes
//
//    A.1. VectorR4: a column vector of length 4
//
// B. Matrix Classes
//
//	  B.1 LinearMapR4 - arbitrary linear map; 4x4 real matrix
//
//	  B.2 RotationMapR4 - orthonormal 4x4 matrix
//

#ifndef LINEAR_R4_H
#define LINEAR_R4_H

#include <math.h>
#include <assert.h>
#include <iostream>
#include "LinearR3.h"
using namespace std;

class VectorR4;				// R4 Vector
class LinearMapR4;			// 4x4 real matrix
class RotationMapR4;		// 4x4 rotation map

// **************************************
// VectorR4 class                       *
// * * * * * * * * * * * * * * * * * * **

class VectorR4 {

public:
	double x, y, z, w;		// The x & y & z & w coordinates.

	static const VectorR4 Zero;
	// Deprecated due to unsafeness of global initialization
	//static const VectorR4 UnitX;
	//static const VectorR4 UnitY;
	//static const VectorR4 UnitZ;
	//static const VectorR4 UnitW;
	//static const VectorR4 NegUnitX;
	//static const VectorR4 NegUnitY;
	//static const VectorR4 NegUnitZ;
	//static const VectorR4 NegUnitW;

public:
	VectorR4( ) : x(0.0), y(0.0), z(0.0), w(0.0) {}
	VectorR4( double xVal, double yVal, double zVal, double wVal )
		: x(xVal), y(yVal), z(zVal), w(wVal) {}
	// VectorR4( const Quaternion& q);			// Definition with Quaternion routines
	
	VectorR4& SetZero() { x=0.0; y=0.0; z=0.0; w=0.0; return *this;}
	VectorR4& SetUnitX() { x=1.0; y=0.0; z=0.0; w=0.0; return *this;}
	VectorR4& SetUnitY() { x=0.0; y=1.0; z=0.0; w=0.0; return *this;}
	VectorR4& SetUnitZ() { x=0.0; y=0.0; z=1.0; w=0.0; return *this;}
	VectorR4& SetUnitW() { x=0.0; y=0.0; z=0.0; w=1.0; return *this;}
	VectorR4& SetNegUnitX() { x=-1.0; y=0.0; z=0.0; w=0.0; return *this;}
	VectorR4& SetNegUnitY() { x=0.0; y=-1.0; z=0.0; w=0.0; return *this;}
	VectorR4& SetNegUnitZ() { x=0.0; y=0.0; z=-1.0; w=0.0; return *this;}
	VectorR4& SetNegUnitW() { x=0.0; y=0.0; z=0.0; w=-1.0; return *this;}
	VectorR4& Set( double xx, double yy, double zz, double ww ) 
			{ x=xx; y=yy; z=zz; w=ww; return *this;}
	VectorR4& Set ( const Quaternion& );		// Defined with Quaternion
	VectorR4& Set ( const VectorHgR3& h ) {x=h.x; y=h.y; z=h.z; w=h.w; return *this; }
	VectorR4& Load( const double* v );
	VectorR4& Load( const float* v );
	void Dump( double* v ) const;
	void Dump( float* v ) const;

	VectorR4& operator+= ( const VectorR4& v ) 
		{ x+=v.x; y+=v.y; z+=v.z; w+=v.w;  return(*this); } 
	VectorR4& operator-= ( const VectorR4& v ) 
		{ x-=v.x; y-=v.y; z-=v.z; w-=v.w;  return(*this); }
	VectorR4& operator*= ( double m ) 
		{ x*=m; y*=m; z*=m; w*=m;  return(*this); }
	VectorR4& operator/= ( double m ) 
			{ register double mInv = 1.0/m; 
			  x*=mInv; y*=mInv; z*=mInv; w*=mInv;
			  return(*this); }
	VectorR4 operator- () const { return ( VectorR4(-x, -y, -z, -w) ); }
	VectorR4& ArrayProd(const VectorR4&);		// Component-wise product
	VectorR4& ArrayProd3(const VectorR3&);		// Component-wise product

	VectorR4& AddScaled( const VectorR4& u, double s );

	double Norm() const { return ( (double)sqrt( x*x + y*y + z*z +w*w) ); }
	double NormSq() const { return ( x*x + y*y + z*z + w*w ); }
	double Dist( const VectorR4& u ) const;	// Distance from u
	double DistSq( const VectorR4& u ) const;	// Distance from u
	double MaxAbs() const;
	VectorR4& Normalize () { *this /= Norm(); return *this; }	// No error checking
	inline VectorR4& MakeUnit();		// Normalize() with error checking
	inline VectorR4& ReNormalize();
	bool IsUnit( ) const
		{ register double norm = Norm();
		  return ( 1.000001>=norm && norm>=0.999999 ); }
	bool IsUnit( double tolerance ) const
		{ register double norm = Norm();
		  return ( 1.0+tolerance>=norm && norm>=1.0-tolerance ); }
	bool IsZero() const { return ( x==0.0 && y==0.0 && z==0.0 && w==0.0); }
	bool NearZero(double tolerance) const { return( MaxAbs()<=tolerance );}
							// tolerance should be non-negative

	VectorR4& RotateUnitInDirection ( const VectorR4& dir);	// rotate in direction dir

};

inline VectorR4 operator+( const VectorR4& u, const VectorR4& v );
inline VectorR4 operator-( const VectorR4& u, const VectorR4& v ); 
inline VectorR4 operator*( const VectorR4& u, double m); 
inline VectorR4 operator*( double m, const VectorR4& u); 
inline VectorR4 operator/( const VectorR4& u, double m); 
inline bool operator==( const VectorR4& u, const VectorR4& v ); 

inline double operator^ (const VectorR4& u, const VectorR4& v ); // Dot Product
inline double InnerProduct(const VectorR4& u, const VectorR4& v ) { return (u^v); }
inline VectorR4 ArrayProd(const VectorR4& u, const VectorR4& v );

inline double Mag(const VectorR4& u) { return u.Norm(); }
inline double Dist(const VectorR4& u, const VectorR4& v) { return u.Dist(v); }
inline double DistSq(const VectorR4& u, const VectorR4& v) { return u.DistSq(v); }
inline double NormalizeError (const VectorR4& u);

const VectorR4 VectorR4::Zero;

double VectorR4::MaxAbs() const
{
	register double m;
	m = (x>0.0) ? x : -x;
	if ( y>m ) m=y;
	else if ( -y >m ) m = -y;
	if ( z>m ) m=z;
	else if ( -z>m ) m = -z;
	if ( w>m ) m=w;
	else if ( -w>m ) m = -w;
	return m;
}


// ********************************************************************
// Matrix4x4     - base class for 4x4 matrices                        *
// * * * * * * * * * * * * * * * * * * * * * **************************

class Matrix4x4 {

public:
	double m11, m12, m13, m14, m21, m22, m23, m24,
		   m31, m32, m33, m34, m41, m42, m43, m44;	
									
	// Implements a 4x4 matrix: m_i_j - row-i and column-j entry

	static const Matrix4x4 Identity;


public:

	Matrix4x4();
	Matrix4x4( const VectorR4&, const VectorR4&, 
					const VectorR4&, const VectorR4& );	// Sets by columns!
	Matrix4x4( double, double, double, double, 
					 double, double, double, double,
					 double, double, double, double,
					 double, double, double, double );	// Sets by columns

	inline void SetIdentity ();		// Set to the identity map
	inline void SetZero ();			// Set to the zero map
	inline void Set ( const Matrix4x4& );	// Set to the matrix.
	inline void Set( const VectorR4&, const VectorR4&, 
						const VectorR4&, const VectorR4& );
	inline void Set( double, double, double, double,
					 double, double, double, double,
					 double, double, double, double,
					 double, double, double, double );
	inline void SetByRows( const VectorR4&, const VectorR4&, 
						const VectorR4&, const VectorR4& );
	inline void SetByRows( double, double, double, double,
							double, double, double, double,
							double, double, double, double,
							double, double, double, double );
	inline void SetColumn1 ( double, double, double, double );
	inline void SetColumn2 ( double, double, double, double );
	inline void SetColumn3 ( double, double, double, double );
	inline void SetColumn4 ( double, double, double, double );
	inline void SetColumn1 ( const VectorR4& );
	inline void SetColumn2 ( const VectorR4& );
	inline void SetColumn3 ( const VectorR4& );
	inline void SetColumn4 ( const VectorR4& );
	inline VectorR4 Column1() const;
	inline VectorR4 Column2() const;
	inline VectorR4 Column3() const;
	inline VectorR4 Column4() const;

	inline void SetDiagonal( double, double, double, double );
	inline void SetDiagonal( const VectorR4& );
	inline double Diagonal( int );

	inline void MakeTranspose();			 	// Transposes it.
	//void Matrix4x4::operator *= (const Matrix4x4 &B); 	// Matrix product	
	void operator *= (const Matrix4x4 &B); 	// Matrix product	

	Matrix4x4& ReNormalize();

	double Trace() const { return m11+m22+m33+m44; }

};

const Matrix4x4 Matrix4x4::Identity(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
									0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);

void Matrix4x4::operator*= (const Matrix4x4& B)	// Matrix product
{
	double t1, t2, t3;		// temporary values
	t1 =  m11*B.m11 + m12*B.m21 + m13*B.m31 + m14*B.m41;
	t2 =  m11*B.m12 + m12*B.m22 + m13*B.m32 + m14*B.m42;
	t3 =  m11*B.m13 + m12*B.m23 + m13*B.m33 + m14*B.m43;
	m14 = m11*B.m14 + m12*B.m24 + m13*B.m34 + m14*B.m44;
	m11 = t1;
	m12 = t2;
	m13 = t3;

	t1 =  m21*B.m11 + m22*B.m21 + m23*B.m31 + m24*B.m41;
	t2 =  m21*B.m12 + m22*B.m22 + m23*B.m32 + m24*B.m42;
	t3 =  m21*B.m13 + m22*B.m23 + m23*B.m33 + m24*B.m43;
	m24 = m21*B.m14 + m22*B.m24 + m23*B.m34 + m24*B.m44;
	m21 = t1;
	m22 = t2;
	m23 = t3;

	t1 =  m31*B.m11 + m32*B.m21 + m33*B.m31 + m34*B.m41;
	t2 =  m31*B.m12 + m32*B.m22 + m33*B.m32 + m34*B.m42;
	t3 =  m31*B.m13 + m32*B.m23 + m33*B.m33 + m34*B.m43;
	m34 = m31*B.m14 + m32*B.m24 + m33*B.m34 + m34*B.m44;
	m31 = t1;
	m32 = t2;
	m33 = t3;

	t1 =  m41*B.m11 + m42*B.m21 + m43*B.m31 + m44*B.m41;
	t2 =  m41*B.m12 + m42*B.m22 + m43*B.m32 + m44*B.m42;
	t3 =  m41*B.m13 + m42*B.m23 + m43*B.m33 + m44*B.m43;
	m44 = m41*B.m14 + m42*B.m24 + m43*B.m34 + m44*B.m44;
	m41 = t1;
	m42 = t2;
	m43 = t3;
}

inline void ReNormalizeHelper ( double &a, double &b, double &c, double &d )
{
	register double scaleF = a*a+b*b+c*c+d*d;		// Inner product of Vector-R4
	scaleF = 1.0-0.5*(scaleF-1.0);
	a *= scaleF;
	b *= scaleF;
	c *= scaleF;
	d *= scaleF;
}

Matrix4x4& Matrix4x4::ReNormalize() {
	ReNormalizeHelper( m11, m21, m31, m41 );	// Renormalize first column
	ReNormalizeHelper( m12, m22, m32, m42 );	// Renormalize second column
	ReNormalizeHelper( m13, m23, m33, m43 );	// Renormalize third column
	ReNormalizeHelper( m14, m24, m34, m44 );	// Renormalize fourth column
	double alpha = 0.5*(m11*m12 + m21*m22 + m31*m32 + m41*m42);	//1st and 2nd cols
	double beta  = 0.5*(m11*m13 + m21*m23 + m31*m33 + m41*m43);	//1st and 3rd cols
	double gamma = 0.5*(m11*m14 + m21*m24 + m31*m34 + m41*m44);	//1st and 4nd cols
	double delta = 0.5*(m12*m13 + m22*m23 + m32*m33 + m42*m43);	//2nd and 3rd cols
	double eps   = 0.5*(m12*m14 + m22*m24 + m32*m34 + m42*m44);	//2nd and 4nd cols
	double phi   = 0.5*(m13*m14 + m23*m24 + m33*m34 + m43*m44);	//3rd and 4nd cols
	double temp1, temp2, temp3;
	temp1 = m11 - alpha*m12 - beta*m13 - gamma*m14;
	temp2 = m12 - alpha*m11 - delta*m13 - eps*m14;
	temp3 = m13 - beta*m11 - delta*m12 - phi*m14;
	m14 -= (gamma*m11 + eps*m12 + phi*m13);
	m11 = temp1;
	m12 = temp2;
	m13 = temp3;
	temp1 = m21 - alpha*m22 - beta*m23 - gamma*m24;
	temp2 = m22 - alpha*m21 - delta*m23 - eps*m24;
	temp3 = m23 - beta*m21 - delta*m22 - phi*m24;
	m24 -= (gamma*m21 + eps*m22 + phi*m23);
	m21 = temp1;
	m22 = temp2;
	m23 = temp3;
	temp1 = m31 - alpha*m32 - beta*m33 - gamma*m34;
	temp2 = m32 - alpha*m31 - delta*m33 - eps*m34;
	temp3 = m33 - beta*m31 - delta*m32 - phi*m34;
	m34 -= (gamma*m31 + eps*m32 + phi*m33);
	m31 = temp1;
	m32 = temp2;
	m33 = temp3;
	temp1 = m41 - alpha*m42 - beta*m43 - gamma*m44;
	temp2 = m42 - alpha*m41 - delta*m43 - eps*m44;
	temp3 = m43 - beta*m41 - delta*m42 - phi*m44;
	m44 -= (gamma*m41 + eps*m42 + phi*m43);
	m41 = temp1;
	m42 = temp2;
	m43 = temp3;
	return *this;
}


inline VectorR4 operator* ( const Matrix4x4&, const VectorR4& );

ostream& operator<< ( ostream& os, const Matrix4x4& A );






// *****************************************
// LinearMapR4 class                       *
// * * * * * * * * * * * * * * * * * * * * *

class LinearMapR4 : public Matrix4x4 {

public:

	LinearMapR4();
	LinearMapR4( const VectorR4&, const VectorR4&, 
					const VectorR4&, const VectorR4& );	// Sets by columns!
	LinearMapR4( double, double, double, double, 
					 double, double, double, double,
					 double, double, double, double,
					 double, double, double, double );	// Sets by columns
	LinearMapR4 ( const Matrix4x4& );

	inline LinearMapR4& operator+= (const LinearMapR4& );
	inline LinearMapR4& operator-= (const LinearMapR4& );
	inline LinearMapR4& operator*= (double);
	inline LinearMapR4& operator/= (double);
	inline LinearMapR4& operator*= (const Matrix4x4& );	// Matrix product

	inline LinearMapR4 Transpose() const;
	double Determinant () const;			// Returns the determinant
	LinearMapR4 Inverse() const;			// Returns inverse
	LinearMapR4& Invert();					// Converts into inverse.
	VectorR4 Solve(const VectorR4&) const;	// Returns solution
	LinearMapR4 PseudoInverse() const;		// Returns pseudo-inverse TO DO
	VectorR4 PseudoSolve(const VectorR4&);	// Finds least squares solution TO DO
};


double LinearMapR4::Determinant () const		// Returns the determinant
{
	double Tbt34C12 = m31*m42-m32*m41;		// 2x2 subdeterminants
	double Tbt34C13 = m31*m43-m33*m41;
	double Tbt34C14 = m31*m44-m34*m41;
	double Tbt34C23 = m32*m43-m33*m42;
	double Tbt34C24 = m32*m44-m34*m42;
	double Tbt34C34 = m33*m44-m34*m43;

	double sd11 = m22*Tbt34C34 - m23*Tbt34C24 + m24*Tbt34C23;	// 3x3 subdeterminants
	double sd12 = m21*Tbt34C34 - m23*Tbt34C14 + m24*Tbt34C13;
	double sd13 = m21*Tbt34C24 - m22*Tbt34C14 + m24*Tbt34C12;
	double sd14 = m21*Tbt34C23 - m22*Tbt34C13 + m23*Tbt34C12;

	return ( m11*sd11 - m12*sd12 + m13*sd13 - m14*sd14 );
}

LinearMapR4 LinearMapR4::Inverse() const			// Returns inverse
{

	double Tbt34C12 = m31*m42-m32*m41;		// 2x2 subdeterminants
	double Tbt34C13 = m31*m43-m33*m41;
	double Tbt34C14 = m31*m44-m34*m41;
	double Tbt34C23 = m32*m43-m33*m42;
	double Tbt34C24 = m32*m44-m34*m42;
	double Tbt34C34 = m33*m44-m34*m43;
	double Tbt24C12 = m21*m42-m22*m41;		// 2x2 subdeterminants
	double Tbt24C13 = m21*m43-m23*m41;
	double Tbt24C14 = m21*m44-m24*m41;
	double Tbt24C23 = m22*m43-m23*m42;
	double Tbt24C24 = m22*m44-m24*m42;
	double Tbt24C34 = m23*m44-m24*m43;
	double Tbt23C12 = m21*m32-m22*m31;		// 2x2 subdeterminants
	double Tbt23C13 = m21*m33-m23*m31;
	double Tbt23C14 = m21*m34-m24*m31;
	double Tbt23C23 = m22*m33-m23*m32;
	double Tbt23C24 = m22*m34-m24*m32;
	double Tbt23C34 = m23*m34-m24*m33;

	double sd11 = m22*Tbt34C34 - m23*Tbt34C24 + m24*Tbt34C23;	// 3x3 subdeterminants
	double sd12 = m21*Tbt34C34 - m23*Tbt34C14 + m24*Tbt34C13;
	double sd13 = m21*Tbt34C24 - m22*Tbt34C14 + m24*Tbt34C12;
	double sd14 = m21*Tbt34C23 - m22*Tbt34C13 + m23*Tbt34C12;
	double sd21 = m12*Tbt34C34 - m13*Tbt34C24 + m14*Tbt34C23;	// 3x3 subdeterminants
	double sd22 = m11*Tbt34C34 - m13*Tbt34C14 + m14*Tbt34C13;
	double sd23 = m11*Tbt34C24 - m12*Tbt34C14 + m14*Tbt34C12;
	double sd24 = m11*Tbt34C23 - m12*Tbt34C13 + m13*Tbt34C12;
	double sd31 = m12*Tbt24C34 - m13*Tbt24C24 + m14*Tbt24C23;	// 3x3 subdeterminants
	double sd32 = m11*Tbt24C34 - m13*Tbt24C14 + m14*Tbt24C13;
	double sd33 = m11*Tbt24C24 - m12*Tbt24C14 + m14*Tbt24C12;
	double sd34 = m11*Tbt24C23 - m12*Tbt24C13 + m13*Tbt24C12;
	double sd41 = m12*Tbt23C34 - m13*Tbt23C24 + m14*Tbt23C23;	// 3x3 subdeterminants
	double sd42 = m11*Tbt23C34 - m13*Tbt23C14 + m14*Tbt23C13;
	double sd43 = m11*Tbt23C24 - m12*Tbt23C14 + m14*Tbt23C12;
	double sd44 = m11*Tbt23C23 - m12*Tbt23C13 + m13*Tbt23C12;


	register double detInv = 1.0/(m11*sd11 - m12*sd12 + m13*sd13 - m14*sd14);

	return( LinearMapR4( sd11*detInv, -sd12*detInv, sd13*detInv, -sd14*detInv,
						 -sd21*detInv, sd22*detInv, -sd23*detInv, sd24*detInv,
						 sd31*detInv, -sd32*detInv, sd33*detInv, -sd34*detInv,
						 -sd41*detInv, sd42*detInv, -sd43*detInv, sd44*detInv ) );
}

LinearMapR4& LinearMapR4::Invert() 			// Converts into inverse.
{
	double Tbt34C12 = m31*m42-m32*m41;		// 2x2 subdeterminants
	double Tbt34C13 = m31*m43-m33*m41;
	double Tbt34C14 = m31*m44-m34*m41;
	double Tbt34C23 = m32*m43-m33*m42;
	double Tbt34C24 = m32*m44-m34*m42;
	double Tbt34C34 = m33*m44-m34*m43;
	double Tbt24C12 = m21*m42-m22*m41;		// 2x2 subdeterminants
	double Tbt24C13 = m21*m43-m23*m41;
	double Tbt24C14 = m21*m44-m24*m41;
	double Tbt24C23 = m22*m43-m23*m42;
	double Tbt24C24 = m22*m44-m24*m42;
	double Tbt24C34 = m23*m44-m24*m43;
	double Tbt23C12 = m21*m32-m22*m31;		// 2x2 subdeterminants
	double Tbt23C13 = m21*m33-m23*m31;
	double Tbt23C14 = m21*m34-m24*m31;
	double Tbt23C23 = m22*m33-m23*m32;
	double Tbt23C24 = m22*m34-m24*m32;
	double Tbt23C34 = m23*m34-m24*m33;

	double sd11 = m22*Tbt34C34 - m23*Tbt34C24 + m24*Tbt34C23;	// 3x3 subdeterminants
	double sd12 = m21*Tbt34C34 - m23*Tbt34C14 + m24*Tbt34C13;
	double sd13 = m21*Tbt34C24 - m22*Tbt34C14 + m24*Tbt34C12;
	double sd14 = m21*Tbt34C23 - m22*Tbt34C13 + m23*Tbt34C12;
	double sd21 = m12*Tbt34C34 - m13*Tbt34C24 + m14*Tbt34C23;	// 3x3 subdeterminants
	double sd22 = m11*Tbt34C34 - m13*Tbt34C14 + m14*Tbt34C13;
	double sd23 = m11*Tbt34C24 - m12*Tbt34C14 + m14*Tbt34C12;
	double sd24 = m11*Tbt34C23 - m12*Tbt34C13 + m13*Tbt34C12;
	double sd31 = m12*Tbt24C34 - m13*Tbt24C24 + m14*Tbt24C23;	// 3x3 subdeterminants
	double sd32 = m11*Tbt24C34 - m13*Tbt24C14 + m14*Tbt24C13;
	double sd33 = m11*Tbt24C24 - m12*Tbt24C14 + m14*Tbt24C12;
	double sd34 = m11*Tbt24C23 - m12*Tbt24C13 + m13*Tbt24C12;
	double sd41 = m12*Tbt23C34 - m13*Tbt23C24 + m14*Tbt23C23;	// 3x3 subdeterminants
	double sd42 = m11*Tbt23C34 - m13*Tbt23C14 + m14*Tbt23C13;
	double sd43 = m11*Tbt23C24 - m12*Tbt23C14 + m14*Tbt23C12;
	double sd44 = m11*Tbt23C23 - m12*Tbt23C13 + m13*Tbt23C12;

	register double detInv = 1.0/(m11*sd11 - m12*sd12 + m13*sd13 - m14*sd14);

	m11 = sd11*detInv;
	m12 = -sd21*detInv;
	m13 = sd31*detInv;
	m14 = -sd41*detInv;
	m21 = -sd12*detInv;
	m22 = sd22*detInv;
	m23 = -sd32*detInv;
	m24 = sd42*detInv;
	m31 = sd13*detInv;
	m32 = -sd23*detInv;
	m33 = sd33*detInv;
	m34 = -sd43*detInv;
	m41 = -sd14*detInv;
	m42 = sd24*detInv;
	m43 = -sd34*detInv;
	m44 = sd44*detInv;

	return ( *this );
}

VectorR4 LinearMapR4::Solve(const VectorR4& u) const	// Returns solution
{												
	// Just uses Inverse() for now.
	return ( Inverse()*u );
}

	
inline LinearMapR4 operator+ (const LinearMapR4&, const LinearMapR4&);
inline LinearMapR4 operator- (const LinearMapR4&);
inline LinearMapR4 operator- (const LinearMapR4&, const LinearMapR4&);
inline LinearMapR4 operator* ( const LinearMapR4&, double);
inline LinearMapR4 operator* ( double, const LinearMapR4& );
inline LinearMapR4 operator/ ( const LinearMapR4&, double );
inline LinearMapR4 operator* ( const Matrix4x4&, const LinearMapR4& ); 
inline LinearMapR4 operator* ( const LinearMapR4&, const Matrix4x4& ); 
								// Matrix product (composition)


// *******************************************
// RotationMapR4 class                        *
// * * * * * * * * * * * * * * * * * * * * * *

class RotationMapR4 : public Matrix4x4 {

public:

	RotationMapR4();
	RotationMapR4( const VectorR4&, const VectorR4&, 
					const VectorR4&, const VectorR4& );	// Sets by columns!
	RotationMapR4( double, double, double, double, 
					 double, double, double, double,
					 double, double, double, double,
					 double, double, double, double );	// Sets by columns!

	RotationMapR4& SetZero();	// IT IS AN ERROR TO USE THIS FUNCTION!

	inline RotationMapR4& operator*= (const RotationMapR4& );	// Matrix product

	inline RotationMapR4 Transpose() const;
	inline RotationMapR4 Inverse() const { return Transpose(); }; // Returns the transpose
	inline RotationMapR4& Invert() { MakeTranspose(); return *this; };	// Transposes it.
	inline VectorR4 Invert(const VectorR4&) const;	// Returns solution
};
	
inline RotationMapR4 operator* ( const RotationMapR4&, const RotationMapR4& ); 
										// Matrix product (composition)


// ***************************************************************
// * 4-space vector and matrix utilities (prototypes)			 *
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

// Returns the angle between vectors u and v.
//		Use SolidAngleUnit if both vectors are unit vectors
inline double SolidAngle( const VectorR4& u, const VectorR4& v);
inline double SolidAngleUnit( const VectorR4 u, const VectorR4 v );

// Returns a righthanded orthonormal basis to complement vectors u,v,w.
//		The vectors u,v,w must be unit and orthonormal.
void GetOrtho( const VectorR4& u,  RotationMapR4& rotmap );
void GetOrtho( const VectorR4& u,  const VectorR4& v, RotationMapR4& rotmap );
void GetOrtho( const VectorR4& u,  const VectorR4& v, const VectorR4& w,
					RotationMapR4& rotmap );
void GetOrtho( int j, RotationMapR4& rotmap);	// Mainly for internal use

// Projections

inline VectorR4 ProjectToUnit ( const VectorR4& u, const VectorR4& v); 
			// Project u onto v
inline VectorR4 ProjectPerpUnit ( const VectorR4& u, const VectorR4 & v); 
			// Project perp to v
inline VectorR4 ProjectPerpUnitDiff ( const VectorR4& u, const VectorR4& v);
// v must be a unit vector.

// Returns the projection of u onto unit v
inline VectorR4 ProjectToUnit ( const VectorR4& u, const VectorR4& v)
{
	return (u^v)*v;
}

// Returns the projection of u onto the plane perpindicular to the unit vector v
inline VectorR4 ProjectPerpUnit ( const VectorR4& u, const VectorR4& v)
{
	return ( u - ((u^v)*v) );
}

// Returns the projection of u onto the plane perpindicular to the unit vector v
//    This one is more stable when u and v are nearly equal.
inline VectorR4 ProjectPerpUnitDiff ( const VectorR4& u, const VectorR4& v)
{
	VectorR4 ans = u;
	ans -= v;
	ans -= ((ans^v)*v);
	return ans;				// ans = (u-v) - ((u-v)^v)*v
}


// Projection maps (LinearMapR4's)

// VectorProjectMap returns map projecting onto a given vector u.
//		u should be a unit vector (otherwise the returned map is
//		scaled according to the magnitude of u.
inline void VectorProjectMap( const VectorR4& u, LinearMapR4& M )
{
	double a = u.x*u.y;
	double b = u.x*u.z;
	double c = u.x*u.w;
	double d = u.y*u.z;
	double e = u.y*u.w;
	double f = u.z*u.w;
	M.Set( u.x*u.x,     a,     b,		 c,
			  a,    u.y*u.y,   d,        e,
		 	  b,        d,   u.z*u.z,    f,
			  c,        e,     f,     u.w*u.w );
}

inline LinearMapR4 VectorProjectMap( const VectorR4& u )
{
	LinearMapR4 result;
	VectorProjectMap( u, result );
	return result;
}

inline LinearMapR4 PerpProjectMap ( const VectorR4& u );
// u - must be unit vector.  

LinearMapR4 TimesTranspose( const VectorR4& u, const VectorR4& v); // u * v^T.
inline void TimesTranspose( const VectorR4& u, const VectorR4& v, LinearMapR4& M);

// Rotation Maps

RotationMapR4 RotateToMap( const VectorR4& fromVec, const VectorR4& toVec);
// fromVec and toVec should be unit vectors



// ***************************************************************
// * Stream Output Routines	(Prototypes)						 *
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

ostream& operator<< ( ostream& os, const VectorR4& u );


// *****************************************************
// * VectorR4 class - inlined functions				   *
// * * * * * * * * * * * * * * * * * * * * * * * * * * *

inline VectorR4& VectorR4::Load( const double* v ) 
{
	x = *v; 
	y = *(v+1);
	z = *(v+2);
	w = *(v+3);
	return *this;
}

inline VectorR4& VectorR4::Load( const float* v ) 
{
	x = *v; 
	y = *(v+1);
	z = *(v+2);
	w = *(v+3);
	return *this;
}

inline 	void VectorR4::Dump( double* v ) const
{
	*v = x; 
	*(v+1) = y;
	*(v+2) = z;
	*(v+3) = w;
}

inline 	void VectorR4::Dump( float* v ) const
{
	*v = (float)x; 
	*(v+1) = (float)y;
	*(v+2) = (float)z;
	*(v+3) = (float)w;
}

inline VectorR4& VectorR4::MakeUnit ()			// Convert to unit vector (or leave zero).
{
	double nSq = NormSq();
	if (nSq != 0.0) {
		*this /= sqrt(nSq);
	}
	return *this;
}

inline VectorR4 operator+( const VectorR4& u, const VectorR4& v ) 
{ 
	return VectorR4(u.x+v.x, u.y+v.y, u.z+v.z, u.w+v.w ); 
}
inline VectorR4 operator-( const VectorR4& u, const VectorR4& v ) 
{ 
	return VectorR4(u.x-v.x, u.y-v.y, u.z-v.z, u.w-v.w); 
}
inline VectorR4 operator*( const VectorR4& u, register double m) 
{ 
	return VectorR4( u.x*m, u.y*m, u.z*m, u.w*m ); 
}
inline VectorR4 operator*( register double m, const VectorR4& u) 
{ 
	return VectorR4( u.x*m, u.y*m, u.z*m, u.w*m ); 
}
inline VectorR4 operator/( const VectorR4& u, double m) 
{ 
	register double mInv = 1.0/m;
	return VectorR4( u.x*mInv, u.y*mInv, u.z*mInv, u.w*mInv ); 
}

inline bool operator==( const VectorR4& u, const VectorR4& v ) 
{
	return ( u.x==v.x && u.y==v.y && u.z==v.z && u.w==v.w );
}

inline double operator^ ( const VectorR4& u, const VectorR4& v ) // Dot Product
{ 
	return ( u.x*v.x + u.y*v.y + u.z*v.z + u.w*v.w ); 
}

inline VectorR4 ArrayProd ( const VectorR4& u, const VectorR4& v )
{
	return ( VectorR4( u.x*v.x, u.y*v.y, u.z*v.z, u.w*v.w ) );
}

inline VectorR4& VectorR4::ArrayProd (const VectorR4& v)		// Component-wise Product
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
	return ( *this );
}

inline VectorR4& VectorR4::ArrayProd3 (const VectorR3& v)		// Component-wise Product
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return ( *this );
}

inline VectorR4& VectorR4::AddScaled( const VectorR4& u, double s ) 
{
	x += s*u.x;
	y += s*u.y;
	z += s*u.z;
	w += s*u.w;
	return(*this);
}

inline VectorR4& VectorR4::ReNormalize()			// Convert near unit back to unit
{
	double nSq = NormSq();
	register double mFact = 1.0-0.5*(nSq-1.0);	// Multiplicative factor
	*this *= mFact;
	return *this;
}

inline double NormalizeError (const VectorR4& u)
{
	register double discrepancy;
	discrepancy = u.x*u.x + u.y*u.y + u.z*u.z + u.w*u.w - 1.0;
	if ( discrepancy < 0.0 ) {
		discrepancy = -discrepancy;
	}
	return discrepancy;
}

inline VectorR3& VectorR3::SetFromHg(const VectorR4& v) {
	double wInv = 1.0/v.w;
	x = v.x*wInv;
	y = v.y*wInv;
	z = v.z*wInv;
	return *this;
}

inline double VectorR4::Dist( const VectorR4& u ) const 	// Distance from u
{
	return sqrt( DistSq(u) );
}

inline double VectorR4::DistSq( const VectorR4& u ) const	// Distance from u
{
	return ( (x-u.x)*(x-u.x) + (y-u.y)*(y-u.y) + (z-u.z)*(z-u.z) + (w-u.w)*(w-u.w) );
}


// *********************************************************
// * Matrix4x4 class - inlined functions				   *
// * * * * * * * * * * * * * * * * * * * * * * * * * * *****

inline Matrix4x4::Matrix4x4() {}

inline Matrix4x4::Matrix4x4( const VectorR4& u, const VectorR4& v, 
							 const VectorR4& s, const VectorR4& t)
{
	m11 = u.x;		// Column 1
	m21 = u.y;
	m31 = u.z;
	m41 = u.w;
	m12 = v.x;		// Column 2
	m22 = v.y;
	m32 = v.z;
	m42 = v.w;
	m13 = s.x;		// Column 3
	m23 = s.y;
	m33 = s.z;
	m43 = s.w;
	m14 = t.x;		// Column 4
	m24 = t.y;
	m34 = t.z;
	m44 = t.w;
}

inline Matrix4x4::Matrix4x4( double a11, double a21, double a31, double a41,
							 double a12, double a22, double a32, double a42,
							 double a13, double a23, double a33, double a43,
							 double a14, double a24, double a34, double a44)
					// Values specified in column order!!!
{
	m11 = a11;		// Row 1
	m12 = a12;
	m13 = a13;
	m14 = a14;
	m21 = a21;		// Row 2
	m22 = a22;
	m23 = a23;
	m24 = a24;
	m31 = a31;		// Row 3
	m32 = a32;
	m33 = a33;
	m34 = a34;
	m41 = a41;		// Row 4
	m42 = a42;
	m43 = a43;
	m44 = a44;
}

/*
inline Matrix4x4::Matrix4x4 ( const Matrix4x4& A)
	: m11(A.m11), m12(A.m12), m13(A.m13), m14(A.m14),
	  m21(A.m21), m22(A.m22), m23(A.m23), m24(A.m24),
	  m31(A.m31), m32(A.m32), m33(A.m33), m34(A.m34),
	  m41(A.m41), m42(A.m42), m43(A.m43), m44(A.m44) {} */

inline void Matrix4x4::SetIdentity ( )
{
	m11 = m22 = m33 = m44 = 1.0;
	m12 = m13 = m14 = m21 = m23 = m24 = m31 = m32 = m34 = m41= m42 = m43 = 0.0;
}

inline void Matrix4x4::Set( const VectorR4& u, const VectorR4& v, 
							 const VectorR4& s, const VectorR4& t )
{
	m11 = u.x;		// Column 1
	m21 = u.y;
	m31 = u.z;
	m41 = u.w;
	m12 = v.x;		// Column 2
	m22 = v.y;
	m32 = v.z;
	m42 = v.w;
	m13 = s.x;		// Column 3
	m23 = s.y;
	m33 = s.z;
	m43 = s.w;
	m14 = t.x;		// Column 4
	m24 = t.y;
	m34 = t.z;
	m44 = t.w;
}

inline void Matrix4x4::Set( double a11, double a21, double a31, double a41,
							 double a12, double a22, double a32, double a42,
							 double a13, double a23, double a33, double a43,
							 double a14, double a24, double a34, double a44)
					// Values specified in column order!!!
{
	m11 = a11;		// Row 1
	m12 = a12;
	m13 = a13;
	m14 = a14;
	m21 = a21;		// Row 2
	m22 = a22;
	m23 = a23;
	m24 = a24;
	m31 = a31;		// Row 3
	m32 = a32;
	m33 = a33;
	m34 = a34;
	m41 = a41;		// Row 4
	m42 = a42;
	m43 = a43;
	m44 = a44;
}
	
inline void Matrix4x4::Set ( const Matrix4x4& M )	// Set to the matrix.
{
	m11 = M.m11;
	m12 = M.m12;
	m13 = M.m13;
	m14 = M.m14;
	m21 = M.m21;
	m22 = M.m22;
	m23 = M.m23;
	m24 = M.m24;
	m31 = M.m31;
	m32 = M.m32;
	m33 = M.m33;
	m34 = M.m34;
	m41 = M.m41;
	m42 = M.m42;
	m43 = M.m43;
	m44 = M.m44;
}

inline void Matrix4x4::SetZero( ) 
{
	m11 = m12 = m13 = m14 = m21 = m22 = m23 = m24 
		= m31 = m32 = m33 = m34 = m41 = m42 = m43 = m44 = 0.0;
}

inline void Matrix4x4::SetByRows( const VectorR4& u, const VectorR4& v, 
							 const VectorR4& s, const VectorR4& t )
{
	m11 = u.x;		// Row 1
	m12 = u.y;
	m13 = u.z;
	m14 = u.w;
	m21 = v.x;		// Row 2
	m22 = v.y;
	m23 = v.z;
	m24 = v.w;
	m31 = s.x;		// Row 3
	m32 = s.y;
	m33 = s.z;
	m34 = s.w;
	m41 = t.x;		// Row 4
	m42 = t.y;
	m43 = t.z;
	m44 = t.w;
}

inline void Matrix4x4::SetByRows( double a11, double a12, double a13, double a14, 
							 double a21, double a22, double a23, double a24,
							 double a31, double a32, double a33, double a34,
							 double a41, double a42, double a43, double a44 )
					// Values specified in row order!!!
{
	m11 = a11;		// Row 1
	m12 = a12;
	m13 = a13;
	m14 = a14;
	m21 = a21;		// Row 2
	m22 = a22;
	m23 = a23;
	m24 = a24;
	m31 = a31;		// Row 3
	m32 = a32;
	m33 = a33;
	m34 = a34;
	m41 = a41;		// Row 4
	m42 = a42;
	m43 = a43;
	m44 = a44;
}

inline void Matrix4x4::SetColumn1 ( double x, double y, double z, double w)
{
	m11 = x; m21 = y; m31= z; m41 = w;
}

inline void Matrix4x4::SetColumn2 ( double x, double y, double z, double w)
{
	m12 = x; m22 = y; m32= z; m42 = w;
}

inline void Matrix4x4::SetColumn3 ( double x, double y, double z, double w)
{
	m13 = x; m23 = y; m33= z; m43 = w;
}

inline void Matrix4x4::SetColumn4 ( double x, double y, double z, double w)
{
	m14 = x; m24 = y; m34= z; m44 = w;
}

inline void Matrix4x4::SetColumn1 ( const VectorR4& u )
{
	m11 = u.x; m21 = u.y; m31 = u.z; m41 = u.w;
}

inline void Matrix4x4::SetColumn2 ( const VectorR4& u )
{
	m12 = u.x; m22 = u.y; m32 = u.z; m42 = u.w;
}

inline void Matrix4x4::SetColumn3 ( const VectorR4& u )
{
	m13 = u.x; m23 = u.y; m33 = u.z; m43 = u.w;
}

inline void Matrix4x4::SetColumn4 ( const VectorR4& u )
{
	m14 = u.x; m24 = u.y; m34 = u.z; m44 = u.w;
}

VectorR4 Matrix4x4::Column1() const
{
	return ( VectorR4(m11, m21, m31, m41) );
}

VectorR4 Matrix4x4::Column2() const
{
	return ( VectorR4(m12, m22, m32, m42) );
}

VectorR4 Matrix4x4::Column3() const
{
	return ( VectorR4(m13, m23, m33, m43) );
}

VectorR4 Matrix4x4::Column4() const
{
	return ( VectorR4(m14, m24, m34, m44) );
}

inline void Matrix4x4::SetDiagonal( double x, double y, 
								    double z, double w)
{
	m11 = x;
	m22 = y;
	m33 = z;
	m44 = w;
}

inline void Matrix4x4::SetDiagonal( const VectorR4& u )
{
	SetDiagonal ( u.x, u.y, u.z, u.w );
}

inline double Matrix4x4::Diagonal( int i ) 
{
	switch (i) {
	case 0:
		return m11;
	case 1:
		return m22;
	case 2:
		return m33;
	case 3:
		return m44;
	default:
		assert(0);
		return 0.0;
	}
}

inline void Matrix4x4::MakeTranspose()	// Transposes it.
{
	register double temp;
	temp = m12;
	m12 = m21;
	m21=temp;
	temp = m13;
	m13 = m31;
	m31 = temp;
	temp = m14;
	m14 = m41;
	m41 = temp;
	temp = m23;
	m23 = m32;
	m32 = temp;
	temp = m24;
	m24 = m42;
	m42 = temp;
	temp = m34;
	m34 = m43;
	m43 = temp;
}

inline VectorR4 operator* ( const Matrix4x4& A, const VectorR4& u)
{
	VectorR4 ret;
	ret.x = A.m11*u.x + A.m12*u.y + A.m13*u.z + A.m14*u.w;
	ret.y = A.m21*u.x + A.m22*u.y + A.m23*u.z + A.m24*u.w;
	ret.z = A.m31*u.x + A.m32*u.y + A.m33*u.z + A.m34*u.w;
	ret.w = A.m41*u.x + A.m42*u.y + A.m43*u.z + A.m44*u.w;
	return ret;
}


// ******************************************************
// * LinearMapR4 class - inlined functions				*
// * * * * * * * * * * * * * * * * * * * * * * * * * * **

inline LinearMapR4::LinearMapR4()
{
	SetZero();
	return;
}	

inline LinearMapR4::LinearMapR4( const VectorR4& u, const VectorR4& v, 
							 const VectorR4& s, const VectorR4& t)
:Matrix4x4 ( u, v, s ,t )
{ }

inline LinearMapR4::LinearMapR4( 
							 double a11, double a21, double a31, double a41,
							 double a12, double a22, double a32, double a42,
							 double a13, double a23, double a33, double a43,
							 double a14, double a24, double a34, double a44)
					// Values specified in column order!!!
:Matrix4x4 ( a11, a21, a31, a41, a12, a22, a32, a42,
			 a13, a23, a33, a43, a14, a24, a34, a44 )
{ }

inline LinearMapR4::LinearMapR4 ( const Matrix4x4& A )
: Matrix4x4 (A) 
{}

	
inline LinearMapR4& LinearMapR4::operator+= (const LinearMapR4& B)
{
	m11 += B.m11;
	m12 += B.m12;
	m13 += B.m13;
	m14 += B.m14;
	m21 += B.m21;
	m22 += B.m22;
	m23 += B.m23;
	m24 += B.m24;
	m31 += B.m31;
	m32 += B.m32;
	m33 += B.m33;
	m34 += B.m34;
	m41 += B.m41;
	m42 += B.m42;
	m43 += B.m43;
	m44 += B.m44;
	return ( *this );
}

inline LinearMapR4& LinearMapR4::operator-= (const LinearMapR4& B)
{
	m11 -= B.m11;
	m12 -= B.m12;
	m13 -= B.m13;
	m14 -= B.m14;
	m21 -= B.m21;
	m22 -= B.m22;
	m23 -= B.m23;
	m24 -= B.m24;
	m31 -= B.m31;
	m32 -= B.m32;
	m33 -= B.m33;
	m34 -= B.m34;
	m41 -= B.m41;
	m42 -= B.m42;
	m43 -= B.m43;
	m44 -= B.m44;
	return( *this );
}

inline LinearMapR4 operator+ (const LinearMapR4& A, const LinearMapR4& B)
{
	return( LinearMapR4( A.m11+B.m11, A.m21+B.m21, A.m31+B.m31, A.m41+B.m41,
						 A.m12+B.m12, A.m22+B.m22, A.m32+B.m32, A.m42+B.m42,
						 A.m13+B.m13, A.m23+B.m23, A.m33+B.m33, A.m43+B.m43,
						 A.m14+B.m14, A.m24+B.m24, A.m34+B.m34, A.m44+B.m44) );
}

inline LinearMapR4 operator- (const LinearMapR4& A)
{
	return( LinearMapR4( -A.m11, -A.m21, -A.m31, -A.m41,
						 -A.m12, -A.m22, -A.m32, -A.m42,
						 -A.m13, -A.m23, -A.m33, -A.m43,
						 -A.m14, -A.m24, -A.m34, -A.m44 ) );
}

inline LinearMapR4 operator- (const LinearMapR4& A, const LinearMapR4& B)
{
	return( LinearMapR4( A.m11-B.m11, A.m21-B.m21, A.m31-B.m31, A.m41-B.m41,
						 A.m12-B.m12, A.m22-B.m22, A.m32-B.m32, A.m42-B.m42,
						 A.m13-B.m13, A.m23-B.m23, A.m33-B.m33, A.m43-B.m43,
						 A.m14-B.m14, A.m24-B.m24, A.m34-B.m34, A.m44-B.m44 ) );
}

inline LinearMapR4& LinearMapR4::operator*= (register double b)
{
	m11 *= b;
	m12 *= b;
	m13 *= b;
	m14 *= b;
	m21 *= b;
	m22 *= b;
	m23 *= b;
	m24 *= b;
	m31 *= b;
	m32 *= b;
	m33 *= b;
	m34 *= b;
	m41 *= b;
	m42 *= b;
	m43 *= b;
	m44 *= b;
	return ( *this);
}

inline LinearMapR4 operator* ( const LinearMapR4& A, register double b)
{
	return( LinearMapR4( A.m11*b, A.m21*b, A.m31*b, A.m41*b,
						 A.m12*b, A.m22*b, A.m32*b, A.m42*b,
						 A.m13*b, A.m23*b, A.m33*b, A.m43*b,
						 A.m14*b, A.m24*b, A.m34*b, A.m44*b) );
}

inline LinearMapR4 operator* ( register double b, const LinearMapR4& A)
{
	return( LinearMapR4( A.m11*b, A.m21*b, A.m31*b, A.m41*b,
						 A.m12*b, A.m22*b, A.m32*b, A.m42*b,
						 A.m13*b, A.m23*b, A.m33*b, A.m43*b,
						 A.m14*b, A.m24*b, A.m34*b, A.m44*b ) );
}

inline LinearMapR4 operator/ ( const LinearMapR4& A, double b)
{
	register double bInv = 1.0/b;
	return ( A*bInv );
}

inline LinearMapR4& LinearMapR4::operator/= (register double b)
{
	register double bInv = 1.0/b;
	return ( *this *= bInv );
}

inline VectorR4 operator* ( const LinearMapR4& A, const VectorR4& u)
{
	return(VectorR4 ( A.m11*u.x + A.m12*u.y + A.m13*u.z + A.m14*u.w,
					  A.m21*u.x + A.m22*u.y + A.m23*u.z + A.m24*u.w,
					  A.m31*u.x + A.m32*u.y + A.m33*u.z + A.m34*u.w,
					  A.m41*u.x + A.m42*u.y + A.m43*u.z + A.m44*u.w ) ); 
}
	
inline LinearMapR4 LinearMapR4::Transpose() const	// Returns the transpose
{
	return (LinearMapR4( m11, m12, m13, m14, 
						 m21, m22, m23, m24,
						 m31, m32, m33, m34,
						 m41, m42, m43, m44 ) );
}

inline LinearMapR4& LinearMapR4::operator*= (const Matrix4x4& B)	// Matrix product
{
	(*this).Matrix4x4::operator*=(B);

	return( *this );
}

inline LinearMapR4 operator* ( const LinearMapR4& A, const Matrix4x4& B)
{
	LinearMapR4 AA(A);
	AA.Matrix4x4::operator*=(B);
	return AA;
}

inline LinearMapR4 operator* ( const Matrix4x4& A, const LinearMapR4& B)
{
	LinearMapR4 AA(A);
	AA.Matrix4x4::operator*=(B);
	return AA;
}



// ******************************************************
// * RotationMapR4 class - inlined functions			*
// * * * * * * * * * * * * * * * * * * * * * * * * * * **

inline RotationMapR4::RotationMapR4()
{
	SetIdentity();
	return;
}
	
inline RotationMapR4::RotationMapR4( const VectorR4& u, const VectorR4& v, 
							 const VectorR4& s, const VectorR4& t)
:Matrix4x4 ( u, v, s ,t )
{ }

inline RotationMapR4::RotationMapR4( 
							 double a11, double a21, double a31, double a41,
							 double a12, double a22, double a32, double a42,
							 double a13, double a23, double a33, double a43,
							 double a14, double a24, double a34, double a44)
					// Values specified in column order!!!
:Matrix4x4 ( a11, a21, a31, a41, a12, a22, a32, a42,
			 a13, a23, a33, a43, a14, a24, a34, a44 )
{ }

inline RotationMapR4 RotationMapR4::Transpose() const	// Returns the transpose
{
	return ( RotationMapR4( m11, m12, m13, m14, 
							m21, m22, m23, m24,
							m31, m32, m33, m34,
							m41, m42, m43, m44 ) );
}

inline VectorR4 RotationMapR4::Invert(const VectorR4& u) const  // Returns solution
{
	return (VectorR4( m11*u.x + m21*u.y + m31*u.z + m41*u.w,
					  m12*u.x + m22*u.y + m32*u.z + m42*u.w,
					  m13*u.x + m23*u.y + m33*u.z + m43*u.w,
					  m14*u.x + m24*u.y + m34*u.z + m44*u.w ) );
}

inline RotationMapR4& RotationMapR4::operator*= (const RotationMapR4& B)  // Matrix product
{
	(*this).Matrix4x4::operator*=(B);

	return( *this );
}

inline RotationMapR4 operator* ( const RotationMapR4& A, const RotationMapR4& B)
{
	RotationMapR4 AA(A);
	AA.Matrix4x4::operator*=(B);
	return AA;
}


// ******************************************************
// * RigidMapR4 class - inlined functions				*
// * * * * * * * * * * * * * * * * * * * * * * * * * * **

inline void Matrix3x4::Transform( VectorR4* u ) const {
	double newX, newY;
	newX = m11*u->x + m12*u->y + m13*u->z + m14*u->w;
	newY = m21*u->x + m22*u->y + m23*u->z + m24*u->w;
	u->z = m31*u->x + m32*u->y + m33*u->z + m34*u->w;
	u->x = newX;
	u->y = newY;
	// u->w value does not change
}

inline void Matrix3x4::Transform( const VectorR4& src, VectorR4* dest ) const {
	dest->x = m11*src.x + m12*src.y + m13*src.z + m14*src.w;
	dest->y = m21*src.x + m22*src.y + m23*src.z + m24*src.w;
	dest->z = m31*src.x + m32*src.y + m33*src.z + m34*src.w;
	dest->w = src.w;
}

// ***************************************************************
// * 4-space vector and matrix utilities (inlined functions)	 *
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

inline void TimesTranspose( const VectorR4& u, const VectorR4& v, LinearMapR4& M)
{
	M.Set ( v.x*u.x, v.x*u.y, v.x*u.z, v.x*u.w,			// Set by columns!
			v.y*u.x, v.y*u.y, v.y*u.z, v.y*u.w,
			v.z*u.x, v.z*u.y, v.z*u.z, v.z*u.w,
			v.w*u.x, v.w*u.y, v.w*u.z, v.w*u.w ) ;
}

// Returns the solid angle between vectors u and v (not necessarily unit vectors)
inline double SolidAngle( const VectorR4& u, const VectorR4& v)
{
	double nSqU = u.NormSq();
	double nSqV = v.NormSq();
	if ( nSqU==0.0 && nSqV==0.0 ) {
		return (0.0);
	}
	else {
		return ( SolidAngleUnit( u/sqrt(nSqU), v/sqrt(nSqV) ) );
	}
}

inline double SolidAngleUnit( const VectorR4 u, const VectorR4 v )
{
	return ( atan2 ( ProjectPerpUnit(v,u).Norm(), u^v ) );
}


#endif	// LINEAR_R4_H
