#ifndef _TIMER_
#define _TIMER_

#include <Windows.h>

class Timer
{
private:
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_currentTime;
	LARGE_INTEGER m_startTime;
	LARGE_INTEGER m_lastTime;

	double m_total;
	double m_delta;

public:
	Timer();
	~Timer();

	void Reset();
	void Update();

	double GetTotal() { return m_total; }
	double GetDelta() { return m_delta; }
};

#endif