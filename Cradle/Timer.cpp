#include "Timer.h"

Timer::Timer()
{
	if (!QueryPerformanceFrequency(&m_frequency))
	{
		// throw exception
	}

	Reset();
}

Timer::~Timer()
{

}

void Timer::Reset()
{
	Update();

	m_startTime = m_currentTime;
	m_total	= 0.0;
	m_delta = 1.0 / 60.0;
}

void Timer::Update()
{
	if (!QueryPerformanceCounter(&m_currentTime))
	{
		// Throw exception
	}

	m_total = static_cast<double>(
			static_cast<double>(m_currentTime.QuadPart - m_startTime.QuadPart) /
			static_cast<double>(m_frequency.QuadPart)
			);

	if (m_lastTime.QuadPart == m_startTime.QuadPart)
	{
		m_delta = 1.0/60.0;
	}
	else
	{
		m_delta = static_cast<double>(
				static_cast<double>(m_currentTime.QuadPart - m_lastTime.QuadPart) / 
				static_cast<double>(m_frequency.QuadPart)
				);

	}

	m_lastTime = m_currentTime;
}

