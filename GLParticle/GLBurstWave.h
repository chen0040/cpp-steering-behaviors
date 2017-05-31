#ifndef _H_GL_BURST_WAVE_H
#define _H_GL_BURST_WAVE_H

#include "GLEmitter.h"

class GameEntity;

class GLBurstWave : public GLEmitter
{
public:
	GLBurstWave(double life_span);
	virtual ~GLBurstWave();

public:
	virtual void Render();
	virtual void Update(const long& lElapsedTicks);

public:
	void SetBroadcastFrequency(double frequency);
	void SetBroadcastCountPerWave(int count);
	void SetPacketProperty(double packet_travelling_distance, double packet_travelling_speed);
	void SetCoreRadius(double radius) { m_core_radius=radius; }

protected:
	void Broadcast();

public:
	virtual bool IsDead() const { return m_life <= 0; }

protected:
	double m_core_radius;
	int m_broadcast_period; // in millisecond;
	int m_broadcast_counter;
	int m_broadcast_count_per_wave;
	double m_packet_travelling_life_span;
	double m_packet_travelling_speed;
	int m_life; // in millisecond;
};
#endif