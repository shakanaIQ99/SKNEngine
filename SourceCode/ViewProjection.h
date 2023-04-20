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
	float nearClip = 0.0f;
	float farClip = 0.0f;
	float angle = 0.0f;
	float distance = 50.0f;

};

