#include "LightGroup.h"
#include<cassert>
ID3D12Device* LightGroup::device = nullptr;


void LightGroup::StaticInitialize(ID3D12Device* _device)
{
	assert(!LightGroup::device);

	assert(_device);

	LightGroup::device = _device;
}

LightGroup* LightGroup::Create()
{
	LightGroup* instance = new LightGroup();


	instance->Initialize();

	return instance;
}

void LightGroup::Initialize()
{

	DefaultLightSetting();

	HRESULT result;
	D3D12_HEAP_PROPERTIES cbHeapProp{};

	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	

	D3D12_RESOURCE_DESC cbResourceDesc{};
	
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0Xff;	//256バイトアライメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = device->CreateCommittedResource(
		&cbHeapProp,		//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);
	assert(SUCCEEDED(result));

	TransferConstBuffer();


}

void LightGroup::Update()
{
	if (dirty)
	{
		TransferConstBuffer();
		dirty = false;
	}

}

void LightGroup::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndex)
{
	cmdList->SetGraphicsRootConstantBufferView(rootParamIndex, constBuff->GetGPUVirtualAddress());
}

void LightGroup::TransferConstBuffer()
{
	HRESULT result;

	ConstBufferData* constMap = nullptr;

	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->ambientColor = ambientColor;

		for (int i = 0; i < DirLightNum; i++)
		{
			if (dirLights[i].IsActive())
			{
				constMap->dirLights[i].active = 1;
				constMap->dirLights[i].lightv = -dirLights[i].GetLightDir();
				constMap->dirLights[i].lightcolor = dirLights[i].GetLightColor();
			}
			else
			{
				constMap->dirLights[i].active = 0;
			}
		}

		for (int i = 0; i < PointLightNum; i++)
		{
			if (pointLights[i].IsActive())
			{
				constMap->pointLights[i].active = 1;
				constMap->pointLights[i].lightpos = pointLights[i].GetLightPos();
				constMap->pointLights[i].lightcolor = pointLights[i].GetLightColor();
				constMap->pointLights[i].lightatten = pointLights[i].GetLightAtten();
			}
			else
			{
				constMap->pointLights[i].active = 0;
			}
		}


		constBuff->Unmap(0, nullptr);
	}
}

void LightGroup::DefaultLightSetting()
{
	dirLights[0].SetActive(true);
	dirLights[0].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLights[0].SetLightDir({ 0.0f, -1.0f, 0.0f });

	//dirLights[1].SetActive(true);
	//dirLights[1].SetLightColor({ 1.0f, 1.0f, 1.0f });
	//dirLights[1].SetLightDir({+0.5f, +0.1f, +0.2f});

	//dirLights[2].SetActive(true);
	//dirLights[2].SetLightColor({ 1.0f, 1.0f, 1.0f });
	//dirLights[2].SetLightDir({ -0.5f, +0.1f, -0.2f });
}

void LightGroup::SetAmbientColor(const Vector3& color)
{
	ambientColor = color;
	dirty = true;
}

void LightGroup::SetDirLightActive(int index, bool active)
{
	assert(0 <= index && index < DirLightNum);

	dirLights[index].SetActive(active);
}

void LightGroup::SetDirLightDir(int index, const Vector3& lightdir)
{
	assert(0 <= index && index < DirLightNum);

	dirLights[index].SetLightDir(lightdir);
	dirty = true;
}

void LightGroup::SetDirLightColor(int index, const Vector3& lightcolor)
{
	assert(0 <= index && index < DirLightNum);

	dirLights[index].SetLightColor(lightcolor);
	dirty = true;
}

void LightGroup::SetPointLightActive(int index, bool active)
{
	assert(0 <= index && index < PointLightNum);
	pointLights[index].SetActive(active);
}

void LightGroup::SetPointLightPos(int index, const Vector3& lightpos)
{
	assert(0 <= index && index < PointLightNum);
	pointLights[index].SetLightPos(lightpos);
	dirty = true;
}

void LightGroup::SetPointLightColor(int index, const Vector3& lightcolor)
{
	assert(0 <= index && index < PointLightNum);
	pointLights[index].SetLightColor(lightcolor);
	dirty = true;
}

void LightGroup::SetPointLightAtten(int index, const Vector3& lightatten)
{
	assert(0 <= index && index < PointLightNum);
	pointLights[index].SetLightAtten(lightatten);
	dirty = true;
}
