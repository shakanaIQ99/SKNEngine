#pragma once
#include"Vector3.h"
#include"Vector2.h"
#include"Float4.h"
#include"Matrix4.h"
#include<cmath>



constexpr float MY_PI = 3.1415926535897932384f;
constexpr float F_Max = 1.000000f;
constexpr float F_Min = -1.000000f;


namespace myMath
{
	template <typename t>

	//正負を返す
	t sign(t A) 
	{
		return static_cast<t>((A > 0) - (A < 0));
	}


	//double 

	/// <summary>
	/// 最短角度補間
	/// </summary>
	float LerpShortAngle(float a, float b, float t);


	//DegreeからRadianへ変換する
	float AngleToRadian(float angle);

	//RadianからDegreeへ変換する
	float RadianToAngle(float radian);

}

