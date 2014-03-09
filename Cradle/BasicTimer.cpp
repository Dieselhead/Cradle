#include "BasicTimer.h"

BasicTimer::BasicTimer()
{
	if (!QueryPerformanceFrequency(&m_frequency))
	{
		// TODO
		// Error
	}

	Reset();
}

void BasicTimer::Reset()
{
	Update();
	m_startTime = m_currentTime;
	m_total = 0.0f;
	m_delta = 1.0f / 60.0f;
}

void BasicTimer::Update()
{
	if (!QueryPerformanceCounter(&m_currentTime))
	{
		// TODO
		// Error
	}

	m_total = static_cast<float>(
		static_cast<double>(m_currentTime.QuadPart - m_startTime.QuadPart) /
		static_cast<double>(m_frequency.QuadPart));

	if (m_lastTime.QuadPart == m_startTime.QuadPart)
	{
		m_delta = 1.0f / 60.0f;
	}
	else
	{
		m_delta = static_cast<float>(
			static_cast<double>(m_currentTime.QuadPart - m_lastTime.QuadPart) /
			static_cast<double>(m_frequency.QuadPart));
	}

	m_lastTime = m_currentTime;
}

float BasicTimer::UpdateFoo()
{
	if (!QueryPerformanceCounter(&m_foo))
	{
		// Error
	}

	m_fooTotal = static_cast<float>(
		static_cast<double>(m_foo.QuadPart - m_startTime.QuadPart) /
		static_cast<double>(m_frequency.QuadPart));

	return GetFoo();
}

float BasicTimer::GetFoo()
{
	return m_fooTotal;
}

float BasicTimer::GetTotal()
{
	return m_total;
}

float BasicTimer::GetDelta()
{
	return m_delta;
}

