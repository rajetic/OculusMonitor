////////////////////////////////////////////////////////////
// KF - Kojack Framework
// Copyright (C) 2018 Kojack (rajetic@gmail.com)
//
// KF is released under the MIT License  
// https://opensource.org/licenses/MIT
////////////////////////////////////////////////////////////

#ifndef KF_TIME_HEADER
#define KF_TIME_HEADER

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include "windows.h"

namespace kf
{
	class Time
	{
	public:
		LARGE_INTEGER m_frequencyHz;
		double m_frequencySec;
		LARGE_INTEGER m_startTime;

		Time()
		{
			QueryPerformanceCounter(&m_startTime);
			QueryPerformanceFrequency(&m_frequencyHz);
			m_frequencySec = 1.0 / m_frequencyHz.QuadPart;
		}

		double getTime()
		{
			return getTicks() * m_frequencySec;
		}

		double deltaTime()
		{
			return deltaTicks() * m_frequencySec;
		}

		long long deltaTicks()
		{
			LARGE_INTEGER current;
			long long delta;
			QueryPerformanceCounter(&current);
			delta = current.QuadPart - m_startTime.QuadPart;
			m_startTime = current;
			return delta;
		}

		long long getTicks()
		{
			LARGE_INTEGER current;
			QueryPerformanceCounter(&current);
			return (current.QuadPart - m_startTime.QuadPart);
		}

		double ticksToSeconds(long long t)
		{
			return t*m_frequencySec;
		}

		long long secondsToTicks(double t)
		{
			return t*m_frequencyHz.QuadPart;
		}

		void reset()
		{
			QueryPerformanceCounter(&m_startTime);
		}
	};
}

#endif

