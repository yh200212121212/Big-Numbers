#pragma once

#include <math.h>
#include "MathLib.h"

class Point3D {                 // A point in 3-dimensional space
public:
  double x, y, z;
  inline Point3D() {
  }
  inline Point3D(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {
  }
  inline double length() const {
    return sqrt(x*x + y*y + z*z);
  }

  Point3D &normalize() {
    const double l = sqrt(x*x+y*y+z*z);
    if(l != 0.0) {
	  x/=l; y/=l; z/=l;
    }
    return *this;
  }

  inline Point3D &operator+=(const Point3D &p) {
    x += p.x; y += p.y; z += p.z; return *this;
  }
  inline Point3D &operator-=(const Point3D &p) {
    x -= p.x; y -= p.y; z -= p.z; return *this;
  }
  inline double operator*(const Point3D &p) const {
    return x*p.x + y*p.y + z*p.z;
  };
  inline Point3D &operator*=(double d) {
    x *= d; y *= d; z *= d; return *this;
  }
  inline Point3D &operator/=(double d) {
    x /= d; y /= d; z /= d; return *this;
  }
  inline Point3D operator+(const Point3D &p) const {
    return Point3D(x + p.x, y + p.y, z + p.z);
  }
  inline Point3D operator-(const Point3D &p) const {
    return Point3D(x - p.x, y - p.y, z - p.z);
  }
  inline Point3D operator*(double d) const {
    return Point3D(x * d, y * d, z * d);
  }
  inline Point3D operator/(double d) const {
    return Point3D(x / d, y / d, z / d);
  }
  inline Point3D operator-() const {
    return Point3D(-x, -y, -z);
  }
  inline double distance(const Point3D &p) const {
    return sqrt(sqr(x-p.x)+sqr(y-p.y)+sqr(z-p.z));
  }
  /*
  inline operator D3DXVECTOR3() const {
    return D3DXVECTOR3((float)x,(float)y,(float)z);
  }
  */
  inline bool operator<(const Point3D &p) const {
    return (x < p.x) && (y < p.y) && (z < p.z);
  }
  inline bool operator<=(const Point3D &p) const {
    return (x <= p.x) && (y <= p.y) && (z <= p.z);
  }

  inline String toString(int dec = 3) const {
    const int n = dec+3;
    return format(_T("(% *.*lf,% *.*lf,% *.*lf)"), n,dec,x, n,dec,y, n,dec,z);
  }
};

double angle(const Point3D &p1, const Point3D &p2);  // angle in radians between p1 and p2

typedef FunctionTemplate<Point3D, Point3D> Point3DFunction;
typedef FunctionTemplate<Point3D, Real>    Function3D;
