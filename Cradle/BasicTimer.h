#ifndef _BASICTIMER_
#define _BASICTIMER_

#include <Windows.h>

class BasicTimer
{
private:
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_currentTime;
	LARGE_INTEGER m_startTime;
	LARGE_INTEGER m_lastTime;
	float m_delta;
	float m_total;

public:
	BasicTimer();
	void Reset();
	void Update();
	float GetTotal();
	float GetDelta();

};


#endif