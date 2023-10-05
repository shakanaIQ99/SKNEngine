#pragma once
class Vector2;

class Vector3
{
public:
	float x;
	float y;
	float z;


public:
	//ƒ[ƒƒxƒNƒgƒ‹
	Vector3();
	//”CˆÓƒxƒNƒgƒ‹
	Vector3(float x, float y, float z);

	//ŠÖ”

	//’·‚³
	float length()const;
	//³‹K‰»
	Vector3& normalize();
	//³‹K‰»‚ğ“üè
	Vector3 getnormalize() const;
	//“àÏ
	float dot(const Vector3& v)const;
	//ŠOÏ
	Vector3 cross(const Vector3& v)const;

	//‹tƒxƒN
	Vector3 operator-()const;

	//”äŠr‘g

	bool operator==(const Vector3& v) const;
	bool operator!=(const Vector3& v) const;

	//‰‰Z‘g

	//‘«‚µZ
	Vector3& operator+=(const Vector3& v);
	//ˆø‚«Z
	Vector3& operator-=(const Vector3& v);
	//Š|‚¯Z
	Vector3& operator*=(float s);
	//œZ
	Vector3& operator/=(float s);

	//‘«‚µZ
	Vector3 operator+(const Vector3& v) const;
	//ˆø‚«Z
	Vector3 operator-(const Vector3& v) const;
	//Š|‚¯Z
	Vector3 operator*(const float s) const;
	//œZ
	Vector3 operator/(const float s) const;

	operator Vector2() const;
};

Vector3 operator*(const float s, const Vector3& v);
