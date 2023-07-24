#include "FPS.h"
#include<thread>

void FPS::Initialize()
{
	count = std::chrono::steady_clock::now();
}

void FPS::Update(float Fps, bool flag)
{
	
	std::chrono::microseconds minTime(uint64_t(1000000.0f / Fps));
	
	std::chrono::microseconds minCheckTime(uint64_t(1000000.0f / (Fps + 5.0f)));

	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	
	std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - count);
	
	if (flag == false)
	{
		if (elapsed < minTime)
		{
			while (std::chrono::steady_clock::now() - count < minTime)
			{
				std::this_thread::sleep_for(std::chrono::microseconds(1));
			}
		}
	}
	else
	{
		if (elapsed < minCheckTime)
		{
			while (std::chrono::steady_clock::now() - count < minCheckTime)
			{
				std::this_thread::sleep_for(std::chrono::microseconds(1));
			}
		}
	}
	count = std::chrono::steady_clock::now();



}
