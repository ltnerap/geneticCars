#include "vector.h"
#include <math.h>

//constructor
Vector::Vector(float xx, float yy)
{
	x=xx; y=yy;
}

//add two vectors together
Vector* Vector::add(Vector* v)
{
	Vector *vnew=new Vector(x,y);
	vnew->x+=v->x;
	vnew->y+=v->y;
	return vnew;
}

//subtract two vectors
Vector* Vector::sub(Vector* v)
{
	Vector *vnew=new Vector(x,y);
	vnew->x-=v->x;
	vnew->y-=v->y;
	return vnew;
}

//multiply a vector by a float
Vector* Vector::mul(float f)
{
	Vector *vnew=new Vector(x,y);
	vnew->x*=f;
	vnew->y*=f;
	return vnew;
}

//take dot product of two vectors
float Vector::dot(Vector* v)
{
	return x*v->x+y*v->y;
}

//normalize vector
Vector* Vector::normalize()
{
	Vector *vnew=new Vector(x,y);
	vnew->x=x/sqrt(x*x+y*y);	
	vnew->y=y/sqrt(x*x+y*y);
	return vnew;
}

//return length of vector
float Vector::length()
{
	return sqrt(x*x+y*y);
}

//return length squared of vector
float Vector::lengthsquared()
{
	return (x*x+y*y);
}

//return angle of vector
float Vector::angle()
{
	return atan2(y,x);
}
