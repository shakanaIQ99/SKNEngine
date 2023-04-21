#include "FbxLoader.h"
#include<cassert>

const std::string FbxLoader::baseDirectory = "Resources/";

using namespace DirectX;

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

    Model* model = new Model();
    model->name = modelname;

    int nodeCount = fbxScene->GetNodeCount();

    model->nodes.reserve(nodeCount);

    ParseNodeRecursive(model, fbxScene->GetRootNode());

    fbxScene->Destroy();


}

void FbxLoader::ParseNodeRecursive(Model* model, FbxNode* fbxNode, Node* parent = nullptr)
{
    model->nodes.emplace_back();
    Node& node = model->nodes.back();

    node.name = fbxNode->GetName();

    FbxDouble3 rotation = fbxNode->LclRotation.Get();
    FbxDouble3 scaling = fbxNode->LclScaling.Get();
    FbxDouble3 translation = fbxNode->LclTranslation.Get();

    node.rotation = { (float)rotation[0],(float)rotation[1],(float)rotation[2],0.0f };
    node.scaling = { (float)scaling[0],(float)scaling[1],(float)scaling[2],0.0f };
    node.translation = { (float)translation[0],(float)translation[1],(float)translation[2],0.0f };

    node.rotation.m128_f32[0] = XMConvertToRadians(node.rotation.m128_f32[0]);
    node.rotation.m128_f32[1] = XMConvertToRadians(node.rotation.m128_f32[1]);
    node.rotation.m128_f32[2] = XMConvertToRadians(node.rotation.m128_f32[2]);

    XMMATRIX matScaling, matRotation, matTranslation;
    matScaling = XMMatrixScalingFromVector(node.scaling);
    matRotation = XMMatrixRotationRollPitchYawFromVector(node.rotation);
    matTranslation = XMMatrixTranslationFromVector(node.translation);

    node.transform = XMMatrixIdentity();
    node.transform *= matScaling;
    node.transform *= matRotation;
    node.transform *= matTranslation;

    node.globalTransform = node.transform;
    if (parent)
    {
        node.parent = parent;

        node.globalTransform *= parent->globalTransform;
    }

    FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();

    if (fbxNodeAttribute)
    {
        if (fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
        {
            model->meshNode = &node;
            ParseMesh(model, fbxNode);
        }
    }



    for (int i = 0; i < fbxNode->GetChildCount(); i++)
    {
        ParseNodeRecursive(model, fbxNode->GetChild(i), &node);
    }
}

void FbxLoader::ParseMesh(Model* model, FbxNode* fbxNode)
{
    FbxMesh* fbxMesh = fbxNode->GetMesh();

    ParseMeshVertices(model, fbxMesh);

    ParseMeshFaces(model, fbxMesh);

    ParseMaterial(model, fbxNode);

}

void FbxLoader::ParseMeshVertices(Model* model, FbxMesh* fbxMesh)
{
    auto& vertices = model->vertices;

    const int contorolPointCount = fbxMesh->GetControlPointsCount();

    Model::VertexPosNormalUv vert{};
    model->vertices.resize(contorolPointCount, vert);

    FbxVector4* pCoord = fbxMesh->GetControlPoints();

    for (int i = 0; i < contorolPointCount; i++)
    {
        Model::VertexPosNormalUv& vertex = vertices[i];

        vertex.pos.x = (float)pCoord[i][0];
        vertex.pos.y = (float)pCoord[i][1];
        vertex.pos.z = (float)pCoord[i][2];
    }

}

void FbxLoader::ParseMeshFaces(Model* model, FbxMesh* fbxMesh)
{
    auto& vertices = model->vertices;
    auto& indices = model->indices;

    assert(indices.size() == 0);

    const int polygonCount = fbxMesh->GetPolygonCount();

    const int textureUVCount = fbxMesh->GetTextureUVCount();

    FbxStringList uvNames;
    fbxMesh->GetUVSetNames(uvNames);

    for (int i = 0; i < polygonCount; i++)
    {
        const int polygonSize = fbxMesh->GetPolygonSize(i);
        assert(polygonSize <= 4);
        for (int j = 0; j < polygonSize; j++)
        {
            int index = fbxMesh->GetPolygonVertex(i, j);
            assert(index >= 0);

            Model::VertexPosNormalUv& vertex = vertices[index];
            FbxVector4 normal;
            if (fbxMesh->GetPolygonVertexNormal(i, j, normal))
            {
                vertex.normal.x = (float)normal[0];
                vertex.normal.y = (float)normal[1];
                vertex.normal.z = (float)normal[2];
            }

        }

    }
}

void FbxLoader::ParseMaterial(Model* model, FbxNode* fbxNode)
{
}
