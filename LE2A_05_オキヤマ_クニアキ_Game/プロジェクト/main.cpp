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
#include"FPS.h"

bool CollsionAABB(WorldTransform a, WorldTransform b);
XMFLOAT3 CollsionBackAABB(WorldTransform a, WorldTransform b, XMFLOAT3 vec);

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	DxWindow* window = nullptr;

	window = DxWindow::GetInstance();
	window->CreateGameWindow();
#ifdef _DEBUG
		//�f�o�b�O���C���[���I����
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

	unique_ptr<FPS>fps;
	fps = std::make_unique<FPS>();
	fps->Initialize();

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
	uint32_t title = texturemanager->LoadTexture("Resources/title.png");
	uint32_t endtitle = texturemanager->LoadTexture("Resources/end.png");

	SpriteCommon* spritecommon = nullptr;
	spritecommon = new SpriteCommon();
	spritecommon->Initialize(dxCommon);
	
	Object3D::StaticInitialize(dxCommon->GetDevice(), DxWindow::window_width, DxWindow::window_height);
	LightGroup::StaticInitialize(dxCommon->GetDevice());
	
	LightGroup* light = nullptr;
	light = LightGroup::Create();

	Object3D::SetLight(light);

	ViewProjection camera;
	camera.Initialize();

	

	
	XMFLOAT3 eye = { 0,0,-50 };

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
	
	Ray ray;

	ray.dir = XMVectorSet(0, -1, 0, 0);



	bool Hit;
	bool rayHit;
	
	light->SetPointLightPos(0, XMFLOAT3(pointLightPos));
	light->SetPointLightColor(0, XMFLOAT3(pointLightColor));
	light->SetPointLightAtten(0, XMFLOAT3(pointLightAtten));

	float a = 0.2f;

	Model* playerModel = Model::LoadFromOBJ("player");
	Model* enemyModel = Model::LoadFromOBJ("muso");
	Model* target = Model::LoadFromOBJ("arrow");
	Model* YUKA = Model::LoadFromOBJ("cube");


	WorldTransform playerWt;
	WorldTransform enemyWt;
	WorldTransform targetWt;
	WorldTransform groundWt;

	Object3D* player;
	Object3D* enemy;
	Object3D* arrow;
	Object3D* ground;

	player = Object3D::Create(&playerWt);
	player->SetModel(playerModel);
	enemy = Object3D::Create(&enemyWt);
	enemy->SetModel(enemyModel);
	arrow = Object3D::Create(&targetWt);
	arrow->SetModel(target);
	ground = Object3D::Create(&groundWt);
	ground->SetModel(YUKA);

	camera.SetEye(eye);
	
	enemyWt.scale_ = { 0.7f,0.7f,0.7f };
	playerWt.scale_ = { 0.7f,0.7f,0.7f };
	playerWt.translation_ = {0,0,-150.0f };
	groundWt.scale_ = { 200.0f,1.0f,200.0f };
	groundWt.translation_.y = -2.0f;
	targetWt.scale_ = { 0.7f,0.7f,0.7f };
	targetWt.rotation_.x = 1.57f;
	
	XMFLOAT3 lightpos = { 0,50.0f,0 };

	light->SetPointLightPos(0, lightpos);

	WorldTransform tex1;

	Sprite2D* sprite = nullptr;
	sprite = new Sprite2D();
	sprite->Initialize(spritecommon, &tex1, title);
	Sprite2D* sprite2 = nullptr;
	sprite2 = new Sprite2D();
	sprite2->Initialize(spritecommon, &tex1, endtitle);

	tex1.translation_ = { 220.0f,160.0f,0 };
	tex1.scale_ = { 8.0f,4.0f,4.0f };

	Sphere Plsp;
	Sphere Ensp;

	Plsp.radius = playerWt.scale_.x;
	Ensp.radius = enemyWt.scale_.x;

	while (true)
	{
		if (window->ProcessMessage())
		{
			break;
		}

		input->InputUpdate();
		imGuiManager->Begin();
		static XMVECTOR lightDir = { 0,1,5,0 };
		
		switch (scenenum)
		{
		case 0:

			if (input->GetPressKey(DIK_SPACE))
			{
				scenenum = 1;
				playerWt.translation_ = { 0,0,-150.0f };
			}

			break;
		case 1:

			Plsp.center = { playerWt.translation_.x,playerWt.translation_.y ,playerWt.translation_.z ,1};
			Ensp.center = { enemyWt.translation_.x,enemyWt.translation_.y ,enemyWt.translation_.z ,1 };
			XMFLOAT3 Vec = { 0,-1,0 };

			if (input->GetKey(DIK_W))
			{
				playerWt.translation_.z += 1.0f;
				Vec.z += 1;

			}
			if (input->GetKey(DIK_S))
			{
				playerWt.translation_.z -= 1.0f;
				Vec.z -= 1;

			}
			if (input->GetKey(DIK_A))
			{
				playerWt.translation_.x -= 1.0f;
				Vec.x -= 1;
			}
			if (input->GetKey(DIK_D))
			{
				playerWt.translation_.x += 1.0f;
				Vec.x += 1;
			}

			playerWt.translation_.y -= 0.2f;

			playerWt.translation_ = CollsionBackAABB(playerWt, groundWt, Vec);

			targetWt.translation_ = { enemyWt.translation_.x,enemyWt.translation_.y + 6.0f ,enemyWt.translation_.z };

			targetWt.rotation_.y += 0.2f;

			if (Collision::CheckSphereToSphere(Plsp, Ensp))
			{
				scenenum = 2;
			}
			if (playerWt.translation_.y < -10.0f)
			{
				scenenum = 2;
			}

			break;
		case 2:
			if (input->GetPressKey(DIK_SPACE))
			{
				scenenum = 0;

			}
			break;
		}




		

		camera.SetTarget(playerWt.translation_);
		camera.SetEye({ playerWt.translation_.x,playerWt.translation_.y + 10.0f,playerWt.translation_.z - 25.0f });

		player->Update(&camera);
		enemy ->Update(&camera);
		arrow ->Update(&camera);	
		ground->Update(&camera);
		light->Update();
		camera.Update();
		sprite->Update();
		sprite2->Update();
		
		
		
		imGuiManager->End();
		dxCommon->PreDraw();

		Object3D::PreDraw(dxCommon->GetCommandList());
		
		if(scenenum==1)
		{
			player->Draw();

			enemy->Draw();

			arrow->Draw();

			ground->Draw();
		}
		
		

		
		

		Object3D::PostDraw();

		spritecommon->PreDraw();
		if (scenenum == 0)
		{
			sprite->Draw();
		}
		if (scenenum == 2)
		{
			sprite2->Draw();
		}
		
		spritecommon->PostDraw();

		imGuiManager->Draw();
		dxCommon->PostDraw();

		fps->Update();

	}
	
	delete input;
	delete playerModel;
	delete enemyModel;
	delete target;
	delete YUKA;
	delete player;
	delete enemy;
	delete arrow;
	delete ground;
	delete light;
	delete sprite;
	delete sprite2;
	delete spritecommon;
	imGuiManager->Finalize();
	delete dxCommon;
	delete imGuiManager;
	texturemanager->DeleteInstance();
	//delete texturemanager;
	window->TerminateGameWindow();
}

