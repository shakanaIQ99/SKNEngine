#include "ViewProjection.h"
#include"DxWindow.h"
#include"myMath.h"

void ViewProjection::Initialize()
{
	eye = { 0,0,-50.0f };
	target = { 0,0,0 };
	up = { 0,1.0f,0 };
	aspect = (float)DxWindow::window_width / DxWindow::window_height;
	

	UpdateMatview();
	UpdateMatProjection();

}

void ViewProjection::Update()
{
	UpdateMatview();
	UpdateMatProjection();
}

void ViewProjection::UpdateMatview()
{
	matView = Matrix4::View(eye, target, up);
}

void ViewProjection::UpdateMatProjection()
{
	matProjection = Matrix4::PerspectiveProjection(myMath::AngleToRadian(45.0f), (float)DxWindow::window_width / DxWindow::window_height, nearClip, farClip);
}

const Matrix4 ViewProjection::GetMAtView()
{
	return matView;
}

const Matrix4 ViewProjection::GetMatProjection()
{
	return matProjection;
}

const Vector3 ViewProjection::Geteye()
{
	return eye;
}

const Vector3 ViewProjection::Gettarget()
{
	return target;
}

const Vector3 ViewProjection::Getup()
{
	return up;
}

void ViewProjection::SetEye(const Vector3& eye_)
{
	eye = eye_;
}

void ViewProjection::SetTarget(const Vector3& target_)
{
	target = target_;
}

void ViewProjection::SetUp(const Vector3& up_)
{
	up = up_;
}
