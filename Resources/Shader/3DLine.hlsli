cbuffer ConstBufferDataMaterial : register(b0)
{
    matrix viewproj;
    matrix mat;
    float3 cameraPos;
    float4 color;
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float4 worldpos : POSITION;
    float3 normal : NORMAL;
};