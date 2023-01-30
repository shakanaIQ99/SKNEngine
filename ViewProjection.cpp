#include "ViewProjection.h"
#include"DxWindow.h"

void ViewProjection::Initialize()
{
	eye = { 0,0,-50.0f };
	target = { 0,0,0 };
	up = { 0,1,0 };
	aspect = (float)DxWindow::window_width / DxWindow::window_height;
	nearClip = 0.1f;
	farClip = 2000.0f;

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
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

void ViewProjection::UpdateMatProjection()
{
	matProjection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), aspect, nearClip, farClip);
}

const XMMATRIX& ViewProjection::GetMAtView()
{
	return matView;
}

const XMMATRIX& ViewProjection::GetMatProjection()
{
	return matProjection;
}

const XMFLOAT3& ViewProjection::Geteye()
{
	return eye;
}

const XMFLOAT3& ViewProjection::Gettarget()
{
	return target;
}

const XMFLOAT3& ViewProjection::Getup()
{
	return up;
}

void ViewProjection::SetEye(const XMFLOAT3& eye_)
{
	eye = eye_;
}

void ViewProjection::SetTarget(const XMFLOAT3& target_)
{
	target = target_;
}

void ViewProjection::SetUp(const XMFLOAT3& up_)
{
	up = up_;
}
