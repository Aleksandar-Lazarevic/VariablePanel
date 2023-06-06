/* Copyright (c) 2023 GameMart inc. */
#pragma once

#include <chrono>

class FPSMonitor{
public:	
	FPSMonitor();
	bool update();
	int getFPS() const;
private:
	std::chrono::steady_clock::time_point m_startTime;
	int m_numFrames = 0;
	int m_lastFPS=0;
};