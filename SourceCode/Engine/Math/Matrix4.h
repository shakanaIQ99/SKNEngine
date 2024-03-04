#pragma once
#include<DirectXMath.h>
class Vector3;
class Float4;

class Matrix4
{


private:

	class Line
	{
		float m[4] = {};

	public:
		float operator[](const size_t i) const;
		float& operator[](const size_t i);

	};


	Line m[4] = {};

public:

	/// <summary>
	/// 単位行列
	/// </summary>
	Matrix4();

	Matrix4(DirectX::XMMATRIX matrix);

	Line operator[](const size_t i) const;
	Line& operator[](const size_t i);

	//逆行列
	Matrix4 operator-() const;


	//加算
	Matrix4 operator+(const Matrix4& a) const;
	Matrix4& operator+=(const Matrix4& a);

	//減算
	Matrix4 operator-(const Matrix4& a) const;
	Matrix4& operator-=(const Matrix4& a);
	
	//掛け算
	Matrix4 operator*(const Matrix4& a) const;
	Matrix4& operator*=(const Matrix4& a);


	//比較
	bool operator==(const Matrix4& a) const;
	bool operator!=(const Matrix4& a) const;
};

