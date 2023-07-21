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
#include"LevelLoder.h"

#include<DirectXMath.h>



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
	
	Model* skydome = Model::LoadFromOBJ("skydome");
	Model* model2 = Model::LoadFromOBJ("UVBall",true);
	Model* gra = Model::LoadFromOBJ("ground");

	WorldTransform ground;
	WorldTransform skywt;
	WorldTransform human;
	WorldTransform ball;
	WorldTransform wt3;
	WorldTransform wt4;
	
	Sprite2D* sprite = nullptr;
	sprite = new Sprite2D();
	sprite->Initialize(spritecommon, &wt3, tex1);
	Sprite2D* sprite2 = nullptr;
	sprite2 = new Sprite2D();
	sprite2->Initialize(spritecommon, &wt4, tex2);

	LightGroup* light = nullptr;
	light = LightGroup::Create();

	Object3D::SetLight(light);

	Object3D* obj2 = nullptr;
	Object3D* objskydome = nullptr;
	Object3D* objground = nullptr;

	skywt.scale_ = { 0.5f,0.5f,0.5f };

	objskydome = Object3D::Create(&skywt);
	objskydome->SetModel(skydome);

	objground = Object3D::Create(&ground);
	objground->SetModel(gra);

	
	obj2 = Object3D::Create(&ball);
	obj2->SetModel(model2);

	//
	sprite2->Wt->translation_.y = 5.0f;
	ViewProjection camera;
	camera.Initialize();

	LevelData* levelData;
	std::map<std::string, Model*> models;
	std::vector<Object3D*> objects;

	//json�ɂ��郂�f���o�^����
	models.insert(std::make_pair("skydome", skydome));
	models.insert(std::make_pair("ground", gra));


	levelData = LevelLoder::LoadFile("");

	// ���x���f�[�^����I�u�W�F�N�g�𐶐��A�z�u
	for (auto& objectData : levelData->objects) {
		// �t�@�C��������o�^�ς݃��f��������
		Model* model = nullptr;
		decltype(models)::iterator it = models.find(objectData.fileName);
		if (it != models.end()) {
			model = it->second;
		}

		WorldTransform objtransform;

		// ���f�����w�肵��3D�I�u�W�F�N�g�𐶐�
		Object3D* newObject = Object3D::Create(&objtransform);
		newObject->SetModel(model);

		// ���W
		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, objectData.translation);
		newObject->Wt->translation_ = pos;

		// ��]�p
		DirectX::XMFLOAT3 rot;
		DirectX::XMStoreFloat3(&rot, objectData.rotation);
		newObject->Wt->rotation_=rot;

		// ���W
		DirectX::XMFLOAT3 scale;
		DirectX::XMStoreFloat3(&scale, objectData.scaling);
		newObject->Wt->scale_=scale;

		// �z��ɓo�^
		objects.push_back(newObject);
	}

	
	XMFLOAT3 eye = { 0,0,-50 };
	XMFLOAT3 target = { 0,0,0 };

	ground.translation_.y = -10.0f;

	human.translation_.x = -10.0f;
	human.translation_.y = -5.0f;
	ball.translation_.x = 10.0f;
	ball.color = { 1.0f,0,0,1.0f };



	float ambientColor[3] = { 1,1,1 };

	float lightDir0[3] = { 1,-1,1 };
	float lightColor0[3] = { 1,1,1 };

	/*float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };*/

	float litX = 0.0f;
	float litY = 0.0f;
	float litZ = 0.0f;

	float pointLightPos[3] = { litX,litY,litZ };
	float pointLightColor[3] = { 1,1,1 };

	float pointLightAtten[3] = { 0.3f,0.1f,0.1f };

	light->SetDirLightActive(0, true);
	light->SetPointLightActive(0, false);

	wt3.translation_ = { 100.0f,100.0f,0.0f };

	
	light->SetPointLightPos(0, XMFLOAT3(pointLightPos));
	light->SetPointLightColor(0, XMFLOAT3(pointLightColor));
	light->SetPointLightAtten(0, XMFLOAT3(pointLightAtten));

	obj2->color = { 1.0f,0,0,1.0f };

	float a = 0.2f;

	wt3.scale_ = { 0.5f,0.5f,0 };
	wt4.scale_ = { 0.5f,0.5f,0 };

	

	while (true)
	{
		if (window->ProcessMessage())
		{
			break;
		}

		input->InputUpdate();
		imGuiManager->Begin();
		static XMVECTOR lightDir = { 0,1,5,0 };

		light->SetAmbientColor(XMFLOAT3(ambientColor));
		light->SetDirLightDir(0, XMVECTOR{ lightDir0[0],lightDir0[1] ,lightDir0[2],0 });
		light->SetDirLightColor(0, XMFLOAT3(lightColor0));
		
		light->SetPointLightPos(0, XMFLOAT3({ litX,litY,litZ }));

		if (input->GetKey(DIK_RIGHT))
		{
			if (input->GetKey(DIK_E))
			{
				eye.x += 1.0f;
			}
			target.x += 1.0f;
		}
		if (input->GetKey(DIK_LEFT))
		{
			if (input->GetKey(DIK_E))
			{
				eye.x -= 1.0f;
			}
			target.x -= 1.0f;
		}
		if (input->GetKey(DIK_UP))
		{
			if (input->GetKey(DIK_E))
			{
				eye.z += 1.0f;
			}
			target.z += 1.0f;
		}
		if (input->GetKey(DIK_DOWN))
		{
			if (input->GetKey(DIK_E))
			{
				eye.z -= 1.0f;
			}
			target.z -= 1.0f;
		}
		if (input->GetKey(DIK_W))
		{
			if (input->GetKey(DIK_SPACE))
			{
				litY += 2.0f;
			}
			else
			{
				litZ += 2.0f;
			}
		}
		if (input->GetKey(DIK_S))
		{
			if (input->GetKey(DIK_SPACE))
			{
				litY -= 2.0f;
			}
			else
			{
				litZ -= 2.0f;
			}
		}
		if (input->GetKey(DIK_A))
		{
			litX -= 2.0f;
		}
		if (input->GetKey(DIK_D))
		{
			litX += 2.0f;
		}
		light->Update();

		obj2->Update(&camera);
		objskydome->Update(&camera);
		objground->Update(&camera);
		sprite->Update();
		sprite2->Update();
		camera.SetEye(eye);
		camera.SetTarget(target);
		camera.Update();
		for (auto& object : objects) {
			object->Update(&camera);
		}


		
		

		imGuiManager->End();
		dxCommon->PreDraw();

		Object3D::PreDraw(dxCommon->GetCommandList());
		objskydome->Draw();
		
		for (auto& object : objects) {
			object->Draw();
		}
		//obj2->Draw();
		objground->Draw();
		



		Object3D::PostDraw();
		ParticleManager::PreDraw(dxCommon->GetCommandList());

		


		/// <summary>
		/// ������3D�I�u�W�F�N�g�̕`�揈����ǉ��ł���
		/// </summary>

		// 3D�I�u�W�F�N�g�`��㏈��
		ParticleManager::PostDraw();

		spritecommon->PreDraw();

		
		spritecommon->PostDraw();

		imGuiManager->Draw();
		dxCommon->PostDraw();

		fps->Update();

	}

	delete input;
	delete obj2;
	delete model2;
	for (Object3D*& object : objects) {
		delete(object);
	}
	delete levelData;
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
	//delete texturemanager;
	window->TerminateGameWindow();
}