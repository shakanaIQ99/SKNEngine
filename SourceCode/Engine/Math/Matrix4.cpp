#include "Matrix4.h"
#include "Vector3.h"
#include "Float4.h"
#include <stdexcept>
#include <cmath>

float Matrix4::Row::operator[](const size_t i) const
{
	if (i > 3)
	{
		throw std::out_of_range("添え字5番目以降desu");
	}
	return c[i];
}

float& Matrix4::Row::operator[](const size_t i)
{
	if (i > 3)
	{
		throw std::out_of_range("添え字5番目以降desu");
	}
	return c[i];
}

void Matrix4::Row::operator=(const Float4& f)
{
	c[0] = f.x;
	c[1] = f.y;
	c[2] = f.z;
	c[3] = f.w;
}

Matrix4::Matrix4()
{
	m[0][0] = 1;
	m[1][1] = 1;
	m[2][2] = 1;
	m[3][3] = 1;
}

Matrix4::Matrix4(Float4 r0, Float4 r1, Float4 r2, Float4 r3)
{
	m[0] = r0;
	m[1] = r1;
	m[2] = r2;
	m[3] = r3;
}

Matrix4::Matrix4(float r0c0, float r0c1, float r0c2, float r0c3, float r1c0, float r1c1, float r1c2, float r1c3, float r2c0, float r2c1, float r2c2, float r2c3, float r3c0, float r3c1, float r3c2, float r3c3)
{
	m[0][0] = r0c0;
	m[0][1] = r0c1;
	m[0][2] = r0c2;
	m[0][3] = r0c3;
	
	m[1][0] = r1c0;
	m[1][1] = r1c1;
	m[1][2] = r1c2;
	m[1][3] = r1c3;
	
	m[2][0] = r2c0;
	m[2][1] = r2c1;
	m[2][2] = r2c2;
	m[2][3] = r2c3;
	
	m[3][0] = r3c0;
	m[3][1] = r3c1;
	m[3][2] = r3c2;
	m[3][3] = r3c3;
}

Matrix4::Matrix4(DirectX::XMMATRIX matrix)
{
	for (int32_t i = 0; i < 4; i++)
	{
		for (int32_t j = 0; j < 4; j++) 
		{
			m[i][j] = matrix.r[i].m128_f32[j];
		}
	}
}

Matrix4::Row Matrix4::operator[](const size_t i) const
{
	if (i > 3) {
		throw std::out_of_range("添え字5番目以降desu");
	}
	return m[i];
}

Matrix4::Row& Matrix4::operator[](const size_t i)
{
	if (i > 3) {
		throw std::out_of_range("添え字5番目以降desu");
	}
	return m[i];
}

Matrix4 Matrix4::operator-() const
{
	Matrix4 temp;
	float mat[4][8] = {};

	float a;

	for (int32_t i = 0; i < 4; i++) {
		for (int32_t j = 0; j < 4; j++) {
			mat[i][j] = m[i][j];

			if (i == j)mat[i][4 + j] = 1;
		}
	}

	for (int32_t k = 0; k < 4; k++) {
		a = 1 / mat[k][k];

		for (int32_t j = 0; j < 8; j++) {
			mat[k][j] *= a;
		}

		for (int32_t i = 0; i < 4; i++) {
			if (i == k) {
				continue;
			}

			a = -mat[i][k];

			for (int32_t j = 0; j < 8; j++) {
				mat[i][j] += mat[k][j] * a;
			}
		}
	}

	for (int32_t i = 0; i < 4; i++) {
		for (int32_t j = 0; j < 4; j++) {
			temp[i][j] = mat[i][4 + j];
		}
	}
	return temp;
}

Matrix4 Matrix4::operator+(const Matrix4& a) const
{
	return Matrix4(
		m[0][0] + a[0][0],
		m[0][1] + a[0][1],
		m[0][2] + a[0][2],
		m[0][3] + a[0][3],
				  
		m[1][0] + a[1][0],
		m[1][1] + a[1][1],
		m[1][2] + a[1][2],
		m[1][3] + a[1][3],
				  
		m[2][0] + a[2][0],
		m[2][1] + a[2][1],
		m[2][2] + a[2][2],
		m[2][3] + a[2][3],
				  
		m[3][0] + a[3][0],
		m[3][1] + a[3][1],
		m[3][2] + a[3][2],
		m[3][3] + a[3][3]
	);
}

