#include "Matrix4.h"
#include "Vector3.h"
#include "Float4.h"
#include <stdexcept>

float Matrix4::Line::operator[](const size_t i) const
{
	if (i > 3)
	{
		throw std::out_of_range("添え字5番目以降desu");
	}
	return m[i];
}

float& Matrix4::Line::operator[](const size_t i)
{
	if (i > 3)
	{
		throw std::out_of_range("添え字5番目以降desu");
	}
	return m[i];
}

Matrix4::Matrix4()
{
	m[0][0] = 1;
	m[1][1] = 1;
	m[2][2] = 1;
	m[3][3] = 1;
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

Matrix4::Line Matrix4::operator[](const size_t i) const
{
	if (i > 3) {
		throw std::out_of_range("添え字5番目以降desu");
	}
	return m[i];
}

Matrix4::Line& Matrix4::operator[](const size_t i)
{
	if (i > 3) {
		throw std::out_of_range("添え字5番目以降desu");
	}
	return m[i];
}
