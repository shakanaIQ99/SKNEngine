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

