#pragma once
#include"Vector3.h"
#include"Vector2.h"
#include"Float4.h"
#include"Matrix4.h"
#include<cmath>



constexpr float PI = 3.1415926535897932384f;

namespace myMath
{


	Vector3 lerp(Vector3 vector, Vector3 vector2, float t);

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

