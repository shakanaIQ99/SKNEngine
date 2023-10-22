#pragma once

class Vector3;

class Vector2
{
public:
	float x;
	float y;

public:
	//零ベクトル生成
	Vector2();			
	//x,y成分を指定して生成
	Vector2(float x, float y);	

	//関数
	
	//長さ
	float length()const;				
	//正規化
	Vector2& normalize();		
	//正規化を入手
	Vector2 getnormalize() const;
	//内積
	float dot(const Vector2& v)const;	
	//外積
	float cross(const Vector2& v)const;	

	//逆ベク
	Vector2 operator-()const;

	//比較組

	bool operator==(const Vector2& v) const;
	bool operator!=(const Vector2& v) const;

	//演算組
	 
	//足し算
	Vector2& operator+=(const Vector2& v);
	//引き算
	Vector2& operator-=(const Vector2& v);
	//掛け算
	Vector2& operator*=(float s);
	//除算
	Vector2& operator/=(float s);

	//足し算
	Vector2 operator+(const Vector2& v) const;
	//引き算
	Vector2 operator-(const Vector2& v) const;
	//掛け算
	Vector2 operator*(const float s) const;
	//除算
	Vector2 operator/(const float s) const;

	operator Vector3() const;

};

Vector2 operator*(const float s, const Vector2& v);
