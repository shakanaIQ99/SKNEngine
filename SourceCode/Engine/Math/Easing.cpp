#include "Easing.h"

float EaseInSine(float start, float end, float movetime, float endtime)
{
	float time = movetime / endtime;
	return start + static_cast<float>((1 - cos((time * PI) / 2))) * (end - start);
}

float EaseInQuint(float start, float end, float movetime, float endtime)
{
	float time = movetime / endtime;
	return start + (time * time * time * time * time) * (end - start);
}

float EaseOutSine(float start, float end, float movetime, float endtime)
{
	float time = movetime / endtime;
	return start + static_cast<float>(sin((time * PI) / 2)) * (end - start);
}

float EaseOutQuint(float start, float end, float movetime, float endtime)
{
	float time = movetime / endtime;

	return start + (1 - static_cast<float>(pow(1 - time, 5))) * (end - start);
}

Vector3 EaseOutQuint(Vector3 start, Vector3 end, float movetime, float endtime)
{
	Vector3 vec;
	vec.x = EaseOutQuint(start.x, end.x, movetime, endtime);
	vec.y = EaseOutQuint(start.y, end.y, movetime, endtime);
	vec.z = EaseOutQuint(start.z, end.z, movetime, endtime);

	return vec;
}

Vector2 EaseOutQuint(Vector2 start, Vector2 end, float movetime, float endtime)
{
	Vector2 vec;
	vec.x = EaseOutQuint(start.x, end.x, movetime, endtime);
	vec.y = EaseOutQuint(start.y, end.y, movetime, endtime);

	return vec;
}

double EaseInBack(float t, int start, int end, int num, double p1)
{
	const double c1 = p1;
	const double c3 = c1 + 1;

	return (end - start) * pow((c3 * t * t * t - c1 * t * t), num) + start;
}