#include "FbxLoader.h"
#include<cassert>

const std::string FbxLoader::baseDirectory = "Resources/";

FbxLoader* FbxLoader::GetInstance()
{
    static FbxLoader instance;
    return &instance;
}

void FbxLoader::Initialize(ID3D12Device* device)
{
    assert(fbxManager == nullptr);

    this->device = device;

    fbxManager = FbxManager::Create();

    FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);

    fbxManager->SetIOSettings(ios);

    fbxImpoter = FbxImporter::Create(fbxManager, "");

}

void FbxLoader::Finalize()
{
    fbxImpoter->Destroy();
    fbxManager->Destroy();


}

void FbxLoader::LoadModelFlomFile(const string& modelname)
{
    const string directoryPath = baseDirectory + modelname + "/";

    const string fileName = modelname + ".fbx";

    const string fullpath = directoryPath + fileName;

    if (!fbxImpoter->Initialize(fullpath.c_str(), -1, fbxManager->GetIOSettings()))
    {
        assert(0);
    }

    FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxScene");

    fbxImpoter->Import(fbxScene);

}
