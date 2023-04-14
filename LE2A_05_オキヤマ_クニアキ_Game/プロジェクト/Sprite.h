#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

using namespace DirectX;

using namespace Microsoft::WRL;

class Sprite
{

private:

	//static const int vertexcount = 3;


public:

	struct VertexPos
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT2 uv;
	};

	struct ConstBufferDataMaterial
	{
		XMFLOAT4 color;
		XMMATRIX mat;
	};


	void StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	/// <param name="cmdList">�`��R�}���h���X�g</param>
	void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �`��㏈��
	/// </summary>
	void PostDraw();




public:

	void Initialize();

	void InitializeGraphicsPipeline();

	void CreateDescriptorHeap();

	void LoadTexture(UINT texnumber, const wchar_t* filename);

	
	


	void Draw();

private:

	static const int SrvCount = 512;

	ID3D12Device* device;

	ComPtr<ID3D12DescriptorHeap> descHeap;

	// �R�}���h���X�g
	ID3D12GraphicsCommandList* commandList;

	ComPtr<ID3D12Resource> vertBuff;

	ComPtr<ID3D12Resource> indexBuff;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;

	ComPtr<ID3D12Resource> texBuff[SrvCount];

	ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	ComPtr<ID3D12PipelineState> pipelinestate;

	D3D12_VERTEX_BUFFER_VIEW vbView;
	D3D12_INDEX_BUFFER_VIEW ibView;
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;

	

	//XMFLOAT3 vertices[vertexcount]; 


	

private:

	
	

};

