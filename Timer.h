#pragma once
#include "pch.h"
#include <chrono>
class Timer
{
	std::chrono::time_point<std::chrono::steady_clock> t = std::chrono::steady_clock::now();
public:
	~Timer()
	{
		std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t).count() << "ms\n";
	}
};