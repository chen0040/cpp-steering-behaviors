#ifndef _H_GL_SPHERICAL_CAMERA_H
#define _H_GL_SPHERICAL_CAMERA_H

#include "GLCamera.h"

class GLSphericalCamera : public GLCamera
{
public:
	GLSphericalCamera();
	virtual ~GLSphericalCamera();

public:
	virtual void Transform();
	virtual void PreTransform();
	virtual void PostTransform();

public:
	virtual void Scroll(const GLVector& delta);

public:
	virtual void Reset();
	virtual void SetFocus(const GLVector& focus);

protected:
	virtual void calculate_vectors();

private:
	double m_radius;
	double m_min_radius;
public:
	void SetMinRadius(double r) { m_min_radius=r; }
	void SetRadius(double r) { m_radius=r; }
	double GetRadius() const { return m_radius; }
	void IncrementRadius(double dr);

private:
	double m_theta;
	double m_min_theta;
	double m_max_theta;
public:
	void SetTheta(double t) { m_theta= t; }
	void SetMaxTheta(double t) { m_max_theta=t; }
	void SetMinTheta(double t) { m_min_theta=t; }
	double GetTheta() const { return m_theta; }
	void IncrementTheta(double t);

private:
	double m_beta;
	double m_min_beta;
	double m_max_beta;
public:
	void SetBeta(double b) { m_beta=b; }
	void SetMaxBeta(double b) { m_max_beta=b; }
	void SetMinBeta(double b) { m_min_beta=b; }
	double GetBeta() const { return m_beta; }
	void IncrementBeta(double db);

protected:
	GLVector m_min_focus;
	GLVector m_max_focus;

public:
	void SetCameraCenterConstraints(const GLVector& min, const GLVector& max);
};
#endif