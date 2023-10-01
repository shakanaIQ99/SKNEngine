#include "Easing.h"

double easeInSine(double start, double end, double movetime, double endtime) 
{
	double time = movetime / endtime;
	return start + (1 - cos((time * PI) / 2)) * (end - start);
}

double easeInQuint(double start, double end, double movetime, double endtime) 
{
	double time = movetime / endtime;
	return start + (time * time * time * time * time) * (end - start);
}

double easeOutSine(double start, double end, double movetime, double endtime) 
{
	double time = movetime / endtime;
	return start + sin((time * PI) / 2) * (end - start);
}

double easeOutQuint(double start, double end, double movetime, double endtime)
{
	double time = movetime / endtime;

	return start + (1 - pow(1 - time, 5)) * (end - start);
}

double EaseInBack(float t, int start, int end, int num, double p1)
{
	const double c1 = p1;
	const double c3 = c1 + 1;

	return (end - start) * pow((c3 * t * t * t - c1 * t * t), num) + start;
}