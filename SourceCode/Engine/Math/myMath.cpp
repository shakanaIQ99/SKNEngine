#include "myMath.h"

XMFLOAT3 myMath::VectorMat(XMFLOAT3 vector, XMMATRIX mat)
{
	XMFLOAT3 changeVector = { 0,0,0 };

	changeVector.x = vector.x * mat.r[0].m128_f32[0] + vector.y * mat.r[1].m128_f32[0] + vector.z * mat.r[2].m128_f32[0] + 0.0f * mat.r[3].m128_f32[0];
	changeVector.y = vector.x * mat.r[0].m128_f32[1] + vector.y * mat.r[1].m128_f32[1] + vector.z * mat.r[2].m128_f32[1] + 0.0f * mat.r[3].m128_f32[1];
	changeVector.z = vector.x * mat.r[0].m128_f32[2] + vector.y * mat.r[1].m128_f32[2] + vector.z * mat.r[2].m128_f32[2] + 0.0f * mat.r[3].m128_f32[2];

	return changeVector;

}

XMFLOAT3 myMath::VectorMat(XMMATRIX mat, XMFLOAT3 vector)
{
	XMFLOAT3 changeVector = { 0,0,0 };

	changeVector.x = mat.r[0].m128_f32[0] * vector.x + mat.r[0].m128_f32[1] * vector.y + mat.r[0].m128_f32[2] * vector.z + mat.r[0].m128_f32[3] * 0.0f;
	changeVector.y = mat.r[1].m128_f32[0] * vector.x + mat.r[1].m128_f32[1] * vector.y + mat.r[1].m128_f32[2] * vector.z + mat.r[1].m128_f32[3] * 0.0f;
	changeVector.z = mat.r[2].m128_f32[0] * vector.x + mat.r[2].m128_f32[1] * vector.y + mat.r[2].m128_f32[2] * vector.z + mat.r[2].m128_f32[3] * 0.0f;

	return changeVector;
}

XMFLOAT3 myMath::lerp(const XMFLOAT3 vector, XMFLOAT3 vector2, float t)
{
	DirectX::XMFLOAT3 temp(vector);
	temp.x = temp.x + t * (vector2.x - temp.x);
	temp.y = temp.y + t * (vector2.y - temp.y);
	temp.z = temp.z + t * (vector2.z - temp.z);



	return temp;
}

float myMath::LerpShortAngle(float a, float b, float t)
{

	//角度差分
	float diff = b - a;

	if (diff > XMConvertToRadians(360.0f)|| diff < XMConvertToRadians(-360.0f))
	{
		diff = static_cast<float>(std::fmod(static_cast<double>(diff), static_cast<double>(XMConvertToRadians(360.0f))));
	}

	if (diff > XMConvertToRadians(180.0f))
	{
		diff -= XMConvertToRadians(360.0f);

	}
	else if(diff< XMConvertToRadians(-180.0f))
	{
		diff += XMConvertToRadians(360.0f);
	}

	return a + diff * t;
	





	//return 0.0f;
}

float myMath::VectorLengthSq(const DirectX::XMFLOAT3& vf3)
{
	float len{};

	auto&& vec{ DirectX::XMLoadFloat3(&vf3) };

	DirectX::XMStoreFloat(&len, DirectX::XMVector3LengthSq(vec));

	return len;
}

float myMath::VectorDot(const DirectX::XMFLOAT3& vec1, const DirectX::XMFLOAT3& vec2)
{
	float rv{};

	const auto&& v1{ DirectX::XMLoadFloat3(&vec1) }, && v2{ DirectX::XMLoadFloat3(&vec2) };

	DirectX::XMStoreFloat(&rv, DirectX::XMVector3Dot(v1, v2));

	return rv;
}
