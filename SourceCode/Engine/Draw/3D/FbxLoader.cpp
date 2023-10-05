#include "FbxLoader.h"
#include<cassert>

const string FbxLoader::baseDirectory = "Resources/";
const string FbxLoader::defaultTextureFileName = "white1x1.png";

using namespace DirectX;

FbxLoader* FbxLoader::GetInstance()
{
    static FbxLoader instance;
    return &instance;
}

void FbxLoader::Initialize(ID3D12Device* _device)
{
    assert(fbxManager == nullptr);
    assert(_device);

    device = _device;

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

Model* FbxLoader::LoadModelFlomFile(const string& modelname)
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

    //fbxScene->Destroy();

    model->fbxScene = fbxScene;

    return model;

}

void FbxLoader::ParseNodeRecursive(Model* model, FbxNode* fbxNode, Node* parent)
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

    //FBXノードのメッシュ情報を解析
    FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();

    if (fbxNodeAttribute)
    {
        if (fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
        {
            model->meshNode = &node;
            ParseMesh(model, fbxNode);
        }
    }
    //子ノードに対して再帰呼び出し


    for (int i = 0; i < fbxNode->GetChildCount(); i++)
    {
        ParseNodeRecursive(model, fbxNode->GetChild(i), &node);
    }
}

void FbxLoader::ParseMesh(Model* model, FbxNode* fbxNode)
{
    //ノードのメッシュを取得
    FbxMesh* fbxMesh = fbxNode->GetMesh();
    //頂点座標読み取り
    ParseMeshVertices(model, fbxMesh);
    //面を構成するデータの読み取り
    ParseMeshFaces(model, fbxMesh);
    //マテリアルの読み取り
    ParseMaterial(model, fbxNode);
    //スキニングの読み取り
    ParseSkin(model, fbxMesh);
}

void FbxLoader::ParseMeshVertices(Model* model, FbxMesh* fbxMesh)
{
    auto& vertices = model->vertices;
    //頂点座標データの数
    const int contorolPointCount = fbxMesh->GetControlPointsCount();
    //必要数だけ頂点データ配列を確保
    Model::VertexPosNormalUvSkin vert{};
    model->vertices.resize(contorolPointCount, vert);
    //FBXメッシュの頂点座標配列を取得
    FbxVector4* pCoord = fbxMesh->GetControlPoints();
    //FBXメッシュの全頂点座標をモデル内の配列にコピーする。
    for (int i = 0; i < contorolPointCount; i++)
    {
        Model::VertexPosNormalUvSkin& vertex = vertices[i];
        //座標のコピー
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
    //面の数
    const int polygonCount = fbxMesh->GetPolygonCount();
    //UVデータの数
    const int textureUVCount = fbxMesh->GetTextureUVCount();
    //UV名リスト
    FbxStringList uvNames;
    fbxMesh->GetUVSetNames(uvNames);
    //面ごとの情報読み取り
    for (int i = 0; i < polygonCount; i++)
    {
        //面を構成する頂点の数を取得(3なら三角形ポリゴン)
        const int polygonSize = fbxMesh->GetPolygonSize(i);
        assert(polygonSize <= 4);

        //1頂点ずつ処理
        for (int j = 0; j < polygonSize; j++)
        {
            //FBX頂点配列インデックス
            int index = fbxMesh->GetPolygonVertex(i, j);
            assert(index >= 0);
            //頂点法線読み込み
            Model::VertexPosNormalUvSkin& vertex = vertices[index];
            FbxVector4 normal;
            if (fbxMesh->GetPolygonVertexNormal(i, j, normal))
            {
                vertex.normal.x = (float)normal[0];
                vertex.normal.y = (float)normal[1];
                vertex.normal.z = (float)normal[2];
            }
            //テクスチャUV読み込み
            if (textureUVCount > 0)
            {
                FbxVector2 uvs;
                bool lUnmappedUV;
                //0番決め打ち
                if (fbxMesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUnmappedUV))
                {
                    vertex.uv.x = (float)uvs[0];
                    vertex.uv.y = (float)uvs[1];
                }
                //インデックス配列に頂点インデックス追加
                //3頂点目までなら
                if (j < 3)
                {
                    //1点追加し、他2点と三角形を構築する
                    indices.push_back(index);
                }
                //4頂点目
                else
                {
                    //3点追加し
                    //シ角形の0,1,2の内,3,0で三角形を構築
                    int index2 = indices[indices.size() - 1];
                    int index3 = index;
                    int index0 = indices[indices.size() - 3];

                    indices.push_back(index2);
                    indices.push_back(index3);
                    indices.push_back(index0);

                }
            }

        }

    }
}

