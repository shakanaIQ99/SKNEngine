#include "Camera.h"
#include <cmath>
#include"Input.h"
#include"ImGuiManager.h"
#include "Matrix4.h"

void Camera::Initialize()
{
	wt.CreateConstBuffer();
	wt.translation_ = { 0.0f,0.0f,0.0f };
	wt.rotation_ = { 0.0f,0.0f,0.0f };
	wt.scale_ = { 1.0f,1.0f,1.0f };
}

void Camera::Reset()
{
	wt.translation_ = { 0.0f,0.0f,0.0f };
	wt.rotation_ = { 0.0f,0.0f,0.0f };
	wt.scale_ = { 1.0f,1.0f,1.0f };
	SetRotate({ 0,0,0 });
	viewProjection_.Initialize();

	cameraRotateX = 0;
	cameraRotateY = 0;
	rotateX = 0;
	rotateY = 0;

	/*if (targetWT)
	{
		interTarget = targetWT->translation_;
		wt.rotation_.y = targetWT->rotation_.y;
	}*/


	targetWT = nullptr;
}

void Camera::Update()
{

	wt.UpdateMatrix(&viewProjection_);
	//ワールド前方ベクトル
	forward = { 0.0f, 0.0f, 1.0f };
	//ワールド上方ベクトル
	Vector3 up(0, 1, 0);
	forward *= wt.matWorld_;

	viewProjection_.SetEye(wt.translation_);

	//視点から前方に適当な距離進んだ位置が注視点
	if (targetFlag)
	{
		viewProjection_.SetTarget(interTarget);
	}
	else
	{
		viewProjection_.SetTarget(wt.translation_ + forward);
	}
	

	//レールカメラの回転を反映(レールカメラの上方ベクトル)
	viewProjection_.SetUp(up * wt.matWorld_);
	if (targetWT&&!DebugMode)
	{
		Vector3 offset = { -20.0f,20.0f,-20.0f };

		Vector2 inputnum = Input::GetRStick(true, true);
		cameraRotateY += (float)inputnum.x * cameraDPI;
		if ((cameraRotateX < 0.27f && (float)inputnum.y / SHRT_MAX>0) || (cameraRotateX > -0.6f && (float)inputnum.y / SHRT_MAX < 0))
		{
			cameraRotateX += (float)inputnum.y * cameraDPI;
			
		}

		Vector3 cameraPoint = targetWT->translation_;

		cameraPoint.y += 5.0f;
		

		offset.x = offset.x * sinf(cameraRotateY);
		offset.z = offset.z * cosf(cameraRotateY);
		offset.y = offset.y * sinf(-cameraRotateX);



		
		viewProjection_.SetEye(cameraPoint + offset);
		
		viewProjection_.SetTarget(cameraPoint);

		forward = cameraPoint - (cameraPoint + offset);


	}


#ifdef _DEBUG
	ImGuiMode();
#endif

	viewProjection_.Update();

}

ViewProjection* Camera::GetView()
{
	return &viewProjection_;
}

Matrix4 Camera::GetMatWorld()
{
	return wt.matWorld_;
}

Vector3 Camera::GetForwardVec()
{

	return forward.GetNormalize();
}


void Camera::SetPos(Vector3 pos)
{
	wt.translation_ = pos;
	wt.UpdateMatrix(&viewProjection_);
}

//void Camera::SetTarget(Vector3 Target)
//{
//	viewProjection_.SetTarget(Target);
//}

void Camera::SetRotate(Vector3 rotate)
{
	wt.rotation_ = rotate;
	wt.UpdateMatrix(&viewProjection_);
}

void Camera::SetWorldMat(Matrix4 woeldMat)
{
	wt.matWorld_ = woeldMat;
}

void Camera::ImGuiMode()
{
	//ImguI
	ImGui::SetNextWindowPos({ ImGui::GetMainViewport()->WorkPos.x, ImGui::GetMainViewport()->WorkPos.y + 20 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize({ 400, 500 });

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoResize;
	ImGui::Begin("Camera", NULL, window_flags);

	ImGui::Text("Position");
	ImGui::DragFloat("X", &wt.translation_.x, 0.5f);
	ImGui::DragFloat("Y", &wt.translation_.y, 0.5f);
	ImGui::DragFloat("Z", &wt.translation_.z, 0.5f);
	ImGui::NewLine();
	ImGui::Text("Rota");
	ImGui::DragFloat("RX", &wt.rotation_.x, myMath::AngleToRadian(2.0f));
	ImGui::DragFloat("RY", &wt.rotation_.y, myMath::AngleToRadian(2.0f));
	ImGui::DragFloat("RZ", &wt.rotation_.z, myMath::AngleToRadian(2.0f));
	ImGui::Checkbox("DebugCamera", &DebugMode);

	ImGui::End();


}


