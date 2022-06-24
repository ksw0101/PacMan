#include "Timer.h"

int Timer::GetTime()
{
	m_timer = time(NULL);
	m_tm = localtime(&m_timer);
	return m_tm->tm_hour;
}
