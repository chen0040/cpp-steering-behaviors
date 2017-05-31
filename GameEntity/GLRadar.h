#ifndef _H_GL_RADAR_H
#define _H_GL_RADAR_H

#include "Obstacle.h"

class GLRadar : public Obstacle
{
public:
	GLRadar(GameWorld* pWorld);
	virtual ~GLRadar();

public:
	virtual void Update(const long& ElapsedTicks);

public:
	void SetMaxScanRadius(double r);
	void SetScanSpeed(double s);
	void SetTransmisionPoint(const GLVector& pt);

public:
	double m_radius;
	double m_max_radius;
	double m_scan_speed;
	GLVector m_transmission_point;
};
#endif