void FbxLoader::ParseMaterial(Model* model, FbxNode* fbxNode)
{
    const int materialCount = fbxNode->GetMaterialCount();
    if (materialCount > 0)
    {
        //先頭のマテリアルを取得
        FbxSurfaceMaterial* material = fbxNode->GetMaterial(0);
        //テクスチャを読み込んだかどうかを表すフラグ
        bool textureLoaded = false;

        if (material)
        {
            //FbxSurfaceLambertクラスかどうかを調べる
            if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
            {
                FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(material);

                model->material = Model::Material();
                //環境反射光
                FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;
                model->material.ambient.x = (float)ambient.Get()[0];
                model->material.ambient.y = (float)ambient.Get()[1];
                model->material.ambient.z = (float)ambient.Get()[2];
                //拡散反射光係数
                FbxPropertyT<FbxDouble3>diffuse = lambert->Diffuse;
                model->material.diffuse.x = (float)diffuse.Get()[0];
                model->material.diffuse.y = (float)diffuse.Get()[1];
                model->material.diffuse.z = (float)diffuse.Get()[2];
                //ディフーズテクスチャを取り出す
                const FbxProperty diffuseProperty = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
                if (diffuseProperty.IsValid())
                {
                    const FbxFileTexture* texture = diffuseProperty.GetSrcObject<FbxFileTexture>();
                    if (texture)
                    {
                        const char* filepath = texture->GetFileName();
                        //ファイルパスからファイル名抽出
                        string path_str(filepath);
                        string name = ExtractFileName(path_str);
                        //テクスチャ読み込み
                        LoadTexture(model, baseDirectory + model->name + "/" + name);
                        textureLoaded = true;
                    }
                }


            }
        }
        //テクスチャがない場合は白テクスチャを貼る
        if (!textureLoaded)
        {
            LoadTexture(model, baseDirectory + defaultTextureFileName);
        }
    }


}

void FbxLoader::ParseSkin(Model* model, FbxMesh* fbxMesh)
{
    FbxSkin* fbxSkin = static_cast<FbxSkin*>(fbxMesh->GetDeformer(0, FbxDeformer::eSkin));

    if (fbxSkin == nullptr)
    {
        for (size_t i = 0; i < model->vertices.size(); i++)
        {
            model->vertices[i].boneIndex[0] = 0;
            model->vertices[i].boneWeight[0] = 1.0f;
        }

        return;
    }

    vector<Model::Bone>& bones = model->bones;

    int clusterCount = fbxSkin->GetClusterCount();
    bones.reserve(clusterCount);

    for (int i = 0; i < clusterCount; i++)
    {
        FbxCluster* fbxCluster = fbxSkin->GetCluster(i);

        const char* boneName = fbxCluster->GetLink()->GetName();

        bones.emplace_back(Model::Bone(boneName));
        Model::Bone& bone = bones.back();

        bone.fbxCluster = fbxCluster;

        FbxAMatrix fbxMat;
        fbxCluster->GetTransformLinkMatrix(fbxMat);

        XMMATRIX initialPose;
        ConvertMatrixFromFbx(&initialPose, fbxMat);

        bone.invInitialPose = XMMatrixInverse(nullptr, initialPose);

    }

    struct WeightSet
    {
        UINT index;
        float weight;
    };

    vector<list<WeightSet>>weightLists(model->vertices.size());

    for (int i = 0; i < clusterCount; i++)
    {
        FbxCluster* fbxCluster = fbxSkin->GetCluster(i);

        int contorolPointIndicesCount = fbxCluster->GetControlPointIndicesCount();

        int* controlPointIndices = fbxCluster->GetControlPointIndices();
        double* controlPointWeights = fbxCluster->GetControlPointWeights();

        for (int j = 0; j < contorolPointIndicesCount; j++)
        {
            int vertIndex = controlPointIndices[j];

            float weight = (float)controlPointWeights[j];

            weightLists[vertIndex].emplace_back(WeightSet{ (UINT)i,weight });
        }
    }

    auto& vertices = model->vertices;

    for (size_t i = 0; i < vertices.size(); i++)
    {
        auto& weightList = weightLists[i];

        weightList.sort([](auto const& lhs, auto const& rhs) {return lhs.weight > rhs.weight; });

        int weightArrayIndex = 0;

        for (auto& WeightSet : weightList)
        {
            vertices[i].boneIndex[weightArrayIndex] = WeightSet.index;
            vertices[i].boneWeight[weightArrayIndex] = WeightSet.weight;

            if (++weightArrayIndex >= Model::MAX_BONE_INDICES)
            {
                float weight = 0.0f;
                for (int j = 1; j < Model::MAX_BONE_INDICES; j++)
                {
                    weight += vertices[i].boneWeight[j];
                }
                vertices[i].boneWeight[0] = 1.0f - weight;
                break;
            }
        }
    }

}

void FbxLoader::LoadTexture(Model* model, const std::string& fullpath)
{
    uint32_t handl = TextureManager::Load(fullpath);

    model->tex = TextureManager::GetTextureData(handl);


}

string FbxLoader::ExtractFileName(const string& path)
{
    size_t pos1;
    //区切り文字　'\\' が出てくる一番最後の部分を検索
    pos1 = path.rfind('\\');
    if (pos1 != string::npos)
    {
        return path.substr(pos1 + 1, path.size() - pos1 - 1);
    }
    //区切り文字 '/' が出てくる一番最後の部分を検索
    pos1 = path.rfind('/');
    if (pos1 != string::npos)
    {
        return path.substr(pos1 + 1, path.size() - pos1 - 1);
    }
    return path;
}

void FbxLoader::ConvertMatrixFromFbx(XMMATRIX* dst, const FbxAMatrix& src)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            dst->r[i].m128_f32[j] = (float)src.Get(i, j);
        }
    }


}
