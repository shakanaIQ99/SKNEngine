#pragma once
#include"Matrix4.h"
#include"Vector3.h"
#include<cmath>

class ViewProjection
{
public:

	void Initialize();

	void Update();

	void UpdateMatview();

	void UpdateMatProjection();

	const Matrix4 GetMAtView();
	const Matrix4 GetMatProjection();
	const Vector3 Geteye();
	const Vector3 Gettarget();
	const Vector3 Getup();
	
	void SetEye(const Vector3& eye_);
	void SetTarget(const Vector3& target_);
	void SetUp(const Vector3& up_);

private:

	Matrix4 matView;

	Matrix4 matProjection;

	Vector3 eye;

	Vector3 target;

	Vector3 up;

	float aspect = 0.0f;
	float nearClip = 0.1f;
	//std::float_t nearClip = 0.1f;
	float farClip = 2000.0f;
	//std::float_t farClip = 2000.0f;
	float angle = 0.0f;
	float distance = 50.0f;

};

