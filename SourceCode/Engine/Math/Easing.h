#pragma once
//イージング

#include <math.h>
#define PI 3.14f

//各イージング関数
float easeInSine(float start, float end, float movetime, float endtime);
float easeInQuint(float start, float end, float movetime, float endtime);
float easeOutSine(float start, float end, float movetime, float endtime);
float easeOutQuint(float start, float end, float movetime, float endtime);
double EaseInBack(float t, int start, int end, int num = 1, double p1 = 1.70158);
