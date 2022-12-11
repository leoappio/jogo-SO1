#ifndef _POINT_H
#define _POINT_H

#include <cmath>
#include <ctime>
#include <cstdlib>
#include "threads.h"
#include "traits.h"

__BEGIN_API
struct Vector;
struct Point {
   double x;
   double y;
   Point();
   Point(double a, double b);
   ~Point();
   Point operator+(Vector v);
   Point operator+(Point p);
   bool operator==(Point p);
   double distance(Point p);
   void rollRandom();
   void rollRandomX();
   void rollRandomY();
   
};

__END_API

#endif
