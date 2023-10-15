#pragma once
//フレームレート
#include <chrono>
class FPS
{
private:

	std::chrono::steady_clock::time_point count;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="Fps"></param>
	/// <param name="flag"></param>
	void Update(float Fps = 60.0f, bool flag = true);



};

