#include "LightGroup.h"
#include<cassert>
#include"DirectXCommon.h"


LightGroup* LightGroup::Create()
{
	LightGroup* instance = new LightGroup();


	instance->Initialize();

	return instance;
}

void LightGroup::Initialize()
{
	//dirLights.Initialize();

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
	result = SKNEngine::DirectXCommon::GetDevice().Get()->CreateCommittedResource(
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

		
		
		if (dirLights.IsActive())
		{
			constMap->dirLights.active = true;
			constMap->dirLights.lightv = -dirLights.GetLightDir();
			constMap->dirLights.lightcolor = dirLights.GetLightColor();
		}
		else
		{
			constMap->dirLights.active = 0;
		}
		

	
		
		if (pointLights.IsActive())
		{
			constMap->pointLights.active = 1;
			constMap->pointLights.lightpos = pointLights.GetLightPos();
			constMap->pointLights.lightcolor = pointLights.GetLightColor();
			constMap->pointLights.lightatten = pointLights.GetLightAtten();
		}
		else
		{
			constMap->pointLights.active = 0;
		}
		


		constBuff->Unmap(0, nullptr);
	}
}

void LightGroup::DefaultLightSetting()
{
	
	dirLights.SetActive(true);
	dirLights.SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLights.SetLightDir({ 0.0f, -1.0f, 0.0f });
	
}

void LightGroup::SetAmbientColor(const Vector3& color)
{
	ambientColor = color;
	dirty = true;
}

void LightGroup::SetDirLightActive(bool active)
{
	dirLights.SetActive(active);
}

void LightGroup::SetDirLightDir(const Vector3& lightdir)
{
	dirLights.SetLightDir(lightdir);
	dirty = true;
}

void LightGroup::SetDirLightColor(const Vector3& lightcolor)
{
	dirLights.SetLightColor(lightcolor);
	dirty = true;
}

void LightGroup::SetPointLightActive(bool active)
{
	pointLights.SetActive(active);
}

void LightGroup::SetPointLightPos(const Vector3& lightpos)
{
	pointLights.SetLightPos(lightpos);
	dirty = true;
}

void LightGroup::SetPointLightColor(const Vector3& lightcolor)
{
	pointLights.SetLightColor(lightcolor);
	dirty = true;
}

void LightGroup::SetPointLightAtten(const Vector3& lightatten)
{
	pointLights.SetLightAtten(lightatten);
	dirty = true;
}
