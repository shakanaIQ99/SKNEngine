#include"DxWindow.h"
#include"DirectXCommon.h"
#include"Object3D.h"
#include"Input.h"
#include<wrl.h>
#include"Sprite2D.h"
#include"SpriteCommon.h"
#include"WorldTronsform.h"
#include"TextureManager.h"
#include"ViewProjection.h"
#include"LightGroup.h"
#include<sstream>
#include<iomanip>
#include"ImGuiManager.h"
#include"imgui.h"
#include"CollisionPrimitive.h"
#include"Collision.h"




int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	DxWindow* window = nullptr;

	window = DxWindow::GetInstance();
	window->CreateGameWindow();
#ifdef _DEBUG
		//デバッグレイヤーをオンに
		ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif	
	DirectXCommon* dxCommon = nullptr;

	dxCommon = new DirectXCommon();
	dxCommon->Initialize(window);

	ImGuiManager* imGuiManager = nullptr;
	imGuiManager = new ImGuiManager();
	imGuiManager->Initialize(window->GetHwnd(),dxCommon);

	Input* input = nullptr;
	input = new Input();
	input->Initialize(window->GetHInstance(),window->GetHwnd());


	//HRESULT result;

	TextureManager* texturemanager = TextureManager::GetInstance();
	//TextureManager* texturemanager = nullptr;
	texturemanager->StaticInitialize(dxCommon);

	uint32_t blank = texturemanager->LoadTexture("Resources/white1x1.png");

	SpriteCommon* spritecommon = nullptr;
	spritecommon = new SpriteCommon();
	spritecommon->Initialize(dxCommon);
	
	Object3D::StaticInitialize(dxCommon->GetDevice(), DxWindow::window_width, DxWindow::window_height);
	LightGroup::StaticInitialize(dxCommon->GetDevice());
	uint32_t tex1 = texturemanager->LoadTexture("Resources/visual.png");
	uint32_t tex2 = texturemanager->LoadTexture("Resources/puragomi.jpg");

	Model* skydome = Model::LoadFromOBJ("skydome");
	Model* model2 = Model::LoadFromOBJ("maru",true);
	Model* model = Model::LoadFromOBJ("chr_sword");
	Model* gra = Model::LoadFromOBJ("ground");
	Model* tri = Model::LoadFromOBJ("sankaku");
	WorldTransform ground;
	WorldTransform skywt;
	WorldTransform human;
	WorldTransform ball;
	WorldTransform wt3;
	WorldTransform wt4;

	Sprite2D* sprite = nullptr;
	sprite = new Sprite2D();
	sprite->Initialize(spritecommon,&wt3,tex1);
	Sprite2D* sprite2 = nullptr;
	sprite2 = new Sprite2D();
	sprite2->Initialize(spritecommon, &wt4,tex2);

	LightGroup* light = nullptr;
	light = LightGroup::Create();

	Object3D::SetLight(light);

	Object3D* obj1 = nullptr;
	Object3D* obj2 = nullptr;
	Object3D* objskydome = nullptr;
	Object3D* objground = nullptr;

	skywt.scale_ = { 0.5f,0.5f,0.5f };

	objskydome = Object3D::Create(&skywt);
	objskydome->SetModel(skydome);

	objground = Object3D::Create(&ground);
	objground->SetModel(gra);

	obj1 = Object3D::Create(&human);
	obj1->SetModel(model);
	obj2 = Object3D::Create(&ball);
	obj2->SetModel(model2);

	//
	sprite2->Wt->translation_.y = 5.0f;
	ViewProjection camera;
	camera.Initialize();

	XMFLOAT3 eye = { 0,0,-50 };

	ground.translation_.y = -10.0f;

	human.translation_.x = -10.0f;
	human.translation_.y = -5.0f;
	ball.translation_.x = 10.0f;

	int scenenum = 0;

	float ambientColor[3] = { 1,1,1 };

	float lightDir0[3] = { 0,0,-1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	float pointLightPos[3] = { 0.5f,1.0f,0 };
	float pointLightColor[3] = { 1,1,1 };

	float pointLightAtten[3] = { 0.3f,0.1f,0.1f };

	light->SetDirLightActive(0, false);
	light->SetDirLightActive(1, false);
	light->SetDirLightActive(2, false);
	light->SetPointLightActive(0, true);

	wt3.translation_ = { 100.0f,100.0f,0.0f };

	Sphere sp;
	Plane pl;
	Triangle triangle;
	triangle.p0 = XMVectorSet(-1.0f, ground.translation_.y, -1.0f, 1);
	triangle.p1 = XMVectorSet(-1.0f, ground.translation_.y, 1.0f, 1);
	triangle.p2 = XMVectorSet(1.0f, ground.translation_.y, -1.0f, 1);
	triangle.normal = XMVectorSet(0.0f, 1.0f, 0.0f, 0);
	pl.normal = XMVectorSet(0, 1, 0, 0);

	bool Hit;
	
	while (true)
	{
		if (window->ProcessMessage())
		{
			break;
		}

		input->InputUpdate();
		imGuiManager->Begin();
		static XMVECTOR lightDir = { 0,1,5,0 };

		if (input->GetKey(DIK_W))
		{
			ball.translation_.z += 0.2f;
		}
		if (input->GetKey(DIK_S))
		{
			ball.translation_.z -= 0.2f;
		}
		if (input->GetKey(DIK_D))
		{
			ball.translation_.x += 0.2f;
		}
		if (input->GetKey(DIK_A))
		{
			ball.translation_.x -= 0.2f;
		}

		if (input->GetKey(DIK_R))
		{
			ball.translation_.y += 0.2f;
		}
		if (input->GetKey(DIK_F))
		{
			ball.translation_.y -= 0.2f;
		}



		human.rotation_.y -= 0.05f;
		
		if (input->GetKey(DIK_0))
		{
			scenenum = 0;

		}
		if (input->GetKey(DIK_1))
		{

			scenenum = 1;
		}
		
		
		
		ball.rotation_.y -= 0.05f;
		
		if (input->GetKey(DIK_RIGHT))
		{
			eye.x += 0.2f;
		}
		if (input->GetKey(DIK_LEFT))
		{
			eye.x -= 0.2f;
		}
		if (input->GetKey(DIK_UP))
		{
			eye.y += 0.2f;
		}
		if (input->GetKey(DIK_DOWN))
		{
			eye.y -= 0.2f;
		}
		sp.center = XMVectorSet(ball.translation_.x, ball.translation_.y, ball.translation_.z, 1);
		sp.radius = ball.scale_.y;

		pl.distance = ground.translation_.y;

		switch (scenenum)
		{
		case 1:
			objground->SetModel(tri);
			light->SetDirLightActive(0, true);
			light->SetDirLightActive(1, true);
			light->SetDirLightActive(2, true);
			light->SetPointLightActive(0, false);
			light->SetAmbientColor(XMFLOAT3(ambientColor));
			light->SetDirLightDir(0, XMVECTOR({ lightDir0[0],lightDir0[1],lightDir0[2],0 }));
			light->SetDirLightColor(0, XMFLOAT3(lightColor0));
			light->SetDirLightDir(1, XMVECTOR({ lightDir1[0],lightDir1[1],lightDir1[2],0 }));
			light->SetDirLightColor(1, XMFLOAT3(lightColor1));
			light->SetDirLightDir(2, XMVECTOR({ lightDir2[0],lightDir2[1],lightDir2[2],0 }));
			light->SetDirLightColor(2, XMFLOAT3(lightColor2));

			Hit=Collision::CheckSphere2Triangle(sp,triangle);
			
			break;
		case 0:
			objground->SetModel(gra);
			light->SetDirLightActive(0, false);
			light->SetDirLightActive(1, false);
			light->SetDirLightActive(2, false);
			light->SetPointLightActive(0, true);

			light->SetPointLightPos(0, XMFLOAT3(pointLightPos));
			light->SetPointLightColor(0, XMFLOAT3(pointLightColor));
			light->SetPointLightAtten(0, XMFLOAT3(pointLightAtten));
			
			Hit = Collision::CheckSphere2Plane(sp, pl);
			
			break;

		}

		

		light->Update();
		
		obj1->Update(&camera);
		obj2->Update(&camera);
		objskydome->Update(&camera);
		objground->Update(&camera);
		sprite->Update();
		sprite2->Update();
		camera.SetEye(eye);
		camera.Update();

		
		ImGui::Text("PosY %4.1f", sp.center.m128_f32[1]-sp.radius);
		ImGui::Text("Plane %4.1f", pl.distance);
		ImGui::Text("Hit %d", Hit);
		
		imGuiManager->End();
		dxCommon->PreDraw();

		Object3D::PreDraw(dxCommon->GetCommandList());

		objskydome->Draw();
		objground->Draw();
		obj1->Draw();
		obj2->Draw();

		Object3D::PostDraw();

		spritecommon->PreDraw();
		
		sprite->Draw();
		sprite2->Draw();
		spritecommon->PostDraw();

		imGuiManager->Draw();
		dxCommon->PostDraw();

	}
	
	delete input;
	delete obj1;
	delete obj2;
	delete model;
	delete model2;
	delete objskydome;
	delete objground;
	delete gra;
	delete skydome;
	delete sprite;
	delete sprite2;
	delete light;
	delete spritecommon;
	imGuiManager->Finalize();
	delete dxCommon;
	delete imGuiManager;
	texturemanager->DeleteInstance();
	window->TerminateGameWindow();
}