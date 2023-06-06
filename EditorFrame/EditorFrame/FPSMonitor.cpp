/* Copyright (c) 2023 GameMart Inc. */
#include "FPSMonitor.hpp"

using namespace std::chrono;

FPSMonitor::FPSMonitor()
{
	m_startTime = steady_clock::now();
}

bool FPSMonitor::update()
{
	m_numFrames++;
	auto nowTime = steady_clock::now();
	if (nowTime - m_startTime < seconds(1))
		return false;

	m_lastFPS = m_numFrames;
	m_numFrames = 0;
	m_startTime = nowTime;


	return true;
}

int FPSMonitor::getFPS() const
{
	return m_lastFPS;
}
