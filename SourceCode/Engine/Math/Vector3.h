#pragma once
class Vector2;

class Vector3
{
public:
	float x;
	float y;
	float z;


public:
	//ゼロベクトル
	Vector3();
	//任意ベクトル
	Vector3(float x, float y, float z);

	Vector3(Vector3* vec);

	Vector3(const float* floatArray) :Vector3(floatArray[0], floatArray[1], floatArray[2]){}

	//関数

	//長さ
	float Length()const;
	//正規化
	Vector3& Normalize();
	//正規化を入手
	Vector3 GetNormalize() const;
	//内積
	float Dot(const Vector3& v)const;
	//外積
	Vector3 Cross(const Vector3& v)const;

	//xz軸の2Dベクトル
	Vector2 GetXZ()const;

	//逆ベク
	Vector3 operator-()const;

	//比較組

	bool operator==(const Vector3& v) const;
	bool operator!=(const Vector3& v) const;

	//演算組

	//足し算
	Vector3& operator+=(const Vector3& v);
	//引き算
	Vector3& operator-=(const Vector3& v);
	//掛け算
	Vector3& operator*=(float s);
	//除算
	Vector3& operator/=(float s);

	//足し算
	Vector3 operator+(const Vector3& v) const;
	//引き算
	Vector3 operator-(const Vector3& v) const;
	//掛け算
	Vector3 operator*(const float s) const;
	//除算
	Vector3 operator/(const float s) const;

	operator Vector2() const;

	
};

Vector3 operator*(const float s, const Vector3& v);

const Vector3 Lerp(const Vector3& start, const Vector3& end, const float t);

const Vector3 Slerp(const Vector3& start, const Vector3& end, const float t);