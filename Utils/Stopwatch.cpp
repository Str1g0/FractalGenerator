#include "Stopwatch.h"

void Misc::Stopwatch::Start()
{
	m_start = clock::now();
}

void Misc::Stopwatch::Stop()
{
	m_duration = clock::now() - m_start;
}

Misc::clock::duration Misc::Stopwatch::GetTime()
{
	return m_duration;
}
