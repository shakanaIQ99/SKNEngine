#include "Vector2.h"
#include "Vector3.h"
#include<cmath>	//sqrt

Vector2::Vector2()
	:x(0), y(0)
{
}

Vector2::Vector2(float x, float y)
	: x(x), y(y)
{
}

float Vector2::length() const
{
	return sqrtf((x * x) + (y * y));
}

Vector2& Vector2::normalize()
{
	float len = length();
	if (len != 0)
	{
		return *this /= len;
	}
	else {
		this->x = 0;
		this->y = 0;
	}
	return *this;
}

Vector2 Vector2::getnormalize() const
{
	float len = length();
	float nx = 0;
	float ny = 0;
	if (len != 0)
	{
		nx = this->x / len;
		ny = this->y / len;
	}
	return Vector2(nx, ny);
}

float Vector2::dot(const Vector2& v) const
{
	return x * v.x + y * v.y;
}

float Vector2::cross(const Vector2& v) const
{
	return x * v.y - y * v.x;
}


Vector2 Vector2::operator-() const
{
	return Vector2(-x, -y);
}

bool Vector2::operator==(const Vector2& v) const
{
	return this->x == v.x && this->y == v.y;
}

bool Vector2::operator!=(const Vector2& v) const
{
	return this->x != v.x || this->y != v.y;
}

Vector2& Vector2::operator+=(const Vector2& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

Vector2& Vector2::operator*=(float s)
{
	x *= s;
	y *= s;
	return *this;
}

Vector2& Vector2::operator/=(float s)
{
	x /= s;
	y /= s;
	return *this;
}

Vector2 Vector2::operator+(const Vector2& v) const
{
	return Vector2(this->x + v.x, this->y + v.y);
}

Vector2 Vector2::operator-(const Vector2& v) const
{
	return Vector2(this->x - v.x, this->y - v.y);
}

Vector2 Vector2::operator*(const float s) const
{
	return Vector2(this->x * s, this->y * s);
}

Vector2 Vector2::operator/(const float s) const
{
	return Vector2(this->x / s, this->y / s);
}

Vector2::operator Vector3() const
{
	return Vector3(x, y, 0);
}

Vector2 operator*(const float s, const Vector2& v)
{
	Vector2 temp;

	temp.x = v.x * s;
	temp.y = v.y * s;
	return temp;
}

