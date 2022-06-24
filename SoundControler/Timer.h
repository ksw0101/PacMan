#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <ctime>
class Timer
{
	time_t	m_timer;
	tm*		m_tm;
public:

	int GetTime();

};

