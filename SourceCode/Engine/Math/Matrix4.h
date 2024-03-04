#pragma once
class Vector3;
class Float4;

class Matrix4
{


private:

	class Row
	{
		float c[4] = {};

	public:
		float operator[](const size_t i) const;
		float& operator[](const size_t i);
		void operator = (const Float4& f);
	};


	Row m[4] = {};

public:

	/// <summary>
	/// 単位行列
	/// </summary>
	Matrix4();

	/// <summary>
	/// Float4を4つで初期化
	/// <summary>
	Matrix4(Float4 r0, Float4 r1, Float4 r2, Float4 r3);

	/// <summary>
	/// floatを16個で初期化
	/// </summary>
	Matrix4(float r0c0, float r0c1, float r0c2, float r0c3,
		float r1c0, float r1c1, float r1c2, float r1c3,
		float r2c0, float r2c1, float r2c2, float r2c3,
		float r3c0, float r3c1, float r3c2, float r3c3);


	Row operator[](const size_t i) const;
	Row& operator[](const size_t i);

	//逆行列
	Matrix4 operator-() const;

	//転置行列
	Matrix4& Transpose();

	//転置行列を取得
	Matrix4 GetTranspose() const;


	//加算
	Matrix4 operator+(const Matrix4& a) const;
	Matrix4& operator+=(const Matrix4& a);

	//減算
	Matrix4 operator-(const Matrix4& a) const;
	Matrix4& operator-=(const Matrix4& a);
	
	//乗算
	Matrix4 operator*(const Matrix4& a) const;
	Matrix4& operator*=(const Matrix4& a);

	Matrix4 SetTranslation(const Float4& f);

	Vector3 GetTranslation();

	//平行移動行列
	static Matrix4 Translation(float x, float y, float z);

	//スケーリング行列
	static Matrix4 Scaling(float x, float y, float z);

	//X軸回転行列
	static Matrix4 RotationX(float radian);
	//Y軸回転行列
	static Matrix4 RotationY(float radian);
	//Z軸回転行列
	static Matrix4 RotationZ(float radian);

	//ZXY(Roll,Pitch,Yaw)回転行列
	static Matrix4 RotationZXY(float radianX, float radianY, float radianZ);

	//ビュー変換行列
	static Matrix4 View(Vector3 eye, Vector3 target, Vector3 up);

	//透視投影変換行列
	static Matrix4 PerspectiveProjection(float fov, float aspect, float nearZ, float farZ);

	//平行投影変換行列
	static Matrix4 OrthoGraphicProjection(float left, float right, float top, float bottom, float nearZ, float farZ);

	//ビューポート行列
	static Matrix4 Viewport(float x, float y, float width, float height, float minDepth, float maxDepth);

	//w除算
	static Vector3 ProjectionDivW(Vector3 pos, Matrix4 mat);
	
};

Vector3 operator*(const Vector3 vec, const Matrix4 mat);
Vector3& operator*=(Vector3& vec, const Matrix4 mat);

Float4 operator*(const Float4 f, const Matrix4 mat);
Float4& operator*=(Float4& f, const Matrix4 mat);