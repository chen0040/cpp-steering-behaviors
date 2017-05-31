#include "GLRay.h"
#include "../GLFloor/GLFloor.h"
#include "../GLShape/GLSphere.h"
#include <cmath>
#include <iostream>
#include <cassert>

GLRay::GLRay()
{

}

GLRay::~GLRay()
{

}

GLRay::GLRay(const GLRay& rhs)
{
	this->direction=rhs.direction;
	this->origin=rhs.origin;
}

GLRay& GLRay::operator=(const GLRay& rhs)
{
	this->direction=rhs.direction;
	this->origin=rhs.origin;

	return *this;
}

bool GLRay::intersect(const GLSphere& sphere, GLVector& ip1, GLVector& ip2, double& closest_distance) const
{
	// Ray Equation Q=P + td where P is the origin and d is the direction of the ray
	// Sphere Equation (Q - C) dot (Q - C) = R^2 where C is the center and R is the radius
	// for intersection (P+td - C) dot (P + td - C) = R^2 
	// let m=P - C, then (m + td) dot (m + td) = R^2
	// t^2 + 2 (m dot d) t + m dot m = R^2
	// let h = (m dot d) and c = (m dot m - R^2)
	// t^2 + 2 h t + c =0 is a quadratic equation.
	// Let Det= ( h^2 - c)
	// then t = - h + sqrt(Det) or t = - h - sqrt(Det)
	// if Det < 0, no intersection
	// if t < 0, no intersection

	const GLVector& P=this->origin;
	const GLVector& d=this->direction;

	const GLVector& C=sphere.center;
	double R=sphere.radius;

	GLVector m=P - C;
	double h=m.dot(d);
	double c=m.dot(m) - R * R;

	double Det = h*h - c;

	if(Det < 0)
	{
		return false;
	}
	if(Det == 0)
	{
		double t= -h;
		if(t < 0)
		{
			return false;
		}
		ip1=ip2=P + d * t;
		closest_distance = (ip1 - P).length();
		return true;
	}

	double t1=-h + sqrt(Det);
	double t2=-h - sqrt(Det);

	if(t1 < 0 && t2 < 0)
	{
		return false;
	}

	ip1=P + d * t1;
	ip2=P + d * t2;

	double d1=(ip1 - P).length();
	double d2=(ip2 - P).length();

	if(d1 < d2)
	{
		closest_distance=d1;
	}
	else
	{
		closest_distance=d2;
	}

	return true;
}

bool GLRay::intersect(const GLFloor* plane, GLVector& intersection_point) const
{
	// Ray Equation P=S + td where S is the origin and d is the directional vector
	// Plane Equation n dot P=D where D is the distance to the origin and n is the normal of the plane
	// for intersection n dot (S+td)=D
	// t= (D - n dot S) / n dot d

	GLVector n=plane->normal();
	double D=plane->distance_to_origin();

	double denom=n.dot(this->direction);

	if(denom==0)
	{
		return false;
	}

	double t=(D - n.dot(this->origin)) / denom;

	if(t < 0)
	{
		return false; 
	}

	intersection_point=this->origin + this->direction * t;

	return true;
}

bool GLRay::intersect(const GLSphere& sphere, double& closest_distance) const
{
	GLVector pt1, pt2;
	return intersect(sphere, pt1, pt2, closest_distance);
}
