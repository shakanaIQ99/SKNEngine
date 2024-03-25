#pragma once
#include <math.h>
#include <Vector3.h>
#define PI 3.14f



float easeInSine(float start, float end, float movetime, float endtime);
float easeInQuint(float start, float end, float movetime, float endtime);
float easeOutSine(float start, float end, float movetime, float endtime);
float easeOutQuint(float start, float end, float movetime, float endtime);

Vector3 easeOutQuint(Vector3 start, Vector3 end, float movetime, float endtime);

double EaseInBack(float t, int start, int end, int num = 1, double p1 = 1.70158);
