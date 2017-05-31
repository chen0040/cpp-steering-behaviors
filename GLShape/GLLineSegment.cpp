#include "GLLineSegment.h"
#include "../GLMath/GLMath.h"
#include "GLPlane.h"
#include <cmath>

GLLineSegment::GLLineSegment(const GLVector &_from, const GLVector &_to)
: from(_from)
, to(_to)
{
}

GLLineSegment::GLLineSegment(const GLLineSegment &rhs)
{
	from=rhs.from;
	to=rhs.to;
}

GLLineSegment& GLLineSegment::operator =(const GLLineSegment &rhs)
{
	from=rhs.from;
	to=rhs.to;
	return *this;
}

bool GLLineSegment::intersect(const GLPlane& plane, GLVector& intersection_point, double & distance)
{
	// segment: P=line.from + t * (line.to - line.from)  0 <= t <= 1
	// plane: P dot n = D
	// for intersection: (line.from + t * (line.to - line.from)) dot n=D
	// t= [ D - n dot line.from ] / [ (line.to - line.from) dot n ]
	double denom=(this->to - this->from).dot(plane.normal);
	if(denom==0)
	{
		return false;
	}
	double t=(plane.D - plane.normal.dot(this->from)) / denom;
	if( t > 1 || t< 0)
	{
		return false;
	}

	intersection_point=this->from + t * (this->to - this->from);
	distance=(this->to - intersection_point).length();

	return true;
}

bool GLLineSegment::intersect(const GLLineSegment& line2, GLVector& intersection_point, double& distance)
{
	// segment1: s1=line1.from + t1 * (line1.to - line1.from)
	// segment2: s2=line2.from + t2 * (line2.to - line2.from)
	// for intersection
	// s1=s2
	// solve three simultaneous equation for t1 and t2
	// if no solution, no intersection
	// if infinite solution, coincide
	// otherwise one solution for the intersection point
	// line1.from.x + t1 * (line1.to.x - line1.from.x) = line2.from.x + t2 * (line2.to.x - line2.from.x) -- (1)
	// line1.from.y + t1 * (line1.to.y - line1.from.y) = line2.from.y + t2 * (line2.to.y - line2.from.y) -- (2)
	// line1.from.z + t1 * (line1.to.z - line1.from.z) = line2.from.z + t2 * (line2.to.z - line2.from.z) -- (3)
	// from (1)
	// t1=[ (line2.from.x - line1.from.x) + t2 * (line2.to.x - line2.from.x) ] / (line1.to.x - line1.from.x) -- (4)
	// sub (4) into (3)
	// (line1.to.z - line1.from.z) * [ (line2.from.x - line1.from.x) + t2 * (line2.to.x - line2.from.x) ] / (line1.to.x - line1.from.x) 
	// = (line2.from.z - line1.from.z) +  t2 * (line2.to.z - line2.from.z) 
	//  t2 * [ (line1.to.z - line1.from.z) (line2.to.x - line2.from.x) / (line1.to.x - line1.from.x) -  (line2.to.z - line2.from.z) ]
	// = (line2.from.z - line1.from.z)  - (line1.to.z - line1.from.z) (line2.from.x - line1.from.x) / (line1.to.x - line1.from.x) 
	// t2 *  [ (line1.to.z - line1.from.z) (line2.to.x - line2.from.x) - (line1.to.x - line1.from.x) (line2.to.z - line2.from.z) ] 
	// = [ (line2.from.z - line1.from.z) (line1.to.x - line1.from.x) - (line1.to.z - line1.from.z) (line2.from.x - line1.from.x) ] 
	// let denom = [ (line1.to.z - line1.from.z) (line2.to.x - line2.from.x) - (line1.to.x - line1.from.x) (line2.to.z - line2.from.z) ] 
	// let num2= [ (line2.from.z - line1.from.z) (line1.to.x - line1.from.x) - (line1.to.z - line1.from.z) (line2.from.x - line1.from.x) ] 
	// t2 = num2 / denom
	// t1 = num2 / denom * (line2.to.x - line2.from.x) / (line1.to.x - line1.from.x) + (line2.from.x - line1.from.x) / (line1.to.x - line1.from.x)
	// t1 = 
	//	{ 
	//			[ (line2.from.z - line1.from.z) (line1.to.x - line1.from.x) (line2.to.x - line2.from.x) ]
	//				- 
	//			[ (line2.from.x - line1.from.x)  (line1.to.x - line1.from.x) (line2.to.z - line2.from.z) ] 
	//	} 
	//	/ 
	//	(line1.to.x - line1.from.x)
	// let num1= [ (line2.from.z - line1.from.z) (line2.to.x - line2.from.x) - (line2.from.x - line1.from.x)  (line2.to.z - line2.from.z) ]
	// t1=num1 / denom

	double denom= (this->to.z - this->from.z) * (line2.to.x - line2.from.x) - (this->to.x - this->from.x) * (line2.to.z - line2.from.z);
	if(denom==0)
	{
		return false;
	}

	double num1= (line2.from.z - this->from.z) * (line2.to.x - line2.from.x) - (line2.from.x - this->from.x) * (line2.to.z - line2.from.z);
	double num2= (line2.from.z - this->from.z) * (this->to.x - this->from.x) - (line2.from.x - this->from.x) * (this->to.z - this->from.z);

	double t1=num1 / denom;
	double t2=num2 / denom;

	if(t1 < 0 || t1>1 || t2<0 || t2>1)
	{
		return false;
	}

	double lhs=this->from.y + t1 * (this->to.y - this->from.y);
	double rhs=line2.from.y + t2 * (line2.to.y - line2.from.y);

	if(abs(lhs-rhs) < glMath.Epsilon())
	{
		intersection_point=this->from + t1 * (this->to - this->from);
		distance = (intersection_point - this->from).length();
		return true;
	}

	return false;
}