bool CollsionAABB(WorldTransform a, WorldTransform b)
{
	float aLeft = a .translation_.x - a.scale_.x;
	float aRight = a.translation_.x + a.scale_.x;
	float aTop = a  .translation_.y + a.scale_.y;
	float aBottom =a.translation_.y - a.scale_.y;
	float aFlont = a.translation_.z + a.scale_.z;
	float aBack = a .translation_.z - a.scale_.z;

	float bLeft = b.translation_.x - b.scale_.x;
	float bRight = b.translation_.x + b.scale_.x;
	float bTop = b. translation_.y + b.scale_.y;
	float bBottom =b.translation_.y - b.scale_.y;
	float bFlont = b.translation_.z + b.scale_.z;
	float bBack = b. translation_.z - b.scale_.z;

	if (
		aLeft < bRight && bLeft < aRight && aBottom < bTop && bBottom < aTop && aBack < bFlont &&
		bBack < aFlont) {
		return true;
	}
	else {
		return false;
	}
}

XMFLOAT3 CollsionBackAABB(WorldTransform a, WorldTransform b, XMFLOAT3 vec)
{
	XMFLOAT3 aa = a.translation_;



	if (CollsionAABB(a, b))
	{
		if (
			vec.x > 0 && a.translation_.x + a.scale_.x > b.translation_.x - b.scale_.x &&
			a.translation_.x <= b.translation_.x - b.scale_.x)
		{
			aa.x = b.translation_.x - b.scale_.x - a.scale_.x;
		}
		if (vec.x < 0 && a.translation_.x - a.scale_.x < b.translation_.x + b.scale_.x &&
			a.translation_.x >= b.translation_.x + b.scale_.x)
		{
			aa.x = b.translation_.x + b.scale_.x + a.scale_.x;
		}
		if (
			vec.y > 0 && a.translation_.y + a.scale_.y > b.translation_.y - b.scale_.y &&
			a.translation_.y <= b.translation_.y - b.scale_.y)
		{
			aa.y = b.translation_.y - b.scale_.y - a.scale_.y;
		}
		if (
			vec.y < 0 && a.translation_.y - a.scale_.y < b.translation_.y + b.scale_.y &&
			a.translation_.y >= b.translation_.y + b.scale_.y)
		{
			aa.y = b.translation_.y + b.scale_.y + a.scale_.y;
		}
		if (vec.z > 0 && a.translation_.z + a.scale_.z > b.translation_.z - b.scale_.z &&
			a.translation_.z <= b.translation_.z - b.scale_.z)
		{
			aa.z = b.translation_.z - b.scale_.z - a.scale_.z;
		}
		if (
			vec.z < 0 && a.translation_.z - a.scale_.z < b.translation_.z + b.scale_.z &&
			a.translation_.z >= b.translation_.z + b.scale_.z)
		{
			aa.z = b.translation_.z + b.scale_.z + a.scale_.z;
		}
	}



	return aa;

}
