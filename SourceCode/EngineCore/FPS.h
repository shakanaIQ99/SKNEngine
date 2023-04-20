#pragma once
#include <chrono>
class FPS
{
private:

	std::chrono::steady_clock::time_point count;

public:

	void Initialize();

	
	void Update(float Fps = 60.0f, bool flag = true);



};

