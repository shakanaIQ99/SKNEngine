#include "GameScene.h"
#include"FbxLoader.h"


void GameScene::Init(DxWindow* dxwindow, DirectXCommon* dxcommon)
{
	
	imGuiManager = new ImGuiManager();
	imGuiManager->Initialize(dxwindow->GetHwnd(), dxcommon);

	input = new Input();
	input->Initialize(dxwindow->GetHInstance(), dxwindow->GetHwnd());

	spritecommon = new SpriteCommon();
	spritecommon->Initialize(dxcommon);

	texturemanager = TextureManager::GetInstance();
	texturemanager->StaticInitialize(dxcommon);
	uint32_t blank = texturemanager->LoadTexture("Resources/white1x1.png");

	tex1 = texturemanager->LoadTexture("Resources/visual.png");
	tex2 = texturemanager->LoadTexture("Resources/puragomi.jpg");
	tex3 = texturemanager->LoadTexture("Resources/effect1.png");
	tex4 = texturemanager->LoadTexture("Resources/effect3.png");
	
	
	light = LightGroup::Create();

	camera.Initialize();
	Object3D::SetCamera(&camera);

	model1 = FbxLoader::GetInstance()->LoadModelFlomFile("cube");
	atm.CreateConstBuffer(dxcommon->GetDevice());

	objec1 = new Object3D();
	objec1->Initilaize(&atm);
	objec1->SetModel(model1);

	sprite = new Sprite2D();
	sprite->Initialize(spritecommon, &wt3, tex1);
	sprite2 = new Sprite2D();
	sprite2->Initialize(spritecommon, &wt4, tex2);

	particleMan = ParticleManager::Create(tex3, &p1);
	particleMan2 = ParticleManager::Create(tex4, &p2);


	sprite2->Wt->translation_.y = 5.0f;

	

	eye = { 0,20,-200 };
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

	light->SetPointLightPos(0, XMFLOAT3(pointLightPos));
	light->SetPointLightColor(0, XMFLOAT3(pointLightColor));
	light->SetPointLightAtten(0, XMFLOAT3(pointLightAtten));

	float a = 0.2f;

	atm.rotation_.y = 0.5f;

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

	//objec1->PlayAnimation();

}

void GameScene::Update()
{

	input->InputUpdate();
	imGuiManager->Begin();
	static XMVECTOR lightDir = { 0,1,5,0 };



	if (input->GetKey(DIK_RIGHT))
	{
		atm.translation_.x += 0.5f;
	}
	if (input->GetKey(DIK_LEFT))
	{
		atm.translation_.x -= 0.5f;
	}
	if (input->GetKey(DIK_UP))
	{
		atm.translation_.y += 0.5f;
	}
	if (input->GetKey(DIK_DOWN))
	{
		atm.translation_.y -= 0.5f;
	}

	

	particleMan->Update(&camera);
	particleMan2->Update(&camera);

	light->Update();


	sprite->Update();
	sprite2->Update();
	camera.SetEye(eye);
	camera.SetTarget({ 0,20,0 });
	camera.Update();
	objec1->Update();




	imGuiManager->End();
}

void GameScene::Draw(DirectXCommon* dxcommon)
{
	objec1->Draw(dxcommon->GetCommandList());
	/*Object3D::PreDraw(dxCommon->GetCommandList());

	Object3D::PostDraw();*/
	ParticleManager::PreDraw(dxcommon->GetCommandList());

	// 3Dオブクジェクトの描画
	//particleMan->Draw();
	//particleMan2->Draw();


	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	ParticleManager::PostDraw();

	spritecommon->PreDraw();

	sprite->Draw({ 0,0 });
	sprite2->DrawClip({ 80.0f,180.0f }, { 200.0f,100.0f }, {});

	spritecommon->PostDraw();

	imGuiManager->Draw();
}

void GameScene::Finalize()
{

	delete input;
	imGuiManager->Finalize();
	delete imGuiManager;
	delete particleMan;
	delete particleMan2;
	delete objec1;
	//model1->Finalize();
	delete model1;
	delete sprite;
	delete sprite2;
	delete light;
	delete spritecommon;
	texturemanager->DeleteInstance();
}
