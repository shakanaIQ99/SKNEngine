#include "3DLine.hlsli"
VSOutput main(float4 pos : POSITION, float3 normal : NORMAL)
{
    float4 wnormal = normalize(mul(mat, float4(normal, 0)));
	
    float4 wpos = mul(mat, pos);

    VSOutput output;
    output.svpos = mul(mul(viewproj, mat), pos);
    output.worldpos = wpos;
    output.normal = wnormal.xyz;
    return output;
}