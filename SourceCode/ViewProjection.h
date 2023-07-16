#pragma once
#include<DirectXMath.h>

using namespace DirectX;

class ViewProjection
{
public:

	void Initialize();

	void Update();

	void UpdateMatview();

	void UpdateMatProjection();

	const XMMATRIX& GetMAtView();
	const XMMATRIX& GetMatProjection();
	const XMFLOAT3& Geteye();
	const XMFLOAT3& Gettarget();
	const XMFLOAT3& Getup();
	
	void SetEye(const XMFLOAT3& eye_);
	void SetTarget(const XMFLOAT3& target_);
	void SetUp(const XMFLOAT3& up_);

private:

	XMMATRIX matView;

	XMMATRIX matProjection;

	XMFLOAT3 eye;

	XMFLOAT3 target;

	XMFLOAT3 up;

	float aspect = 0.0f;
	float_t nearClip = 0.1f;
	float_t farClip = 1000.0f;
	float angle = 0.0f;
	float distance = 50.0f;

};

