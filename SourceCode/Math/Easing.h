#pragma once
#include <math.h>
#define PI 3.14f

double easeInSine(double start, double end, double movetime, double endtime);
double easeInQuint(double start, double end, double movetime, double endtime);
double easeOutSine(double start, double end, double movetime, double endtime);
double easeOutQuint(double start, double end, double movetime, double endtime);
double EaseInBack(float t, int start, int end, int num = 1, double p1 = 1.70158);