Matrix4& Matrix4::operator+=(const Matrix4& a)
{
	m[0][0] += a[0][0];
	m[0][1] += a[0][1];
	m[0][2] += a[0][2];
	m[0][3] += a[0][3];
			   
	m[1][0] += a[1][0];
	m[1][1] += a[1][1];
	m[1][2] += a[1][2];
	m[1][3] += a[1][3];
			   
	m[2][0] += a[2][0];
	m[2][1] += a[2][1];
	m[2][2] += a[2][2];
	m[2][3] += a[2][3];
			   
	m[3][0] += a[3][0];
	m[3][1] += a[3][1];
	m[3][2] += a[3][2];
	m[3][3] += a[3][3];

	return *this;
}

Matrix4 Matrix4::operator-(const Matrix4& a) const
{
	return Matrix4(
		m[0][0] - a[0][0],
		m[0][1] - a[0][1],
		m[0][2] - a[0][2],
		m[0][3] - a[0][3],

		m[1][0] - a[1][0],
		m[1][1] - a[1][1],
		m[1][2] - a[1][2],
		m[1][3] - a[1][3],

		m[2][0] - a[2][0],
		m[2][1] - a[2][1],
		m[2][2] - a[2][2],
		m[2][3] - a[2][3],

		m[3][0] - a[3][0],
		m[3][1] - a[3][1],
		m[3][2] - a[3][2],
		m[3][3] - a[3][3]
	);
}

Matrix4& Matrix4::operator-=(const Matrix4& a)
{
	m[0][0] -= a[0][0];
	m[0][1] -= a[0][1];
	m[0][2] -= a[0][2];
	m[0][3] -= a[0][3];

	m[1][0] -= a[1][0];
	m[1][1] -= a[1][1];
	m[1][2] -= a[1][2];
	m[1][3] -= a[1][3];

	m[2][0] -= a[2][0];
	m[2][1] -= a[2][1];
	m[2][2] -= a[2][2];
	m[2][3] -= a[2][3];

	m[3][0] -= a[3][0];
	m[3][1] -= a[3][1];
	m[3][2] -= a[3][2];
	m[3][3] -= a[3][3];

	return *this;
}

Matrix4 Matrix4::operator*(const Matrix4& a) const
{
	return Matrix4(
		m[0][0] * a[0][0] + m[0][1] * a[1][0] + m[0][2] * a[2][0] + m[0][3] * a[3][0],
		m[0][0] * a[0][1] + m[0][1] * a[1][1] + m[0][2] * a[2][1] + m[0][3] * a[3][1],
		m[0][0] * a[0][2] + m[0][1] * a[1][2] + m[0][2] * a[2][2] + m[0][3] * a[3][2],
		m[0][0] * a[0][3] + m[0][1] * a[1][3] + m[0][2] * a[2][3] + m[0][3] * a[3][3],

		m[1][0] * a[0][0] + m[1][1] * a[1][0] + m[1][2] * a[2][0] + m[1][3] * a[3][0],
		m[1][0] * a[0][1] + m[1][1] * a[1][1] + m[1][2] * a[2][1] + m[1][3] * a[3][1],
		m[1][0] * a[0][2] + m[1][1] * a[1][2] + m[1][2] * a[2][2] + m[1][3] * a[3][2],
		m[1][0] * a[0][3] + m[1][1] * a[1][3] + m[1][2] * a[2][3] + m[1][3] * a[3][3],

		m[2][0] * a[0][0] + m[2][1] * a[1][0] + m[2][2] * a[2][0] + m[2][3] * a[3][0],
		m[2][0] * a[0][1] + m[2][1] * a[1][1] + m[2][2] * a[2][1] + m[2][3] * a[3][1],
		m[2][0] * a[0][2] + m[2][1] * a[1][2] + m[2][2] * a[2][2] + m[2][3] * a[3][2],
		m[2][0] * a[0][3] + m[2][1] * a[1][3] + m[2][2] * a[2][3] + m[2][3] * a[3][3],

		m[3][0] * a[0][0] + m[3][1] * a[1][0] + m[3][2] * a[2][0] + m[3][3] * a[3][0],
		m[3][0] * a[0][1] + m[3][1] * a[1][1] + m[3][2] * a[2][1] + m[3][3] * a[3][1],
		m[3][0] * a[0][2] + m[3][1] * a[1][2] + m[3][2] * a[2][2] + m[3][3] * a[3][2],
		m[3][0] * a[0][3] + m[3][1] * a[1][3] + m[3][2] * a[2][3] + m[3][3] * a[3][3]
	);
}

Matrix4& Matrix4::operator*=(const Matrix4& a)
{
	*this = *this * a;
	return *this;
}

