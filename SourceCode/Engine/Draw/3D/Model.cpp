#include "Model.h"
#include<cassert>
#include<fstream>
#include<sstream>
#include <DirectXTex.h>


Model::~Model()
{
	fbxScene->Destroy();
}

//void Model::Finalize()
//{
//	fbxScene->Destroy();
//}

void Model::CreateBuffers(ID3D12Device* device)
{
	vertexBuffer = make_unique<VertexBuffer>();
	vertexBuffer->Create(device, vertices.size(), sizeof(VertexPosNormalUvSkin), vertices.data());

	indexBuffer = make_unique<IndexBuffer>();
	indexBuffer->Create(device, indices.size(),indices.data());

	

}

void Model::Draw(ID3D12GraphicsCommandList* cmdList)
{
	vertexBuffer->Update(vertices.data());
	indexBuffer->Update(indices.data());

	vbView = vertexBuffer->GetView();
	ibView = indexBuffer->GetView();

	cmdList->IASetVertexBuffers(0, 1,&vbView);
	cmdList->IASetIndexBuffer(&ibView);

	cmdList->SetDescriptorHeaps(1, tex->srvHeap.GetAddressOf());

	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = tex->gpuHandle;

	cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);

}







/*-----------------------------------------以下OBJ----------------------------------------------------
ID3D12Device* Model::device = nullptr;

Model* Model::LoadFromOBJ(const string& modelname, bool smoothing)
{
    Model* model = new Model();

	model->LoadFromOBJInternal(modelname,smoothing);

	model->CreateBuffers();

    return model;
}

void Model::Draw(ID3D12GraphicsCommandList* commandList, UINT rootParamIndexMaterial)
{
	commandList->IASetVertexBuffers(0, 1, &vbView);

	commandList->IASetIndexBuffer(&ibView);

	commandList->SetGraphicsRootConstantBufferView(rootParamIndexMaterial, constBuffB1->GetGPUVirtualAddress());

	commandList->SetDescriptorHeaps(1, tex->srvHeap.GetAddressOf());

	if (material.textureFilename.size() > 0)
	{
		commandList->SetGraphicsRootDescriptorTable(2, tex->gpuHandle);
	}
	//commandList->SetGraphicsRootDescriptorTable(2, srvGpuHandle);
	commandList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}



void Model::LoadFromOBJInternal(const string& modelname, bool smoothing)
{
	HRESULT result;

	ifstream file;
	//file.open("Resources/triangle_tex/triangle_tex.obj");
	//const string modelname = "triangle_mat";
	const string filename = modelname + ".obj";
	const string directoryPath = "Resources/" + modelname + "/";
	file.open(directoryPath + filename);
	assert(!file.fail());

	vector<XMFLOAT3> positions;
	vector<XMFLOAT3> normals;
	vector<XMFLOAT2> texcoords;

	string line;
	while (getline(file, line))
	{

		istringstream line_stream(line);
		string key;
		getline(line_stream, key, ' ');

		if (key == "mtllib")
		{
			string filename;
			line_stream >> filename;

			LoadMaterial(directoryPath, filename);
		}
		if (key == "v")
		{
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			positions.emplace_back(position);
		}
		if (key == "vt")
		{
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;

			texcoord.y = 1.0f - texcoord.y;

			texcoords.emplace_back(texcoord);
		}
		if (key == "vn")
		{
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;

			normals.emplace_back(normal);
		}
		if (key == "f")
		{
			string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexNormal;

				VertexPos vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices.emplace_back(vertex);
				if (smoothing)
				{
					AddSmoothData(indexPosition, (unsigned short)vertices.size() - 1);
				}

				indices.emplace_back((unsigned short)indices.size());
			}
		}
	}

	file.close();

	if (smoothing)
	{
		CaliculateSmoothedVertexNormals();
	}

	UINT sizeVB = static_cast<UINT>(sizeof(VertexPos) * vertices.size());

	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);
	assert(SUCCEEDED(result));


	// 頂点バッファへのデータ転送
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		//memcpy(vertMap, vertices, sizeof(vertices));
		copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	//UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());

	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;

	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;	//インデックス情報が入るサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = device->CreateCommittedResource(
		&heapProp,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);

	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result))
	{
		//memcpy(indexMap, indices, sizeof(indices));
		copy(indices.begin(), indices.end(), indexMap);
		indexBuff->Unmap(0, nullptr);
	}

	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
}

void Model::LoadMaterial(const string& directoryPath, const string& filename)
{
	ifstream file;
	file.open(directoryPath + filename);
	if (file.fail())
	{
		assert(0);
	}

	string line;
	while (getline(file, line))
	{
		istringstream line_stream(line);
		string key;
		getline(line_stream, key, ' ');

		if (key[0] == '\t')
		{
			key.erase(key.begin());
		}
		if (key == "newmtl")
		{
			line_stream >> material.name;
		}
		if (key == "Ka")
		{
			line_stream >> material.ambient.x;
			line_stream >> material.ambient.y;
			line_stream >> material.ambient.z;
		}
		if (key == "Kd")
		{
			line_stream >> material.diffuse.x;
			line_stream >> material.diffuse.y;
			line_stream >> material.diffuse.z;
		}
		if (key == "Ks")
		{
			line_stream >> material.specular.x;
			line_stream >> material.specular.y;
			line_stream >> material.specular.z;
		}
		if (key == "map_Kd")
		{
			line_stream >> material.textureFilename;
			LoadTexture(directoryPath, material.textureFilename);
		}
	}
	file.close();
}

void Model::LoadTexture(const string& directoryPath, const string& filename)
{
	string filepath = directoryPath + filename;

	uint32_t handl = TextureManager::Load(filepath);

	tex = TextureManager::GetTextureData(handl);
}

void Model::CreateBuffers()
{
	HRESULT result = S_FALSE;

	D3D12_HEAP_PROPERTIES cbHeapPropB1{};

	cbHeapPropB1.Type = D3D12_HEAP_TYPE_UPLOAD;
	D3D12_RESOURCE_DESC cbResourceDescB1{};
	//リソース設定
	cbResourceDescB1.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDescB1.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0Xff;	//256バイトアライメント
	cbResourceDescB1.Height = 1;
	cbResourceDescB1.DepthOrArraySize = 1;
	cbResourceDescB1.MipLevels = 1;
	cbResourceDescB1.SampleDesc.Count = 1;
	cbResourceDescB1.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = device->CreateCommittedResource(
		&cbHeapPropB1,		//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDescB1,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB1)
	);
	assert(SUCCEEDED(result));

	ConstBufferDataMaterial* constMap1 = nullptr;
	result = constBuffB1->Map(0, nullptr, (void**)&constMap1);
	assert(SUCCEEDED(result));
	//constMap->color = color;
	//constMap->mat =matWorld* matView * matProjection;
	//constBuff->Unmap(0, nullptr);
	constMap1->ambient = material.ambient;
	constMap1->diffuse = material.diffuse;
	constMap1->specular = material.specular;
	constMap1->alpha = material.alpha;
	constBuffB1->Unmap(0, nullptr);

}

void Model::AddSmoothData(unsigned short indexPosition, unsigned short indexVertex)
{
	smoothData[indexPosition].emplace_back(indexVertex);
}

void Model::CaliculateSmoothedVertexNormals()
{
	auto itr = smoothData.begin();
	for (; itr != smoothData.end(); ++itr)
	{
		std::vector<unsigned short>& v = itr->second;

		XMVECTOR normal = {};
		for (unsigned short index : v)
		{
			normal += XMVectorSet(vertices[index].normal.x, vertices[index].normal.y, vertices[index].normal.z, 0);
		}
		normal = XMVector3Normalize(normal / (float)v.size());
		for (unsigned short index : v)
		{
			vertices[index].normal = { normal.m128_f32[0],normal.m128_f32[1],normal.m128_f32[2] };
		}

	}
}
*/

