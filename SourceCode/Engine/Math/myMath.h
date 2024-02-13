#pragma once
#include<DirectXMath.h>
#include<cmath>

using namespace DirectX;

#define PAI_SKN 3.1415926535897932384;

namespace myMath
{
	/// <summary>
	/// 行列とベクトルの計算(左側が行列計算の先)
	/// </summary>
	XMFLOAT3 VectorMat(XMFLOAT3 vector, XMMATRIX mat);

	/// <summary>
	/// 行列とベクトルの計算(左側が行列計算の先)
	/// </summary>
	XMFLOAT3 VectorMat(XMMATRIX mat, XMFLOAT3 vector);

	XMFLOAT3 lerp(XMFLOAT3 vector, XMFLOAT3 vector2, float t);

	//double 

	/// <summary>
	/// 最短角度補間
	/// </summary>
	float LerpShortAngle(float a, float b, float t);

	float VectorLengthSq(const DirectX::XMFLOAT3& vf3);
	
	float VectorDot(const DirectX::XMFLOAT3& vec1, const DirectX::XMFLOAT3& vec2);
	



}

