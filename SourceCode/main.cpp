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
#include"ParticleManager.h"




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
	imGuiManager->Initialize(window->GetHwnd(), dxCommon);

	Input* input = nullptr;
	input = new Input();
	input->Initialize(window->GetHInstance(), window->GetHwnd());

	unique_ptr<FPS>fps;
	fps = std::make_unique<FPS>();
	fps->Initialize();

	//HRESULT result;

	TextureManager* texturemanager = TextureManager::GetInstance();
	//TextureManager* texturemanager = nullptr;
	texturemanager->StaticInitialize(dxCommon);

	uint32_t blank = texturemanager->LoadTexture("Resources/white1x1.png");

	SpriteCommon* spritecommon = nullptr;
	spritecommon = new SpriteCommon();
	spritecommon->Initialize(dxCommon);

	Object3D::StaticInitialize(dxCommon->GetDevice(), DxWindow::window_width, DxWindow::window_height);
	ParticleManager::StaticInitialize(dxCommon->GetDevice());
	LightGroup::StaticInitialize(dxCommon->GetDevice());
	uint32_t tex1 = texturemanager->LoadTexture("Resources/visual.png");
	uint32_t tex2 = texturemanager->LoadTexture("Resources/puragomi.jpg");
	uint32_t tex3 = texturemanager->LoadTexture("Resources/effect1.png");
	uint32_t tex4 = texturemanager->LoadTexture("Resources/effect3.png");

	Model* skydome = Model::LoadFromOBJ("skydome");
	Model* model2 = Model::LoadFromOBJ("maru", true);
	Model* model = Model::LoadFromOBJ("chr_sword");
	Model* gra = Model::LoadFromOBJ("ground");
	Model* tri = Model::LoadFromOBJ("sankaku");
	Model* box = Model::LoadFromOBJ("boxobj");
	WorldTransform ground;
	WorldTransform skywt;
	WorldTransform human;
	WorldTransform ball;
	WorldTransform wt3;
	WorldTransform wt4;
	WorldTransform raybox;
	WorldTransform p1;
	WorldTransform p2;

	Sprite2D* sprite = nullptr;
	sprite = new Sprite2D();
	sprite->Initialize(spritecommon, &wt3, tex1);
	Sprite2D* sprite2 = nullptr;
	sprite2 = new Sprite2D();
	sprite2->Initialize(spritecommon, &wt4, tex2);

	LightGroup* light = nullptr;
	light = LightGroup::Create();

	Object3D::SetLight(light);

	Object3D* obj1 = nullptr;
	Object3D* obj2 = nullptr;
	Object3D* objskydome = nullptr;
	Object3D* objground = nullptr;
	Object3D* Objray = nullptr;

	ParticleManager* particleMan = nullptr;
	ParticleManager* particleMan2 = nullptr;

	particleMan = ParticleManager::Create(tex3,&p1);
	particleMan2 = ParticleManager::Create(tex4,&p2);

	Objray = Object3D::Create(&raybox);
	Objray->SetModel(box);

	skywt.scale_ = { 0.5f,0.5f,0.5f };

	objskydome = Object3D::Create(&skywt);
	objskydome->SetModel(skydome);

	objground = Object3D::Create(&ground);
	objground->SetModel(gra);

	obj1 = Object3D::Create(&human);
	obj1->SetModel(model);
	obj2 = Object3D::Create(&ball);
	obj2->SetModel(model);

	//
	sprite2->Wt->translation_.y = 5.0f;
	ViewProjection camera;
	camera.Initialize();

	raybox.translation_ = { 0,-14.0f,0 };
	raybox.scale_ = { 0.5f,10.0f,0.5f };


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
	Ray ray;

	ray.dir = XMVectorSet(0, -1, 0, 0);

	triangle.p0 = XMVectorSet(-10.0f, ground.translation_.y, -10.0f, 1);
	triangle.p1 = XMVectorSet(-10.0f, ground.translation_.y, 10.0f, 1);
	triangle.p2 = XMVectorSet(10.0f, ground.translation_.y, -10.0f, 1);
	triangle.normal = XMVectorSet(0.0f, 1.0f, 0.0f, 0);
	pl.normal = XMVectorSet(0, 1, 0, 0);

	bool Hit;
	bool rayHit;

	light->SetPointLightPos(0, XMFLOAT3(pointLightPos));
	light->SetPointLightColor(0, XMFLOAT3(pointLightColor));
	light->SetPointLightAtten(0, XMFLOAT3(pointLightAtten));

	float a = 0.2f;

	wt3.scale_ = { 0.5f,0.5f,0 };
	wt4.scale_ = { 0.5f,0.5f,0 };

	for (int i = 0; i < 100; i++)
	{
		const float rnd_pos = 20.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		particleMan->Add(150, pos, vel, acc, 1.0f, 0.0f);

	}
	for (int j = 0; j < 100; j++)
	{
		const float rnd_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 1.0f;
		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 1.0f;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 1.0f;

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 4.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 4.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 4.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		particleMan2->Add(300, pos, vel, acc, 1.0f, 0.0f);

	}


	while (true)
	{
		if (window->ProcessMessage())
		{
			break;
		}

		input->InputUpdate();
		imGuiManager->Begin();
		static XMVECTOR lightDir = { 0,1,5,0 };

		if (input->GetKey(DIK_0))
		{
			scenenum = 0;
			ball.translation_.x = 10.0f;

		}
		if (input->GetKey(DIK_1))
		{

			scenenum = 1;
			ball.translation_ = { 10.0f,-5.0f,-3.0f };
			raybox.translation_ = { 0,-14.0f,2.0f };
		}
		if (input->GetKey(DIK_2))
		{

			scenenum = 2;
			ball.translation_ = { 0,-2.0f,0 };

			raybox.translation_ = { 0,0,0 };
		}

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
		ray.start = XMVectorSet(raybox.translation_.x, raybox.translation_.y + raybox.scale_.y, raybox.translation_.z, 1.0f);
		pl.distance = ground.translation_.y;

		switch (scenenum)
		{
		case 1:
			objground->SetModel(tri);
			if (ball.translation_.y < -20.0f || ball.translation_.y > 10.0f)
			{
				a *= -1.0f;
			}
			ball.translation_.y += a;
			raybox.translation_.z -= a;


			if (Hit = Collision::CheckSphere2Triangle(sp, triangle))
			{
				obj2->color = { 1,0,0,1 };

			}
			else
			{
				obj2->color = { 1,1,1,1 };

			}
			if (rayHit = Collision::CheckRay2Triangle(ray, triangle))
			{
				Objray->color = { 1,0,0,1 };
			}
			else
			{
				Objray->color = { 1,1,1,1 };
			}
			break;


			break;
		case 0:
			objground->SetModel(gra);

			if (ball.translation_.y < -10.0f || ball.translation_.y > 10.0f)
			{
				a *= -1.0f;
			}
			ball.translation_.y += a;
			raybox.translation_.y += a;


			if (Hit = Collision::CheckSphere2Plane(sp, pl))
			{
				obj2->color = { 1,0,0,1 };

			}
			else
			{
				obj2->color = { 1,1,1,1 };

			}
			if (rayHit = Collision::CheckRay2Plane(ray, pl))
			{
				Objray->color = { 1,0,0,1 };
			}
			else
			{
				Objray->color = { 1,1,1,1 };
			}
			break;

		case 2:
			if (raybox.translation_.x < -20.0f || raybox.translation_.x > 20.0f)
			{
				a *= -1.0f;
			}
			raybox.translation_.x += a;
			if (rayHit = Collision::CheckRay2Sphere(ray, sp))
			{
				Objray->color = { 1,0,0,1 };
			}
			else
			{
				Objray->color = { 1,1,1,1 };
			}
			break;
		}

		particleMan->Update(&camera);
		particleMan2->Update(&camera);

		light->Update();

		obj1->Update(&camera);
		obj2->Update(&camera);
		objskydome->Update(&camera);
		objground->Update(&camera);
		Objray->Update(&camera);
		sprite->Update();
		sprite2->Update();
		camera.SetEye(eye);
		camera.Update();


		ImGui::Text("PosY %4.1f", sp.center.m128_f32[1] - sp.radius);
		ImGui::Text("Plane %4.1f", pl.distance);
		ImGui::Text("Hit %d", Hit);
		ImGui::Text("RayHit %d", rayHit);

		imGuiManager->End();
		dxCommon->PreDraw();

		Object3D::PreDraw(dxCommon->GetCommandList());
		objskydome->Draw();
		switch (scenenum)
		{
		case 1:
			//obj1->Draw();
			obj2->Draw();
			objground->Draw();
			Objray->Draw();
			break;
		case 0:
			obj2->Draw();
			Objray->Draw();
			objground->Draw();
			break;
		case 2:
			obj2->Draw();
			Objray->Draw();
			break;
		}




		Object3D::PostDraw();
		ParticleManager::PreDraw(dxCommon->GetCommandList());

		// 3Dオブクジェクトの描画
		particleMan->Draw();
		particleMan2->Draw();


		/// <summary>
		/// ここに3Dオブジェクトの描画処理を追加できる
		/// </summary>

		// 3Dオブジェクト描画後処理
		ParticleManager::PostDraw();

		spritecommon->PreDraw();

		sprite->Draw({0,0});
		sprite2->DrawClip({ 80.0f,180.0f },{200.0f,100.0f},{});

		spritecommon->PostDraw();

		imGuiManager->Draw();
		dxCommon->PostDraw();

		fps->Update();

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
	delete particleMan;
	delete particleMan2;
	delete box;
	delete tri;
	delete Objray;
	delete sprite;
	delete sprite2;
	delete light;
	delete spritecommon;
	imGuiManager->Finalize();
	delete dxCommon;
	delete imGuiManager;
	texturemanager->DeleteInstance();
	//delete texturemanager;
	window->TerminateGameWindow();
}