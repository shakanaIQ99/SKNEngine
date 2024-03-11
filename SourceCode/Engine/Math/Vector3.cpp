#include "Vector3.h"
#include "Vector2.h"
#include<cmath>	//sqrt

Vector3::Vector3() : x(0), y(0), z(0)
{

}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z)
{
}


float Vector3::length() const
{
	return sqrtf((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
}

Vector3& Vector3::normalize()
{
	float len = length();
	if (len != 0)
	{
		return *this /= len;
	}
	else
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}
	return *this;
}

Vector3 Vector3::getnormalize() const
{
	float len = length();
	float nx = 0;
	float ny = 0;
	float nz = 0;
	if (len != 0)
	{
		nx = this->x / len;
		ny = this->y / len;
		nz = this->z / len;
	}
	return Vector3(nx, ny, nz);
}

float Vector3::dot(const Vector3& v) const
{
	return this->x * v.x + this->y * v.y + this->z * v.z;
}

Vector3 Vector3::cross(const Vector3& v) const
{
	return Vector3(this->y * v.z - this->z * v.y, this->z * v.x - this->x * v.z, this->x * v.y - this->y * v.x);
}

Vector2 Vector3::GetXZ() const
{
	return Vector2(x, z);
}

bool Vector3::operator==(const Vector3& a) const
{
	return this->x == a.x && this->y == a.y && this->z == a.z;
}

bool Vector3::operator!=(const Vector3& a) const
{
	return false;
}


Vector3 Vector3::operator-() const
{
	return Vector3(-this->x, -this->y, -this->z);
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;

	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;

	return *this;
}

Vector3& Vector3::operator*=(float s)
{
	this->x *= s;
	this->y *= s;
	this->z *= s;
	return *this;
}

Vector3& Vector3::operator/=(float s)
{
	this->x /= s;
	this->y /= s;
	this->z /= s;
	return *this;
}

Vector3 Vector3::operator+(const Vector3& v) const
{
	return Vector3(this->x + v.x, this->y + v.y, this->z + v.z);
}

Vector3 Vector3::operator-(const Vector3& v) const
{
	return Vector3(this->x - v.x, this->y - v.y, this->z - v.z);
}

Vector3 Vector3::operator*(const float s) const
{
	return Vector3(this->x * s, this->y * s, this->z * s);
}

Vector3 Vector3::operator/(const float s) const
{
	return Vector3(this->x / s, this->y / s, this->z / s);
}

Vector3::operator Vector2() const
{
	return Vector2(x, y);
}

Vector3 operator*(const float s, const Vector3& v)
{
	Vector3 temp;

	temp.x = v.x * s;
	temp.y = v.y * s;
	temp.z = v.z * s;

	return temp;
}

const Vector3 lerp(const Vector3& start, const Vector3& end, const float t)
{
	return start + (end - start) * t;
}