Matrix4 Matrix4::Translation(float x, float y, float z)
{
	Matrix4 mat;
	mat[3][0] = x;
	mat[3][1] = y;
	mat[3][2] = z;
	return mat;
}

Matrix4 Matrix4::Scaling(float x, float y, float z)
{
	Matrix4 mat;
	mat[0][0] = x;
	mat[1][1] = y;
	mat[2][2] = z;
	return mat;
}

Matrix4 Matrix4::RotationX(float radian)
{
	Matrix4 mat;
	mat[1][1] = cosf(radian);
	mat[1][2] = sinf(radian);
	mat[2][1] = -sinf(radian);
	mat[2][2] = cosf(radian);
	return mat;
}

Matrix4 Matrix4::RotationY(float radian)
{
	Matrix4 mat;
	mat[0][0] = cosf(radian);
	mat[0][2] = -sinf(radian);
	mat[2][0] = sinf(radian);
	mat[2][2] = cosf(radian);
	return mat;
}

Matrix4 Matrix4::RotationZ(float radian)
{
	Matrix4 mat;
	mat[0][0] = cosf(radian);
	mat[0][1] = sinf(radian);
	mat[1][0] = -sinf(radian);
	mat[1][1] = cosf(radian);
	return mat;
}

Matrix4 Matrix4::RotationZXY(float radianX, float radianY, float radianZ)
{
	Matrix4 mat;
	mat *= RotationZ(radianZ);
	mat *= RotationX(radianX);
	mat *= RotationY(radianY);
	return mat;
}

Matrix4 Matrix4::View(Vector3 eye, Vector3 target, Vector3 up)
{
	Matrix4 mat;

	Vector3 direction = target - eye;
	direction.normalize();

	Vector3 xVec = up.cross(direction);
	xVec.normalize();
	Vector3 yVec = direction.cross(xVec);
	yVec.normalize();

	mat[0][0] = xVec.x;
	mat[0][1] = xVec.y;
	mat[0][2] = xVec.z;
	mat[1][0] = yVec.x;
	mat[1][1] = yVec.y;
	mat[1][2] = yVec.z;
	mat[2][0] = direction.x;
	mat[2][1] = direction.y;
	mat[2][2] = direction.z;
	mat[3][0] = eye.x;
	mat[3][1] = eye.y;
	mat[3][2] = eye.z;

	mat = -mat;
	return mat;
}


Matrix4 Matrix4::PerspectiveProjection(float fov, float aspect, float nearZ, float farZ)
{
	Matrix4 mat;

	Matrix4 _a; 
	_a[0][0] = 1 / aspect;

	Matrix4 _b; 
	_b[0][0] = 1 / tanf(fov / 2);
	_b[1][1] = 1 / tanf(fov / 2);

	Matrix4 _c; 
	_c[2][2] = farZ * (1 / (farZ - nearZ));
	_c[3][2] = farZ * (-nearZ / (farZ - nearZ));

	mat = _a * _b * _c;
	mat[2][3] = 1;
	mat[3][3] = 0;

	return mat;
}

Vector3 operator*(const Vector3 vec, const Matrix4 mat)
{
	Vector3 temp = vec;
	temp.x = vec.x * mat[0][0] + vec.y * mat[1][0] + vec.z * mat[2][0];
	temp.y = vec.x * mat[0][1] + vec.y * mat[1][1] + vec.z * mat[2][1];
	temp.z = vec.x * mat[0][2] + vec.y * mat[1][2] + vec.z * mat[2][2];
	return temp;
}

Vector3& operator*=(Vector3& vec, const Matrix4 mat)
{
	Vector3 temp = vec * mat;
	vec = temp;
	return vec;
}

Float4 operator*(const Float4 f, const Matrix4 mat)
{
	Float4 temp = f;
	temp.x = f.x * mat[0][0] + f.y * mat[1][0] + f.z * mat[2][0] + f.w * mat[3][0];
	temp.y = f.x * mat[0][1] + f.y * mat[1][1] + f.z * mat[2][1] + f.w * mat[3][1];
	temp.z = f.x * mat[0][2] + f.y * mat[1][2] + f.z * mat[2][2] + f.w * mat[3][2];
	temp.w = f.x * mat[0][3] + f.y * mat[1][3] + f.z * mat[2][3] + f.w * mat[3][3];
	return temp;
}

Float4& operator*=(Float4& f, const Matrix4 mat)
{
	Float4 temp = f * mat;
	f = temp;
	return f;
}
