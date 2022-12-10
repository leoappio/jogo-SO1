#ifndef _VECTOR_H
#define _VECTOR_H

#include "traits.h"

__BEGIN_API
class Point;
struct Vector {
   double x;
   double y;

   
   Vector();
   Vector(double a, double b);
   ~Vector();

   
   /**
    *  @fn Vector operator * (double scalar);
    *  @brief takes a scalar value and multiplies x and y by it
    *  @param scalar 
    *  @return Vector
    */
   Vector operator * (double scalar);
   Vector operator + (Vector v);   
   void rollRandom();
   void rollReallyRandom();    
   
   
   // range [-200, 200]
   void rollRandomY();
   // range [-200,-1]
   void rollRandomX();   
   void reflectY();
   void reflectX();
   void flip();
   void Angle(Point p, Point p2, double time);
   
};

__END_API
#endif
