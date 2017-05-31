#ifndef _H_GL_PROJECTILE_H
#define _H_GL_PROJECTILE_H

#include "GLEmitter.h"

class GLProjectile : GLEmitter
{
public:
	GLProjectile(double life);
	virtual ~GLProjectile();

public:
	virtual void Update(const long& lElapsedTicks);
	void set_broadcast_period(int period);

public:
	virtual bool IsDead() const;

public:
	int m_broadcast_counter;
	int m_broadcast_period;
	double m_radius;
	double m_life_span;
};
#endif
