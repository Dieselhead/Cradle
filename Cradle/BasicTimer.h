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



	LARGE_INTEGER m_foo;
	float m_fooTotal;

public:
	BasicTimer();
	void Reset();
	void Update();
	float GetTotal();
	float GetDelta();

	float UpdateFoo();
	float GetFoo();
};


#endif