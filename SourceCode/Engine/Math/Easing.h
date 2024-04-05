#pragma once
#include <math.h>
#include <Vector3.h>
#define PI 3.14f



float EaseInSine(float start, float end, float movetime, float endtime);
float EaseInQuint(float start, float end, float movetime, float endtime);
float EaseOutSine(float start, float end, float movetime, float endtime);
float EaseOutQuint(float start, float end, float movetime, float endtime);

Vector3 EaseOutQuint(Vector3 start, Vector3 end, float movetime, float endtime);

double EaseInBack(float t, int start, int end, int num = 1, double p1 = 1.70158);
