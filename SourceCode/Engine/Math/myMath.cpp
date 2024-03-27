#include "myMath.h"



float myMath::LerpShortAngle(float a, float b, float t)
{

	//角度差分
	float diff = b - a;

	if (diff > AngleToRadian(360.0f)|| diff < AngleToRadian(-360.0f))
	{
		diff = static_cast<float>(std::fmod(static_cast<double>(diff), static_cast<double>(AngleToRadian(360.0f))));
	}

	if (diff > AngleToRadian(180.0f))
	{
		diff -= AngleToRadian(360.0f);

	}
	else if(diff< AngleToRadian(-180.0f))
	{
		diff += AngleToRadian(360.0f);
	}

	return a + diff * t;
	





	//return 0.0f;
}

float myMath::AngleToRadian(float angle)
{
	return angle * (MY_PI / 180.0f);
}

float myMath::RadianToAngle(float radian)
{
	return radian * (180 / MY_PI);
}

