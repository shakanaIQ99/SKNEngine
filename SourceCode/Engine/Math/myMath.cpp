#include "myMath.h"



Vector3 myMath::lerp(const Vector3 vector, Vector3 vector2, float t)
{
	Vector3 temp(vector);
	temp.x = temp.x + t * (vector2.x - temp.x);
	temp.y = temp.y + t * (vector2.y - temp.y);
	temp.z = temp.z + t * (vector2.z - temp.z);



	return temp;
}

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
	return angle * (PI / 180.0f);
}

float myMath::RadianToAngle(float radian)
{
	return radian * (180 / PI);
}

