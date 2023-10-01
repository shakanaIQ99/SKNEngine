#include "Easing.h"

float easeInSine(float start, float end, float movetime, float endtime)
{
	float time = movetime / endtime;
	return start + static_cast<float>((1 - cos((time * PI) / 2))) * (end - start);
}

float easeInQuint(float start, float end, float movetime, float endtime)
{
	float time = movetime / endtime;
	return start + (time * time * time * time * time) * (end - start);
}

float easeOutSine(float start, float end, float movetime, float endtime)
{
	float time = movetime / endtime;
	return start + static_cast<float>(sin((time * PI) / 2)) * (end - start);
}

float easeOutQuint(float start, float end, float movetime, float endtime)
{
	float time = movetime / endtime;

	return start + (1 - static_cast<float>(pow(1 - time, 5))) * (end - start);
}

double EaseInBack(float t, int start, int end, int num, double p1)
{
	const double c1 = p1;
	const double c3 = c1 + 1;

	return (end - start) * pow((c3 * t * t * t - c1 * t * t), num) + start;
}